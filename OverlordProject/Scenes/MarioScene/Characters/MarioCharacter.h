#pragma once
#include "Prefabs/Character.h"

class MarioCharacter : public Character
{

public:
	MarioCharacter(const CharacterDesc& characterDesc, XMFLOAT3 startLocation);
	~MarioCharacter();

	MarioCharacter(const MarioCharacter& other) = delete;
	MarioCharacter(MarioCharacter&& other) noexcept = delete;
	MarioCharacter& operator=(const MarioCharacter& other) = delete;
	MarioCharacter& operator=(MarioCharacter&& other) noexcept = delete;

	void DrawImGui();
	void AddStar();
	XMFLOAT3 GetCharacterLocation() { return m_pControllerComponent->GetTransform()->GetPosition(); };
	void Respawn(const SceneContext& sceneContext);

	static int m_NrStars;
	static int m_NrLives;

	bool m_IsDead{ false };



protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;


private:
	ControllerComponent* m_pControllerComponent{};

	GameObject* m_pModelContainer{ nullptr };

	CameraComponent* m_pCameraComponent{};
	FixedCamera* m_pCamera{};

	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)

	ModelAnimator* pAnimator{};
	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };

	char** m_ClipNames{};
	UINT m_ClipCount{};

	GameObject* m_pCameraHolder{ nullptr };




	ParticleEmitterComponent* m_pEmitter{};

	XMFLOAT3 m_StartLocation{};
	int m_CurrLives{ 3 };
	float m_TimeToRespawn{ 3.5f };
	float m_TimeSinceDead{ 0.0f };

	enum class State {
		idle,
		running,
		jumping,
		dead,
		respawn
	};

	State m_State = State::idle;

	FMOD::Sound* m_pDeadSound{ nullptr };
	FMOD::Sound* m_pJumpSound{ nullptr };



};


