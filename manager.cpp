//Main
#include"main.h"
#include"manager.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"Time.h"
#include"Object.h"
#include"Component.h"
#include"Behaviour.h"
#include"Transform.h"
#include"Tag.h"
#include"Renderer.h"
#include"GameObject.h"
#include"SceneManager.h"
#include"Input.h"
#include"Renderer.h"

//Objects
#include"camera.h"
#include"Polygon.h"
#include"field.h"
#include"model.h"

#include"RemoveObject.h"
#include"CameraFollow.h"

//Scene
#include"TestScene.h"

CModel* pModel;

void CManager::Init()
{
	Input::Init();

	SceneManager::Create();
	SceneManager::CreateScene<TestScene>();
	SceneManager::LoadScene("TestScene");

	SceneManager::RunActiveScene(Component::Initialize);
}

void CManager::Update()
{
	Input::Update();
	SceneManager::RunActiveScene(Component::Update);
}

void CManager::FixedUpdate()
{
	
}

void CManager::Render(void)
{
	SceneManager::RunActiveScene(Component::Render);
}


void CManager::Uninit()
{
	Input::Uninit();
	SceneManager::RunActiveScene(Component::Finalize);
	SceneManager::Destroy();
}