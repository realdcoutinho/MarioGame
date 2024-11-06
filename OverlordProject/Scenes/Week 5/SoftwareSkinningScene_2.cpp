#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"


SoftwareSkinningScene_2::SoftwareSkinningScene_2() : GameScene(L"SoftwareSkinningScene_2")
{
}

void SoftwareSkinningScene_2::Initialize()
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

void SoftwareSkinningScene_2::Update()
{
	CalculateRotation();
	BoneTransformations();
	UpdateVertices();
}

void SoftwareSkinningScene_2::CalculateRotation()
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

void SoftwareSkinningScene_2::BoneTransformations()
{
	auto bone0BindPose = m_pBone0->GetBindPose();
	auto bone0WorldMatrix = m_pBone0->GetTransform()->GetWorld();


	// ... Initialize matrix1 and matrix2 ...

	// Convert the matrices to XMMatrix format
	XMMATRIX xmMatrix1 = XMLoadFloat4x4(&bone0BindPose);
	XMMATRIX xmMatrix2 = XMLoadFloat4x4(&bone0WorldMatrix);

	// Declare an output matrix
	XMFLOAT4X4 resultMatrix;

	// Multiply matrix1 and matrix2 and store the result in resultMatrix
	XMMATRIX xmResultMatrix = XMMatrixMultiply(xmMatrix1, xmMatrix2);

	// Convert the result back to XMFLOAT4X4 format
	XMStoreFloat4x4(&resultMatrix, xmResultMatrix);




	auto bone1BindPose = m_pBone1->GetBindPose();
	auto bone1WorldMatrix = m_pBone1->GetTransform()->GetWorld();


	// ... Initialize matrix1 and matrix2 ...

	// Convert the matrices to XMMatrix format
	XMMATRIX xmMatrix3 = XMLoadFloat4x4(&bone1BindPose);
	XMMATRIX xmMatrix4 = XMLoadFloat4x4(&bone1WorldMatrix);

	// Declare an output matrix
	XMFLOAT4X4 resultMatrix1;

	// Multiply matrix1 and matrix2 and store the result in resultMatrix
	XMMATRIX xmResultMatrix1 = XMMatrixMultiply(xmMatrix3, xmMatrix4);

	// Convert the result back to XMFLOAT4X4 format
	XMStoreFloat4x4(&resultMatrix1, xmResultMatrix1);


	for (size_t pos{ 0 }; pos < m_SkinnedVertices.size(); ++pos)
	{
		XMFLOAT3 float3;
		XMVECTOR position{ m_SkinnedVertices[pos].originalVertex.Position.x, m_SkinnedVertices[pos].originalVertex.Position.y, m_SkinnedVertices[pos].originalVertex.Position.z };

		XMVECTOR newPosition;
		if (pos < 24)
		{
			newPosition = XMVector3TransformCoord(position, xmResultMatrix);
		}
		else
		{
			newPosition = XMVector3TransformCoord(position, xmResultMatrix1);
		}
		float3.x = XMVectorGetX(newPosition);
		float3.y = XMVectorGetY(newPosition);
		float3.z = XMVectorGetZ(newPosition);

		//XMFLOAT3 newPos = { newPosition };
		m_SkinnedVertices[pos].transformedVertex.Position = float3;
	}
}

void SoftwareSkinningScene_2::UpdateVertices()
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


void SoftwareSkinningScene_2::OnGUI()
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









void SoftwareSkinningScene_2::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion
}