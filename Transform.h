#pragma once

#include"DirectXStruct.h"

namespace DirectX
{
	class GameObject;

	//変換
	class Transform final :public Component
	{
	private:
		std::weak_ptr<Transform> pParent;					//親
		std::list<std::weak_ptr<Transform>> pChildren;		//子
	private:
		Vector3		m_Position;
		Quaternion	m_Rotation;
		Vector3		m_Scale;

		XMMATRIX worldMatrix;
	public:
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		Transform();
	public:
		//Getter
		Vector3 position() {
			XMMATRIX matrix = MatrixTranslation();
			return Vector3(matrix.r[3]);
		}
		Quaternion rotation() {
			Quaternion q = Quaternion::Identity();
			if (!pParent.expired())
				q = pParent.lock()->rotation();
			return m_Rotation * q;
		}
		Vector3 scale()	{ 
			XMMATRIX matrix = MatrixScaling();
			return Vector3(matrix.r[0].m128_f32[0],matrix.r[1].m128_f32[1],matrix.r[2].m128_f32[2]);
		}

		Vector3 localPosition(){
			return m_Position;
		}
		Quaternion localRotation(){
			return m_Rotation;
		}
		Vector3 localScale(){
			return m_Scale;
		}
	public:
		//Setter
		void position(Vector3 position) {
			if (!pParent.expired())
				position -= pParent.lock()->position();
			m_Position = position;
		}
		void rotation(Quaternion rotation) {
			Quaternion q = Quaternion::Identity();
			if (!pParent.expired())
				q = pParent.lock()->rotation();
			m_Rotation = rotation *q.conjugate();
		}
		void scale(Vector3 scale) {
			if (!pParent.expired())
				scale = pParent.lock()->scale() / scale;
			this->m_Scale = scale;
		}

		void localposition(Vector3 position)
		{
			this->m_Position = position;
		}
		void localrotation(Quaternion rotation){
			this->m_Rotation = rotation;
		}
		void localscale(Vector3 scale) {
			this->m_Scale = scale;
		}
	public:
		//親子関係
		void SetParent(std::weak_ptr<Transform> parent);
		void SetParent(GameObject* parent);

		void DetachParent()
		{
			pParent.reset();
		}
		void DetachChildren()
		{
			pChildren.clear();
		}
	public:
		//Direction
		Vector3 right()		{ return tagVector3(MatrixQuaternion().r[0]).normalize(); };
		Vector3 left()		{ return right()*-1.0f; };
		Vector3 up()		{ return tagVector3(MatrixQuaternion().r[1]).normalize(); };
		Vector3 down()		{ return up() * -1.0f; };
		Vector3 forward()	{ return tagVector3(MatrixQuaternion().r[2]).normalize(); };
		Vector3 back()		{ return forward() * -1.0f; };
	public:
		//Matrix
		XMMATRIX MatrixQuaternion();
		XMMATRIX MatrixTranslation();
		XMMATRIX MatrixScaling();
		XMMATRIX WorldMatrix();
	};
}