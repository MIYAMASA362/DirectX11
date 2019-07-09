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
		Vector3		m_Position;				//位置
		Quaternion	m_Rotation;				//回転
		Vector3		m_Scale;				//サイズ

		XMMATRIX  m_WorldMatrix;				//ワールド行列
	public:
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Transform(Vector3 position, Vector3 rotation, Vector3 scale);
		Transform();
	private:
		//親を離す
		void detachParent()
		{
			Vector3 position = this->position();
			Vector3 scale = this->Scale();
			Quaternion q = Quaternion::AtMatrix(this->MatrixQuaternion());
			pParent.reset();

			this->position(position);
			this->localScale(scale);
			this->rotation(q);
		}
		//親がターゲットの子を見つけると削除する
		void detachChildSearch(Transform* target)
		{
			for (auto itr = pChildren.begin(); itr != pChildren.end(); itr++)
				if (itr->lock().get() == target)
				{
					pChildren.erase(itr);
					return;
				}
		}
		//回転行列を使ってDirectionを変換
		Vector3 TransformDirection(Vector3 direction){
			return XMVector3Normalize(XMVector3Transform(direction,this->rotation().toMatrix()));
		}
		//子の行列に変更を加える
		void childTransformUpdate(){
			if (pChildren.size() != 0)
				for (auto child : pChildren)
					child.lock()->WorldMatrix();
		}
	public:
		Vector3 position()		{ return this->WorldMatrix().r[3]; }
		Quaternion rotation()	{
			Quaternion q = this->m_Rotation;
			if (!pParent.expired())
				q *= pParent.lock()->rotation();
			return q;
		}
		Vector3 Scale()			{ return pParent.expired() ? m_Scale : m_Scale *= pParent.lock()->Scale();}

		Vector3 localPosition()		{ return m_Position; }
		Quaternion localRotation()	{ return m_Rotation; }
		Vector3 localScale()		{ return m_Scale; }
	public:
		void position(Vector3 position)	{
			this->m_Position += position - this->position();			
		}
		void rotation(Quaternion rotation)	{
			if (!pParent.expired())
				rotation = pParent.lock()->rotation().conjugate() * rotation;
			this->m_Rotation = rotation;
		}

		void localPosition(Vector3 position) {
			this->m_Position = position;
		}
		void localRotation(Quaternion rotation)	{ 
			this->m_Rotation = rotation;
		}
		void localScale(Vector3 scale)	{ 
			this->m_Scale = scale;
		}
	public:
		//親子関係
		void SetParent(std::weak_ptr<Transform> parent);
		void SetParent(GameObject* parent);

		//親を離す
		void DetachParent()
		{
			if (pParent.expired()) return;
			pParent.lock()->detachChildSearch(this);
			detachParent();
		}
		//子を離す
		void DetachChildren()
		{
			//子を参照
			for (auto itr = pChildren.begin(); itr != pChildren.end(); itr++)
			{
				//親と離す
				itr->lock()->detachParent();
			}
			//子リストを削除
			pChildren.clear();
		}
	public:
		//Direction
		Vector3 right()		{ return TransformDirection(Vector3::right()); }
		Vector3 left()		{ return right()*-1.0f; }
		Vector3 up()		{ return TransformDirection(Vector3::up());}
		Vector3 down()		{ return up() * -1.0f; }
		Vector3 forward()	{ return TransformDirection(Vector3::forward()); }
		Vector3 back()		{ return forward() * -1.0f; }
	public:
		//Matrix
		XMMATRIX MatrixQuaternion();
		XMMATRIX MatrixTranslation();
		XMMATRIX MatrixScaling();
		XMMATRIX WorldMatrix();
	public:
		//その方向を向く (XZ平面のみ対応)
		void LookAt(std::weak_ptr<Transform> target)
		{
			Vector3 mPos = this->position();
			Vector3 pPos = target.lock()->position();
			this->m_Rotation = Quaternion::LookRotation(target.lock()->position() - this->position(),this->up());
			if (!pParent.expired())
				this->m_Rotation *= pParent.lock()->rotation().conjugate();
		}
	};
}