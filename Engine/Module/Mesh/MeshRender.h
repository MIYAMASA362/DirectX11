#pragma once

class Mesh;
struct TextureMaterial;

//MeshRenderer
class MeshRender :public Renderer3D
{
	friend class Physics;
private:
	//メッシュフィルタ
	std::shared_ptr<MeshFilter> _MeshFilter;
	//マテリアル
	TextureMaterial* _TextureMaterial;

public:
	MeshRender(EntityID OwnerID);
	virtual ~MeshRender();

public:
	//描画処理
	void Render(XMMATRIX& worldMatrix) override;

	//メッシュの設定
	void SetMesh(std::weak_ptr<Mesh> mesh) { _MeshFilter->AddMesh(mesh); };
	//メッシュの取得
	std::weak_ptr<Mesh> GetMesh(unsigned int index) { return _MeshFilter->GetMesh(index); };
	//メッシュ数
	size_t GetNumMesh() { return _MeshFilter->GetNumMesh(); };

	//マテリアル設定
	void SetMaterial(TextureMaterial* material) { _TextureMaterial = material; };
};