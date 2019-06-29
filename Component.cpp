#include"main.h"
#include"Object.h"
#include"Tag.h"
#include"SceneManager.h"
#include"Component.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"camera.h"

using namespace DirectX;

void Component::SetEnable(bool isEnable)
{
	this->IsEnable = isEnable;
}
