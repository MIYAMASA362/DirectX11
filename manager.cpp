//Main
#include"main.h"

//System
#include"SystemAssets.h"

//Components
#include"ComponentAssets.h"

//Scene
#include"SceneAssets.h"

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
	audio = AudioManager::LoadAudio("asset/Sound/Experimental_Model_long .wav");
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