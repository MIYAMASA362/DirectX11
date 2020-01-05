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
	//�`��C���f�b�N�X
	static std::vector<Renderer*> RendererIndex;

	bool _IsEnable = true;			//�`��̗L��
	RenderTarget _RenderTarget;		//�`��ݒ�@2D/3D

	D3D11_PRIMITIVE_TOPOLOGY _PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	//�g�|���W�[

	Material* _Material;	//�}�e���A��

public:

	//�R���X�g���N�^
	Renderer(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~Renderer();

	//�`��̗L���@set/get
	void SetEnable(bool enable) { _IsEnable = enable; }
	bool GetEnable() { return _IsEnable; }

	//�}�e���A���ݒ�@set/get
	void SetMaterial(Material* material) { _Material = material; }
	Material* GetMaterial() { return _Material; }

	//�`�揈��
	virtual void Render(XMMATRIX& worldMatrix) = 0;

	//RendererIndex���̕`������s
	static void BeginRender();

};
