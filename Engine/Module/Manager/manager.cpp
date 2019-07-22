#include<list>
#include<map>
#include<memory>
#include<string>
#include<Windows.h>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"manager.h"

#include"../Audio/Audio.h"
#include"../Input/Input.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

#include"Module\Transform\Transform.h"
#include"Module\Behaviour\Behaviour.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

//Component Module
#include"Module\Renderer\Renderer.h"
#include"Module\Field\field.h"
#include"Module\Model\model.h"
#include"Module\Camera\camera.h"
#include"Module\Collision\Collision.h"

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

static Audio* audio;

void CManager::Initialize()
{
	Input::Init();

	SceneManager::Create();
	SceneManager::CreateScene<TestScene>();
	SceneManager::CreateScene<TestScene2>();
	SceneManager::CreateScene<TestScene3>();
	SceneManager::LoadScene("TestScene2");

	AudioManager::Create();
	audio = AudioManager::LoadAudio("Asset/Sound/Experimental_Model_long .wav");
	audio->Play(true);
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
	AudioManager::Destroy();
	SceneManager::Destroy();
	Input::Uninit();
}