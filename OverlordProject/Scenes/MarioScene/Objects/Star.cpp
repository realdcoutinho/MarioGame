#include "stdafx.h"
#include "Star.h"
#include "Scenes/MarioScene/Characters/MarioCharacter.h"
#include "Prefabs/Character.h"
#include "Materials/DiffuseMaterial.h"
#include <Materials/UberMaterial.h>

Star::Star(XMFLOAT3& startPos)
{
	m_StartPos = startPos;
}

void Star::Initialize(const SceneContext&)
{
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
	settings.rotationSpeed = m_RotationSpeed / 6.0f;

	settings.rotationAxis = { 0.0f, 1.0f, 0.0f };
	settings.rotationDisplacement = 0.0f;

	const auto pObject = AddChild(new GameObject);
	XMFLOAT3 starLocation{ m_StartPos.x, m_StartPos.y + 3, m_StartPos.z };
	m_pEmitter = pObject->AddComponent(new ParticleEmitterComponent(L"Textures/star.png", settings, 10, starLocation));
	m_pEmitter->GetTransform()->Translate(starLocation);
		

	auto& physx = PxGetPhysics();

	auto pPhysicsMaterial = physx.createMaterial(0.0f, 0.0f, 0.0f);

	ModelComponent* starMC{ new ModelComponent(L"Meshes/Star/Star.ovm", true, true) };
	m_pStar = new GameObject{};
	m_pStar->AddComponent<ModelComponent>(starMC);
	AddChild(m_pStar);

	auto pConvexComponent = ContentManager::Load<PxConvexMesh>(L"Meshes/Star/Star.ovpc");
	auto pRigidBody = m_pStar->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxConvexMeshGeometry{ pConvexComponent }, *pPhysicsMaterial, true);

	//Mario* pMarioCharacter

	m_pStar->SetOnTriggerCallBack([=](GameObject* /*pStar*/, GameObject* /*pObject*/, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER && !m_IsHit)
			{
				OnTrigger();
				MarioCharacter::m_NrStars++;
			}
		});

	UberMaterial* pEyes = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pEyes->SetDiffuseTexture(L"Textures/eye.png");
	pEyes->SetOpacityTexture(L"Textures/Star_HeightMap.jpg");
	pEyes->SetVariable_Scalar(L"gTextureOpacityIntensity", true);

	DiffuseMaterial* pBody = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBody->SetDiffuseTexture(L"Textures/body.png");

	starMC->SetMaterial(pEyes->GetMaterialId(), 0);
	starMC->SetMaterial(pBody->GetMaterialId(), 1);

	m_pStar->GetTransform()->Translate(m_StartPos.x, m_StartPos.y + 3, m_StartPos.z);


	//SoundManager::Get()->GetSystem()->createSound("Resources/Sound/star.wav", FMOD_LOOP_NORMAL | FMOD_UNIQUE, nullptr, &m_pSound);
	//SoundManager::Get()->GetSystem()->createSound("Resources/Sound/star.wav", FMOD_LOOP_NORMAL | FMOD_UNIQUE, nullptr, &m_pSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Sounds/star.wav", FMOD_INIT_NORMAL, nullptr, &m_pSound);

}

void Star::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();



	if (!m_IsHit)
	{
		m_Yaw += m_RotationSpeed * sceneContext.pGameTime->GetElapsed();
		m_pStar->GetTransform()->Rotate(0.0f, m_Yaw, 0.f);
	}
	if (m_IsHit)
	{

		m_Yaw += (m_RotationSpeed + 360) * sceneContext.pGameTime->GetElapsed();
		m_pStar->GetTransform()->Rotate(0.0f, m_Yaw, 0.f);
		m_Ypos.y += deltaTime;
		m_pStar->GetTransform()->Translate(m_pStar->GetTransform()->GetPosition().x, m_pStar->GetTransform()->GetPosition().y + m_Ypos.y, m_pStar->GetTransform()->GetPosition().z);
	}

	

	if (m_IsHit)
	{
		if (!m_IsPlayingSound)
		{
			SoundManager::Get()->GetSystem()->playSound(m_pSound, nullptr, false, nullptr);
			m_IsPlayingSound = true;
		}
		if (!m_IsDeactivated)
		{
			DeativateParticle(deltaTime);

		}
		if (!m_StarDestroyed && m_IsDeactivated)
		{
			m_StarDestroyed = true;
			GetScene()->RemoveChild(this, true);
		}
	}
}

void Star::OnTrigger()
{
		auto& settings = m_pEmitter->GetSettings();
		settings.velocity = { 0.f, 10.f,0.f };
		settings.minSize = 2.f;
		settings.maxSize = 2.f;
		settings.minScale = 1.5f;
		settings.maxScale = 3.0f;
		settings.minEmitterRadius = 2.0f;
		settings.maxEmitterRadius = 2.0f;
		settings.minEnergy = m_MaxTime;
		settings.maxEnergy = m_MaxTime;
		m_IsHit = true;
}

void Star::DeativateParticle(float dt)
{
	m_TimeSinceActive += dt;
	if (m_TimeSinceActive >= m_MaxTime)
	{
		auto& settings = m_pEmitter->GetSettings();
		settings.velocity = { 0.f,0.f,0.f };
		settings.minEnergy = 0.0f;
		settings.maxEnergy = 0.0f;
		settings.minSize = 0.0f;
		settings.maxSize = 0.0f;
		settings.minScale = 0.0f;
		settings.maxScale = 0.0f;

		m_IsDeactivated = true;

	}
}
