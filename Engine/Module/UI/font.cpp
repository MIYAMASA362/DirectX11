#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Shlwapi.h>
#include<assert.h>
#include<string>

#include<d3d11.h>
#include<DirectXMath.h>

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
	DESIGNVECTOR design;

	//���\�[�X�ǂݍ���
	//�Q�l:https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-addfontresourceexa
	if (AddFontResourceEx(fileName, FR_PRIVATE, &design))
	{
		//Font�����ɂ���
		//�Q�l:http://chokuto.ifdef.jp/urawaza/struct/LOGFONT.html

		//Font�̐���
		//�Q�l:http://www.kumei.ne.jp/c_lang/sdk/sdk_27.htm
		this->m_font = CreateFont(
			0,							//�����Z���܂��͕����̍���
			0,							//���ϕ�����
			0,							//��������̕�����X���Ƃ̊p�x
			0,							//�����̊p�x
			0,							//�t�H���g�̑���
			0,							//�C�^���b�N�̎w��
			0,							//����
			0,							//������
			SHIFTJIS_CHARSET,			//�����ݒ�
			OUT_TT_ONLY_PRECIS,			//�o�͐��x
			CLIP_DEFAULT_PRECIS,		//�N���b�s���O���x
			PROOF_QUALITY,				//�o�͕i��
			FIXED_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~��
			name						//�t�H���g��
		);

		//���\�[�X�j��
		RemoveFontResourceEx(fileName,FR_PRIVATE,&design);
	}
}

//HFONT����Font�e�N�X�`���𐶐�
//�Q�l:https://blogoftrueone.wordpress.com/2013/06/02/directx11%E3%81%A7%E6%96%87%E5%AD%97%E6%8F%8F%E7%94%BB%E3%81%A8%E3%83%95%E3%82%A9%E3%83%B3%E3%83%88%E8%AA%AD%E3%81%BF%E8%BE%BC%E3%81%BF/
//�Q�l:http://marupeke296.com/DXG_No67_NewFont.html
//�Q�l:http://marupeke296.com/WINT_GetGlyphOutline.html
DirectX::Texture* DirectX::Font::TransformToTexture(const char* pChar)
{
	HRESULT hr;

	//�������`���
	GLYPHMETRICS gm;
	TEXTMETRIC tm;

	int len = IsDBCSLeadByte(pChar[0]) ? 2 : 1;
	UINT uChar = (len == 2 ? (unsigned char)pChar[0] << 8 | (unsigned char)pChar[1] : (unsigned char)pChar[0]);

	BYTE* pFontBMP;				//�t�H���g�r�b�g�}�b�v
	ID3D11Texture2D* texture;	//�e�N�X�`��
	ID3D11ShaderResourceView* srv;
	
	//�t�H���g�r�b�g�}�b�v�擾
	{
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, this->m_font);
		GetTextMetrics(hdc, &tm);
		const MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };

		//�t�H���g�r�b�g�}�b�v�̃������T�C�Y�擾
		DWORD size = GetGlyphOutline(
			hdc,
			uChar,				//�\������
			GGO_GRAY4_BITMAP,	//�r�b�g�}�b�v
			&gm,				//�r�b�g�}�b�v���
			0,					//�r�b�g�}�b�v�T�C�Y
			NULL,				//�t�H���g�r�b�g�}�b�v
			&mat				//����ϊ�
		);

		//�t�H���g�r�b�g�}�b�v�̎擾
		pFontBMP = new BYTE[size];
		GetGlyphOutline(
			hdc,
			uChar,
			GGO_GRAY4_BITMAP,
			&gm,
			size,
			pFontBMP,
			&mat
		);

		//���
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);
	}

	int charNum = 2;

	//�e�N�X�`������
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc,sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = gm.gmCellIncX;
		desc.Height = tm.tmHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;				//���I�m�ۂ��`
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X�Ƃ��Ē�`
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		hr = D3DApp::GetDevice()->CreateTexture2D(&desc,NULL,&texture);
		if (FAILED(hr))
			assert(false);

		//�t�H���g�����e�N�X�`���ɏ�������
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
		
		//�e�N�X�`���̉摜�����擾
		BYTE* pBits = (BYTE*)hMappedResource.pData;

		//�x�[�X���C�������낦���z�u
		int iofs_x = (gm.gmptGlyphOrigin.x);
		int iofs_y = (tm.tmAscent - gm.gmptGlyphOrigin.y);
		int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX %4)) %4;
		int iBmp_h = gm.gmBlackBoxY;

		int level = 17;

		DWORD alpha, color;
		ZeroMemory(&pBits,sizeof(BYTE));
		for(int y = iofs_y,loopY = iofs_y+iBmp_h ; y <loopY; y++)
			for(int x = iofs_x,loopX = iofs_x + iBmp_w; x < loopX; x++)
			{
				alpha = (255 * pFontBMP[x - iofs_x+iBmp_w*(y-iofs_y)])/(level-1);
				color = 0x00ffffff | (alpha << 24);

				memcpy(
					(BYTE*)pBits + hMappedResource.RowPitch*y + 4 * x,
					&color,
					sizeof(DWORD)
				);
			}

		D3DApp::GetDeviceContext()->Unmap(texture,0);
	}

	//SRV�ݒ�
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
