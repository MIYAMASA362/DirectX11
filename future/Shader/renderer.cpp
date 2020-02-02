
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include <io.h>


D3D_FEATURE_LEVEL       CRenderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           CRenderer::m_D3DDevice = NULL;
ID3D11DeviceContext*    CRenderer::m_ImmediateContext = NULL;
IDXGISwapChain*         CRenderer::m_SwapChain = NULL;

ID3D11RenderTargetView* CRenderer::m_RenderTargetView = NULL;
ID3D11DepthStencilView* CRenderer::m_DepthStencilView = NULL;

ID3D11DepthStencilState* CRenderer::m_DepthStateEnable;
ID3D11DepthStencilState* CRenderer::m_DepthStateDisable;

ID3D11DepthStencilView*   CRenderer::m_ShadowDepthStencilView;
ID3D11ShaderResourceView* CRenderer::m_ShadowDepthShaderResourceView;


void CRenderer::Init()
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetWindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_ImmediateContext);


	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();

	//ステンシル用テクスチャ(リソース)
	ID3D11Texture2D* depthTexture = NULL;

	//ステンシル用テクスチャ設定
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td,sizeof(td));
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32_TYPELESS;	//ステンシルバッファ形式
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;	//ステンシル取得フラグ | シェーダーリソースフラグ
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	//テクスチャ(リソース)の生成
	m_D3DDevice->CreateTexture2D(&td,NULL,&depthTexture);

	//ステンシルターゲット(サブリソース)の設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//ステンシルリソースに2Dテクスチャデータ形式でアクセス
	dsvd.Flags = 0;	//読み取り専用フラグ

	//リソースデータへのアクセス(サブリソース)の生成
	m_D3DDevice->CreateDepthStencilView(depthTexture,&dsvd,&m_DepthStencilView);

	//出力マージステージのレンダー対象設定
	m_ImmediateContext->OMSetRenderTargets(1,&m_RenderTargetView,m_DepthStencilView);

	////シャドウ用リソース設定
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;	//Double型 R値形式

	//シャドウ用サブリソースの作成
	m_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_ShadowDepthStencilView);

	//シェーダーリソース設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = DXGI_FORMAT_R32_FLOAT;	//float R値形式
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//2DTextureとして取得
	srvd.Texture2D.MipLevels = 1;

	//シェーダーリソースの生成
	m_D3DDevice->CreateShaderResourceView(depthTexture, &srvd, &m_ShadowDepthShaderResourceView);
	

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports( 1, &vp );



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_NONE; 
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 

	ID3D11RasterizerState *rs;
	m_D3DDevice->CreateRasterizerState( &rd, &rs );

	m_ImmediateContext->RSSetState( rs );




	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
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

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState( &blendDesc, &blendState );
	m_ImmediateContext->OMSetBlendState( blendState, blendFactor, 0xffffffff );



	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateDisable );//深度無効ステート

	m_ImmediateContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState( &samplerDesc, &samplerState );

	m_ImmediateContext->PSSetSamplers( 0, 1, &samplerState );



}



void CRenderer::Uninit()
{
	// オブジェクト解放
	if( m_ImmediateContext )	m_ImmediateContext->ClearState();
	if( m_RenderTargetView )	m_RenderTargetView->Release();
	if( m_SwapChain )			m_SwapChain->Release();
	if( m_ImmediateContext )	m_ImmediateContext->Release();
	if( m_D3DDevice )			m_D3DDevice->Release();

}



void CRenderer::Begin()
{
	// バックバッファクリア
	float ClearColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
	//レンダーターゲットを塗りつぶし
	m_ImmediateContext->ClearRenderTargetView( m_RenderTargetView, ClearColor );

	//ステンシルリソースをクリア
	m_ImmediateContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//レンダーターゲット設定
	m_ImmediateContext->OMSetRenderTargets(1,&m_RenderTargetView,m_DepthStencilView);

}



void CRenderer::End()
{
	m_SwapChain->Present( 1, 0 );
}




void CRenderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_ImmediateContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_ImmediateContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}


void CRenderer::SetVertexBuffers( ID3D11Buffer* VertexBuffer )
{

	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { VertexBuffer };
	m_ImmediateContext->IASetVertexBuffers( 0, 1, vb, &stride, &offset );

}



void CRenderer::SetIndexBuffer( ID3D11Buffer* IndexBuffer )
{

	m_ImmediateContext->IASetIndexBuffer( IndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

}


void CRenderer::SetTexture(ID3D11ShaderResourceView** TextureIndex ,UINT slotNum,UINT viewNum)
{
	m_ImmediateContext->PSSetShaderResources( slotNum, viewNum, TextureIndex );

}


void CRenderer::DrawIndexed( unsigned int IndexCount, unsigned int StartIndexLocation, int BaseVertexLocation )
{

	m_ImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_ImmediateContext->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );

}

void CRenderer::BeginShadow()
{
	m_ImmediateContext->OMSetRenderTargets(0,NULL,m_ShadowDepthStencilView);
	m_ImmediateContext->ClearDepthStencilView(m_ShadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::SetShadowTexture(unsigned int slot)
{
	ID3D11ShaderResourceView* src[1] = {
		m_ShadowDepthShaderResourceView
	};

	m_ImmediateContext->PSSetShaderResources(slot,1,src);

}

