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

uniform extern texture gTex;

uniform extern float4 gAmbientMtrl;
uniform extern float4 gAmbientLight;
uniform extern float4 gDiffuseMtrl;
uniform extern float4 gDiffuseLight;
uniform extern float4 gSpecularMtrl;
uniform extern float4 gSpecularLight;
uniform extern float  gSpecularPower;
uniform extern float3 gLightVec;





// Samplers for textures
sampler TexSampler = sampler_state
{
	Texture = <gTex>;
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
VertexShaderOutput TransformVS(VertexShaderInput input)
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
	
	// Compute the ambient, diffuse and specular terms separatly. 
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
float4 TransformPS(VertexShaderOutput input) : COLOR
{
	// Get texel from texture map that gets mapped to this pixel.
	float3 texColor = tex2D(TexSampler, input.TexCoord).rgb;
	float3 diffuse = input.Diffuse.rgb * texColor;
    return float4(diffuse + input.Specular.rgb, input.Diffuse.a); 
}




// Technic discription
technique TransformTech
{
	pass P0
	{
		// Specify the vertex and pixel shader associated with this pass
		vertexShader = compile vs_3_0 TransformVS();
		pixelShader  = compile ps_3_0 TransformPS();

		// Specify the render/device states associated with this pass
		FillMode = Solid;
	}
}

