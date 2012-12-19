
struct Light {
	float3 pos;				
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

struct Material {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float shininess;
};

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

float4x4 gWorldMatrix;
float4x4 gViewProjectionMatrix;
float4x4 gWorldInverseTransposeMatrix;
float3 gCameraPos;
float4 gGlobalAmbient;

Light gLight;
Material gMaterial;

//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------

texture gColorMapTexture;

sampler2D colorMap = sampler_state
{
	Texture = <gColorMapTexture>;
    MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP; 
    AddressV  = WRAP;
};

//-----------------------------------------------------------------------------
// Vertex Shaders.
//-----------------------------------------------------------------------------

struct VS_INPUT {
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT_DIR {
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color: COLOR0;
};

VS_OUTPUT_DIR VS_DirLighting(VS_INPUT IN) {
	VS_OUTPUT_DIR OUT = (VS_OUTPUT_DIR)0;

	float4x4 worldViewProjection = mul(gWorldMatrix, gViewProjectionMatrix);
	OUT.position = mul(float4(IN.position, 1.0f), worldViewProjection);
	
	//OUT.texCoord = IN.texCoord;

	float4 worldPos = mul(float4(IN.position, 1.0f), gWorldMatrix);
	float3 lightVec = normalize(gLight.pos - worldPos);
	float4 normal = normalize(mul(IN.normal, gWorldInverseTransposeMatrix));
	float4 diffuseColor = max(0, dot(normal, lightVec)) * gLight.diffuse;
	
	//diffuseColor = max(0, diffuseColor);

	OUT.color.rgb = diffuseColor + gLight.ambient;
	OUT.color.a = diffuseColor.a;

	return OUT;
}

//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float4 PS_DirLighting(VS_OUTPUT_DIR IN) : COLOR {
	return IN.color;
}

technique NormalMappingDirectionalLighting {
	pass {
		LIGHTING = TRUE;
		FILLMODE = SOLID;
		VertexShader = compile vs_2_0 VS_DirLighting();
		PixelShader = compile ps_2_0 PS_DirLighting();
	}
}

technique NormalMappingDirectionalLightingWireframe {
	pass {
		FILLMODE = WIREFRAME;
		VertexShader = compile vs_2_0 VS_DirLighting();
		PixelShader = compile ps_2_0 PS_DirLighting();
	}
}
