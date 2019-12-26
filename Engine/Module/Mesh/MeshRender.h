#pragma once

class Mesh;
struct TextureMaterial;

//MeshRenderer
class MeshRender :public Renderer3D
{
	friend class Physics;
private:
	//���b�V���t�B���^
	std::shared_ptr<MeshFilter> _MeshFilter;
	//�}�e���A��
	TextureMaterial* _TextureMaterial;

public:
	MeshRender(EntityID OwnerID);
	virtual ~MeshRender();

public:
	//�`�揈��
	void Render(XMMATRIX& worldMatrix) override;

	//���b�V���̐ݒ�
	void SetMesh(std::weak_ptr<Mesh> mesh) { _MeshFilter->AddMesh(mesh); };
	//���b�V���̎擾
	std::weak_ptr<Mesh> GetMesh(unsigned int index) { return _MeshFilter->GetMesh(index); };
	//���b�V����
	size_t GetNumMesh() { return _MeshFilter->GetNumMesh(); };

	//�}�e���A���ݒ�
	void SetMaterial(TextureMaterial* material) { _TextureMaterial = material; };
};