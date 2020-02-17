#pragma once

class Mesh;
class Model;
struct TextureMaterial;

//*********************************************************************************************************************
//
//	MeshRenderer
//
//*********************************************************************************************************************
class MeshRender :public Renderer
{
	friend class Physics;
private:
	std::weak_ptr<MeshFilter> _meshfilter;


public:
	//コンストラクタ
	MeshRender();
	//デストラクタ
	virtual ~MeshRender();

	//描画処理
	void Render(XMMATRIX& worldMatrix) override;

	//メッシュフィルタの取得
	std::shared_ptr<MeshFilter> GetMeshFilter();

	virtual void OnDebugImGui() override;
};