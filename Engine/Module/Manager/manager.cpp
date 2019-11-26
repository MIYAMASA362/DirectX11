#include"Common.h"

//--- Module ------------------------------------
#include"Module\Module.h"

#include"Module\IMGUI\GUI_ImGui.h"

#include"manager.h"

//--- Project Component -------------------------
#include"../Project/CameraMouse.h"
#include"../Project/CameraFollow.h"
#include"../Project/RemoveObject.h"
#include"../Project/Bullet.h"

#include"../Project/Enemy.h"
#include"../Project/Player.h"

#include"../Project/ItemSlotScript.h"
#include"../Project/MapUIScript.h"

#include"../Project/BallScript.h"

//--- Scene -------------------------------------

//Test
#include"../Project/SceneChange.h"
#include"../Project/TestScene.h"
#include"../Project/TestScene2.h"
#include"../Project/TestScene3.h"
#include"../Project/BallTest.h"

//Main
#include"../Project/Title.h"
#include"../Project/GameMain.h"

//------------------------------------------------

#include"cereal\cereal.hpp"
#include"cereal\archives\json.hpp"
#include<fstream>

using namespace DirectX;

void CManager::Initialize()
{
	//ECS
	EntityManager::Create();
	ComponentManager::Create();

	//GUI
	GUI::guiImGui::Create(D3DApp::GetWindow(),D3DApp::GetDevice(),D3DApp::GetDeviceContext());

	//Time
	TimeManager::Create(D3DApp::GetFps());

	Input::Initialize();
	Input::Mouse::SetScreenLoop(false);

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
	ModelManager::LoadAsset(ModelAsset("Cube", "Cube", "cube.obj"));
	ModelManager::LoadAsset(ModelAsset("Ball", "Ball", "ball.obj"));

	//Audio
	AudioManager::CreateDevice();
	AudioManager::LoadAsset(AudioAsset("Experimental","Experimental_Model_long .wav"));

	//Scene
	SceneManager::CreateScene<TestScene>();
	SceneManager::CreateScene<TestScene2>();
	SceneManager::CreateScene<TestScene3>();
	SceneManager::CreateScene<BallTest>();

	SceneManager::CreateScene<TitleScene>();
	SceneManager::CreateScene<GameMain>();

	Renderer::Create();


	SphereCollider::SetRenderBuffer();
	BoxCollider::SetRenderBuffer();

	SceneManager::LoadScene(SceneManager::GetSceneByName("TestScene"));

	ComponentManager::SendComponentMessage("Start");
}

void CManager::Run()
{
	TimeManager::Update();

	bool IsUpdate = TimeManager::IsUpdate();
	bool IsFixedUpdate = TimeManager::IsFixedUpdate();

	if (!IsUpdate && !IsFixedUpdate)return;

	if (IsUpdate) CManager::Update();

	if (IsFixedUpdate) CManager::FixedUpdate();

	if (IsUpdate) CManager::Render();

	if (IsUpdate || IsFixedUpdate) SceneManager::ChangeScene();

	ObjectManager::ClearnUp();
}

void CManager::Update()
{
	Input::Update();

	ComponentManager::SendComponentMessage("Update");
}

void CManager::FixedUpdate()
{
	ComponentManager::SendComponentMessage("FixedUpdate");
	Rigidbody::ApplyRigidbody();
	Rigidbody::CollisionRigidbody();
}

void CManager::Render()
{
	D3DApp::Renderer::ClearRenderTargetView(Color::gray());

	Camera::Render(Renderer3D::BeginRender, D3DApp::Renderer::Begin);

	Renderer2D::BeginRender();

	CManager::DebugRender();

	D3DApp::Renderer::End();
}

void CManager::DebugRender()
{
	GUI::guiImGui::SetFrame();

	Input::DebugGUI();
	SceneManager::DebugGUI_ActiveScene();
	TimeManager::DebugGUI_Time();

	ImGui::Begin("ScreenToWorld");
	Vector3 vec = Camera::ScreenToWorldPosition(Vector3::one());
	ImGui::Text("X:%f",vec.x);
	ImGui::Text("Y:%f",vec.y);
	ImGui::Text("Z:%f",vec.z);
	ImGui::End();

	GUI::guiImGui::Render();
}

void CManager::Finalize()
{
	BoxCollider::ReleaseRenderBuffer();
	SphereCollider::ReleaseRenderBuffer();

	Renderer::Destroy();

	AudioManager::Release();
	ModelManager::Release();
	TextureManager::Release();

	TimeManager::Destroy();
	GUI::guiImGui::Destroy();

	ComponentManager::Release();
	EntityManager::Release();

	ObjectManager::Release();
}