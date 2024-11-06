#pragma once
class ModelTestScene final : public GameScene
{
public:
	ModelTestScene() : GameScene(L"ModelTestScene") {}
	~ModelTestScene() override = default;

	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;
private:

	void Initialize() override;
	GameObject* m_pChair{ nullptr };
};
