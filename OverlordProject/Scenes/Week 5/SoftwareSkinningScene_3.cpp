#include "stdafx.h"
#include "SoftwareSkinningScene_3.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"


SoftwareSkinningScene_3::SoftwareSkinningScene_3() : GameScene(L"SoftwareSkinningScene_3")
{
}

void SoftwareSkinningScene_3::Initialize()
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

	GameObject* pBoxDrawer = new GameObject{};
	m_pMeshDrawer = new MeshDrawComponent(24, true);
	pBoxDrawer->AddComponent<MeshDrawComponent>(m_pMeshDrawer);
	AddChild(pBoxDrawer);


	InitializeVertices(15.0f);
}

void SoftwareSkinningScene_3::Update()
{
	CalculateRotation();
	BoneTransformations();
	UpdateVertices();
}

void SoftwareSkinningScene_3::CalculateRotation()
{
	if (m_IsAutoRotate)
	{
		m_BoneRotation += m_RotSpeed * m_SceneContext.pGameTime->GetElapsed() * m_RotationSign;
		if (m_BoneRotation > maxAngle)
		{
			m_RotationSign *= -1;
		}
		if (m_BoneRotation < -maxAngle)
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

void SoftwareSkinningScene_3::BoneTransformations()
{
	XMFLOAT4X4 bone0BindPose{ m_pBone0->GetBindPose() };
	XMFLOAT4X4 bone0World{ m_pBone0->GetTransform()->GetWorld() };
	XMMATRIX bind0{ XMLoadFloat4x4(&bone0BindPose) };
	XMMATRIX world0{ XMLoadFloat4x4(&bone0World) };

	XMFLOAT4X4 bone1BindPose{ m_pBone1->GetBindPose() };
	XMFLOAT4X4 bone1World{ m_pBone1->GetTransform()->GetWorld() };
	XMMATRIX bind1{ XMLoadFloat4x4(&bone1BindPose) };
	XMMATRIX world1{ XMLoadFloat4x4(&bone1World) };

	XMMATRIX xmResultMatrix0 = XMMatrixMultiply(bind0, world0);
	XMMATRIX xmResultMatrix1 = XMMatrixMultiply(bind1, world1);


	for (size_t i{ 0 }; i < m_SkinnedVertices.size(); ++i)
	{
		auto vertex{ m_SkinnedVertices[i] };
		XMVECTOR originalPos{ XMLoadFloat3(&vertex.originalVertex.Position) };

		if (m_SkinnedVertices[i].blendWeight0 < 1.0f || m_SkinnedVertices[i].blendWeight1 < 1.0f)
		{
			auto transformedCoord{ XMVector3TransformCoord(originalPos, xmResultMatrix0) };

			auto transformedCoord2{ XMVector3TransformCoord(originalPos, xmResultMatrix1) };



			XMFLOAT3 pos{};
			XMFLOAT3 pos2{};
			XMStoreFloat3(&pos, transformedCoord);
			XMStoreFloat3(&pos2, transformedCoord2);

			XMVECTOR blend1{ XMLoadFloat3(&pos) * vertex.blendWeight0 };
			XMVECTOR blend2{ XMLoadFloat3(&pos2) * vertex.blendWeight1 };
			auto blendTotal{ blend1 + blend2 };


			XMFLOAT3 blendResult{};
			XMStoreFloat3(&blendResult, blendTotal);

			m_SkinnedVertices[i].transformedVertex.Position = blendResult;
			continue;
		}
		XMVECTOR newPosition;
		XMFLOAT3 pos;
		if (i < (m_SkinnedVertices.size()/2))
		{
			newPosition = XMVector3TransformCoord(originalPos, XMMatrixMultiply(bind0, world0));
		}
		else
		{
			newPosition = XMVector3TransformCoord(originalPos, XMMatrixMultiply(bind1, world1));
		}

		XMStoreFloat3(&pos, newPosition);


		m_SkinnedVertices[i].transformedVertex.Position = pos;

	}
}

void SoftwareSkinningScene_3::UpdateVertices()
{
	m_pMeshDrawer->RemoveTriangles();
	for (size_t pos{ 0 }; pos < m_SkinnedVertices.size(); pos += 4)
	{
		VertexPosNormCol vert1 = m_SkinnedVertices[pos].transformedVertex;
		VertexPosNormCol vert2 = m_SkinnedVertices[pos + 1].transformedVertex;
		VertexPosNormCol vert3 = m_SkinnedVertices[pos + 2].transformedVertex;
		VertexPosNormCol vert4 = m_SkinnedVertices[pos + 3].transformedVertex;
		QuadPosNormCol quad{ vert1 , vert2, vert3, vert4 };
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();
}


void SoftwareSkinningScene_3::OnGUI()
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









void SoftwareSkinningScene_3::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.0f, 0.0f); //top left
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //top right //this point
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //bottom right //this point
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.0f, 0.0f); //bottom left
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x  +pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); // top right //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.0f, 0.0f); //top left
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.0f, 0.0f); //bottom left
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //bottom right //this one
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x  + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.0f, 0.0f); //far left
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);// far right //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //close right //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x  +pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.0f, 0.0f); //close left
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.0f, 0.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x +pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); // this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.0f, 0.0f);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.0f, 0.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.0f, 0.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.0f, 0.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.0f, 0.0f);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); // thisd one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x  + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);//this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f); //this one
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f); // this one
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.0f, 1.0f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.0f, 1.0f);
#pragma endregion
}