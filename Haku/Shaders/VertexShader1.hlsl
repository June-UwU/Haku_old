struct VertexData
{
	float3 pos : POSITION;
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
	Color.pos = mul(float4(Data.pos,1.0f), RotationZ);
	Color.RGBA = float4(1.0f,1.0f,1.0f,1.0f);
	return Color;
}