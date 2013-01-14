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
	// Render from light's perspective.
	posH = mul(float4(posW, 1.0f), lightWVP);
	
	// Propagate z- and w-coordinates.
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

	// Build TBN-basis.
	float3x3 TBN;
	TBN[0] = tangentL;
	TBN[1] = binormalL;
	TBN[2] = normalL;

	// Matrix transforms from object space to tangent space.
	float3x3 toTangentSpace = transpose(TBN);

	// Transform eye position to local space.
	float3 eyePosL = mul(float4(gEyePosW, 1.0f), gWorld);
	
	// Transform to-eye vector to tangent space.
	float3 toEyeL = eyePosL - posL;
	toEyeT = mul(toEyeL, toTangentSpace);
	
	// Transform light direction to tangent space.
	float3 lightDirL = mul(float4(gLight.dirW, 0.0f), gWorld).xyz;
	lightDirT  = mul(lightDirL, toTangentSpace);

	float4x4 WVP = mul(gWorld, gVP);

	// Transform to homogeneous clip space.
	oPosH = mul(float4(posL, 1.0f), WVP);
	
	// Transform vertex position to world space.
	oPosW = mul(float4(posL, 1.0f), gWorld).xyz;
	
	// Transform normal to world space (assume no non-uniform scaling).
	oNormalW = mul(float4(normalL, 0.0f), gWorld).xyz;
	
	// Compute the unit vector from the vertex to the eye.
	oToEyeW = gEyePosW - oPosW;
	
	// Pass on texture coords to PS
	oTex0 = tex0;
	
	float4x4 lightWVP = mul(gWorld, gLightVP);

	// Generate projective texture coordinates.
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

	// Light vector is opposite the direction of the light.
	float3 lightVecT = -lightDirT;
	
	// Sample normal map.
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
	float3 ambient = gMaterial.ambient*gLight.ambient;
	
	// Sample decal map.
	float4 texColor = tex2D(TexS, tex0); 
	
	// Project the texture coords and scale/offset to [0, 1].
	projTex.xy /= projTex.w;            
	projTex.x =  0.5f*projTex.x + 0.5f; 
	projTex.y = -0.5f*projTex.y + 0.5f;
	
	// Compute pixel depth for shadowing.
	float depth = projTex.z / projTex.w;
 
	// Transform to texel space
    float2 texelpos = SMAP_SIZE * projTex.xy;
        
    // Determine the lerp amounts.           
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
	
	//// Light/Texture pixel.  Note that shadow coefficient only affects diffuse/spec.
	float3 litColor = ambient*texColor.rgb + (diffuse*texColor.rgb + spec);
	//
	return float4(litColor, gMaterial.diffuse.a*texColor.a);
}


//struct OutputVS
//{
//    float4 posH      : POSITION0;
//    float3 toEyeT    : TEXCOORD0;
//    float3 lightDirT : TEXCOORD1;
//    float2 tex0      : TEXCOORD2;
//};
//
//
//OutputVS LightShadowVS(float3 posL      : POSITION0,
//					   float3 normalL   : NORMAL0,
//					   float2 tex0      : TEXCOORD0,
//                     float3 tangentL  : TANGENT0,
//                     float3 binormalL : BINORMAL0,
//                     
//                     )
//{
//    // Zero out our output.
//	OutputVS outVS = (OutputVS)0;
//	
//	// Build TBN-basis.
//	float3x3 TBN;
//	TBN[0] = tangentL;
//	TBN[1] = binormalL;
//	TBN[2] = normalL;
//	
//	// Matrix transforms from object space to tangent space.
//	float3x3 toTangentSpace = transpose(TBN);
//	
//	// Transform eye position to local space.
//	float3 eyePosL = mul(float4(gEyePosW, 1.0f), gWorld);
//	
//	// Transform to-eye vector to tangent space.
//	float3 toEyeL = eyePosL - posL;
//	outVS.toEyeT = mul(toEyeL, toTangentSpace);
//	
//	// Transform light direction to tangent space.
//	float3 lightDirL = mul(float4(gLight.dirW, 0.0f), gWorld).xyz;
//	outVS.lightDirT  = mul(lightDirL, toTangentSpace);
//	
//
//	float4x4 wvp = mul(gVP, gWorld);
//
//	// Transform to homogeneous clip space.
//	outVS.posH = mul(float4(posL, 1.0f), wvp);
//	
//	// Pass on texture coordinates to be interpolated in rasterization.
//	outVS.tex0 = tex0;
//	
//	// Done--return the output.
//    return outVS;
//}
//
//float4 LightShadowPS(float3 toEyeT    : TEXCOORD0,
//                   float3 lightDirT : TEXCOORD1,
//                   float2 tex0      : TEXCOORD2) : COLOR
//{
//	// Interpolated normals can become unnormal--so normalize.
//	toEyeT    = normalize(toEyeT);
//	lightDirT = normalize(lightDirT);
//	
//	// Light vector is opposite the direction of the light.
//	float3 lightVecT = -lightDirT;
//	
//	// Sample normal map.
//	float3 normalT = tex2D(NormalMapS, tex0);
//	
//	// Expand from [0, 1] compressed interval to true [-1, 1] interval.
//    normalT = 2.0f*normalT - 1.0f;
//    
//    // Make it a unit vector.
//	normalT = normalize(normalT);
//	
//	// Compute the reflection vector.
//	float3 r = reflect(-lightVecT, normalT);
//	
//	// Determine how much (if any) specular light makes it into the eye.
//	float t  = pow(max(dot(r, toEyeT), 0.0f), gMaterial.specPower);
//	
//	// Determine the diffuse light intensity that strikes the vertex.
//	float s = max(dot(lightVecT, normalT), 0.0f);
//	
//	// If the diffuse light intensity is low, kill the specular lighting term.
//	// It doesn't look right to add specular light when the surface receives 
//	// little diffuse light.
//	if(s <= 0.0f)
//	     t = 0.0f;
//	
//	// Compute the ambient, diffuse and specular terms separatly. 
//	float3 spec = t*(gMaterial.spec*gLight.spec).rgb;
//	float3 diffuse = s*(gMaterial.diffuse*gLight.diffuse).rgb;
//	float3 ambient = gMaterial.ambient*gLight.ambient;
//	
//	// Get the texture color.
//	float4 texColor = tex2D(TexS, tex0);
//	
//	// Combine the color from lighting with the texture color.
//	float3 color = (ambient + diffuse)*texColor.rgb + spec;
//	
//	// Output the color and the alpha.
//    return float4(color, gMaterial.diffuse.a*texColor.a);
//}

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
