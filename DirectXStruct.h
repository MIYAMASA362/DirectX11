#pragma once

class CTexture;

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
	
	//Mathf
	struct Mathf {

		static const float kEpsilon;
		static const float PI2;				//PI*2
		static const float PI;				//PI 3.141592741F
		static const float PI_2;			//PI/2
		static const float PI_4;			//PI/4

		static const float Deg2Rad;			//PI * 2.0f /360.0f
		static const float Rad2Deg;			//1.0f / Deg2Rad

		//valueをmin < value < maxの範囲にClamp
		static float Clamp(float value,const float min, const float max) {
			value = max < value ? max : value;
			value = min > value ? min : value;
			return value;
		};
		//valueを0.0f < value < 1.0fの範囲にClamp
		static float Clamp01(float value) {
			value = 1.0f < value ? 1.0f : value;
			value = 0.0f > value ? 0.0f : value;
			return value;
		};
		//線形補間
		static float Lerp(const float start, const float end, float time)
		{
			time = Clamp01(time);
			return (1 - time)*start + time*end;
		};
		//球面線形補間
		static float Slerp(const float start,const float end,float time)
		{
			time = Clamp01(time);
			return sinf((1.0f - time) * Mathf::PI_2) * start + sinf(time * Mathf::PI_2) * end;
		}
		//ラジアン角へ変更
		static float ToRadian(float euler) 
		{
			return euler * ((float)XM_PI / 180.0f);
		};
		//オイラー角へ変更
		static float ToEuler(float radian) {
			return radian * 180.0f / (float)XM_PI;
		};
		//内積が許容範囲内で等しいのか
		static bool IsEqualUsingDot(float dot) {
			return dot > 1.0f - kEpsilon;
		};
		//逆Sinf
		static float aSinf(float angle) {
			return asinf(angle);
		};
		//逆Cosf
		static float aCosf(float angle){
			return acosf(angle);
		};
	};

	//tagVector3
	typedef struct tagVector3
	{
		float x;
		float y;
		float z;

		tagVector3() :x(0.0f), y(0.0f), z(0.0f) {};
		tagVector3(float _x, float _y, float _z) :x(_x),y(_y),z(_z) {};
		tagVector3(XMVECTOR vec):x(vec.m128_f32[0]),y(vec.m128_f32[1]),z(vec.m128_f32[2]) { };

		//	tagVector3(0.0f,0.0f,0.0f)
		static tagVector3 zero()	{ 
			return tagVector3( 0.0f,  0.0f,  0.0f);
		};
		//	tagVector3(1.0f,1.0f,1.0f)
		static tagVector3 one()		{ 
			return tagVector3( 1.0f,  1.0f,  1.0f);
		};
		// tagVector3(0.0f,1.0f,0.0f)
		static tagVector3 up(){
			return tagVector3( 0.0f,  1.0f,  0.0f);
		};
		// tagVector3(0.0f,-1.0f,0.0f)
		static tagVector3 down(){ 
			return tagVector3( 0.0f, -1.0f,  0.0f); 
		};
		// tagVector3(1.0f,0.0f,0.0f)
		static tagVector3 right(){
			return tagVector3( 1.0f,  0.0f,  0.0f);
		};
		// tagVector3(-1.0f,0.0f,0.0f)
		static tagVector3 left(){ 
			return tagVector3(-1.0f,  0.0f,  0.0f); 
		};
		// tagVector3(0.0f,0.0f,1.0f)
		static tagVector3 forward(){
			return tagVector3( 0.0f,  0.0f,  1.0f);
		};
		// tagVector3(0.0f,0.0f,-1.0f)
		static tagVector3 back(){ 
			return tagVector3( 0.0f,  0.0f, -1.0f);
		};

		//2つのベクトルで各成分の一番大きな値を使用してベクトルを作成
		static tagVector3 Max(const tagVector3 vec1, const tagVector3 vec2){ 
			return{
				vec1.x > vec2.x ? vec1.x : vec2.x,
				vec1.y > vec2.y ? vec1.y : vec2.y,
				vec1.z > vec2.z ? vec1.z : vec2.z
			}; 
		};
		//2つのベクトルで各成分の一番小さな値を使用してベクトルを作成
		static tagVector3 Min(const tagVector3 vec1, const tagVector3 vec2) { 
			return{ 
				vec1.x < vec2.x ? vec1.x : vec2.x,
				vec1.y < vec2.y ? vec1.y : vec2.y,
				vec1.z < vec2.z ? vec1.z : vec2.z
			}; 
		};
		//2つのベクトルの内積
		static float Dot(const tagVector3 vec1, const tagVector3 vec2) {
			return 
				vec1.x * vec2.x + 
				vec1.y * vec2.y + 
				vec1.z * vec2.z;
		};
		//2つのベクトルの外積
		static tagVector3 Cross(const tagVector3 vec1, const tagVector3 vec2) { 
			return{
				vec1.y * vec2.z - vec1.z * vec2.y,
				vec1.z * vec2.x - vec1.x * vec2.z,
				vec1.x * vec2.y - vec1.y * vec2.x
			}; 
		};
		//tagVector3(vec1 + vec2)
		static tagVector3 Add(const tagVector3 vec1, const tagVector3 vec2) {
			return{
				vec1.x + vec2.x,
				vec1.y + vec2.y,
				vec1.z + vec1.z
			};
		};
		//tagVector3(vec1 - vec2)
		static tagVector3 Subtract(const tagVector3 vec1,const tagVector3 vec2){
			return{
				vec1.x - vec2.x,
				vec1.y - vec2.y,
				vec1.z - vec2.z
			};
		};
		//tagVector3(vec1 * scale)
		static tagVector3 Scale(const tagVector3 vec1, const float value) {
			return{
				vec1.x * value,
				vec1.y * value,
				vec1.z * value
			};
		};
		//tagVector3 Lerp(time:0.0f～1.0f)
		static tagVector3 Lerp(const tagVector3 vec1, const tagVector3 vec2, const float time) {
			float t = Mathf::Clamp(0.0f,1.0f,time);
			return{
				(1 - t)*vec1.x + t*vec2.x,
				(1 - t)*vec1.y + t*vec2.y,
				(1 - t)*vec1.z + t*vec2.z
			};
		};
		//tagVector3 SLerp(0.0f～1.0f)未完成
		static tagVector3 Slerp(const tagVector3 vec1, const tagVector3 vec2, const float time) {
			float t = Mathf::Clamp(0.0f,1.0f,time);
			return tagVector3::one();
		};

		static float Length(const tagVector3 vec)
		{
			return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		}

		static float LengthSq(const tagVector3 vec)
		{
			return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
		}

		static tagVector3 Normalize(const tagVector3 vec1)
		{
			float length = tagVector3::Length(vec1);
			return tagVector3(vec1.x / length,vec1.y /length,vec1.z/length);
		};
		
		//ベクトルの長さ
		float Length()	 { 
			return sqrtf(x*x + y*y + z*z); 
		};
		//ベクトル長の2乗
		float LengthSq() { 
			return x*x + y*y + z*z; 
		};
		//単位化
		tagVector3 normalize()
		{
			return tagVector3::Normalize(*this);
		}

		operator const float*() { return &x; }
		operator const XMVECTOR() { return XMVectorSet(x, y, z, 0.0f); };
		tagVector3 operator= (const XMVECTOR& vec) { x = vec.m128_f32[0]; y = vec.m128_f32[1]; z = vec.m128_f32[2]; return *this; };
		tagVector3 operator+ (const tagVector3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; };
		tagVector3 operator- (const tagVector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; };
		tagVector3 operator/ (const float& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; };
		tagVector3 operator* (const float& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; };
		tagVector3 operator+= (const tagVector3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; };
		tagVector3 operator-= (const tagVector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; };
		bool operator== (const tagVector3& vec) { return (x == vec.x && y == vec.y && z == vec.z); };
	}Vector3;


	//tagVector4
	typedef struct tagVector4
	{
		float x;
		float y;
		float z;
		float w;

		tagVector4():x(0.0f), y(0.0f), z(0.0f), w(0.0f){};
		tagVector4(float _x, float _y, float _z, float _w) :x(_x),y(_y),z(_z),w(_w) {};

		static tagVector4 zero() { return tagVector4(0.0f, 0.0f, 0.0f, 0.0f); };
		static tagVector4 one() { return tagVector4(1.0f, 1.0f, 1.0f, 1.0f); };

		operator const float*() { return &x; };
		
	}Vector4;

	//tagQuaternion
	typedef struct tagQuaternion
	{
		float x;
		float y;
		float z;
		float w;

		tagQuaternion():x(0.0f),y(0.0f),z(0.0f),w(0.0f){};
		tagQuaternion(float _x, float _y, float _z, float _w):x(_x),y(_y),z(_z),w(_w){};
		tagQuaternion(tagVector3 rotation) 
		{
			FXMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(rotation);
			*this = q;
		};
		tagQuaternion(XMVECTOR vec) 
		{
			x = vec.m128_f32[0];
			y = vec.m128_f32[1];
			z = vec.m128_f32[2];
			w = vec.m128_f32[3];
		};

		//Quaternion Length
		static float Length(tagQuaternion q) {
			return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w *q.w);
		};
		//Quaternion LengthSq
		static float LengthSq(tagQuaternion q) {
			return q.x * q.x + q.y * q.y + q.z * q.z + q.w *q.w;
		};
		//Quaternion Dot 内積
		static float Dot(tagQuaternion q1, tagQuaternion q2) {
			return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
		};
		//Quaternion Angle
		static float Angle(tagQuaternion q1, tagQuaternion q2) {
			float dot = tagQuaternion::Dot(q1,q2);
			return Mathf::IsEqualUsingDot(dot) ? 0.0f : Mathf::aCosf(min(abs(dot),1.0f)) * 2.0f * Mathf::Rad2Deg;
		};
		//Quaternion ToEulerAngle(未推奨)
		static tagVector3 ToEulerAngles(tagQuaternion q) {
			XMMATRIX matrix = tagQuaternion::ToMatrix(q);
			return tagVector3(
				atan2f(matrix.r[2].m128_f32[1], matrix.r[2].m128_f32[2]),
				asinf(-matrix.r[2].m128_f32[0]),
				atan2f(matrix.r[1].m128_f32[0],matrix.r[0].m128_f32[0])
			);
		};
		//Quaternion Identity
		static tagQuaternion Identity() {
			return tagQuaternion(0.0f,0.0f,0.0f,1.0f);
		};
		//Quaternion IsIdentity
		static bool IsIdentity(tagQuaternion q1) {
			return q1.x == 0.0f && q1.y == 0.0f && q1.z == 0.0f && q1.w == 1.0f;
		};
		//Quaternion Normalize
		static tagQuaternion Normalize(tagQuaternion q) {
			float mag = sqrtf(Dot(q,q));
			if (mag < 1.17549435E-38f)
				return tagQuaternion::Identity();
			return tagQuaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
		};
		//Quaternion Conjugate 共役なクォータニオン
		static tagQuaternion Conjugate(tagQuaternion q1) {
			return tagQuaternion(-q1.x,-q1.y,-q1.z,q1.w);
		};
		//Quaternion オイラー角を使った回転   //不具合あり
		static tagQuaternion Euler(tagVector3 vec) {
			float x	= Mathf::ToRadian(vec.x);
			float y = Mathf::ToRadian(vec.y);
			float z = Mathf::ToRadian(vec.z);

			float cX = cosf(x * 0.5f);
			float cY = cosf(y * 0.5f);
			float cZ = cosf(z * 0.5f);

			float sX = sinf(x * 0.5f);
			float sY = sinf(y * 0.5f);
			float sZ = sinf(z * 0.5f);

			return tagQuaternion(
				sX * cY * cZ + cX * sY * sZ,
				cX * sY * cZ - sX * cY * sZ,
				cX * cY * sZ - sX * sY * cZ,
				cX * cY * cZ + sX * sY * sZ
			);
		}
		//Quaternion Axisを中心にAngle角回転します
		static tagQuaternion AngleAxis(const float angle,tagVector3 axis){
			return tagQuaternion(
				axis.x * sinf(Mathf::ToRadian(angle) * 0.5f),
				axis.y * sinf(Mathf::ToRadian(angle) * 0.5f),
				axis.z * sinf(Mathf::ToRadian(angle) * 0.5f),
				cosf(Mathf::ToRadian(angle)*0.5f)
			);
		};
		//Quaternion Quaternionを行列に変換します
		static XMMATRIX ToMatrix(tagQuaternion q){
			float xx = q.x * q.x * 2.0f;
			float yy = q.y * q.y * 2.0f;
			float zz = q.z * q.z * 2.0f;

			float xy = q.x * q.y * 2.0f;
			float yz = q.y * q.z * 2.0f;
			float zx = q.z * q.x * 2.0f;

			float xw = q.x * q.w * 2.0f;
			float yw = q.y * q.w * 2.0f;
			float zw = q.z * q.w * 2.0f;

			XMMATRIX matrix;

			matrix.r[0].m128_f32[0] = 1.0f - yy - zz;
			matrix.r[0].m128_f32[1] = xy + zw;
			matrix.r[0].m128_f32[2] = zx - yw;
			matrix.r[0].m128_f32[3] = 0.0f;

			matrix.r[1].m128_f32[0] = xy - zw;
			matrix.r[1].m128_f32[1] = 1.0f - zz - xx;
			matrix.r[1].m128_f32[2] = yz + xw;
			matrix.r[1].m128_f32[3] = 0.0f;

			matrix.r[2].m128_f32[0] = zx + yw;
			matrix.r[2].m128_f32[1] = yz - xw;
			matrix.r[2].m128_f32[2] = 1.0f - xx - yy;
			matrix.r[2].m128_f32[3] = 0.0f;

			matrix.r[3].m128_f32[0] = 0.0f;
			matrix.r[3].m128_f32[1] = 0.0f;
			matrix.r[3].m128_f32[2] = 0.0f;
			matrix.r[3].m128_f32[3] = 1.0f;

			return matrix;
		};
		//Quaternion 行列から変換します
		static tagQuaternion AtMatrix(XMMATRIX matrix){
			
			tagQuaternion q;

			q.x = (sqrtf( matrix.r[0].m128_f32[0] - matrix.r[1].m128_f32[1] - matrix.r[2].m128_f32[2] + matrix.r[3].m128_f32[3])) * 0.5f;
			q.y = (sqrtf(-matrix.r[0].m128_f32[0] + matrix.r[1].m128_f32[1] - matrix.r[2].m128_f32[2] + matrix.r[3].m128_f32[3])) * 0.5f;
			q.z = (sqrtf(-matrix.r[0].m128_f32[0] - matrix.r[1].m128_f32[1] + matrix.r[2].m128_f32[2] + matrix.r[3].m128_f32[3])) * 0.5f;
			q.w = (sqrtf( matrix.r[0].m128_f32[0] + matrix.r[1].m128_f32[1] + matrix.r[2].m128_f32[2] + matrix.r[3].m128_f32[3])) * 0.5f;

			return q;
		}
		//Quaternion Multiply 合成
		static tagQuaternion Multiply(tagQuaternion q1,tagQuaternion q2)
		{
			tagQuaternion q;
			q.x = q1.x * q2.w + ( q1.w * q2.x) + (-q1.z * q2.y) + ( q1.y  * q2.z);
			q.y = q1.y * q2.w + ( q1.z * q2.x) + ( q1.w * q2.y) + (-q1.x  * q2.z);
			q.z = q1.z * q2.w + (-q1.y * q2.x) + ( q1.x * q2.y) + ( q1.w  * q2.z);
			q.w = q1.w * q2.w + (-q1.x * q2.x) + (-q1.y * q2.y) + (-q1.z  * q2.z);
			return q;
		}
		//Quaternion Lerp
		static tagQuaternion Lerp(tagQuaternion start,tagQuaternion end,float time)
		{
			time = Mathf::Clamp01(time);
			return tagQuaternion(
				start.x * (1.0f - time) + end.x * time,
				start.y * (1.0f - time) + end.y * time,
				start.z * (1.0f - time) + end.z * time,
				start.w * (1.0f - time) + end.w * time
			);
		}
		//Quaternion Slerp　球面線形補間
		static tagQuaternion Slerp(tagQuaternion start,tagQuaternion end,float time)
		{
			time = Mathf::Clamp01(time);
			float dot = tagQuaternion::Dot(start,end);
			float sinDot = sinf(dot);
			float value1 = sinf((1.0f - time)*dot) / sinDot;
			float value2 = sinf(time * dot) / sinDot;
			return tagQuaternion(
				value1*start.x + value2*end.x,
				value1*start.y + value2*end.y,
				value1*start.z + value2*end.z,
				value1*start.w + value2*end.w
			);
		}

		float length() {
			return Length(*this);
		};
		float lengthSq(){
			return LengthSq(*this);
		}
		float dot(const tagQuaternion& q) {
			return Dot(*this,q);
		};
		bool isIdentity() {
			return IsIdentity(*this);
		};
		tagQuaternion conjugate(){
			return Conjugate(*this);
		}
		XMMATRIX toMatrix()
		{
			return Quaternion::ToMatrix(*this);
		}

		//Casting
		operator const XMVECTOR()  { return XMVectorSet(x, y, z, w); };

		//assigment 
		tagQuaternion& operator =  (const FXMVECTOR& vec) 
		{ 
			this->x = vec.m128_f32[0];
			this->y = vec.m128_f32[1];
			this->z = vec.m128_f32[2];
			this->w = vec.m128_f32[3];
			return *this;
		};
		tagQuaternion& operator += (const tagQuaternion& q){
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;
			return *this;
		}
		tagQuaternion& operator -= (const tagQuaternion& q) { 
			x -= q.x;
			y -= q.y;
			z -= q.z;
			w -= q.w;
			return *this;
		};
		tagQuaternion& operator *= (const tagQuaternion& q) {
			return *this = Multiply(*this,q);
		};

		tagQuaternion& operator *= (const float& scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		};
		tagQuaternion& operator /= (const float& scalar) {
			float fInv = 1.0f / scalar;
			x *= fInv;
			y *= fInv;
			z *= fInv;
			w *= fInv;
			return *this;
		};

		tagQuaternion& operator +	(const tagQuaternion& q) {
			return tagQuaternion(x + q.x,y + q.y,z + q.z,w + q.w);
		};
		tagQuaternion& operator -	(const tagQuaternion& q) {
			return tagQuaternion(x-q.x,y-q.y,z-q.z,w-q.w);
		};
		tagQuaternion& operator *	(const tagQuaternion& q) {
			return Multiply(*this,q);
		};
		tagQuaternion& operator *	(const float& scalar){
			return tagQuaternion( x * scalar, y * scalar, z * scalar, w * scalar);
		};
		tagQuaternion& operator /	(const float& scalar) {
			float fInv = 1.0f / scalar;
			return tagQuaternion(w*fInv, y*fInv, z*fInv, w*fInv);
		};

		tagQuaternion operator - (tagQuaternion q){
			return tagQuaternion::Conjugate(q);
		};

		bool operator== (const tagQuaternion& q) {
			return x == q.x && y == q.y && z == q.z && w == q.w;
		};
		bool operator!= (const tagQuaternion& q) {
			return x != q.x || y != q.y || z != q.z || w != q.w;
		};

	}Quaternion;

	//tagColor
	typedef struct tagColor
	{
		float r;
		float g;
		float b;
		float a;

		//コンストラクタ
		tagColor() :r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
		tagColor(float _r, float _g, float _b, float _a) :r(_r), g(_g), b(_b), a(_a) {}

		//共通処理
		static tagColor clear()		{ return tagColor( 0.0f,  0.0f,  0.0f,  0.0f); };
		static tagColor black()		{ return tagColor( 0.0f,  0.0f,  0.0f,  1.0f); };
		static tagColor white()		{ return tagColor( 1.0f,  1.0f,  1.0f,  1.0f); };
		static tagColor red()		{ return tagColor( 1.0f,  0.0f,  0.0f,  1.0f); };
		static tagColor green()		{ return tagColor( 0.0f,  0.0f,  1.0f,  1.0f); };
		static tagColor blue()		{ return tagColor( 0.0f,  0.0f,  1.0f,  1.0f); };
		static tagColor cyan()		{ return tagColor( 0.0f,  1.0f,  1.0f,  1.0f); };
		static tagColor gray()		{ return tagColor( 0.5f,  0.5f,  0.5f,  1.0f); };
		static tagColor magenta()	{ return tagColor( 1.0f,  0.0f,  1.0f,  1.0f); };
		static tagColor yellow()	{ return tagColor( 1.0f, 0.92f, 0.016f, 1.0f); };

		//コンストラクタ
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
		CTexture* Texture;
	}DX11_MODEL_MATERIAL;


	//SubSet
	typedef struct tagDX11_SUBSET
	{
		unsigned short	StartIndex;
		unsigned short	IndexNum;
		DX11_MODEL_MATERIAL	Material;
	}DX11_SUBSET;

	
}