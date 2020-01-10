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
	Mesh* _Mesh;	//���b�V��
	
	unsigned int _IndexNum;			//�C���f�b�N�X��
	unsigned int _IndexStartNum;	//�J�n�C���f�b�N�X��


public:
	//�R���X�g���N�^
	MeshRender(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~MeshRender();

	//�`�揈��
	void Render(XMMATRIX& worldMatrix) override;

	//���b�V���ݒ�
	void SetMesh(Mesh* mesh) { _Mesh = mesh; }
	Mesh* GetMesh() { return _Mesh; }
	
	//�C���f�b�N�X�ݒ�
	void SetIndexNum(unsigned int indexNum) { _IndexNum = indexNum; }
	//�J�n�C���f�b�N�X�ݒ�
	void SetStartNum(unsigned int indexStart) { _IndexStartNum = indexStart; }


};