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
	float3 NormalWorld : TEXCOORD1;
    float3 PositionWorld : TEXCOORD2;
};




// Vertex shader
VertexShaderOutput PhongVS(VertexShaderInput input)
{
	// Zero out our output
	VertexShaderOutput output = (VertexShaderOutput)0;

	// Transform normal and position to world space
	output.NormalWorld = normalize(mul(float4(input.Normal, 0.0f), gWorldInverseTranspose).xyz);
	output.PositionWorld  = mul(input.Position, gWorld).xyz;
	
	// Transform to homogeneous clip space
	output.Position = mul(input.Position, gWorld);
	output.Position = mul(output.Position, gView);
    output.Position = mul(output.Position, gProjection);
	  
	// Pass on texture coordinates to be interpolated in rasterization
	output.TexCoord = input.TexCoord.xy;

	// Return the output.
	return output;
}




// Pixel shader
float4 PhongPS(VertexShaderOutput input) : COLOR
{
	// Get normal
	float3 normal = normalize(input.NormalWorld);
	
	// Get eye position
	float3 toEye = normalize(gEyePosisiton - input.PositionWorld);
	
	
	// Compute the reflection vector.
	float3 r = reflect(-gLightVec, normal);
	float t  = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	float s = max(dot(gLightVec, normal), 0.0f);
	float a = 1.0f;
	
	
	// Ambient component
	float3 texColorAmbient = tex2D(TexSamplerDiffuse, input.TexCoord).rgb;
	float3 ambient = a * texColorAmbient * gAmbientLight.rgb;

	// Diffuse component
	float3 texColorDiffuse = tex2D(TexSamplerDiffuse, input.TexCoord).rgb;
	float3 diffuse = s * texColorDiffuse * gDiffuseLight.rgb;
	
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

