struct ColorData
{
	float4 RGBA : COLOR;
	float4 pos : SV_POSITION;
};

float4 main(ColorData Data ) : SV_Target
{
	 return Data.RGBA;
}