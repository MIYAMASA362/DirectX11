#pragma once

class Shader;

namespace DirectX
{
	struct XMMATRIX;


	//=== Renderer ============================================================
	class Renderer:public Component<Renderer>
	{
		using RendererList = std::list<std::weak_ptr<Renderer>>;
	public:
		enum RendererTarget
		{
			RenderTarget2D,
			RenderTarget3D
		};

		using RendererIndex = std::map<RendererTarget, RendererList>;

	public:
		static RendererIndex _RendererIndex;

		bool m_IsEnable = true;
		unsigned int _sort = 0;			// 描画順
		RendererTarget _RendererTarget;	// 3D/2Dタイプ
	
		D3D11_PRIMITIVE_TOPOLOGY _PrimitiveTopology;	//描画タイプ
		Shader* _Shader;				//シェーダ
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

	class Renderer3D:public Renderer
	{
	public:
		static void BeginRender();

	public:
		Renderer3D(EntityID OwnerID);
		virtual ~Renderer3D();

		virtual void Render(XMMATRIX& worldMatrix) =0;
	};

	class Renderer2D:public Renderer
	{
	public:
		static void BeginRender();

	public:
		Renderer2D(EntityID OwnerID);
		virtual ~Renderer2D();

		virtual void Render(XMMATRIX& worldMatrix) = 0;
	};
}
