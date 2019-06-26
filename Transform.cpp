#include"main.h"
#include"texture.h"
#include"DirectXStruct.h"
#include"Transform.h"

Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale)
	:m_Position(position), m_Rotation(rotation),m_Scale(scale)
{

}

Transform::Transform(Vector3 position,Vector3 rotation,Vector3 scale)
	:Transform(position,Quaternion::Euler(rotation),scale){}
Transform::Transform() 
	:Transform(Vector3::zero(), Vector3::zero(), Vector3::one()){}

Vector3 Transform::forward(){ return Vector3::forward();}
Vector3 Transform::back()	{ return Vector3::back();	}
Vector3 Transform::left()	{ return Vector3::left();	}
Vector3 Transform::right()	{ return Vector3::right();	}
Vector3 Transform::up()		{ return Vector3::up();		}
Vector3 Transform::down()	{ return Vector3::down();	}

XMMATRIX Transform::MatrixTranslation()
{
	return XMMatrixTranslation(position().x, position().y, position().z);
}
XMMATRIX Transform::MatrixScaling()
{
	return XMMatrixScaling(scale().x, scale().y, scale().z);
}
XMMATRIX Transform::WorldMatrix()
{
	return MatrixScaling() * this->rotation().toMatrix() * MatrixTranslation();
}