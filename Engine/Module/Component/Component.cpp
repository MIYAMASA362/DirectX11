#include<memory>
#include<typeinfo>
#include<string>
#include"../Object/Object.h"
#include"Component.h"
using namespace DirectX;

void Component::SetEnable(bool isEnable)
{
	this->IsEnable = isEnable;
}