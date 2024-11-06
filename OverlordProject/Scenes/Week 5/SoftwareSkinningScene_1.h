#pragma once

#pragma once

class BoneObject;
class SoftwareSkinningScene_1 :
	public GameScene
{
public:
	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{};
	BoneObject* m_pBone1{};

	float m_BoneRotation{0};
	int m_RotationSign{ 1 };

	bool m_IsAutoRotate{ true };
	bool m_IsWindowOpen{ false };

	float m_RotSpeed{ 45.0f };
	XMFLOAT3 m_Bone0Rot{0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_Bone1Rot{ 0.0f, 0.0f, 0.0f };
};
