struct Material {
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float specPower;
};

struct SpotLight {
	float3 posW;
	float3 dirW;  
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float spotPower;
};

static const float SHADOW_EPSILON = 0.00005f;
static const float SMAP_SIZE = 1024.0f;

uniform extern float4x4 gLightVP;
uniform extern float4x4 gWorld;
uniform extern float4x4  gVP;
uniform extern Material  gMaterial;
uniform extern SpotLight gLight;
uniform extern float3 gEyePosW;
uniform extern texture gTex;
uniform extern texture gShadowMap;
uniform extern texture gNormalMap;

sampler ShadowMapS = sampler_state
{
	Texture = <gShadowMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	AddressU  = CLAMP; 
    AddressV  = CLAMP;
};

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP; 
    AddressV  = WRAP;
};

sampler NormalMapS = sampler_state
{
	Texture = <gNormalMap>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

void BuildShadowMapVS(float3 posW : POSITION0,
                      out float4 posH : POSITION0,
                      out float2 depth : TEXCOORD0)
{
	float4x4 lightWVP = mul(gWorld, gLightVP);
	// rendering from light pos
	posH = mul(float4(posW, 1.0f), lightWVP);
	// depth stored in z/w
	depth = posH.zw;
}

float4 BuildShadowMapPS(float2 depth : TEXCOORD0) : COLOR
{
	// Each pixel in the shadow map stores the pixel depth from the 
	// light source in normalized device coordinates.
	return depth.x / depth.y; // z / w
}

technique BuildShadowMapTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 BuildShadowMapVS();
        pixelShader  = compile ps_2_0 BuildShadowMapPS();
	}
}

void LightShadowVS(float3 posL         : POSITION0,
                   float3 normalL      : NORMAL0,
                   float2 tex0         : TEXCOORD0,
				   float3 tangentL	   : TANGENT0,
				   float3 binormalL	   : BINORMAL0,
                   out float4 oPosH    : POSITION0,
                   out float3 oPosW    : TEXCOORD0,
                   out float3 oNormalW : TEXCOORD1,
                   out float3 oToEyeW  : TEXCOORD2,
                   out float2 oTex0    : TEXCOORD3,
                   out float4 oProjTex : TEXCOORD4,
				   out float3 lightDirT: TEXCOORD5,
				   out float3 toEyeT   : TEXCOORD6)
{

	// world axis in tangent space (TBN matrix)
	float3x3 TBN;
	TBN[0] = tangentL;
	TBN[1] = binormalL;
	TBN[2] = normalL;

	// matrix transforms from object space to tangent space.
	float3x3 toTangentSpace = transpose(TBN);

	// transform eye(camera) position to local space.
	float3 eyePosL = mul(float4(gEyePosW, 1.0f), gWorld);
	
	// transform vertex-to-eye position vector to tangent space.
	float3 toEyeL = eyePosL - posL;
	toEyeT = mul(toEyeL, toTangentSpace);
	
	// transform light direction to tangent space.
	float3 lightDirL = mul(float4(gLight.dirW, 0.0f), gWorld).xyz;
	lightDirT  = mul(lightDirL, toTangentSpace);

	float4x4 WVP = mul(gWorld, gVP);

	// transform to homogeneous clip space.
	oPosH = mul(float4(posL, 1.0f), WVP);
	
	// transform vertex position to world space.
	oPosW = mul(float4(posL, 1.0f), gWorld).xyz;
	
	// transform normal to world space (assume no non-uniform scaling)
	// otherwise the inverse world matrix is needed
	oNormalW = mul(float4(normalL, 0.0f), gWorld).xyz;
	
	// compute the unit vector from the vertex to the eye.
	oToEyeW = gEyePosW - oPosW;
	
	// pass on color texture coords to PS
	oTex0 = tex0;
	
	float4x4 lightWVP = mul(gWorld, gLightVP);

	// position of light is stored in projtext
	oProjTex = mul(float4(posL, 1.0f), lightWVP);
}

float4 LightShadowPS(float3 posW      : TEXCOORD0,
                     float3 normalW   : TEXCOORD1,
                     float3 toEyeW    : TEXCOORD2,
                     float2 tex0      : TEXCOORD3,
                     float4 projTex   : TEXCOORD4,
					 float3 lightDirT : TEXCOORD5,
					 float3 toEyeT    : TEXCOORD6) : COLOR
{
	// Interpolated normals can become unnormal--so normalize.
	normalW = normalize(normalW);
	toEyeW  = normalize(toEyeW);
	toEyeT    = normalize(toEyeT);
	lightDirT = normalize(lightDirT);

	// lightvector points toward the sceen, therefore is the opposite of lights direction
	float3 lightVecT = -lightDirT;
	
	// sample current normal value from normal map
	float3 normalT = tex2D(NormalMapS, tex0);

	// Expand from [0, 1] compressed interval to true [-1, 1] interval.
    normalT = 2.0f*normalT - 1.0f;
    
    // Make it a unit vector.
	normalT = normalize(normalT);
	
	// Light vector is from pixel to spotlight position.
	float3 lightVecW = normalize(gLight.posW - posW);
	
	// Compute the reflection vector.
	float3 r = reflect(-lightVecT, normalT);
	
	// Determine how much (if any) specular light makes it into the eye.
	float t  = pow(max(dot(r, toEyeT), 0.0f), gMaterial.specPower);
	
	// Determine the diffuse light intensity that strikes the vertex.
	float s = max(dot(lightVecT, normalT), 0.0f);

	if(s <= 0.0f)
	     t = 0.0f;


	// Compute the ambient, diffuse and specular terms separately. 
	float3 spec = t*(gMaterial.spec*gLight.spec).rgb;
	float3 diffuse = s*(gMaterial.diffuse*gLight.diffuse.rgb);
	float3 ambient = (float3(0.2f,0.2f,0.2f)*gLight.ambient)+(s/2.0f);
	
	// Sample decal map.
	float4 texColor = tex2D(TexS, tex0); 
	
	// Project the texture coords and scale/offset to [0, 1].
	projTex.xy /= projTex.w;            
	projTex.x =  0.5f*projTex.x + 0.5f; 
	projTex.y = -0.5f*projTex.y + 0.5f;
	
	// depth is aquired through the normalized z coord of the current pos in lightspace 
	float depth = projTex.z / projTex.w;
 
	// transform to texel space
    float2 texelpos = SMAP_SIZE * projTex.xy;
        
    // get fractal part for determing lerp amounts    
    float2 lerps = frac( texelpos );
    
    // 2x2 percentage closest filter.
    float dx = 1.0f / SMAP_SIZE;
	float s0 = (tex2D(ShadowMapS, projTex.xy).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s1 = (tex2D(ShadowMapS, projTex.xy + float2(dx, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s2 = (tex2D(ShadowMapS, projTex.xy + float2(0.0f, dx)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s3 = (tex2D(ShadowMapS, projTex.xy + float2(dx, dx)).r   + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	
	float shadowCoeff = lerp( lerp( s0, s1, lerps.x ),
                              lerp( s2, s3, lerps.x ),
                              lerps.y );
	
	// shadow coefficient only affects diffuse color
	float3 litColor = ambient*texColor.rgb + shadowCoeff*(diffuse*texColor.rgb + spec);
	
	return float4(litColor, gMaterial.diffuse.a*texColor.a);
}

technique LightShadowTech
{
    pass P0
    {
		ANTIALIASEDLINEENABLE = FALSE;
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 LightShadowVS();
        pixelShader  = compile ps_2_0 LightShadowPS();
    }
}
