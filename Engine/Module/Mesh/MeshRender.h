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
	Mesh* _Mesh;	//メッシュ
	
	unsigned int _IndexNum;			//インデックス数
	unsigned int _IndexStartNum;	//開始インデックス数


public:
	//コンストラクタ
	MeshRender(EntityID OwnerID);
	//デストラクタ
	virtual ~MeshRender();

	//描画処理
	void Render(XMMATRIX& worldMatrix) override;

	//メッシュ設定
	void SetMesh(Mesh* mesh) { _Mesh = mesh; }
	Mesh* GetMesh() { return _Mesh; }
	
	//インデックス設定
	void SetIndexNum(unsigned int indexNum) { _IndexNum = indexNum; }
	//開始インデックス設定
	void SetStartNum(unsigned int indexStart) { _IndexStartNum = indexStart; }


};