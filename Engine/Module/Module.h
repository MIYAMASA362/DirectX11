#pragma once
#include<typeinfo>
#include<memory>
#include<map>
#include<list>
#include<string>

//DirectX
#include<d3d11.h>
#include<DirectXMath.h>
#include"DirectX\DirectXStruct.h"
#include"DirectX\DirectX.h"

#include"SystemManager\SystemManager.h"

//Device
#include"Input\Input.h"
#include"Time\Time.h"

//Asset
#include"AssetData\AssetData.h"
#include"Audio\Audio.h"
#include"Texture\texture.h"

#include"Physics\Physics.h"

//ECS
#include"ECSEngine.h"

//Component
#include"Tag\Tag.h"
#include"GameObject\GameObject.h"
#include"Mesh\Mesh.h"

#include"Audio\AudioSource.h"

//Component Module
#include"Renderer\Renderer.h"
#include"Field\field.h"
#include"Billboard\Billboard.h"
#include"Model\model.h"
#include"Camera\camera.h"
#include"Physics\Collision.h"
#include"Physics\Rigidbody.h"

//UI(CanvasModule)
#include"UI\UI.h"
#include"UI\font.h"

//Manager
#include"Texture\TextureManager.h"
#include"Model\ModelManager.h"
#include"Audio\AudioManager.h"
#include"Scene\SceneManager.h"
