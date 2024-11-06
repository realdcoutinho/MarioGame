
class Aberration : public PostProcessingMaterial
{
public:
	Aberration();
	~Aberration() override = default;
	Aberration(const Aberration& other) = delete;
	Aberration(Aberration&& other) noexcept = delete;
	Aberration& operator=(const Aberration& other) = delete;
	Aberration& operator=(Aberration&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

