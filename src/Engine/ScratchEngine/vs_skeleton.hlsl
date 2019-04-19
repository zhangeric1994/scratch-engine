cbuffer cbSkinned : register(b4)
{
	matrix gBoneTransforms[96];
};

struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 weight : BLENDWEIGHT;
	float4 boneIndex : BLENDINDICES;
};

cbuffer CameraData : register(b2)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
};

cbuffer ObjectData : register(b3)
{
	matrix world;
};

struct VertexToPixel
{
	float4 svPosition : SV_POSITION;
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

VertexToPixel main(VertexShaderInput vin)
{
	VertexToPixel vout;

	// balance the bone weight
	float weight0 = vin.weight.x;
	float weight1 = vin.weight.y;
	float weight2 = vin.weight.z;
	float weight3 = vin.weight.w;
	float sum = weight0 + weight1 + weight2 + weight3;
	weight0 /= sum;
	weight1 /= sum;
	weight2 /= sum;
	weight3 /= sum;

	// offset position by bone matrices, using weights to scale
	float4 p =  mul(float4(vin.position, 1.0f), 1);
	//p += weight1 * mul(float4(vin.position, 1.0f), gBoneTransforms[vin.boneIndex[1]]);
	//p += weight2 * mul(float4(vin.position, 1.0f), gBoneTransforms[vin.boneIndex[2]]);
	//p += weight3 * mul(float4(vin.position, 1.0f), gBoneTransforms[vin.boneIndex[3]]);
	p.w = 1.0f;

	// offset normal by bone matrices, using weights to scale
	float4 n =  mul(float4(vin.normal, 0.0f), 1);
	//n += weight1 * mul(float4(vin.normal, 0.0f), gBoneTransforms[vin.boneIndex[1]]);
	//n += weight2 * mul(float4(vin.normal, 0.0f), gBoneTransforms[vin.boneIndex[2]]);
	//n += weight3 * mul(float4(vin.normal, 0.0f), gBoneTransforms[vin.boneIndex[3]]);
	n.w = 0.0f;

	// offset tangent by bone matrices, using weights to scale
	//float4 t = weight0 * mul(float4(vin.Tan.xyz, 0.0f), gBoneTransforms[vin.BoneIndex[0]]);
	//t += weight1 * mul(float4(vin.Tan.xyz, 0.0f), gBoneTransforms[vin.BoneIndex[1]]);
	//t += weight2 * mul(float4(vin.Tan.xyz, 0.0f), gBoneTransforms[vin.BoneIndex[2]]);
	//t += weight3 * mul(float4(vin.Tan.xyz, 0.0f), gBoneTransforms[vin.BoneIndex[3]]);
	//t.w = 0.0f;


	// Transform to world space space.
	vout.position = mul(p, world);
	vout.normal = normalize(mul(n.xyz, (float3x3)world));
	//vout.TangentW = float4(mul(t, (float3x3)gWorld), vin.Tan.w);

	matrix WVP = mul(world, viewProjection);
	// Transform to homogeneous clip space.
	vout.svPosition = mul(p, WVP);

	vout.uv = vin.uv;

	return vout;
}