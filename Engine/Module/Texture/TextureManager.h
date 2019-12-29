#pragma once

//Texture管理
//
//
class TextureManager final
{
	using TextureIndex = std::map<std::string, std::shared_ptr<Texture>>;
private:
	//Singleton Instance
	static TextureManager* g_pInstance;

	//テクスチャ保管
	TextureIndex _TextureIndex;

	//コンストラクタ
	TextureManager();
	//デストラクタ
	~TextureManager();


public:
	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Release();

	//テクスチャ読み込み aaa.xxxのaaawをテクスチャ名とする
	static std::weak_ptr<Texture> LoadTexture(std::string filePath);
	//テクスチャ取得
	static std::weak_ptr<Texture> GetTexture(std::string name);

	//エディタ用ウィンドウ
	static void EditorWindow();
};