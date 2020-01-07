
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
#include<map>
#include<memory>
#include<algorithm>

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
//	RenderStatus
//
//*********************************************************************************************************************

//RenderStatus
//	コンストラクタ
//
RenderStatus::RenderStatus(HWND hWnd, D3DRenderer * renderer,unsigned int fps)
	:
	_D3DRenderer(renderer)
{
	HRESULT hr;

	//ウィンドウサイズの取得
	RECT rect;
	GetClientRect(hWnd,&rect);
	unsigned int ScreenWidth = rect.right - rect.left;
	unsigned int ScreenHeight = rect.bottom - rect.top;

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
	sd.BufferDesc.RefreshRate.Numerator = fps;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	if (FAILED(_D3DRenderer->CreateSwapChain(&_SwapChain, &sd))) {
		MessageBox(NULL, "SwapChainの生成に失敗しました。", "RenderStatus", MB_OK);
		return;
	}

	// レンダーターゲットビュー生成、設定
	CreateRenderTargetView();

	//ステンシル用テクスチャー作成
	CreateDepthStencilView();

	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	//深度有効ステート
	_D3DRenderer->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &_DepthStateEnable);

	//depthStencilDesc.DepthEnable = FALSE; //深度無効ステート
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	_D3DRenderer->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &_DepthStateDisable);

	_D3DRenderer->GetDeviceContext()->OMSetDepthStencilState(_DepthStateEnable, NULL);

	//ビューポート設定
	_ViewPort.Width = (float)ScreenWidth;
	_ViewPort.Height = (float)ScreenHeight;
	_ViewPort.MinDepth = 0.0f;
	_ViewPort.MaxDepth = 1.0f;
	_ViewPort.TopLeftX = 0;
	_ViewPort.TopLeftY = 0;
	SetViewport(1);

	_IsReleased = false;
}

//~RendererStatus
//	デストラクタ
//
RenderStatus::~RenderStatus()
{
	if (_IsReleased) return;
	if (_RenderTargetView) _RenderTargetView->Release();
	if (_DepthStencilView) _DepthStencilView->Release();

	if (_DepthStateEnable) _DepthStateEnable->Release();
	if (_DepthStateDisable) _DepthStateDisable->Release();

	if (_SwapChain) _SwapChain->Release();
}

//CreateRenderTargetView
//	RenderTargetView生成
//
void RenderStatus::CreateRenderTargetView()
{
	ID3D11Texture2D* pBackBuffer = NULL;
	_SwapChain->GetBuffer(NULL,__uuidof(ID3D11Texture2D),(LPVOID*)&pBackBuffer);
	_D3DRenderer->GetDevice()->CreateRenderTargetView(pBackBuffer,NULL,&_RenderTargetView);
	pBackBuffer->Release();
}

//CreateDepthStencilView
//	DepthStencilViewの生成
//
void RenderStatus::CreateDepthStencilView()
{
	DXGI_SWAP_CHAIN_DESC sd = GetSwapChainDesc();
	
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
	_D3DRenderer->GetDevice()->CreateTexture2D(&td, NULL, &depthTexture);
	
	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	_D3DRenderer->GetDevice()->CreateDepthStencilView(depthTexture, &dsvd, &_DepthStencilView);
	
	_D3DRenderer->GetDeviceContext()->OMSetRenderTargets(1, &_RenderTargetView, _DepthStencilView);
}

//SetViewport
//	ビューポート設定
//
void RenderStatus::SetViewport(UINT NumViewports)
{
	_NumViewPortSrot = NumViewports;
	_D3DRenderer->GetDeviceContext()->RSSetViewports(_NumViewPortSrot, &_ViewPort);

	XMMATRIX matrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)_ViewPort.Width, (float)_ViewPort.Height, 0.0f, 0.0f, 1.0f);
	D3DRenderer::GetInstance()->GetConstantBuffer()->UpdateSubresource(CONSTANT_BUFFER_PROJECTION, &matrix);
}

void RenderStatus::SetClientViewport(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd,&rect);

	_ViewPort.Width = rect.right - rect.left;
	_ViewPort.Height = rect.bottom - rect.top;
	_ViewPort.MaxDepth = 1.0f;
	_ViewPort.MinDepth = 0.0f;
	_ViewPort.TopLeftX = 0;
	_ViewPort.TopLeftY = 0;
}

