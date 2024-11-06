#include "stdafx.h"
#include "Goomba.h"

#include "Scenes/MarioScene/Characters/MarioCharacter.h"
#include "Scenes/MarioScene/MarioBombScene.h"

//MATERIALS
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterials_FlipNormals.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_NegativeNormals.h"

Goomba::Goomba(XMFLOAT3& startPos)
{
	m_ModelComponentPath = L"Meshes/Characters/gomba.ovm" ;
	m_ConvexMeshPath =  L"Meshes/Characters/gomba.ovpc";
	m_TriangleMeshPath = L"Meshes/Characters/gomba.ovpt";
	m_TexturePath = L"goomba_tex.PNG";
	m_StartPos = startPos;

	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,0.f,0.f };
	settings.minSize = 0.0f;
	settings.maxSize = 0.0f;
	settings.minEnergy = 0.f;
	settings.maxEnergy = 0.f;
	settings.minScale = 0.0f;
	settings.maxScale = 0.0f;
	settings.minEmitterRadius = 0.0f;
	settings.maxEmitterRadius = 0.0f;
	settings.color = { 1.f,1.f,1.f, 5.f };
	settings.isFacingCamera = false;
	settings.isRotateUV = false;
	settings.isRotating = true;
	settings.rotationSpeed = 30.f;

	//settings.rotationSpeed = m_RotationSpeed / 6.0f;

	settings.rotationAxis = { 0.0f, 1.0f, 0.0f };
	settings.rotationDisplacement = 5.0f;

	const auto pObject = AddChild(new GameObject);
	XMFLOAT3 starLocation{ startPos.x, startPos.y - 5, startPos.z };

	m_pEmitter = pObject->AddComponent(new ParticleEmitterComponent(L"Textures/GombaDead.png", settings, 1, starLocation));
	m_pEmitter->GetTransform()->Translate(starLocation);




}

Goomba::Goomba(std::vector<XMFLOAT3>& startPosVector)
{
	m_ModelComponentPath = L"Meshes/Characters/gomba.ovm";
	m_ConvexMeshPath = L"Meshes/Characters/gomba.ovpc";
	m_TriangleMeshPath = L"Meshes/Characters/gomba.ovpt";
	m_TexturePath = L"goomba_tex.PNG";
	m_StartPosVector = startPosVector;
}

void Goomba::Initialize(const SceneContext& /*sceneContext*/)
{
	float scaleFactor = 0.035f;
	XMFLOAT3 scaleVector{ scaleFactor, scaleFactor, scaleFactor };
	float scaleFactor1 = 0.045f;

	//CREATE GOMBA
	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(0.0f, 0.0f, 0.0f);
	m_pGomba = new GameObject{};
	m_pGombaBottomCollider = new GameObject{};
	m_pGombaTopCollider = new GameObject{};

	//create and add model component to empty game object above
	ModelComponent* gombaModelComponent{ new ModelComponent{m_ModelComponentPath} };
	m_pGomba->AddComponent<ModelComponent>(gombaModelComponent);
	DiffuseMaterial_Shadow_NegativeNormals* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_NegativeNormals>();
	pMaterial->SetDiffuseTexture(L"Textures/GombaTextures/" + m_TexturePath);
	gombaModelComponent->SetMaterial(pMaterial->GetMaterialId(), 0U);


	//get the convex mesh data
	auto pConvexComponentGomba = ContentManager::Load<PxConvexMesh>(L"Meshes/Characters/gomba.ovpc");


	m_pGomba->AddComponent(new RigidBodyComponent(false));
	
	auto pRigid = m_pGomba->AddComponent(new RigidBodyComponent(false));
	pRigid->AddCollider(PxConvexMeshGeometry{ pConvexComponentGomba, PxMeshScale({ scaleFactor, scaleFactor, scaleFactor }) }, *pBouncyMaterial, false);

	//m_pGomba->AddChild(pGomba);
	//m_pGomba->AddChild(pGomba2);

	XMFLOAT3 dimensionsCollider{ 2.7f, .5f, 2.7f };

	//create a rigid body
	auto pTriggerBottom = m_pGombaBottomCollider->AddComponent(new RigidBodyComponent(true));
	pTriggerBottom->AddCollider(PxConvexMeshGeometry{ pConvexComponentGomba, PxMeshScale({ scaleFactor1, scaleFactor1, scaleFactor1 }) }, *pBouncyMaterial, true);

	
	auto pTriggerTop= m_pGombaTopCollider->AddComponent(new RigidBodyComponent(true));
	pTriggerTop->AddCollider(PxBoxGeometry{ dimensionsCollider.x / 2, dimensionsCollider.y / 2, dimensionsCollider.z / 2 }, *pBouncyMaterial, true);
	//pTriggerTop->AddCollider(PxConvexMeshGeometry{ pConvexComponentGomba, PxMeshScale({ scaleFactor, scaleFactor, scaleFactor }) }, *pBouncyMaterial, true);

	m_pGombaBottomCollider->SetOnTriggerCallBack([=](GameObject* /*pGomba*/, GameObject* pObject, PxTriggerAction /*action*/)
		{
			if (!pObject) return;
			if (pObject == m_pGomba) return;
				OnTriggerBottom();

		});

	m_pGombaTopCollider->SetOnTriggerCallBack([=](GameObject* /*pGomba*/, GameObject* pObject, PxTriggerAction /*action*/)
		{
						if (!pObject) return;
							OnTriggerTop();


		});

	AddChild(m_pGomba);
	m_pGomba->GetTransform()->Scale(scaleVector);
	m_pGomba->GetTransform()->Translate(m_StartPos);
	m_pGomba->GetTransform()->Rotate(0, 90, 0);

	AddChild(m_pGombaBottomCollider);
	m_pGombaBottomCollider->GetTransform()->Scale(scaleVector);
	m_pGombaBottomCollider->GetTransform()->Translate(m_StartPos);
	m_pGombaBottomCollider->GetTransform()->Rotate(0, 90, 0);

	AddChild(m_pGombaTopCollider);
	m_pGombaTopCollider->GetTransform()->Scale(scaleVector);
	m_pGombaTopCollider->GetTransform()->Translate(m_StartPos);
	m_pGombaTopCollider->GetTransform()->Rotate(0, 90, 0);


	//SoundManager::Get()->GetSystem()->createSound("Resources/Sounds/goomba_dead.wav", FMOD_INIT_NORMAL, nullptr, &m_pSound);

}

