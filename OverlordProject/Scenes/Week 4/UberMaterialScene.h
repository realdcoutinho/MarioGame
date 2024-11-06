#pragma once
#include <Materials/UberMaterial.h>
class UberMaterialScene final : public GameScene
{
public:
	UberMaterialScene() : GameScene(L"UberMaterialScene") {}
	~UberMaterialScene() override = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;
private:

	void Initialize() override;
	void Update() override;
	void OnGUI() override;
	
	GameObject* m_pSphere{ nullptr };
	float m_Angle{ 0 };
	float m_Speed{ -25.0f };
	bool m_MaterialPopupWindowOpwn{ false };

	UberMaterial* m_pUberMaterial;
};