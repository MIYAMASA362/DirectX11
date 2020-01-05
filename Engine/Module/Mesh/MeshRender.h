#pragma once

class Mesh;
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
	//���b�V���t�B���^
	std::shared_ptr<MeshFilter> _MeshFilter;

	//���f��
	std::weak_ptr<Model> _Model;

public:
	//�R���X�g���N�^
	MeshRender(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~MeshRender();

	//�`�揈��
	void Render(XMMATRIX& worldMatrix) override;

	//���b�V���̐ݒ�
	void SetMesh(std::weak_ptr<Mesh> mesh) { _MeshFilter->AddMesh(mesh); };
	//���b�V���̎擾
	std::weak_ptr<Mesh> GetMesh(unsigned int index) { return _MeshFilter->GetMesh(index); };
	//���b�V����
	size_t GetNumMesh() { return _MeshFilter->GetNumMesh(); };


	//���f��
	void SetModel(std::shared_ptr<Model> model) { _Model = model; }
};