void Goomba::Update(const SceneContext& sceneContext)
{
	float dt = sceneContext.pGameTime->GetElapsed();
	if (m_KilledMario)
	{
		m_TimeSinceKill += dt;
		if (m_TimeSinceKill > 4.0f)
		{
			m_KilledMario = false;
			m_TimeSinceKill = 0.0f;
		}
	}
	

	if (!m_IsDeleted)
	{
		XMFLOAT3 pos = m_pGomba->GetTransform()->GetPosition();

		m_pGombaBottomCollider->GetTransform()->Translate(pos.x, pos.y, pos.z);
		m_pGombaTopCollider->GetTransform()->Translate(pos.x, pos.y + 3.7f, pos.z);
	}


	
	if (m_IsKilled && !m_IsDeleted)
	{
		m_TimeSinceDeath += dt;
		if (m_TimeSinceDeath >= m_MaxTime)
		{
			GetScene()->RemoveChild(this, true);
			m_IsDeleted = true;
		}
	}
}

void Goomba::CreateMultiple()
{
	float scaleFactor = 0.025f;
	XMFLOAT3 scaleVector{ scaleFactor, scaleFactor, scaleFactor };


	auto& physx = PxGetPhysics();

	auto pBouncyMaterial = physx.createMaterial(0.0f, 0.0f, 0.0f);

	for (size_t pos{ 0 }; pos < m_StartPosVector.size(); ++pos)
	{
		ModelComponent* gombaModelComponent{ new ModelComponent{m_ModelComponentPath} };
		auto gomba = new GameObject{};
		gomba->AddComponent<ModelComponent>(gombaModelComponent);
		gomba->GetTransform()->Scale(scaleVector);
		AddChild(gomba);

		auto pConvexComponentGomba = ContentManager::Load<PxConvexMesh>(L"Meshes/Characters/gomba.ovpc");
		auto pRigidBody = gomba->AddComponent(new RigidBodyComponent(false));
		pRigidBody->AddCollider(PxConvexMeshGeometry{ pConvexComponentGomba, PxMeshScale({ scaleFactor, scaleFactor, scaleFactor }) }, *pBouncyMaterial);

		gomba->GetTransform()->Translate(m_StartPosVector[pos]);
		gomba->GetTransform()->Rotate(0, 90, 0);

		DiffuseMaterial_Shadow_NegativeNormals* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_NegativeNormals>();
		pMaterial->SetDiffuseTexture(L"Textures/GombaTextures/" + m_TexturePath);
		gombaModelComponent->SetMaterial(pMaterial->GetMaterialId(), 0U);
	}
}

void Goomba::OnTriggerBottom()
{
	if (m_IsKilled) return;
	if (m_KilledMario) return;

		MarioCharacter::m_NrLives--;
		m_KilledMario = true;
}

void Goomba::OnTriggerTop()
{
	auto& settings = m_pEmitter->GetSettings();
	settings.velocity = { 0.f,0.f,0.f };
	settings.minSize = 2.f;
	settings.maxSize = 2.f;
	settings.minScale = 1.5f;
	settings.maxScale = 3.0f;
	settings.minEnergy = 2;
	settings.maxEnergy = 2;
	m_IsKilled = true;
	m_pGomba->GetTransform()->Scale(0.035f, 0.012f, 0.035f);
}

void Goomba::OnTriggerArea()
{
	if (!m_pMarioTrigger) return;

}
