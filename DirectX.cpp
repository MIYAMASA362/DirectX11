#define _CRT_SECURE_NO_WARNINGS

#include<Windows.h>
#include<stdio.h>
#include<io.h>
#include<string>

#include"main.h"
#include"Window.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"texture.h"
#include"manager.h"
#include"Time.h"

#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Tag.h"
#include"Renderer.h"
#include"GameObject.h"
#include"Behaviour.h"

#include"Tag.h"
#include"SceneManager.h"
#include"camera.h"

using namespace DirectX;

//--- D3DApp ------------------------------------------------------------------

D3DApp* D3DApp::pInstance = NULL;

HRESULT D3DApp::Create(System::Window* pAppWind)
{
	HRESULT hr = E_FAIL;
	if (pInstance) return hr;
	pInstance = new D3DApp();

	pInstance->hWnd = pAppWind->Get_Window();

	RECT rect;
	GetClientRect(pInstance->hWnd,&rect);
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
	sd.OutputWindow = pInstance->hWnd;
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
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	pInstance->D3DDevice->CreateRasterizerState(&rd, &rs);

	pInstance->ImmediateContext->RSSetState(rs);




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

	pInstance->D3DDevice->CreateDepthStencilState(&depthStencilDesc, &pInstance->DepthStateEnable);//深度有効ステート

																				 //depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pInstance->D3DDevice->CreateDepthStencilState(&depthStencilDesc, &pInstance->DepthStateDisable);//深度無効ステート

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




	// 頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("vertexShader.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		pInstance->D3DDevice->CreateVertexShader(buffer, fsize, NULL, &pInstance->VertexShader);


		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);

		pInstance->D3DDevice->CreateInputLayout(layout,
			numElements,
			buffer,
			fsize,
			&pInstance->VertexLayout);

		delete[] buffer;
	}



	// ピクセルシェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("pixelShader.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		pInstance->D3DDevice->CreatePixelShader(buffer, fsize, NULL, &pInstance->PixelShader);

		delete[] buffer;
	}

	// 入力レイアウト設定
	pInstance->ImmediateContext->IASetInputLayout(pInstance->VertexLayout);

	// シェーダ設定
	pInstance->ImmediateContext->VSSetShader(pInstance->VertexShader, NULL, 0);
	pInstance->ImmediateContext->PSSetShader(pInstance->PixelShader, NULL, 0);



	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	pInstance->D3DDevice->CreateBuffer(&hBufferDesc, NULL, &pInstance->WorldBuffer);
	pInstance->ImmediateContext->VSSetConstantBuffers(0, 1, &pInstance->WorldBuffer);

	pInstance->D3DDevice->CreateBuffer(&hBufferDesc, NULL, &pInstance->ViewBuffer);
	pInstance->ImmediateContext->VSSetConstantBuffers(1, 1, &pInstance->ViewBuffer);

	pInstance->D3DDevice->CreateBuffer(&hBufferDesc, NULL, &pInstance->ProjectionBuffer);
	pInstance->ImmediateContext->VSSetConstantBuffers(2, 1, &pInstance->ProjectionBuffer);


	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	pInstance->D3DDevice->CreateBuffer(&hBufferDesc, NULL, &pInstance->MaterialBuffer);
	pInstance->ImmediateContext->VSSetConstantBuffers(3, 1, &pInstance->MaterialBuffer);


	hBufferDesc.ByteWidth = sizeof(LIGHT);

	pInstance->D3DDevice->CreateBuffer(&hBufferDesc, NULL, &pInstance->LightBuffer);
	pInstance->ImmediateContext->VSSetConstantBuffers(4, 1, &pInstance->LightBuffer);


	// ライト初期化
	LIGHT light;
	light.Direction = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	light.Diffuse = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Ambient = COLOR(0.1f, 0.1f, 0.1f, 1.0f);
	Renderer::SetLight(light);


	// マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	return S_OK;
}

