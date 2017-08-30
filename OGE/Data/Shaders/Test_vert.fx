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




// Structures

// VS input
struct VertexShaderInput
{
    float4 Position : POSITION0;    
    float3 Normal : NORMAL0;  
	float2 TexCoord : TEXCOORD0;
};

// VS output
struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float4 Diffuse : COLOR0;
    float4 Specular : COLOR1;
    float2 TexCoord : TEXCOORD0;
};




// Vertex shader
VertexShaderOutput PhongVS(VertexShaderInput input)
{
	// Zero out our output
	VertexShaderOutput output = (VertexShaderOutput)0;

	// Preparations
	float3 normal = normalize(mul(float4(input.Normal, 0.0f), gWorldInverseTranspose).xyz);
	float3 pos  = mul(input.Position, gWorld).xyz;
	float3 toEye = normalize(gEyePosisiton - pos);
	
	// Compute the reflection vector.
	float3 r = reflect(-gLightVec, normal);
	float t  = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	float s = max(dot(gLightVec, normal), 0.0f);
	
	// Compute the ambient, diffuse and specular light
	float3 ambient = gAmbientMtrl * gAmbientLight;
	float3 diffuse = s * (gDiffuseMtrl * gDiffuseLight).rgb;
	float3 spec = t * (gSpecularMtrl * gSpecularLight).rgb;
	
	// Sum all the terms together and copy over the diffuse alpha.
	output.Diffuse = float4(ambient + diffuse, gDiffuseMtrl.a);
	output.Specular = float4(spec, 0.0f);

	// Transform to homogeneous clip space
	float4x4 transformMatrix = mul(gWorld, gView);
	transformMatrix = mul(transformMatrix, gProjection);
    output.Position = mul(input.Position, transformMatrix);
	  
	// Pass on texture coordinates to be interpolated in rasterization
	output.TexCoord = input.TexCoord;

	// Return the output.
	return output;
}




// Pixel shader
float4 PhongPS(VertexShaderOutput input) : COLOR
{
	// Ambient component


	// Diffuse component
	float3 texColorDiffuse = tex2D(TexSamplerDiffuse, input.TexCoord).rgb;
	float3 diffuse = input.Diffuse.rgb * texColorDiffuse;
	
	// Specular component
	float3 texColorSpecular = tex2D(TexSamplerSpecular, input.TexCoord).rgb;
	float3 specular = input.Specular.rgb * texColorSpecular;
	
    return float4(diffuse + specular, input.Diffuse.a); 
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

