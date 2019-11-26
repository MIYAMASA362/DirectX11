#pragma once

namespace DirectX
{
	struct XMMATRIX;


	//=== Renderer ============================================================
	class Renderer:public Component<Renderer>
	{
	public:
		enum RendererTarget
		{
			RenderTarget2D,
			RenderTarget3D
		};

		using RendererList = std::list<std::weak_ptr<Renderer>>;
		using RendererIndex = std::map<RendererTarget, RendererList>;
	
	protected:
		static RendererIndex _RendererIndex;

	public:
		static void Create();
		static void Destroy();

	protected:
		bool m_IsEnable = true;
		unsigned int _sort = 0;		//•`‰æ‡
		RendererTarget _RendererTarget;	//•`‰æƒ^ƒCƒv
	
	public:
		Renderer(EntityID OwnerID);
		virtual ~Renderer() = default;

		void SetEnable(bool enable);
		bool GetEnable();

		void SetSort(unsigned int sort);

		virtual void Render(XMMATRIX& worldMatrix)=0;
		void Start();
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
