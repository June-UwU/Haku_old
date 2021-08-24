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
	void ReadModel(std::string& path, float ThetaZ, float translation);
	void Draw() noexcept;
private:
	Graphics* GFX = nullptr;
	std::vector<Model> EntityVector;
};
