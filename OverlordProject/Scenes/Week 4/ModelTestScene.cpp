#include "stdafx.h"
#include "ModelTestScene.h"
#include <Materials/ColorMaterial.h>
#include <Materials/DiffuseMaterial.h>


#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include <functional>

void ModelTestScene::Initialize()
{
	auto& physx = PxGetPhysics();

	auto pBouncyMaterial = physx.createMaterial(0.0f, 0.0f, 0.0f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	auto materialManager = MaterialManager::Get();

	//DiffuseMaterial
	auto materialDiffuse = materialManager->CreateMaterial<DiffuseMaterial>();
	materialDiffuse->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	auto materialDiffuseID = materialDiffuse->GetMaterialId();


	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Chair.ovm"} };
	m_pChair = new GameObject{};
	m_pChair->AddComponent<ModelComponent>(modelComponent);
	AddChild(m_pChair);

	auto pConvexChair = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	auto pRigidBody = m_pChair->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxConvexMeshGeometry{ pConvexChair }, *pBouncyMaterial);

	m_pChair->GetTransform()->Translate(XMVECTOR{ 0.f,10.f,0.f });

	//ColorMaterial
	//modelComponent->SetMaterial(marterialColorID);

	//DiffuseMaterial
	modelComponent->SetMaterial(materialDiffuseID);
}