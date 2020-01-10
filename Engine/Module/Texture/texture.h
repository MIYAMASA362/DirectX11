#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

//Texture
class Texture final
{
private:
	std::string _filePath;				//ファイルパス

	ID3D11Texture2D* _texture;			//テクスチャ
	ID3D11ShaderResourceView* _srv;		//シェーダーリソース

public:
	Texture();
	Texture(ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv);
	~Texture();

	ID3D11Texture2D* GetTexture(){ return _texture; };
	ID3D11ShaderResourceView* GetShaderResourceView() { return _srv; };

	//リソース設定
	void SetResource();
	//テクスチャ読み込み
	void LoadTexture(std::string filePath);
};
