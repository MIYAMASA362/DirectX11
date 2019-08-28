#pragma once
#include<typeinfo>
#include<memory>
#include<list>
#include<string>

//DirectX
#include<d3d11.h>
#include<DirectXMath.h>
#include"DirectX\DirectXStruct.h"
#include"DirectX\DirectX.h"

//Device
#include"Input\Input.h"

//Asset
#include"AssetData\AssetData.h"
#include"Audio\Audio.h"
#include"Texture\texture.h"

#include"Physics\Physics.h"

//Component
#include"Object\Object.h"
#include"Component\Component.h"

#include"Transform\Transform.h"
#include"Behaviour\Behaviour.h"
#include"Tag\Tag.h"
#include"GameObject\GameObject.h"

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
