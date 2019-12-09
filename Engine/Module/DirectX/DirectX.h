#pragma once

using namespace DirectX;

class Shader;
class Light;
class Material;

class ConstantBuffer;

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
	HWND hWnd;
	HINSTANCE hInstance;

	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device*			D3DDevice;
	ID3D11DeviceContext*	ImmediateContext;
	IDXGISwapChain*			SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;

	ID3D11DepthStencilState* DepthStateEnable;
	ID3D11DepthStencilState* DepthStateDisable;

	ConstantBuffer* _ConstantBuffer;	//コンスタントバッファ
	Shader* _Shader;

	Light* _Light;	//ライト
	Material* _Material; //マテリアル
private:
	D3DApp() = default;
	~D3DApp() = default;
public:
	static HRESULT Create(HWND hWnd, HINSTANCE hInstance, unsigned int fps);
	static void Destroy();
public:
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();
	static HWND GetWindow();
	static HINSTANCE GethInstance();
	static unsigned int GetScreenWidth();
	static unsigned int GetScreenHeight();
	static unsigned int GetFps();

	static Shader* GetShader() { return pInstance->_Shader; };
	static ConstantBuffer* GetConstBuffer() { return pInstance->_ConstantBuffer; };

public:
	class Renderer
	{
	public:
		static void Begin();
		static void End();
		static void ClearRenderTargetView(Color clearColor);

		static void SetDepthEnable(bool Enable);
		static void SetRasterize(D3D11_FILL_MODE fillmode, D3D11_CULL_MODE cullmode);

		static void SetVertexBuffer(ID3D11Buffer* VertexBuffer);
		static void SetIndexBuffer(ID3D11Buffer* IndexBuffer);

		static void SetProjectionMatrix2D();
	};
};