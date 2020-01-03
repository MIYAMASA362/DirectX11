#pragma once

class Shader;

struct XMMATRIX;

//�`��
class Renderer:public Component<Renderer>
{
	//�`�惊�X�g
	using RendererList = std::list<std::weak_ptr<Renderer>>;
public:
	enum RendererTarget
	{
		RenderTarget2D,
		RenderTarget3D
	};

	using RendererIndex = std::map<RendererTarget, RendererList>;

public:
	//�`��z��
	static RendererIndex _RendererIndex;

	//�L���E����
	bool m_IsEnable = true;
	// �`�揇
	unsigned int _sort = 0;
	// 3D/2D�^�C�v
	RendererTarget _RendererTarget;
	
	D3D11_PRIMITIVE_TOPOLOGY _PrimitiveTopology;	//�`��^�C�v
	Shader* _Shader;								//�V�F�[�_
public:
	static void Create();
	static void Destroy();

	Renderer(EntityID OwnerID);
	virtual ~Renderer();

	void SetEnable(bool enable);
	bool GetEnable();

	void SetSort(unsigned int sort);

	virtual void Render(XMMATRIX& worldMatrix)=0;
	void Start();

	Shader* GetShader() { return _Shader; };
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY Topology) { _PrimitiveTopology = Topology; };
};

//3D�`��
class Renderer3D:public Renderer
{
public:
	static void BeginRender();

public:
	Renderer3D(EntityID OwnerID);
	virtual ~Renderer3D();

	virtual void Render(XMMATRIX& worldMatrix) =0;
};

//2D�`��
class Renderer2D:public Renderer
{
public:
	static void BeginRender();

public:
	Renderer2D(EntityID OwnerID);
	virtual ~Renderer2D();

	virtual void Render(XMMATRIX& worldMatrix) = 0;
};