void D3DApp::Destroy()
{
	if (!pInstance) return;

	if (pInstance->WorldBuffer) pInstance->WorldBuffer->Release();
	if (pInstance->ViewBuffer) pInstance->ViewBuffer->Release();
	if (pInstance->ProjectionBuffer) pInstance->ProjectionBuffer->Release();

	if (pInstance->MaterialBuffer) pInstance->MaterialBuffer->Release();
	if (pInstance->VertexLayout) pInstance->VertexLayout->Release();
	if (pInstance->VertexShader) pInstance->VertexShader->Release();
	if (pInstance->PixelShader) pInstance->PixelShader->Release();

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

unsigned int D3DApp::GetScreenWidth()
{
	return pInstance->ScreenWidth;
}

unsigned int D3DApp::GetScreenHeight()
{
	return pInstance->ScreenHeight;
}


int D3DApp::Run(unsigned int fps)
{
	//フレームカウント初期化
	TimeManager::Create(fps);

	CManager::Init();

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			TimeManager::Update();
			
			bool IsUpdate = TimeManager::IsUpdate();
			bool IsFixedUpdate = TimeManager::IsFixedUpdate();

			//更新処理
			if (IsUpdate) CManager::Update();

			//一定更新
			if(IsFixedUpdate) CManager::FixedUpdate();

			//Destroyされた物を削除
			if (IsFixedUpdate || IsUpdate) SceneManager::CleanUp();

			// 描画処理
			if(IsUpdate)
			{
				pInstance->ImmediateContext->ClearRenderTargetView(pInstance->RenderTargetView, Color::gray());
				Camera::BeginRun(CManager::Render,D3DApp::Renderer::Begin);
				D3DApp::Renderer::End();
			}
		}
	}

	CManager::Uninit();

	Camera::Release();
	TimeManager::Destroy();

	D3DApp::Destroy();
	
	return (int)msg.wParam;
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

void D3DApp::Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		pInstance->ImmediateContext->OMSetDepthStencilState(pInstance->DepthStateEnable, NULL);
	else
		pInstance->ImmediateContext->OMSetDepthStencilState(pInstance->DepthStateDisable, NULL);
}

void D3DApp::Renderer::SetWorldViewProjection2D()
{
	XMMATRIX world;
	world = XMMatrixIdentity();
	pInstance->ImmediateContext->UpdateSubresource(pInstance->WorldBuffer, 0, NULL, &XMMatrixTranspose(world), 0, 0);

	XMMATRIX view;
	view = XMMatrixIdentity();
	pInstance->ImmediateContext->UpdateSubresource(pInstance->ViewBuffer, 0, NULL, &XMMatrixTranspose(view), 0, 0);

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, (float)pInstance->ScreenWidth, (float)pInstance->ScreenHeight, 0.0f, 0.0f, 1.0f);
	pInstance->ImmediateContext->UpdateSubresource(pInstance->ProjectionBuffer, 0, NULL, &XMMatrixTranspose(projection), 0, 0);
}

void D3DApp::Renderer::SetWorldMatrix(XMMATRIX* WorldMatrix)
{
	XMMATRIX world;
	world = *WorldMatrix;
	pInstance->ImmediateContext->UpdateSubresource(pInstance->WorldBuffer, 0, NULL, &XMMatrixTranspose(world), 0, 0);
}

void D3DApp::Renderer::SetViewMatrix(XMMATRIX* ViewMatrix)
{
	XMMATRIX view;
	view = *ViewMatrix;
	pInstance->ImmediateContext->UpdateSubresource(pInstance->ViewBuffer, 0, NULL, &XMMatrixTranspose(view), 0, 0);
}

void D3DApp::Renderer::SetProjectionMatrix(XMMATRIX* ProjectionMatrix)
{
	XMMATRIX projection;
	projection = *ProjectionMatrix;
	pInstance->ImmediateContext->UpdateSubresource(pInstance->ProjectionBuffer, 0, NULL, &XMMatrixTranspose(projection), 0, 0);
}

void D3DApp::Renderer::SetMaterial(MATERIAL Material)
{
	pInstance->ImmediateContext->UpdateSubresource(pInstance->MaterialBuffer, 0, NULL, &Material, 0, 0);
}

void D3DApp::Renderer::SetLight(LIGHT Light)
{
	pInstance->ImmediateContext->UpdateSubresource(pInstance->LightBuffer, 0, NULL, &Light, 0, 0);
}

void D3DApp::Renderer::SetVertexBuffer(ID3D11Buffer* VertexBuffer)
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { VertexBuffer };
	pInstance->ImmediateContext->IASetVertexBuffers(0, 1, vb, &stride, &offset);
}

void D3DApp::Renderer::SetIndexBuffer(ID3D11Buffer* IndexBuffer)
{
	pInstance->ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void D3DApp::Renderer::SetTexture(CTexture* Texture)
{
	ID3D11ShaderResourceView* srv[1] = { Texture->GetShaderResourceView() };
	pInstance->ImmediateContext->PSSetShaderResources(0, 1, srv);
}

void D3DApp::Renderer::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation)
{
	pInstance->ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pInstance->ImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}