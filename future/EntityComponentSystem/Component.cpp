#include<iostream>
#include<typeinfo>

#include"Object.h"
#include "Component.h"

IComponent::IComponent(unsigned int id)
	:
	Object(id)
{
}

Transform::Transform(unsigned int id)
	:
	IComponent(id),
	m_Position({0.0f,0.0f,0.0f})
{

}

void Transform::Print()
{
	std::cout << "x:" << m_Position.x << std::endl;
	std::cout << "y:" << m_Position.y << std::endl;
	std::cout << "z:" << m_Position.z << std::endl;
}
