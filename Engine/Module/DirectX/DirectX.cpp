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
#include"DirectXStruct.h"
#include"DirectX.h"

#include"Module\Shader\Shader.h"
#include"Module\Light\Light.h"
#include"Module\Material\Material.h"

using namespace DirectX;

//--- D3DApp ------------------------------------------------------------------

D3DApp* D3DApp::pInstance = NULL;

HRESULT D3DApp::Create(HWND hWnd, HINSTANCE hInstance,unsigned int fps)
{
	HRESULT hr = E_FAIL;
	if (pInstance) return hr;

	pInstance = new D3DApp();
	pInstance->hWnd = hWnd;
	pInstance->hInstance = hInstance;
	pInstance->fps = fps;


	RECT rect;
	GetClientRect(hWnd,&rect);
	pInstance->ScreenWidth = rect.right - rect.left;
	pInstance->ScreenHeight = rect.bottom - rect.top;

	// デバイス、スワップチェーン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = pInstance->ScreenWidth;
	sd.BufferDesc.Height = pInstance->ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pInstance->SwapChain,
		&pInstance->D3DDevice,
		&pInstance->featureLevel,
		&pInstance->ImmediateContext
	);

	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	pInstance->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pInstance->D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pInstance->RenderTargetView);
	pBackBuffer->Release();


	//ステンシル用テクスチャー作成
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
	pInstance->D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	pInstance->D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &pInstance->DepthStencilView);


	pInstance->ImmediateContext->OMSetRenderTargets(1, &pInstance->RenderTargetView, pInstance->DepthStencilView);


	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)pInstance->ScreenWidth;
	vp.Height = (float)pInstance->ScreenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pInstance->ImmediateContext->RSSetViewports(1, &vp);



	// ラスタライザステート設定
	Renderer::SetRasterize(D3D11_FILL_SOLID,D3D11_CULL_BACK);

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
	pInstance->D3DDevice->CreateBlendState(&blendDesc, &blendState);
	pInstance->ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);




	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	//深度有効ステート
	pInstance->D3DDevice->CreateDepthStencilState(&depthStencilDesc, &pInstance->DepthStateEnable);

	//depthStencilDesc.DepthEnable = FALSE; //深度無効ステート
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pInstance->D3DDevice->CreateDepthStencilState(&depthStencilDesc, &pInstance->DepthStateDisable);

	pInstance->ImmediateContext->OMSetDepthStencilState(pInstance->DepthStateEnable, NULL);




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
	pInstance->D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	pInstance->ImmediateContext->PSSetSamplers(0, 1, &samplerState);

	//シェーダ
	{
		//定数バッファ生成
		pInstance->_ConstantBuffer = new ConstantBuffer();
		pInstance->_ConstantBuffer->CreateBuffer();

		//Light
		pInstance->_Light = new Light();

		//Material
		pInstance->_Material = new Material();

		//シェーダ設定
		pInstance->_Shader = new Shader();
		VERTEX_INPUT_LAYOUT layout[] =
		{
			VSIL_POSITION,
			VSIL_NORMAL,
			VSIL_COLOR,
			VSIL_TEXCOORD
		};
		pInstance->_Shader->SetVertexShader("Asset/Shader/vertexShader.cso", layout, ARRAYSIZE(layout));
		pInstance->_Shader->SetPixelShader("Asset/Shader/pixelShader.cso");

		pInstance->_Shader->SetShader();

		//定数バッファ設定
		pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_WORLD, 0);
		pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_VIEW, 1);
		pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_PROJECTION, 2);
		pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_MATERIAL, 3);
		pInstance->_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_LIGHT, 4);

		pInstance->_Material->SetResource();
		pInstance->_Light->SetResource();

		pInstance->_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION, &Renderer::GetProjectionMatrix2D());
	}

	return S_OK;
}

