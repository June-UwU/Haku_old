struct VertexData
{
	float4 pos : POSITION;
	float4 color : COLOR;
};
struct ColorData
{
	float4 RGBA;
};

ColorData main(VertexData Data)
{
	ColorData Color = Data.color;
	return Color;
}