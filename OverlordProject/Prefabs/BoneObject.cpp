#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float lenght) :
	m_pMaterial{pMaterial},
	m_Length{lenght}
{
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject{};
	AddChild(pEmpty);
	ModelComponent* pModel{ new ModelComponent{L"Meshes/Bone.ovm"} };
	pModel->SetMaterial(m_pMaterial);
	pEmpty->AddComponent(pModel);

	pEmpty->GetTransform()->Rotate(0.0f, -90.0f, 0.0f, true);
	XMFLOAT3 scale{ m_Length, m_Length, m_Length };
	pEmpty->GetTransform()->Scale(scale);
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	const auto worldMatrix{ this->GetTransform()->GetWorld() };
	auto determinant{ XMVECTOR{0.0f, 0.0f, 0.0f} };

	XMFLOAT4X4 worldMatrixInverse;
	auto matrixInverse{ XMMatrixInverse(&determinant, DirectX::XMLoadFloat4x4(&worldMatrix)) };

	DirectX::XMStoreFloat4x4(&worldMatrixInverse, matrixInverse);
	 
	m_BindPose = worldMatrixInverse;

	auto children = GetChild<BoneObject>();
	if (children)
	{
		children->CalculateBindPose();
	}
}