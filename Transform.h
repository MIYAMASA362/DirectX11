#pragma once

#include"DirectXStruct.h"

namespace DirectX
{
	class GameObject;

	//ïœä∑
	class Transform:public Component
	{
	private:
		std::weak_ptr<Transform> pParent;					//êe
		std::list<std::weak_ptr<Transform>> pChildren;		//éq
	private:
		Vector3		m_Position;
		Quaternion	m_Rotation;
		Vector3		m_Scale;
		unsigned int hierarchy;	//äKëw
	public:
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		Transform();
	private:
		Vector3 VecDirection(Vector3 vec);
	public:
		//Getter
		Vector3 position() { return m_Position; }
		Quaternion rotation() { return m_Rotation; }
		Vector3 scale() { return m_Scale; }
	public:
		//Setter
		void position(Vector3 position) { this->m_Position = position; }
		void rotation(Quaternion rotation) { this->m_Rotation = rotation; }
		void scale(Vector3 scale) { this->m_Scale = scale; }
	public:
		//êeéqä÷åW
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
		Vector3 forward()	{ return VecDirection(Vector3::forward());	};
		Vector3 back()		{ return VecDirection(Vector3::back());		};
		Vector3 left()		{ return VecDirection(Vector3::left());		};
		Vector3 right()		{ return VecDirection(Vector3::right());	};
		Vector3 up()		{ return VecDirection(Vector3::up());		};
		Vector3 down()		{ return VecDirection(Vector3::down());		};
	public:
		//Matrix
		XMMATRIX MatrixQuaternion();
		XMMATRIX MatrixTranslation();
		XMMATRIX MatrixScaling();
		XMMATRIX WorldMatrix();
	};
}