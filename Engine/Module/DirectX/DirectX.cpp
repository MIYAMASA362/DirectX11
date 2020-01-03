
//Link
//
//複数ウィンドウ
// ref :https://blog.techlab-xe.net/directx11%E3%81%A7%E8%A4%87%E6%95%B0%E3%82%A6%E3%82%A3%E3%83%B3%E3%83%89%E3%82%A6%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%A6%E3%81%BF%E3%82%8B/
//

#define _CRT_SECURE_NO_WARNINGS

#include<Windows.h>
#include<stdio.h>
#include<io.h>
#include<string>

#include<d3d11.h>
#include<DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")

//DirectX
#include"DirectX.h"

#include"Module\Shader\Shader.h"
#include"Module\Light\Light.h"
#include"Module\Material\Material.h"


using namespace DirectX;






//*********************************************************************************************************************
//
//	D3DApp
//
//*********************************************************************************************************************

//D3DApp
//	コンストラクタ
//
D3DApp::D3DApp()
{

}

//~D3DApp
//	デストラクタ
//
D3DApp::~D3DApp()
{
	if (ImmediateContext) ImmediateContext->ClearState();
	if (RenderTargetView) RenderTargetView->Release();
	if (SwapChain) SwapChain->Release();

	if (ImmediateContext) ImmediateContext->Release();
	if (_pFactory) _pFactory->Release();
	if (_pAdapter) _pAdapter->Release();
	if (_pDXGI)	_pDXGI->Release();
	if (D3DDevice)	D3DDevice->Release();

}

//Create
//	D3DApp初期化処理
//
HRESULT D3DApp::Create(HWND hWnd, unsigned int fps)
{
	HRESULT hr = E_FAIL;
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtrA(hWnd,GWLP_HINSTANCE);

	this->hWnd = hWnd;
	this->fps = fps;

	//ウィンドウサイズの取得
	RECT rect;
	GetClientRect(hWnd, &rect);
	ScreenWidth = rect.right - rect.left;
	ScreenHeight = rect.bottom - rect.top;

	D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL_11_0 };

	//Create a Device that represents the display adapter
	// ref : https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	//
	hr = D3D11CreateDevice(
		NULL,							//Default Adapter
		D3D_DRIVER_TYPE_HARDWARE,		//Driver Type : D3D_DRIVER_TYPE_HARDWARE 最高品質
		NULL,							//Software Rasterizer DLL Handler : Driver Type が SoftwareならNULLは駄目
		0,								//Device parameters
		featureLevels,					//FeatureLevels
		1,								//FeatureLevels Num
		D3D11_SDK_VERSION,				//SDK Version
		&D3DDevice,			//D3DDevice
		&featureLevel,		//successful FeatureLevel
		&ImmediateContext	//DeviceContext
	);


	if (FAILED(hr)) {
		MessageBox(NULL, "Deviceの生成に失敗しました。", "D3DApp", MB_OK);
		return hr;
	}

	//QuearyInterface
	//	IDXGIDevice1 のIntarfaceを取得
	//	D3D11Device は IUnkownクラスを継承している
	//	ref : https://docs.microsoft.com/ja-jp/cpp/atl/introduction-to-com?view=vs-2019
	//
	//	COMについて
	//	ref : http://chokuto.ifdef.jp/urawaza/com/com.html	
	//
	D3DDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&_pDXGI);

	//GetAdapter
	//	DeviceのApapter取得
	//	ref : https://docs.microsoft.com/ja-jp/windows/win32/api/dxgi/nn-dxgi-idxgidevice
	//
	_pDXGI->GetAdapter(&_pAdapter);

	//GetParent
	//	DXGIAdapterの親 IDXGIFactoryを取得
	//
	//	DXGIObject
	//	ref : https://docs.microsoft.com/ja-jp/windows/win32/api/dxgi/nn-dxgi-idxgiobject
	//
	_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&_pFactory);


	// スワップチェーン設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.BufferDesc.Width = ScreenWidth;
	sd.BufferDesc.Height = ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Windowed = TRUE;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	//CreateSwapChain
	//	
	//
	hr = _pFactory->CreateSwapChain(D3DDevice, &sd, &SwapChain);

	// レンダーターゲットビュー生成、設定
	this->CreateRenderTargetView();

	//ステンシル用テクスチャー作成
	this->CreateDepthStencilView();

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)ScreenWidth;
	vp.Height = (float)ScreenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ImmediateContext->RSSetViewports(1, &vp);


	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	D3DDevice->CreateRasterizerState(&rd, &rs);

	ImmediateContext->RSSetState(rs);

	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	D3DDevice->CreateBlendState(&blendDesc, &blendState);
	ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);




	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	//深度有効ステート
	D3DDevice->CreateDepthStencilState(&depthStencilDesc, &DepthStateEnable);

	//depthStencilDesc.DepthEnable = FALSE; //深度無効ステート
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	D3DDevice->CreateDepthStencilState(&depthStencilDesc, &DepthStateDisable);

	ImmediateContext->OMSetDepthStencilState(DepthStateEnable, NULL);




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	ImmediateContext->PSSetSamplers(0, 1, &samplerState);


	return S_OK;
}

