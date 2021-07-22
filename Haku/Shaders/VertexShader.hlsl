struct VertexData
{
	float2 pos : POSITION;
	float3 color : COLOR;
};
struct ColorData
{
	float4 RGBA : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer Rotation
{
	matrix RotationZ;
};

ColorData main(VertexData Data)
{
	ColorData Color;
	Color.pos = mul(float4(Data.pos, 0.0f, 1.0f), RotationZ);
	Color.RGBA = float4(Data.color,1.0f);
	return Color;
}