//ClearnupRenderTargetView
//	RenderTargetViewの削除・破棄
//
void RenderStatus::CleanupRenderTargetView()
{
	if (_RenderTargetView == nullptr) return;
	_RenderTargetView->Release();
	_RenderTargetView = nullptr;
}

//ClearnupDepthStencilView
//	DepthStencilViewの削除・破棄
//
void RenderStatus::CleanupDepthStencilView()
{
	if (_DepthStencilView == nullptr) return;
	_DepthStencilView->Release();
	_DepthStencilView = nullptr;
}

void RenderStatus::ClearRenderTargetView(Color clearColor)
{
	SetViewport(1);
	_D3DRenderer->GetDeviceContext()->OMSetRenderTargets(1, &_RenderTargetView, _DepthStencilView);
	_D3DRenderer->GetDeviceContext()->ClearRenderTargetView(_RenderTargetView,clearColor);
}

void RenderStatus::Begin()
{
	_D3DRenderer->GetDeviceContext()->ClearDepthStencilView(_DepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);
}

void RenderStatus::End()
{
	_SwapChain->Present(0,0);
}

void RenderStatus::SetDepthEnable(bool Enable)
{
	if (Enable)
		_D3DRenderer->GetDeviceContext()->OMSetDepthStencilState(_DepthStateDisable, NULL);
	else
		_D3DRenderer->GetDeviceContext()->OMSetDepthStencilState(_DepthStateEnable,NULL);
}

unsigned int RenderStatus::GetRefreshRate()
{
	auto sd = GetSwapChainDesc();
	return sd.BufferDesc.RefreshRate.Numerator / sd.BufferDesc.RefreshRate.Denominator;
}

void RenderStatus::Release()
{
	if (_RenderTargetView) _RenderTargetView->Release();
	if (_DepthStencilView) _DepthStencilView->Release();

	if (_DepthStateEnable) _DepthStateEnable->Release();
	if (_DepthStateDisable) _DepthStateDisable->Release();

	if (_SwapChain) _SwapChain->Release();
	_IsReleased = true;
}

bool RenderStatus::IsProcess()
{
	return _ViewPort.Width >0 && _ViewPort.Height > 0;
}

DXGI_SWAP_CHAIN_DESC RenderStatus::GetSwapChainDesc()
{
	DXGI_SWAP_CHAIN_DESC sd;
	
	//DXGI_SWAP_CHAIN_DESCの取得
	//	WARNING : Direct3D 11.1 の場合、DESC取得に推奨されない関数
	//	ref: https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getdesc
	//
	_SwapChain->GetDesc(&sd);
	return sd;
}



//*********************************************************************************************************************
//
//	D3DRenderer
//
//*********************************************************************************************************************

D3DRenderer* D3DRenderer::pInstance = nullptr;
RenderStatus* D3DRenderer::pTargetRenderStatus = nullptr;

//D3DRenderer
//	コンストラクタ
//
D3DRenderer::D3DRenderer()
{
	HRESULT hr = E_FAIL;

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
		&_D3DDevice,			//D3DDevice
		&featureLevel,		//successful FeatureLevel
		&_ImmediateContext	//DeviceContext
	);


	if (FAILED(hr)) {
		MessageBox(NULL, "Deviceの生成に失敗しました。", "D3DRenderer", MB_OK);
		return;
	}

	//QuearyInterface
	//	IDXGIDevice1 のIntarfaceを取得
	//	D3D11Device は IUnkownクラスを継承している
	//	ref : https://docs.microsoft.com/ja-jp/cpp/atl/introduction-to-com?view=vs-2019
	//
	//	COMについて
	//	ref : http://chokuto.ifdef.jp/urawaza/com/com.html	
	//
	_D3DDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&_pDXGI);

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

	// ラスタライザステート設定
	SetRasterize(D3D11_FILL_SOLID, D3D11_CULL_BACK);

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
	_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);


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
	_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	_ImmediateContext->PSSetSamplers(0, 1, &samplerState);
}

