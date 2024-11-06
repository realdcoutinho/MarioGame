#pragma once
class Level : public GameScene
{


	public:
		Level() :GameScene(L"Level Field") {}
		~Level() override = default;
		//Level(const Level& other) = delete;
		//Level(Level&& other) noexcept = delete;
		//Level& operator=(const Level& other) = delete;
		//Level& operator=(Level&& other) noexcept = delete;

		void Initialize() override;

	protected:

		//void OnGUI() override;

	private:

		void CreateLevel();
};

