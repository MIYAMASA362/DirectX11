#pragma once

#include"DirectXStruct.h"

namespace DirectX
{
	//•ÏŠ·
	class Transform
	{
	private:
		Vector3		m_Position;
		Quaternion	m_Rotation;
		Vector3		m_Scale;
	public:
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		Transform();
	public:
		Vector3 position()		{ return m_Position; }
		Quaternion rotation()	{ return m_Rotation; }
		Vector3 scale()			{ return m_Scale; }
	public:
		void position(Vector3 position)		{ this->m_Position = position; }
		void rotation(Quaternion rotation)	{ this->m_Rotation = rotation; }
		void scale(Vector3 scale)			{ this->m_Scale	   = scale; }
	public:
		Vector3 forward();
		Vector3 back();
		Vector3 left();
		Vector3 right();
		Vector3 up();
		Vector3 down();
	public:
		XMMATRIX MatrixTranslation();
		XMMATRIX MatrixScaling();
		XMMATRIX WorldMatrix();
	};
}