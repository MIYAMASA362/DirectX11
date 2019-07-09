#include"main.h"
#include"Object.h"
#include"Tag.h"
#include"Component.h"
#include"SceneManager.h"
#include"Behaviour.h"
#include"GameObject.h"
#include"camera.h"

using namespace DirectX;

void Component::SetEnable(bool isEnable)
{
	this->IsEnable = isEnable;
}