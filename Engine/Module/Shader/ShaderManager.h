#pragma once

#define DEFAULT_SHADER ("Default")

//*********************************************************************************************************************
//
//	ShaderManager
//
//*********************************************************************************************************************
class ShaderManager final
{
private:
	//インスタンス
	static ShaderManager* pInstance;
	//リソース インデックス
	std::map<std::string, std::shared_ptr<Shader>> _ShaderIndex;

	//コンストラクタ
	ShaderManager();
	//デストラクタ
	~ShaderManager();


public:
	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Release();

	//Shaderの追加
	static std::weak_ptr<Shader> RegisterShader(std::string name,Shader* shader);
	static std::weak_ptr<Shader> RegisterShader(std::string name,std::string vertexShader,VERTEX_INPUT_LAYOUT* layout,unsigned int size, std::string PixelShader);
	//Shaderの取得
	static std::weak_ptr<Shader> GetShader(std::string name = DEFAULT_SHADER);


};