
#pragma once
class W3_PongScene final : public GameScene
{
public:
	W3_PongScene() : GameScene(L"PongGame") {}
	~W3_PongScene() override = default;

	W3_PongScene(const W3_PongScene& other) = delete;
	W3_PongScene(W3_PongScene&& other) noexcept = delete;
	W3_PongScene& operator=(const W3_PongScene& other) = delete;
	W3_PongScene& operator=(W3_PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;

	PxPhysics& m_Physx{ PxGetPhysics() };


	void InitializePeddles();
	GameObject* m_pLeftPeddle{ nullptr };
	XMFLOAT3 m_pLeftPos{};
	RigidBodyComponent* m_pLeftTrigger{ nullptr };
	GameObject* m_pRightPeddle{ nullptr };
	XMFLOAT3 m_pRightPos{};
	RigidBodyComponent* m_pRightTrigger{ nullptr };

	void PeddleMovement();
	float m_Speed{ 15.0f };
	float m_Size{ 8 };

	void InitializeBall();
	GameObject* m_pBall{ nullptr };

	void InitializeBorders();
	RigidBodyComponent* m_pTopWall{ nullptr };
	RigidBodyComponent* m_pBottomWall{ nullptr };
	RigidBodyComponent* m_pLeftWall{ nullptr };
	RigidBodyComponent* m_pRightWall{ nullptr };
	bool m_OutOfBounds{ false };


	void BallMovement();
	XMFLOAT3 m_BallSpeed{};

	virtual void OnSceneActivated() override;

	void Reset();
	bool m_GameStarted{ false };


	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pSound{};
	void InitializeSounds();
	void PlaySounds();

};

