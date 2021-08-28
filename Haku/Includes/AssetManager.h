#pragma once
#include "Graphics.h"
#include "Model.h"
#include <vector>

class AssetManager
{
public:
	AssetManager();
	AssetManager(Graphics* GFX);
	void SetGraphics(Graphics* Pointer) noexcept;
	void ReadModel(const std::string& path);
	void Draw() noexcept;

	void UpdateTestData(ConstVertexModifer& Ref) noexcept;

private:
	Graphics*		   GFX = nullptr;
	std::vector<Model> EntityVector;

	ConstVertexModifer TestData;
};
