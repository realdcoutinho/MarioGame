#pragma once
#include <Materials/SpikyMaterial.h>
class SpikyScene final : public GameScene
{
public:
	SpikyScene() : GameScene(L"SpikyScene") {}
	~SpikyScene() override = default;

	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;
private:

	void Update() override;
	void Initialize() override;
	void OnGUI() override;


	GameObject* m_pSphere{ nullptr };
	SpikyMaterial* m_pMaterialSpiky{ nullptr };
	float m_Speed{ 20.0f };
	float m_Angle{ 0.0f };
};
