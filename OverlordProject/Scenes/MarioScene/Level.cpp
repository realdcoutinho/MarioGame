#include "stdafx.h"
#include "Level.h"

#include "Materials/DiffuseMaterial.h"
#include "Materials/ColorMaterial.h"

void Level::Initialize()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);


	//Simple Level
	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Field.ovm"));
	pLevelMesh->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());
	pLevelMesh->GetTransform()->Scale({ 20, 20, 20 });
	pLevelMesh->GetTransform()->Rotate(0, 0, 0);


	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Field.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ 20, 20, 20 })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale({ 20, 20, 20 });
}