//CreateBuffer
//	DirectX CreateBuffer
//
void D3DApp::CreateBuffer(unsigned int BindFlag, unsigned int byteWidth, const void * subresource, ID3D11Buffer ** buffer)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = BindFlag;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = subresource;

	D3DDevice->CreateBuffer(&bd, &sd, buffer);
}

void D3DApp::CreateRenderTargetView()
{
	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
	pBackBuffer->Release();
}

void D3DApp::CreateDepthStencilView()
{
	DXGI_SWAP_CHAIN_DESC sd;

	//DXGI_SWAP_CHAIN_DESCの取得
	//	WARNING : Direct3D 11.1 の場合、DESC取得に推奨されない関数
	//	ref: https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getdesc
	//
	SwapChain->GetDesc(&sd);

	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &DepthStencilView);


	ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
}

void D3DApp::CleanupRenderTargetView()
{
	if (RenderTargetView == nullptr) return;
	RenderTargetView->Release();
	RenderTargetView = nullptr;
}

void D3DApp::CleanupDepthStencilView()
{
	if (DepthStencilView == nullptr) return;
	DepthStencilView->Release();
	DepthStencilView = nullptr;
}








//*********************************************************************************************************************
//
//	D3DApp::Renderer
//
//*********************************************************************************************************************


D3DApp* D3DApp::Renderer::_pDevice = nullptr;					//Renderに使うD3DApp

Shader* D3DApp::Renderer::_Shader = nullptr;					//Shader
ConstantBuffer* D3DApp::Renderer::_ConstantBuffer = nullptr;	//ConstantBuffer
Light* D3DApp::Renderer::_Light = nullptr;						//Light
Material* D3DApp::Renderer::_Material = nullptr;				//Material

//RegisterDevice
//	登録
//
void D3DApp::Renderer::RegisterDevice(D3DApp * d3dapp)
{
	_pDevice = d3dapp;

	//シェーダ

	//定数バッファ生成
	_ConstantBuffer = new ConstantBuffer();
	_ConstantBuffer->CreateBuffer();

	//Light
	_Light = new Light();

	//Material
	_Material = new Material();

	//シェーダ設定
	_Shader = new Shader();
	VERTEX_INPUT_LAYOUT layout[] =
	{
		VSIL_POSITION,
		VSIL_NORMAL,
		VSIL_COLOR,
		VSIL_TEXCOORD
	};
	_Shader->SetVertexShader("Asset/Shader/vertexShader.cso", layout, ARRAYSIZE(layout));
	_Shader->SetPixelShader("Asset/Shader/pixelShader.cso");

	_Shader->SetShader();

	//定数バッファ設定
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_WORLD, 0);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_VIEW, 1);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_PROJECTION, 2);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_MATERIAL, 3);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_LIGHT, 4);

	_Material->SetResource();
	_Light->SetResource();

	XMMATRIX matrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)_pDevice->ScreenWidth, (float)_pDevice->ScreenHeight, 0.0f, 0.0f, 1.0f);
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION, &matrix);
}

