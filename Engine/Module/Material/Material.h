#pragma once

class Texture;
struct aiMaterial;

//*********************************************************************************************************************
//
//	Material
//
//*********************************************************************************************************************
class Material final
{
public:
	struct Constant
	{
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Emission;
		float Shininess;
		float Dummy[3];	//16bitサイズに合わせる
	};

	//テクスチャ
	std::weak_ptr<Texture> _Texture;
	//シェーダ
	std::weak_ptr<Shader> _Shader;
	//マテリアル色
	Material::Constant _constant;


public:
	//コンストラクタ
	Material();
	//デストラクタ
	virtual ~Material();

	//リソース設定
	void SetResource();

	void SetResourceMaterial();
	void SetResourceTexture();
	void SetResourceShader();


};