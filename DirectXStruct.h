#pragma once

namespace DirectX
{
	typedef int Count;

	//頂点構造体 : Position,Normal,Diffuse,TexCoord
	struct VERTEX_3D
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT4 Diffuse;
		XMFLOAT2 TexCoord;
	};

	//tagVector3
	typedef struct tagVector3 :public XMFLOAT3

	{
		tagVector3() :XMFLOAT3(0.0f, 0.0f, 0.0f) {}
		tagVector3(float _x, float _y, float _z) :XMFLOAT3(_x, _y, _z) {}

		static tagVector3 zero() { return tagVector3(0.0f, 0.0f, 0.0f); };
		static tagVector3 one() { return tagVector3(1.0f, 1.0f, 1.0f); };
		static tagVector3 up() { return tagVector3(0.0f, 1.0f, 0.0f); };
		static tagVector3 down() { return tagVector3(0.0f, -1.0f, 0.0f); };
		static tagVector3 right() { return tagVector3(1.0f, 0.0f, 0.0f); };
		static tagVector3 left() { return tagVector3(-1.0f, 0.0f, 0.0f); };
		static tagVector3 forward() { return tagVector3(0.0f, 0.0f, 1.0f); };
		static tagVector3 back() { return tagVector3(0.0f, 0.0f, -1.0f); };

		operator const float*() { return &x; }
		operator const XMVECTOR() { return XMVectorSet(x, y, z, 0.0f); };

	}Vector3;


	//tagVector4
	typedef struct tagVector4 :public XMFLOAT4
	{
		tagVector4() :XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}
		tagVector4(float _x, float _y, float _z, float _w) :XMFLOAT4(_x, _y, _z, _w) {}

		static tagVector4 zero() { return tagVector4(0.0f, 0.0f, 0.0f, 0.0f); };
		static tagVector4 one() { return tagVector4(1.0f, 1.0f, 1.0f, 1.0f); };

		operator const float*() { return &x; }
		
	}Vector4;

	//tagQuaternion
	typedef struct tagQuaternion:public tagVector4
	{
		tagQuaternion():tagVector4() {};
		tagQuaternion(float _x, float _y, float _z, float _w) :tagVector4(_x, _y, _z, _w) {}

		operator const XMVECTOR()							{ return XMVectorSet(x,y,z,w); };
		tagQuaternion& operator= (const tagVector4& vec)	{ x = vec.x, y = vec.y, z = vec.z, w = vec.z; return *this; };
	}Quaternion;

	//tagColor
	typedef struct tagColor
	{
		float r;
		float g;
		float b;
		float a;

		tagColor() :r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
		tagColor(float _r, float _g, float _b, float _a) :r(_r), g(_g), b(_b), a(_a) {}

		static tagColor clear() { return tagColor(0.0f, 0.0f, 0.0f, 0.0f); };
		static tagColor black() { return tagColor(0.0f, 0.0f, 0.0f, 1.0f); };
		static tagColor white() { return tagColor(1.0f, 1.0f, 1.0f, 1.0f); };
		static tagColor red() { return tagColor(1.0f, 0.0f, 0.0f, 1.0f); };
		static tagColor green() { return tagColor(0.0f, 0.0f, 1.0f, 1.0f); };
		static tagColor blue() { return tagColor(0.0f, 0.0f, 1.0f, 1.0f); };
		static tagColor cyan() { return tagColor(0.0f, 1.0f, 1.0f, 1.0f); };
		static tagColor gray() { return tagColor(0.5f, 0.5f, 0.5f, 1.0f); };
		static tagColor magenta() { return tagColor(1.0f, 0.0f, 1.0f, 1.0f); };
		static tagColor yellow() { return tagColor(1.0f, 0.92f, 0.016f, 1.0f); };

		operator const float*() { return &r; }

	}Color, COLOR;


	//tagMaterial
	typedef struct tagMaterial
	{
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Emission;
		float Shininess;
		float Dummy[3];	//16bit境界用

		tagMaterial() {};
	}Material, MATERIAL;


	//tagLight
	typedef struct tagLight {
		Vector4 Direction;
		Color Diffuse;
		Color Ambient;
	}Light, LIGHT;


	//Model用のマテリアル
	typedef struct tagDX11_MODEL_MATERIAL
	{
		Material Material;
		class CTexture* Texture;
	}DX11_MODEL_MATERIAL;


	//SubSet
	typedef struct tagDX11_SUBSET
	{
		unsigned short	StartIndex;
		unsigned short	IndexNum;
		DX11_MODEL_MATERIAL	Material;
	}DX11_SUBSET;
}