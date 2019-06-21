#include"main.h"
#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Tag.h"
#include"GameObject.h"
#include"SceneManager.h"

void DirectX::GameObject::Destroy()
{
	scene->SetIsCeanUp(true);
	IsDestroy = true;
}
