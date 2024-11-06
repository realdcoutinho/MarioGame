
#pragma once
class DiffuseMaterials_FlipNormals final : public Material<DiffuseMaterials_FlipNormals>
{
public:
	DiffuseMaterials_FlipNormals();
	~DiffuseMaterials_FlipNormals() override = default;

	DiffuseMaterials_FlipNormals(const DiffuseMaterials_FlipNormals& other) = delete;
	DiffuseMaterials_FlipNormals(DiffuseMaterials_FlipNormals&& other) noexcept = delete;
	DiffuseMaterials_FlipNormals& operator=(const DiffuseMaterials_FlipNormals& other) = delete;
	DiffuseMaterials_FlipNormals& operator=(DiffuseMaterials_FlipNormals&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void FlipNormals();
	void InitializeEffectVariables() override;



private:
	TextureData* m_pDiffuseTexture{ nullptr };
};

