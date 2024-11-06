
#pragma once

struct Button
{
	float width{};
	float height{};
	XMFLOAT2 center{};

	int id{};

	bool Clicked(XMFLOAT2 mousePos) 
	{
		float left = center.x - (width / 2);
		float right = center.x + (width / 2);
		float top = center.y + (height / 2);
		float bottom = center.y - (height / 2);

		return (mousePos.x >= left && mousePos.x <= right && mousePos.y >= bottom && mousePos.y <= top);
	}
};

class MainMenuScene : public GameScene
{
public:
	MainMenuScene() :GameScene(L"MarioMainMenuScene") {}
	~MainMenuScene() override = default;
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;

private:

	bool m_IsControlsActive{ false };

	XMFLOAT2 m_MousePos{};
	GameObject* m_pBackgroundSprite{};
	GameObject* m_pButtonQuestion{};
	GameObject* m_pControlWindow{};
	GameObject* m_pButtonStart{};

	Button m_ControlsButton{};
	Button m_StartButton{};
	Button m_ExitGameButton{};

};

