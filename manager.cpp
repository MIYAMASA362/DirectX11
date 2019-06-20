//Main
#include"main.h"
#include"manager.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"Time.h"
#include"Component.h"
#include"Behaviour.h"
#include"Transform.h"
#include"Tag.h"
#include"GameObject.h"
#include"SceneManager.h"

//Objects
#include"camera.h"
#include"Polygon.h"
#include"field.h"
#include"model.h"

//Scene
#include"TestScene.h"

CModel* pModel;

void CManager::Init()
{
	SceneManager::Create();
	SceneManager::CreateScene<TestScene>();
	SceneManager::LoadScene("TestScene");

	SceneManager::Initialize();
}

void CManager::Update()
{
	OutputDebugString(("\nUpdate:" + std::to_string(Time::Get_DeltaTime())).c_str());
	SceneManager::Update();
}

void CManager::FixedUpdate()
{
	OutputDebugString(("\nFixedUpdate:" + std::to_string(Time::Get_FixedDeltaTime())).c_str());
}

void CManager::Draw()
{
	SceneManager::Render();
}


void CManager::Uninit()
{
	SceneManager::Finalize();
	SceneManager::Destroy();
}