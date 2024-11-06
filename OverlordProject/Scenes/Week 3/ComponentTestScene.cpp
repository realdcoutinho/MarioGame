#include "stdafx.h"
#include "ComponentTestScene.h"
#include "Prefabs/SpherePrefab.h"

void ComponentTestScene::Initialize()
{
	auto& physx = PxGetPhysics();

	auto pBouncyMaterial = physx.createMaterial(0.5f, 0.5f, 1.0f);

	//Ground Plane 
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	//RED SPHERE (GROUP 0)
	const auto pSphereRed = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Red });
	AddChild(pSphereRed);

	pSphereRed->GetTransform()->Translate(0.f, 30.f, 0.f);
	auto pRigidBody = pSphereRed->AddComponent(new RigidBodyComponent(false));

	pRigidBody->AddCollider(PxSphereGeometry{ 1.0f }, *pBouncyMaterial);
	
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	pRigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);
	//pRigidBody->SetConstraint()


	{
	//	auto& phys = PxGetPhysics();
	//	auto pBouncyMaterial = phys.createMaterial(.5f, .5f, 1.f);

	//	//ground plane
	//	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	//	//Red sphere
	//	const auto pSphereRed = new SpherePrefab(1, 10, XMFLOAT4(Colors::Red));
	//	AddChild(pSphereRed);

	//	pSphereRed->GetTransform()->Translate(0, 30, 0);

	//	auto pRigidBody = pSphereRed->AddComponent(new RigidBodyComponent(false));
	//	pRigidBody->AddCollider(PxSphereGeometry(1.f), *pBouncyMaterial);
	//	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	//	pRigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);
	}



	//gREEN SPHERE (GROUP 1)
	const auto pSphereGreen = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Green });
	AddChild(pSphereGreen);

	pSphereGreen->GetTransform()->Translate(0.f, 20.f, 0.f);
	pRigidBody = pSphereGreen->AddComponent(new RigidBodyComponent(false));

	pRigidBody->AddCollider(PxSphereGeometry(1.0f), *pBouncyMaterial);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);




	//Blue SPHERE (GROUP 2)
	const auto pSphereBlue = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Blue });
	AddChild(pSphereBlue);

	pSphereBlue->GetTransform()->Translate(0.f, 10.f, 0.f);
	pRigidBody = pSphereBlue->AddComponent(new RigidBodyComponent(false));

	pRigidBody->AddCollider(PxSphereGeometry(1.0f), *pBouncyMaterial);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group2);
}
