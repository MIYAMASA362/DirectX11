#pragma once

class Shader;

struct XMMATRIX;

//描画
class Renderer:public Component<Renderer>
{
	//描画リスト
	using RendererList = std::list<std::weak_ptr<Renderer>>;
public:
	enum RendererTarget
	{
		RenderTarget2D,
		RenderTarget3D
	};

	using RendererIndex = std::map<RendererTarget, RendererList>;

public:
	//描画配列
	static RendererIndex _RendererIndex;

	//有効・無効
	bool m_IsEnable = true;
	// 描画順
	unsigned int _sort = 0;
	// 3D/2Dタイプ
	RendererTarget _RendererTarget;
	
	D3D11_PRIMITIVE_TOPOLOGY _PrimitiveTopology;	//描画タイプ
	Shader* _Shader;								//シェーダ
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

//3D描画
class Renderer3D:public Renderer
{
public:
	static void BeginRender();

public:
	Renderer3D(EntityID OwnerID);
	virtual ~Renderer3D();

	virtual void Render(XMMATRIX& worldMatrix) =0;
};

//2D描画
class Renderer2D:public Renderer
{
public:
	static void BeginRender();

public:
	Renderer2D(EntityID OwnerID);
	virtual ~Renderer2D();

	virtual void Render(XMMATRIX& worldMatrix) = 0;
};
