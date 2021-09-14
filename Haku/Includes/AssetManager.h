#pragma once
#include "Graphics.h"
#include "Model.h"
#include "Lighting.h"
#include <vector>

class AssetManager
{
public:
	AssetManager();
	void AddLights();
	void Draw() noexcept;
	AssetManager(Graphics* GFX);
	void ReadModel(const std::string& path);
	void SetGraphics(Graphics* Pointer) noexcept;
	void UpdateTestData(ConstVertexModifer& Ref) noexcept;

private:
	Graphics*								   GFX = nullptr;
	ConstVertexModifer						   TestData;
	std::vector<Model>						   EntityVector;
	std::unique_ptr<Haku::Lights>			   LightPointer;
	std::shared_ptr<Haku::IndexBuffer>		   LightModelIndexBuffer;
	std::shared_ptr<Haku::VertexBuffer<Point>> LightModelVertexBuffer;
};
