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

//Char�̃o�C�g��F������
//int len = IsDBCSLeadByte(pChar[0]) ? 2 : 1;
//UINT uChar = (len == 2 ? (unsigned char)pChar[0] << 8 | (unsigned char)pChar[1] : (unsigned char)pChar[0]);

//HFONT����Font�e�N�X�`���𐶐�
//�Q�l:https://blogoftrueone.wordpress.com/2013/06/02/directx11%E3%81%A7%E6%96%87%E5%AD%97%E6%8F%8F%E7%94%BB%E3%81%A8%E3%83%95%E3%82%A9%E3%83%B3%E3%83%88%E8%AA%AD%E3%81%BF%E8%BE%BC%E3%81%BF/
//�Q�l:http://marupeke296.com/DXG_No67_NewFont.html
//�Q�l:http://marupeke296.com/WINT_GetGlyphOutline.html
DirectX::Texture* DirectX::Font::TransformToTexture()
{
	if (this->m_texture != nullptr)
		delete this->m_texture;

	HRESULT hr;

	//�������`���
	//GLYPHMETRICS gm;
	TEXTMETRIC tm;

	//�e�N�X�`��
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	//���o�͈�
	const BYTE hs_bit = 0x81;	//��ʊJ�nbit
	const BYTE he_bit = 0xea;	//��ʏI��bit

	const BYTE ss_bit = 0x40;	//���ʊJ�nbit
	const BYTE se_bit = 0xfc;	//���ʏI��bit

	const UINT s_bit = ((hs_bit << 8) | ss_bit);	//�J�nbit
	const UINT e_bit = ((he_bit << 8) | se_bit);	//�I��bit

	const int length = e_bit - s_bit;	//�z��

	GLYPHMETRICS* index = new GLYPHMETRICS[length];	//�t�H���g�ʒu
	BYTE** pFontBMP		= new BYTE*[length];		//�r�b�g�}�b�v

	//�e�N�X�`��
	int texSizeX = 0;
	int texSizeY = 0;

	//�r�b�g�}�b�v�̐���
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

				//�t�H���g�r�b�g�}�b�v�擾
				{
					//�t�H���g�r�b�g�}�b�v�̃������T�C�Y�擾
					DWORD size = GetGlyphOutline(
						hdc,
						uChar,				//�\������
						GGO_GRAY4_BITMAP,	//�r�b�g�}�b�v
						&index[count],		//�r�b�g�}�b�v���
						0,					//�r�b�g�}�b�v�T�C�Y
						NULL,				//�t�H���g�r�b�g�}�b�v
						&mat				//����ϊ�
					);

					//�t�H���g�r�b�g�}�b�v�̎擾
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

				//�e�N�X�`���̉���
				x += index[count].gmCellIncX;
				count++;
			}
			//�e�N�X�`���T�C�Y
			texSizeX = texSizeX < x ? x : texSizeX;
			texSizeY += tm.tmHeight;
			x = 0;
		}

		//���
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);
	}

	//�e�N�X�`������
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
		desc.Usage = D3D11_USAGE_DYNAMIC;				//���I�m�ۂ��`
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X�Ƃ��Ē�`
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		hr = D3DApp::GetDevice()->CreateTexture2D(&desc, NULL, &texture);
		if (FAILED(hr))
			assert(false);
	}

	//�t�H���g�����e�N�X�`���ɏ�������
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
		//�e�N�X�`���̉摜�����擾
		BYTE* pBits = (BYTE*)hMappedResource.pData;

		int m_x = 0, m_y = 0,count = 0;
		for (BYTE hightbyte = hs_bit; hightbyte <= he_bit; hightbyte++) {
			m_x = index[count].gmptGlyphOrigin.x;
			for (BYTE subordbyte = ss_bit; subordbyte <= se_bit; subordbyte++)
			{
				GLYPHMETRICS gm = index[count];
				//�x�[�X���C�������낦���z�u
				int iofs_y = m_y + tm.tmAscent - index[count].gmptGlyphOrigin.y;
				int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
				int iBmp_h = gm.gmBlackBoxY;

				int level = 17;

				DWORD alpha, color;
				ZeroMemory(&pBits, sizeof(BYTE));

				//�f�[�^��������
				for (int y = iofs_y; y < iofs_y + iBmp_h; y++){
					for (int x = m_x; x < m_x + iBmp_w; x++){
						BYTE bot = pFontBMP[count][x - m_x + iBmp_w*(y - iofs_y)];
						alpha = (255 * bot) / (level - 1);
						color = 0x00ffffff | (alpha << 24);
						//�Z�b�g
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

	//�������
	{
		for (int count = 0; count < sizeof(pFontBMP)/sizeof(pFontBMP[0]); count++)
			delete pFontBMP[count];

		delete[] pFontBMP;
		delete[] index;
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
