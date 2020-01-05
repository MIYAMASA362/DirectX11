#pragma once

class Shader;

namespace DirectX
{
	struct XMMATRIX;
}

class Material;
class Shader;

//*********************************************************************************************************************
//
//	RenderTarget
//
//*********************************************************************************************************************
enum RenderTarget
{
	RenderTarget2D,
	RenderTarget3D
};

//*********************************************************************************************************************
//
//	Renderer Intarface
//
//*********************************************************************************************************************
class Renderer : public Component<Renderer>
{
public:
	//描画インデックス
	static std::vector<Renderer*> RendererIndex;

	bool _IsEnable = true;			//描画の有無
	RenderTarget _RenderTarget;		//描画設定　2D/3D

	D3D11_PRIMITIVE_TOPOLOGY _PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	//トポロジー

	Material* _Material;	//マテリアル

public:

	//コンストラクタ
	Renderer(EntityID OwnerID);
	//デストラクタ
	virtual ~Renderer();

	//描画の有無　set/get
	void SetEnable(bool enable) { _IsEnable = enable; }
	bool GetEnable() { return _IsEnable; }

	//マテリアル設定　set/get
	void SetMaterial(Material* material) { _Material = material; }
	Material* GetMaterial() { return _Material; }

	//描画処理
	virtual void Render(XMMATRIX& worldMatrix) = 0;

	//RendererIndex内の描画を実行
	static void BeginRender();

};
