#include"main.h"
#include"texture.h"
#include"DirectXStruct.h"
#include"Object.h"
#include"Transform.h"
#include"Tag.h"
#include"Component.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"Input.h"
#include "RemoveObject.h"

void RemoveObjectMethod::Update()
{
	if (Input::GetKeyPress(VK_SPACE))
		this->gameObject->Destroy();
}

void RemoveObjectMethod::Destroy()
{
	int i = 0;
}

void RemoveObjectMethod::OnDestroy()
{
	int i = 0;
}
