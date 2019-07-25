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
#include"../Scene/SceneManager.h"
#include"../Project/RemoveObject.h"

//Scene
#include"../Project/SceneChange.h"
#include"../Project/TestScene.h"
#include"../Project/TestScene2.h"
#include"../Project/TestScene3.h"

using namespace DirectX;

void CManager::Initialize()
{
	//Input
	Input::Init();

	//Texture
	TextureManager::LoadAsset(TextureAsset("field004","field004.tga"));
	TextureManager::LoadAsset(TextureAsset("number","number.tga"));
	TextureManager::LoadAsset(TextureAsset("k-on0664","k-on0664.tga"));

	//Model
	ModelManager::LoadAsset(ModelAsset("Miku","Miku","miku_01.obj"));

	//Audio
	AudioManager::CreateDevice();
	AudioManager::LoadAsset(AudioAsset("Experimental","Experimental_Model_long .wav"));

	//Scene
	SceneManager::Create();
	SceneManager::CreateScene<TestScene>();
	SceneManager::CreateScene<TestScene2>();
	SceneManager::CreateScene<TestScene3>();
	SceneManager::LoadScene("TestScene2");
}

void CManager::Update()
{
	Input::Update();
	SceneManager::RunActiveScene(Component::Update);
}

void CManager::FixedUpdate()
{
	SceneManager::RunActiveScene(Component::FixedUpdate);
	SceneManager::OnTriggerUpdate();
	SceneManager::OnCollisionUpdate();
}

void CManager::Render(void)
{
	SceneManager::RunActiveScene(Component::Render);
}

void CManager::Uninit()
{
	CameraManager::Release();
	SceneManager::Destroy();
	AudioManager::Release();
	ModelManager::Release();
	TextureManager::Release();
	Input::Uninit();
}

