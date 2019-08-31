#include<list>
#include<map>
#include<memory>
#include<string>
#include<Windows.h>
#include<d3d11.h>
#include<DirectXMath.h>

//Module
#include"Module\Module.h"

#include"manager.h"

//Project Component
#include"../Project/CameraMouse.h"
#include"../Project/CameraFollow.h"
#include"../Project/RemoveObject.h"
#include"../Project//Bullet.h"

#include"../Project/Enemy.h"
#include"../Project/Player.h"

#include"../Project/ItemSlotScript.h"
#include"../Project/MapUIScript.h"

//Scene

//Test
#include"../Project/SceneChange.h"
#include"../Project/TestScene.h"
#include"../Project/TestScene2.h"
#include"../Project/TestScene3.h"

//Main
#include"../Project/Title.h"
#include"../Project/GameMain.h"

using namespace DirectX;

void CManager::Initialize()
{
	SystemManager::CreateManager<SceneManager>();
	SystemManager::CreateManager<Input>();

	SystemManager::Initialize();

	//Texture
	TextureManager::LoadAsset("field004","field004.tga");
	TextureManager::LoadAsset("number","number.tga");
	TextureManager::LoadAsset("k-on0664","k-on0664.tga");
	TextureManager::LoadAsset("sky","sky.tga");
	TextureManager::LoadAsset("bg","background.tga");
	TextureManager::LoadAsset("ItemSlot","ItemSlot.tga");
	TextureManager::LoadAsset("MapBG","MapBG.tga");
	TextureManager::LoadAsset("Compass","Compass.tga");
	TextureManager::LoadAsset("ItemSlotBG","ItemSlotBG.tga");
	TextureManager::LoadAsset("PressMessage","PressMessage.tga");

	//Model
	ModelManager::LoadAsset(ModelAsset("Miku","Miku","miku_01.obj"));
	ModelManager::LoadAsset(ModelAsset("Miku02","Miku02","miku_02.obj"));
	ModelManager::LoadAsset(ModelAsset("Monster","Monster","monster.obj"));
	ModelManager::LoadAsset(ModelAsset("Rock","Rock","rock.obj"));

	//Audio
	AudioManager::CreateDevice();
	AudioManager::LoadAsset(AudioAsset("Experimental","Experimental_Model_long .wav"));

	//Scene
	SceneManager::CreateScene<TestScene>();
	SceneManager::CreateScene<TestScene2>();
	SceneManager::CreateScene<TestScene3>();

	SceneManager::CreateScene<TitleScene>();
	SceneManager::CreateScene<GameMain>();

	SceneManager::LoadScene("TitleScene");

	SphereCollider::SetRenderBuffer();
	BoxCollider::SetRenderBuffer();
}

void CManager::Update()
{
	SystemManager::Update();
}

void CManager::FixedUpdate()
{
	
}

void CManager::DebugRender()
{
	Input::DebugGUI();
}

void CManager::Render(void)
{
	SystemManager::Render();
}

void CManager::Uninit()
{
	BoxCollider::ReleaseRenderBuffer();
	SphereCollider::ReleaseRenderBuffer();

	CameraManager::Release();
	AudioManager::Release();
	ModelManager::Release();
	TextureManager::Release();

	SystemManager::Finalize();
	SystemManager::Release();
}