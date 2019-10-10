#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Shlwapi.h>
#include<assert.h>

#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "font.h"

#include"Module\Texture\texture.h"

#pragma comment(lib,"shlwapi.lib")

DirectX::Font::Font()
{

}

DirectX::Font::~Font()
{
	delete this->m_texture;
	DeleteObject(this->m_font);
}

void DirectX::Font::Load(const char* fileName,const char* name)
{
	DeleteObject(this->m_font);
	DESIGNVECTOR design;

	//リソース読み込み
	//参考:https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-addfontresourceexa
	if (AddFontResourceEx(fileName, FR_PRIVATE, &design))
	{
		//Font属性について
		//参考:http://chokuto.ifdef.jp/urawaza/struct/LOGFONT.html

		//Fontの生成
		//参考:http://www.kumei.ne.jp/c_lang/sdk/sdk_27.htm
		this->m_font = CreateFont(
			0,							//文字セルまたは文字の高さ
			0,							//平均文字幅
			0,							//文字送りの方向とX軸との角度
			0,							//文字の角度
			0,							//フォントの太さ
			0,							//イタリック体指定
			0,							//下線
			0,							//抹消線
			SHIFTJIS_CHARSET,			//文字設定
			OUT_TT_ONLY_PRECIS,			//出力精度
			CLIP_DEFAULT_PRECIS,		//クリッピング精度
			PROOF_QUALITY,				//出力品質
			FIXED_PITCH | FF_MODERN,	//ピッチとファミリ
			name						//フォント名
		);

		//リソース破棄
		RemoveFontResourceEx(fileName,FR_PRIVATE,&design);
	}
}

//Charのバイトを認識する
//int len = IsDBCSLeadByte(pChar[0]) ? 2 : 1;
//UINT uChar = (len == 2 ? (unsigned char)pChar[0] << 8 | (unsigned char)pChar[1] : (unsigned char)pChar[0]);

//HFONTからFontテクスチャを生成
//参考:https://blogoftrueone.wordpress.com/2013/06/02/directx11%E3%81%A7%E6%96%87%E5%AD%97%E6%8F%8F%E7%94%BB%E3%81%A8%E3%83%95%E3%82%A9%E3%83%B3%E3%83%88%E8%AA%AD%E3%81%BF%E8%BE%BC%E3%81%BF/
//参考:http://marupeke296.com/DXG_No67_NewFont.html
//参考:http://marupeke296.com/WINT_GetGlyphOutline.html
DirectX::Texture* DirectX::Font::TransformToTexture()
{
	if (this->m_texture != nullptr)
		delete this->m_texture;

	HRESULT hr;

	//文字造形情報
	//GLYPHMETRICS gm;
	TEXTMETRIC tm;

	//テクスチャ
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	//検出範囲
	const BYTE hs_bit = 0x81;	//上位開始bit
	const BYTE he_bit = 0xea;	//上位終了bit

	const BYTE ss_bit = 0x40;	//下位開始bit
	const BYTE se_bit = 0xfc;	//下位終了bit

	const UINT s_bit = ((hs_bit << 8) | ss_bit);	//開始bit
	const UINT e_bit = ((he_bit << 8) | se_bit);	//終了bit

	const int length = e_bit - s_bit;	//配列長

	GLYPHMETRICS* index = new GLYPHMETRICS[length];	//フォント位置
	BYTE** pFontBMP		= new BYTE*[length];		//ビットマップ

	//テクスチャ
	int texSizeX = 0;
	int texSizeY = 0;

	//ビットマップの生成
	{
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, this->m_font);
		GetTextMetrics(hdc, &tm);
		const MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };

		int count = 0;
		int x = 0;
		for (BYTE hightbyte = hs_bit; hightbyte <= he_bit; hightbyte++) {
			for (BYTE subordbyte = ss_bit; subordbyte <= se_bit; subordbyte++) {
				UINT uChar = ((hightbyte << 8) | subordbyte);

				//フォントビットマップ取得
				{
					//フォントビットマップのメモリサイズ取得
					DWORD size = GetGlyphOutline(
						hdc,
						uChar,				//表示文字
						GGO_GRAY4_BITMAP,	//ビットマップ
						&index[count],		//ビットマップ情報
						0,					//ビットマップサイズ
						NULL,				//フォントビットマップ
						&mat				//特殊変換
					);

					//フォントビットマップの取得
					pFontBMP[count] = new BYTE[size];
					GetGlyphOutline(
						hdc,
						uChar,
						GGO_GRAY4_BITMAP,
						&index[count],
						size,
						pFontBMP[count],
						&mat
					);
				}

				//テクスチャの横幅
				x += index[count].gmCellIncX;
				count++;
			}
			//テクスチャサイズ
			texSizeX = texSizeX < x ? x : texSizeX;
			texSizeY += tm.tmHeight;
			x = 0;
		}

		//解放
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);
	}

	//テクスチャ生成
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = texSizeX;
		desc.Height = texSizeY;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;				//動的確保を定義
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//シェーダリソースとして定義
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		hr = D3DApp::GetDevice()->CreateTexture2D(&desc, NULL, &texture);
		if (FAILED(hr))
			assert(false);
	}

	//フォント情報をテクスチャに書き込み
	{
		D3D11_MAPPED_SUBRESOURCE hMappedResource = {};

		hr = D3DApp::GetDeviceContext()->Map(
			texture,
			NULL,
			D3D11_MAP_WRITE_DISCARD,
			NULL,
			&hMappedResource
		);

		if (FAILED(hr))
			assert(false);
		//テクスチャの画像情報を取得
		BYTE* pBits = (BYTE*)hMappedResource.pData;

		int m_x = 0, m_y = 0,count = 0;
		for (BYTE hightbyte = hs_bit; hightbyte <= he_bit; hightbyte++) {
			m_x = index[count].gmptGlyphOrigin.x;
			for (BYTE subordbyte = ss_bit; subordbyte <= se_bit; subordbyte++)
			{
				GLYPHMETRICS gm = index[count];
				//ベースラインをそろえた配置
				int iofs_y = m_y + tm.tmAscent - index[count].gmptGlyphOrigin.y;
				int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
				int iBmp_h = gm.gmBlackBoxY;

				int level = 17;

				DWORD alpha, color;
				ZeroMemory(&pBits, sizeof(BYTE));

				//データ書き込み
				for (int y = iofs_y; y < iofs_y + iBmp_h; y++){
					for (int x = m_x; x < m_x + iBmp_w; x++){
						BYTE bot = pFontBMP[count][x - m_x + iBmp_w*(y - iofs_y)];
						alpha = (255 * bot) / (level - 1);
						color = 0x00ffffff | (alpha << 24);
						//セット
						memcpy(
							(BYTE*)pBits + hMappedResource.RowPitch*y + 4 * x,
							&color,
							sizeof(DWORD)
						);
					}
				}
				m_x += iBmp_w;
				count++;
			}
			m_y += tm.tmHeight;
		}
	}
	D3DApp::GetDeviceContext()->Unmap(texture, 0);

	//解放処理
	{
		for (int count = 0; count < sizeof(pFontBMP)/sizeof(pFontBMP[0]); count++)
			delete pFontBMP[count];

		delete[] pFontBMP;
		delete[] index;
	}

	//SRV設定
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = D3DApp::GetDevice()->CreateShaderResourceView(texture,&SRVDesc,&srv);
	}

	this->m_texture = new Texture(texture, srv);
	return this->m_texture;
}
