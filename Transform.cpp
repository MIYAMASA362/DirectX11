#include"main.h"
#include"texture.h"
#include"DirectXStruct.h"
#include"Transform.h"

Transform::Transform(Vector3 position, Vector3 scale, Vector3 rotation) :
	position(position),
	scale(scale),
	rotation(rotation)
{

}

Transform::Transform() :Transform(Vector3::zero(), Vector3::one(), Vector3::zero()) 
{

}

Vector3 Transform::forward()
{ 
	return Vector3::forward();
}

Vector3 Transform::back()
{
	return Vector3::back();
}

Vector3 Transform::left()
{ 
	return Vector3::left();
}

Vector3 Transform::right() { return Vector3::right(); }
Vector3 Transform::up() { return Vector3::up(); }
Vector3 Transform::down() { return Vector3::down(); }

XMMATRIX Transform::MatrixRotationRollPitchYaw()
{
	return XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
}
XMMATRIX Transform::MatrixTranslation()
{
	return XMMatrixTranslation(position.x, position.y, position.z);
}
XMMATRIX Transform::MatrixScaling()
{
	return XMMatrixScaling(scale.x, scale.y, scale.z);
}
XMMATRIX Transform::WorldMatrix()
{
	return MatrixScaling() * MatrixRotationRollPitchYaw() * MatrixTranslation();
}