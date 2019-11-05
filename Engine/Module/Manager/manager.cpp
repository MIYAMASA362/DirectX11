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

//--- Scene -------------------------------------

//Test
#include"../Project/SceneChange.h"
#include"../Project/TestScene.h"
#include"../Project/TestScene2.h"
#include"../Project/TestScene3.h"

//Main
#include"../Project/Title.h"
#include"../Project/GameMain.h"

//------------------------------------------------
CameraMouse::ComponentIndex CameraMouse::ComponentType::Index;
WASDMove::ComponentIndex WASDMove::ComponentType::Index;
SceneChange::ComponentIndex SceneChange::ComponentType::Index;
LookAt::ComponentIndex LookAt::ComponentType::Index;
CameraHorizontal::ComponentIndex CameraHorizontal::ComponentType::Index;
Shot::ComponentIndex Shot::ComponentType::Index;

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

	//System
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

	SceneManager::LoadScene(SceneManager::GetSceneByName("TestScene"));

	SphereCollider::SetRenderBuffer();
	BoxCollider::SetRenderBuffer();
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

	ObjectManager::ClearnUp();

	if (IsUpdate || IsFixedUpdate) SceneManager::ChangeScene();
}

void CManager::Update()
{
	SystemManager::Update();

	ComponentManager::SendComponentMessage("Update");
}

void CManager::FixedUpdate()
{
	SystemManager::FixedUpdate();

	ComponentManager::SendComponentMessage("FixedUpdate");
}

void CManager::Render()
{
	D3DApp::Renderer::ClearRenderTargetView(Color::gray());

	Camera::Render(Renderer::BeginRender, D3DApp::Renderer::Begin);

	CManager::DebugRender();

	D3DApp::Renderer::End();
}

void CManager::DebugRender()
{
	GUI::guiImGui::SetFrame();

	Input::DebugGUI();
	SceneManager::DebugGUI_ActiveScene();
	TimeManager::DebugGUI_Time();

	GUI::guiImGui::Render();
}

void CManager::Finalize()
{
	BoxCollider::ReleaseRenderBuffer();
	SphereCollider::ReleaseRenderBuffer();

	AudioManager::Release();
	ModelManager::Release();
	TextureManager::Release();

	SystemManager::Finalize();
	SystemManager::Release();

	TimeManager::Destroy();
	GUI::guiImGui::Destroy();

	ObjectManager::Release();

	ComponentManager::Release();
	EntityManager::Release();
}