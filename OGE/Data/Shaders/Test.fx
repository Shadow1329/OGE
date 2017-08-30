//==================================================================
// Basic FX that simply transforms geometry from local space to
// homogeneous clip space, and draws the geometry in wireframe mode.
//==================================================================




// FX parameters (global variables to the shader)
uniform extern float4x4 gWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;
uniform extern float4x4 gWorldInverseTranspose;
uniform extern float3 gEyePosisiton;

uniform extern texture gTexReflect;
uniform extern texture gTexDiffuse;
uniform extern texture gTexSpecular;
uniform extern texture gTexNormal;

uniform extern float4 gAmbientMtrl;
uniform extern float4 gAmbientLight;
uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;
uniform extern float4 gSpecularMtrl;
uniform extern float4 gSpecularLight;
uniform extern float  gSpecularPower;
uniform extern float3 gLightVec;





// Samplers for textures
sampler TexSamplerReflect = sampler_state
{
	Texture = <gTexReflect>;
	MAGFILTER = ANISOTROPIC;
    MINFILTER = ANISOTROPIC;
    MIPFILTER = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
	MaxAnisotropy = 4;
};

sampler TexSamplerDiffuse = sampler_state
{
	Texture = <gTexDiffuse>;
	MAGFILTER = ANISOTROPIC;
    MINFILTER = ANISOTROPIC;
    MIPFILTER = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
	MaxAnisotropy = 4;
};

sampler TexSamplerSpecular = sampler_state
{
	Texture = <gTexSpecular>;
	MAGFILTER = ANISOTROPIC;
    MINFILTER = ANISOTROPIC;
    MIPFILTER = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
	MaxAnisotropy = 4;
};

sampler TexSamplerNormal = sampler_state
{
	Texture = <gTexNormal>;
	MAGFILTER = ANISOTROPIC;
    MINFILTER = ANISOTROPIC;
    MIPFILTER = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
	MaxAnisotropy = 4;
};




// Structures

// VS input
struct VertexShaderInput
{
    float4 Position : POSITION0; 	
	float3 Tangent : TANGENT0;
	float3 Binormal : BINORMAL0;
	float3 Normal : NORMAL0;  
	float2 TexCoord : TEXCOORD0;
};

// VS output
struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
	float3 EyeTangent : TEXCOORD1;
    float3 LightDirTangent : TEXCOORD2;
};




// Vertex shader
VertexShaderOutput PhongVS(VertexShaderInput input)
{
	// Zero out our output
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	// Build TBN-basis.
	float3x3 TBN;
	TBN[0] = input.Tangent;
	TBN[1] = input.Binormal;
	TBN[2] = input.Normal;
	float3x3 toTangentSpace = transpose(TBN);
	
	// Transform eye position to tangent space.
	float3 eyePosLocal = mul(float4(gEyePosisiton, 1.0f), gWorldInverseTranspose);
	float3 toEyeLocal = eyePosLocal - mul(input.Position, gWorld).xyz;
	output.EyeTangent = mul(toEyeLocal, toTangentSpace);
	
	// Transform light direction to tangent space.
	float3 lightDirLocal = mul(float4(gLightVec, 0.0f), gWorldInverseTranspose).xyz;
	output.LightDirTangent  = mul(lightDirLocal, toTangentSpace);

	
	// Transform to homogeneous clip space
	output.Position = mul(input.Position, gWorld);
	output.Position = mul(output.Position, gView);
    output.Position = mul(output.Position, gProjection);
	  
	// Pass on texture coordinates to be interpolated in rasterization
	output.TexCoord = input.TexCoord;

	// Return the output.
	return output;
}




// Pixel shader
float4 PhongPS(VertexShaderOutput input) : COLOR
{
	// Normalize.
	float3 eyeT = normalize(input.EyeTangent);
	float3 lightVecT = normalize(input.LightDirTangent);


	// Get normal
	float3 normalT = tex2D(TexSamplerNormal, input.TexCoord);
    normalT = normalize(2 * normalT - 1.0f);
	

	// Compute the reflection vector.
	float3 r = reflect(-lightVecT, normalT);
	float t  = pow(max(dot(r, eyeT), 0.0f), gSpecularPower);
	float s = max(dot(lightVecT, normalT), 0.0f);
	float a = 1.0;
	
	
	// Reflect component
	float3 envMapTex = reflect(-eyeT, normalT);
    float3 reflectedColor = texCUBE(TexSamplerReflect, envMapTex);
	
	// Ambient component
	float3 texColorAmbient = tex2D(TexSamplerDiffuse, input.TexCoord).rgb;
	float3 ambient = a * (reflectedColor + texColorAmbient) / 2.0f * gAmbientLight.rgb;

	// Diffuse component
	float3 texColorDiffuse = tex2D(TexSamplerDiffuse, input.TexCoord).rgb;
	float3 diffuse = s * (reflectedColor + texColorDiffuse) / 2.0f * gDiffuseLight.rgb;
	
	// Specular component
	float3 texColorSpecular = tex2D(TexSamplerSpecular, input.TexCoord).rgb;
	float3 specular = t * texColorSpecular * gSpecularLight.rgb;
	
    return float4(ambient + diffuse + specular, 1.0); 
}




// Technic discription
technique PhongTech
{
	pass P0
	{
		// Specify the vertex and pixel shader associated with this pass
		vertexShader = compile vs_3_0 PhongVS();
		pixelShader  = compile ps_3_0 PhongPS();

		// Specify the render/device states associated with this pass
		FillMode = Solid;
	}
}

