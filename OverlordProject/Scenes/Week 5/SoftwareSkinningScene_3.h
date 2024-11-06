
class BoneObject;
class SoftwareSkinningScene_3 :
	public GameScene
{
public:
	SoftwareSkinningScene_3();
	~SoftwareSkinningScene_3() override = default;

	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{};
	BoneObject* m_pBone1{};

	float m_BoneRotation{ 0 };
	int m_RotationSign{ 1 };
	float maxAngle{ 45.0f };

	bool m_IsAutoRotate{ false };
	bool m_IsWindowOpen{ false };

	float m_RotSpeed{ 45.0f };
	XMFLOAT3 m_Bone0Rot{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_Bone1Rot{ 0.0f, 0.0f, 0.0f };

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float bWeight0, float bWeight1) :
			transformedVertex{ position, normal, color },
			originalVertex{ position, normal, color },
			blendWeight0{ bWeight0 },
			blendWeight1{ bWeight1 } {}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};

		float blendWeight0{};
		float blendWeight1{};
	};

	void InitializeVertices(float lenght);
	void CalculateRotation();
	void BoneTransformations();
	void UpdateVertices();


	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};
