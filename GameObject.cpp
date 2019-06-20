#include"main.h"
#include"Transform.h"
#include"Component.h"
#include"Tag.h"
#include"GameObject.h"
#include"SceneManager.h"

void DirectX::GameObject::Destroy()
{
	scene->RemoveObject(this);
	return;
}
