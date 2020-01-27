Texture2D	 Texture:register(t0);
SamplerState Sampler:register(s0);

void main(
	in float4 inPosition	:SV_POSITION,
	in float4 inDiffuse		:COLOR0,
	in float2 inTexCoord	:TEXCOORD0,
	out float4 outDiffuse	:SV_Target
)
{
	outDiffuse  = Texture.Sample(Sampler,inTexCoord);
	outDiffuse.a = 1.0f;

	//頂点色をブレンド
	//outDiffuse *= inDiffuse;

	//モノクロ化
	//float mono = dot(outDiffuse.rgb,1.0/3.0);
	//outDiffuse.rgb *= mono;

	//マルチテクスチャ

}