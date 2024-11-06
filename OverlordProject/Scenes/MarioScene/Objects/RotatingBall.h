#pragma once
#include "Prefabs/SpherePrefab.h"

class RotatingBall : public GameObject
{
public:
	RotatingBall(const XMFLOAT3& ballPos, const XMFLOAT3& ballTrigger);
	~RotatingBall() override = default;

	RotatingBall(const RotatingBall& other) = delete;
	RotatingBall(RotatingBall&& other) noexcept = delete;
	RotatingBall& operator=(const RotatingBall& other) = delete;
	RotatingBall& operator=(RotatingBall&& other) noexcept = delete;

	XMFLOAT3 ReturnBallPosition();
	float GetBallRadius() { return m_Radius; };
	bool GetIsBallDestroyed() { return m_IsBallDestroyed; };
	void OnTrigger();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;

private:
	XMFLOAT3 m_StartingPos{};
	XMFLOAT3 m_TriggerPos{};
	GameObject* m_pTriggerObject{};
	GameObject* m_pTriggerMovement{};
	bool m_IsBallDestroyed{ false };
	float m_TimeSinceKill{};
	bool m_Killed{ false };
	float m_Radius{3.0f};
	int m_Skip{ 3 };

	SpherePrefab* m_pBall{nullptr};

};