//Release
//	リソースの破棄・解放
//
void D3DApp::Renderer::Release()
{
	_pDevice = nullptr;

	if (_Light) delete _Light;
	if (_Material) delete _Material;
	if (_Shader) delete _Shader;
	if (_ConstantBuffer) delete _ConstantBuffer;
}

//Begin
//	描画開始
//
void D3DApp::Renderer::Begin()
{
	_pDevice->ImmediateContext->ClearDepthStencilView(_pDevice->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//End
//	描画終了
//
void D3DApp::Renderer::End()
{
	_pDevice->SwapChain->Present(1, 0);
}

//ClearRenderTargetView
//	コンテキストリセット
//
void D3DApp::Renderer::ClearRenderTargetView(Color clearColor)
{
	_pDevice->ImmediateContext->ClearRenderTargetView(_pDevice->RenderTargetView, clearColor);
}

//SetDepthEnable
//	
//
void D3DApp::Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		_pDevice->ImmediateContext->OMSetDepthStencilState(_pDevice->DepthStateEnable, NULL);
	else
		_pDevice->ImmediateContext->OMSetDepthStencilState(_pDevice->DepthStateDisable, NULL);
}

//SetRasterize
//
//
void D3DApp::Renderer::SetRasterize(D3D11_FILL_MODE fillmode, D3D11_CULL_MODE cullmode)
{
	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = fillmode;
	rd.CullMode = cullmode;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	_pDevice->D3DDevice->CreateRasterizerState(&rd, &rs);

	_pDevice->ImmediateContext->RSSetState(rs);
}

//SetWorldViewProjection2D
//
//
void D3DApp::Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(&XMMatrixIdentity());
	SetProjectionMatrix2D();
}

//SetWorldMatrix
//
//
void D3DApp::Renderer::SetWorldMatrix(XMMATRIX* WorldMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_WORLD, &XMMatrixTranspose(*WorldMatrix));
}

//SetViewMatrix
//
//
void D3DApp::Renderer::SetViewMatrix(XMMATRIX* ViewMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_VIEW, &XMMatrixTranspose(*ViewMatrix));
}

//SetProjectionMatrix
//
//
void D3DApp::Renderer::SetProjectionMatrix(XMMATRIX* ProjectionMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION, &XMMatrixTranspose(*ProjectionMatrix));
}

//SetProjectionMatrix2D
//
//
void D3DApp::Renderer::SetProjectionMatrix2D()
{
	SetViewMatrix(&XMMatrixIdentity());
	SetProjectionMatrix(&Renderer::GetProjectionMatrix2D());
}

//SetVertexBuffer
//
//
void D3DApp::Renderer::SetVertexBuffer(ID3D11Buffer * VertexBuffer, UINT stride, UINT offset)
{
	_pDevice->ImmediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
}

//SetIndexBuffer
//
//
void D3DApp::Renderer::SetIndexBuffer(ID3D11Buffer* IndexBuffer)
{
	_pDevice->ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

//Comment
//	SetTexture
//
void D3DApp::Renderer::SetTexture(ID3D11ShaderResourceView* texture, UINT slot, UINT numView)
{
	ID3D11ShaderResourceView* srv[1] = { texture };
	_pDevice->ImmediateContext->PSSetShaderResources(slot, numView, srv);
}

//DrawIndexed
//
//
void D3DApp::Renderer::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation)
{
	_pDevice->ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_pDevice->ImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

//GetProjectionMatrix2D
//	TODO : Shaderベースの変換にする
//
XMMATRIX D3DApp::Renderer::GetProjectionMatrix2D()
{
	return XMMatrixOrthographicOffCenterLH(0.0f, (float)_pDevice->ScreenWidth, (float)_pDevice->ScreenHeight, 0.0f, 0.0f, 1.0f);
}