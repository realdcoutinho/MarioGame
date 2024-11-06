#pragma once
#include <Materials/UberMaterial.h>
class Star : public GameObject
{
public:
	Star(XMFLOAT3& startPos);
	~Star() override = default;

	Star(const Star& other) = delete;
	Star(Star&& other) noexcept = delete;
	Star& operator=(const Star& other) = delete;
	Star& operator=(Star&& other) noexcept = delete;
	bool GetMarkedForDelete() { return m_StarDestroyed; };



	//void GuiCall();
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;



private:
	void OnTrigger();
	void DeativateParticle(float dt);


	XMFLOAT3 m_StartPos{};
	ParticleEmitterComponent* m_pEmitter{};
	UberMaterial* m_pEyes{};
	GameObject* m_pStar{};
	
	XMFLOAT3 m_Ypos{ 0.0f, 0.0f, 0.0f };

	bool m_IsDeactivated{ false };
	bool m_IsHit{ false };
	bool m_StarDestroyed{ false };
	float m_RotationSpeed{ 180.0f };
	float m_Yaw{ 0.0f };
	bool m_IsPlayingSound{ false };

	float m_MaxTime{ 1.3f };
	float m_TimeSinceActive{ 0.0f };
	FMOD::Sound* m_pSound{ nullptr };
};

