#pragma once
#include "Scenes/MarioScene/Characters/MarioCharacter.h"
class HUD : public GameObject
{

public:
	HUD();
	~HUD() override = default;

	HUD(const HUD& other) = delete;
	HUD(HUD&& other) noexcept = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD& operator=(HUD&& other) noexcept = delete;
private:

	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;


	int m_NrLives{ 0 };
	int m_NrStars{ 0 };

	GameObject* m_pSpriteMarioLives{};
	GameObject* m_pSpriteMarioStars{};


	SpriteFont* m_pFontLives{};
	SpriteFont* m_pFontStars{};

	std::string m_Text{ "Hello World! (Change Text/Position/Color in Scene Parameters)" };
	XMFLOAT2 m_TextPosition1{ 95, 40};
	XMFLOAT2 m_TextPosition2{ 1130, 40 };
	XMFLOAT4 m_TextColor{ 1.f,1.f,1.f,1.f };

};

