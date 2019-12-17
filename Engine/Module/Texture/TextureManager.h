#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

//Texture管理
class TextureManager final
{
	using TextureIndex = std::map<std::string, std::shared_ptr<Texture>>;
private:
	//テクスチャ保管
	static TextureIndex textureIndex;

	TextureManager() = delete;
	~TextureManager() = delete;
public:
	//テクスチャ読み込み aaa.xxxのaaawをテクスチャ名とする
	static std::weak_ptr<Texture> LoadTexture(std::string filePath);
	//解放
	static void Release();
	//テクスチャ取得
	static std::weak_ptr<Texture> GetTexture(std::string name);
};