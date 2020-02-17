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
	//�R���X�g���N�^
	MeshRender();
	//�f�X�g���N�^
	virtual ~MeshRender();

	//�`�揈��
	void Render(XMMATRIX& worldMatrix) override;

	//���b�V���t�B���^�̎擾
	std::shared_ptr<MeshFilter> GetMeshFilter();

	virtual void OnDebugImGui() override;
};