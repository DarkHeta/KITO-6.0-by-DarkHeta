/////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////

struct vertexInput
{ 
	float4				iPosition			: POSITION;
	float2				iTexCoord			: TEXCOORD0;	
};

struct vertexOutput 
{ 
	float4				oPosition			: POSITION;
	float2				oTexCoord			: TEXCOORD0;
};

vertexOutput main_vp(	vertexInput			IN,
						uniform float4x4	worldViewProj
					 )
{
	vertexOutput OUT;

	OUT.oPosition = mul(worldViewProj, IN.iPosition);
	OUT.oTexCoord = IN.iTexCoord;
	return OUT;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////

float4 main_fp(	vertexOutput		IN,
				uniform sampler2D	normal_scene				: register(s0),
				uniform sampler2D	volumetric_light_scene		: register(s1)
			  )	:	COLOR
{
	float3 color = tex2D(normal_scene, IN.oTexCoord).rgb;
	float3 color2 = tex2D(volumetric_light_scene, IN.oTexCoord).rgb;
	return float4(color + color2, 1.0);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////