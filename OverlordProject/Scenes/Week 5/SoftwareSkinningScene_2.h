#pragma once

class BoneObject;
class SoftwareSkinningScene_2 :
	public GameScene
{
public:
	SoftwareSkinningScene_2();
	~SoftwareSkinningScene_2() override = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{};
	BoneObject* m_pBone1{};

	float m_BoneRotation{ 0 };
	int m_RotationSign{ 1 };

	bool m_IsAutoRotate{ false };
	bool m_IsWindowOpen{ false };

	float m_RotSpeed{ 45.0f };
	XMFLOAT3 m_Bone0Rot{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_Bone1Rot{ 0.0f, 0.0f, 0.0f };

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color) :
			transformedVertex{ position, normal, color },
			originalVertex{ position, normal, color } {}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
	};

	void InitializeVertices(float lenght);
	void CalculateRotation();
	void BoneTransformations();
	void UpdateVertices();


	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};
