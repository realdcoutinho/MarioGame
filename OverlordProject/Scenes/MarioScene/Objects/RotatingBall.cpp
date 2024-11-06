#include "stdafx.h"
#include "RotatingBall.h"
#include "Scenes/MarioScene/Characters/MarioCharacter.h"

RotatingBall::RotatingBall(const XMFLOAT3& ballPos, const XMFLOAT3& ballTrigger) :
	m_StartingPos(ballPos),
	m_TriggerPos(ballTrigger)
{

}

void RotatingBall::Initialize(const SceneContext&)
{
	auto pBouncyMaterial = PxGetPhysics().createMaterial(1.0f, 1.0f, 1.0f);
	float radius = 3.0f;


	m_pBall = new SpherePrefab(radius, 10, XMFLOAT4{ Colors::Red });
	m_pBall->GetTransform()->Translate(m_StartingPos);
	AddChild(m_pBall);



	auto pRigidBody = m_pBall->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry{ radius }, *pBouncyMaterial);

	m_pTriggerObject = new GameObject();
	auto pTrigger = m_pTriggerObject->AddComponent(new RigidBodyComponent(true));
	pTrigger->AddCollider(PxSphereGeometry{ radius + 0.7f }, *pBouncyMaterial, true);
	AddChild(m_pTriggerObject);
}

void RotatingBall::Update(const SceneContext& sceneContext)
{
	if (!m_IsBallDestroyed)
	{
		//XMFLOAT3 pos = m_pBall->GetTransform()->GetPosition();

		//m_pTriggerObject->GetTransform()->Translate(pos.x, pos.y, pos.z);

		if (m_Killed)
		{
			m_TimeSinceKill += sceneContext.pGameTime->GetElapsed();
			if (m_TimeSinceKill > 4.0f)
			{
				GetScene()->RemoveChild(this, true);
				m_IsBallDestroyed = true;
			}
		}
	}

}


void RotatingBall::OnTrigger()
{
	if (!m_Killed)
	{
		MarioCharacter::m_NrLives--;
		m_Killed = true;
	}

}


XMFLOAT3 RotatingBall::ReturnBallPosition()
{
	if (m_pBall)
		return  m_pBall->GetTransform()->GetPosition();
	else
		return XMFLOAT3{ 0.0f, 0.0f, 0.0f };
}