//~D3DRenderer
//	デストラクタ
//
D3DRenderer::~D3DRenderer()
{
	for(auto status : _ClientRenderStatus)
	{
		delete status.second;
	}

	if (_ConstantBuffer) delete _ConstantBuffer;
	if (_Light) delete _Light;
	if (_Material) delete _Material;
	if (_ImmediateContext)_ImmediateContext->ClearState();

	if (_ImmediateContext) _ImmediateContext->Release();
	if (_pFactory) _pFactory->Release();
	if (_pAdapter) _pAdapter->Release();
	if (_pDXGI)	_pDXGI->Release();
	if (_D3DDevice)	_D3DDevice->Release();
}

HRESULT D3DRenderer::Create()
{
	if (pInstance != nullptr) return E_FAIL;
	pInstance = new D3DRenderer();

	pInstance->_ConstantBuffer = new ConstantBuffer();
	pInstance->_ConstantBuffer->CreateBuffer();

	pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_WORLD,0);
	pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_VIEW,1);
	pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_PROJECTION,2);
	pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_MATERIAL,3);
	pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_LIGHT,4);

	pInstance->_Material = new Material();
	pInstance->_Material->SetResourceMaterial();

	pInstance->_Light = new Light();
	pInstance->_Light->SetResource();

	
	return S_OK;
}

HRESULT D3DRenderer::Destroy()
{
	if (pInstance == nullptr) return E_FAIL;
	delete pInstance;
	pInstance = nullptr;
	return S_OK;
}

RenderStatus * D3DRenderer::GetRenderStatus(HWND hWnd)
{
	return pInstance->_ClientRenderStatus.at(hWnd);
}


//CreateRenderStatus
//	RenderStatusの生成
//
HRESULT D3DRenderer::CreateRenderStatus(HWND hWnd,RenderStatus** renderstatus, unsigned int fps)
{
	HRESULT hr;

	//クライアント追加
	*renderstatus = new RenderStatus(hWnd,this,fps);

	//追加
	_ClientRenderStatus.emplace(hWnd,*renderstatus);

	return S_OK;
}

void D3DRenderer::CreateBuffer(unsigned int BindFlag, unsigned int byteWidth, const void * subresource, ID3D11Buffer ** buffer)
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

	_D3DDevice->CreateBuffer(&bd, &sd, buffer);
}

void D3DRenderer::SetVertexBuffer(ID3D11Buffer * VertexBuffer, UINT stride, UINT offset)
{
	_ImmediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
}

void D3DRenderer::SetIndexBuffer(ID3D11Buffer * IndexBuffer)
{
	_ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void D3DRenderer::SetTexture(ID3D11ShaderResourceView * texture, UINT slot, UINT numView)
{
	ID3D11ShaderResourceView* srv[1] = { texture };
	_ImmediateContext->PSSetShaderResources(slot, numView, srv);
}

void D3DRenderer::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation)
{
	_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_ImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

HRESULT D3DRenderer::CreateSwapChain(IDXGISwapChain** SwapChain, DXGI_SWAP_CHAIN_DESC* sd)
{
	return _pFactory->CreateSwapChain(_D3DDevice,sd,SwapChain);
}

void D3DRenderer::SetRasterize(D3D11_FILL_MODE fillmode, D3D11_CULL_MODE cullmode)
{
	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = fillmode;
	rd.CullMode = cullmode;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	_D3DDevice->CreateRasterizerState(&rd, &rs);

	_ImmediateContext->RSSetState(rs);
}

void D3DRenderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(&XMMatrixIdentity());
	SetProjectionMatrix2D();
}

void D3DRenderer::SetWorldMatrix(XMMATRIX * WorldMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_WORLD, &XMMatrixTranspose(*WorldMatrix));
}

void D3DRenderer::SetViewMatrix(XMMATRIX * ViewMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_VIEW, &XMMatrixTranspose(*ViewMatrix));
}

void D3DRenderer::SetProjectionMatrix(XMMATRIX * ProjectionMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION, &XMMatrixTranspose(*ProjectionMatrix));
}

void D3DRenderer::SetProjectionMatrix2D()
{
	SetViewMatrix(&XMMatrixIdentity());
	SetProjectionMatrix(&GetProjectionMatrix2D());
}

XMMATRIX D3DRenderer::GetProjectionMatrix2D()
{
	DXGI_SWAP_CHAIN_DESC sd = pTargetRenderStatus->GetSwapChainDesc();
	return XMMatrixOrthographicOffCenterLH(0.0f, (float)sd.BufferDesc.Width, (float)sd.BufferDesc.Height, 0.0f, 0.0f, 1.0f);
}

