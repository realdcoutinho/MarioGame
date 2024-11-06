#pragma once
#include "Scenes/MarioScene/Enemies/NPC.h"

//class NPC;

class Goomba : public NPC
{
public:
	Goomba(XMFLOAT3& startPos);
	Goomba(std::vector<XMFLOAT3>& startPosVector);


	//~Goomba() override = default;

	Goomba(const Goomba& other) = delete;
	Goomba(Goomba&& other) noexcept = delete;
	Goomba& operator=(const Goomba& other) = delete;
	Goomba& operator=(Goomba&& other) noexcept = delete;

	void CreateMultiple();
	void OnTriggerBottom();
	void OnTriggerTop();
	void OnTriggerArea();

protected:
	void Initialize(const SceneContext& ) override;
	void Update(const SceneContext& sceneContext) override;
	//void Movement();
	//void Killed();

private:


	GameObject* m_pGomba{ nullptr };
	GameObject* m_pGombaBottomCollider{ nullptr };
	GameObject* m_pGombaTopCollider{ nullptr };
	ParticleEmitterComponent* m_pEmitter{};
	GameObject* m_pMarioTrigger{ nullptr };

	bool m_IsKilled{ false };
	float m_TimeSinceDeath{ 0.0f };
	float m_MaxTime{ 2.0f };
	bool m_IsDeleted{ false };

	float m_TimeSinceKill{ 0.0f };
	bool m_KilledMario{ false };


	//FMOD::Sound* m_pSound{ nullptr };


	//bool m_IsPlayingSound{ false };


	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	//XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	//XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)
};

