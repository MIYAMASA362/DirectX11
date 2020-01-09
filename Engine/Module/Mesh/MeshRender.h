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
	

	//メッシュフィルタ
	std::shared_ptr<MeshFilter> _MeshFilter;

	//モデル
	std::weak_ptr<Model> _Model;

public:
	//メッシュ
	Mesh* _Mesh;
	unsigned int _IndexNum;
	unsigned int _IndexStartNum;

	//コンストラクタ
	MeshRender(EntityID OwnerID);
	//デストラクタ
	virtual ~MeshRender();

	//描画処理
	void Render(XMMATRIX& worldMatrix) override;

	//メッシュの設定
	void SetMesh(std::weak_ptr<Mesh> mesh) { _MeshFilter->AddMesh(mesh); };

	//メッシュの取得
	std::weak_ptr<Mesh> GetMesh(unsigned int index) { return _MeshFilter->GetMesh(index); };
	//メッシュ数
	size_t GetNumMesh() { return _MeshFilter->GetNumMesh(); };


	//モデル
	void SetModel(std::shared_ptr<Model> model) { _Model = model; }
};