#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"


SoftwareSkinningScene_1::SoftwareSkinningScene_1() : GameScene(L"SoftwareSkinningScene_1")
{
}

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	//////ColorMaterial
	auto materialColor = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	materialColor->SetColor(XMFLOAT4{ Colors::Red });
	GameObject* pRoot = new GameObject{};
	AddChild(pRoot);

	


	m_pBone0 = new BoneObject{ materialColor, 15.0f };
	pRoot->AddChild(m_pBone0);
	m_pBone1 = new BoneObject{ materialColor, 15.0f };
	m_pBone0->AddBone(m_pBone1);

	m_pBone0->CalculateBindPose();
	//m_pBone0->GetBindPose();
}

void SoftwareSkinningScene_1::Update()
{
	if (m_IsAutoRotate)
	{
		m_BoneRotation += m_RotSpeed * m_SceneContext.pGameTime->GetElapsed() * m_RotationSign;
		if (m_BoneRotation > 45.0f)
		{
			m_RotationSign *= -1;
		}
		if (m_BoneRotation < -45.0f)
		{
			m_RotationSign *= -1;
		}
		m_pBone0->GetTransform()->Rotate(m_Bone0Rot.x, m_Bone0Rot.y, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(m_Bone1Rot.x, m_Bone1Rot.y, -m_BoneRotation * 2.0f);

		m_Bone0Rot.z = m_BoneRotation;
		m_Bone1Rot.z = -m_BoneRotation * 2.0f;
	}
	if (!m_IsAutoRotate)
	{
		m_pBone0->GetTransform()->Rotate(m_Bone0Rot.x, m_Bone0Rot.y, m_Bone0Rot.z);
		m_pBone1->GetTransform()->Rotate(m_Bone1Rot.x, m_Bone1Rot.y, m_Bone1Rot.z);
	}
}

void SoftwareSkinningScene_1::OnGUI()
{
	const std::string buttonText{ "Rotation Parameters" };
	if (ImGui::Button(buttonText.c_str()))
	{
		m_IsWindowOpen = true;
	}
	if (m_IsWindowOpen)
	{
		if (ImGui::Begin("Dear ImGui Demo", &m_IsWindowOpen))
		{
			ImGui::Checkbox("Auto Rotate", &m_IsAutoRotate);

			ImGui::DragFloat3("Bone 0 - ROT", ConvertUtil::ToImFloatPtr(m_Bone0Rot), 1.f);
			ImGui::DragFloat3("Bone 1 - ROT", ConvertUtil::ToImFloatPtr(m_Bone1Rot), 1.f);


			ImGui::End();
			return;
		}
	}
}