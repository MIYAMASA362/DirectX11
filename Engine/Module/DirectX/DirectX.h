#pragma once

namespace System
{
	class Window;
}

namespace DirectX
{
	class D3DApp
	{
	private:
		static D3DApp* pInstance;
	private:
		unsigned int ScreenWidth;
		unsigned int ScreenHeight;
		const std::string path = "";	//Applicaion Path
		unsigned int fps;
	private:
		System::Window* window;
		HWND hWnd;
		HINSTANCE hInstance;

		D3D_FEATURE_LEVEL featureLevel;

		ID3D11Device*			D3DDevice;
		ID3D11DeviceContext*	ImmediateContext;
		IDXGISwapChain*			SwapChain;
		ID3D11RenderTargetView* RenderTargetView;
		ID3D11DepthStencilView* DepthStencilView;

		ID3D11VertexShader*		VertexShader;
		ID3D11PixelShader*		PixelShader;
		ID3D11InputLayout*		VertexLayout;
		ID3D11Buffer*			WorldBuffer;
		ID3D11Buffer*			ViewBuffer;
		ID3D11Buffer*			ProjectionBuffer;

		ID3D11Buffer*			MaterialBuffer;
		ID3D11Buffer*			LightBuffer;

		ID3D11DepthStencilState* DepthStateEnable;
		ID3D11DepthStencilState* DepthStateDisable;
	private:
		D3DApp() = default;
		~D3DApp() = default;
	public:
		static HRESULT Create(HWND hWnd,HINSTANCE hInstance,unsigned int fps);
		static void Destroy();
	public:
		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();
		static HWND GetWindow();
		static HINSTANCE GethInstance();
		static unsigned int GetScreenWidth();
		static unsigned int GetScreenHeight();
		static unsigned int GetFps();
	public:
		static int Run();
	public:
		class Renderer
		{
		public:
			static void Begin();
			static void End();
			static void ClearRenderTargetView(Color clearColor);

			static void SetDepthEnable(bool Enable);
			static void SetRasterize(D3D11_FILL_MODE fillmode,D3D11_CULL_MODE cullmode);

			static void SetWorldViewProjection2D();
			static void SetWorldMatrix(XMMATRIX* WorldMatrix);
			static void SetViewMatrix(XMMATRIX* ViewMatrix);
			static void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);
			static void SetProjectionMatrix2D();

			static void SetMaterial(MATERIAL Material);
			static void SetLight(LIGHT Light);

			static void SetVertexBuffer(ID3D11Buffer* VertexBuffer);
			static void SetIndexBuffer(ID3D11Buffer* IndexBuffer);

			static void SetTexture(ID3D11ShaderResourceView* Texture);
			static void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation);
		};
	};
}