void D3DApp::Destroy()
{
	if (!pInstance) return;
	if (pInstance->_ConstantBuffer) delete pInstance->_ConstantBuffer;
	if (pInstance->_Shader)	delete pInstance->_Shader;

	if (pInstance->ImmediateContext)	pInstance->ImmediateContext->ClearState();
	if (pInstance->RenderTargetView)	pInstance->RenderTargetView->Release();
	if (pInstance->SwapChain)			pInstance->SwapChain->Release();
	if (pInstance->ImmediateContext)	pInstance->ImmediateContext->Release();
	if (pInstance->D3DDevice)			pInstance->D3DDevice->Release();
}

ID3D11Device* D3DApp::GetDevice()
{
	return pInstance->D3DDevice;
}

ID3D11DeviceContext* D3DApp::GetDeviceContext()
{
	return pInstance->ImmediateContext;
}

HWND D3DApp::GetWindow()
{
	return pInstance->hWnd;
}

HINSTANCE D3DApp::GethInstance()
{
	return pInstance->hInstance;
}

unsigned int D3DApp::GetScreenWidth()
{
	return pInstance->ScreenWidth;
}

unsigned int D3DApp::GetScreenHeight()
{
	return pInstance->ScreenHeight;
}

unsigned int D3DApp::GetFps()
{
	return pInstance->fps;
}

//--- D3DApp::Renderer --------------------------------------------------------

void D3DApp::Renderer::Begin()
{
	pInstance->ImmediateContext->ClearDepthStencilView(pInstance->DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DApp::Renderer::End()
{
	pInstance->SwapChain->Present(1, 0);
}

void D3DApp::Renderer::ClearRenderTargetView(Color clearColor)
{
	pInstance->ImmediateContext->ClearRenderTargetView(pInstance->RenderTargetView, clearColor);
}

void D3DApp::Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		pInstance->ImmediateContext->OMSetDepthStencilState(pInstance->DepthStateEnable, NULL);
	else
		pInstance->ImmediateContext->OMSetDepthStencilState(pInstance->DepthStateDisable, NULL);
}

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
	pInstance->D3DDevice->CreateRasterizerState(&rd, &rs);

	pInstance->ImmediateContext->RSSetState(rs);
}

void D3DApp::Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(&XMMatrixIdentity());
	SetProjectionMatrix2D();
}

void D3DApp::Renderer::SetWorldMatrix(XMMATRIX* WorldMatrix)
{
	pInstance->_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_WORLD, &XMMatrixTranspose(*WorldMatrix));
}

void D3DApp::Renderer::SetViewMatrix(XMMATRIX* ViewMatrix)
{
	pInstance->_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_VIEW,&XMMatrixTranspose(*ViewMatrix));
}

void D3DApp::Renderer::SetProjectionMatrix(XMMATRIX* ProjectionMatrix)
{
	pInstance->_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION,&XMMatrixTranspose(*ProjectionMatrix));
}

void D3DApp::Renderer::SetProjectionMatrix2D()
{
	SetViewMatrix(&XMMatrixIdentity());
	SetProjectionMatrix(&Renderer::GetProjectionMatrix2D());
}

void D3DApp::Renderer::SetVertexBuffer(ID3D11Buffer * VertexBuffer, UINT stride, UINT offset)
{
	pInstance->ImmediateContext->IASetVertexBuffers(0,1,&VertexBuffer,&stride,&offset);
}

void D3DApp::Renderer::SetIndexBuffer(ID3D11Buffer* IndexBuffer)
{
	pInstance->ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void D3DApp::Renderer::SetTexture(ID3D11ShaderResourceView* texture,UINT slot,UINT numView)
{
	ID3D11ShaderResourceView* srv[1] = { texture };
	pInstance->ImmediateContext->PSSetShaderResources(slot, numView, srv);
}

void D3DApp::Renderer::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation)
{
	pInstance->ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pInstance->ImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

XMMATRIX D3DApp::Renderer::GetProjectionMatrix2D()
{
	return XMMatrixOrthographicOffCenterLH(0.0f, (float)pInstance->ScreenWidth, (float)pInstance->ScreenHeight, 0.0f, 0.0f, 1.0f);
}
