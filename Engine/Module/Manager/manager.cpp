#include"Common.h"
#include"Module\DirectX\DirectX.h"

//--- Module ------------------------------------
#include"Module\Module.h"

#include"Module\IMGUI\GUI_ImGui.h"


#include"manager.h"
#include"Module\Shader\Shader.h"
#include"Module\Shader\ShaderManager.h"

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

//*********************************************************************************************************************
//
//	CManager
//
//*********************************************************************************************************************

bool CManager::_IsUpdate = false;
bool CManager::_IsFixedUpdate = false;

void CManager::Initialize(HWND hWnd ,unsigned int fps)
{
	//ECS
	EntityManager::Create();
	ComponentManager::Create();

	TimeManager::Create(fps);

	Input::Initialize(hWnd);
	Input::Mouse::SetScreenLoop(false);


	//Texture
	TextureManager::Create();
	TextureManager::LoadTexture("Asset/Texture/field004.tga");
	TextureManager::LoadTexture("Asset/Texture/number.tga");
	TextureManager::LoadTexture("Asset/Texture/k-on0664.tga");
	TextureManager::LoadTexture("Asset/Texture/sky.tga");
	TextureManager::LoadTexture("Asset/Texture/background.tga");
	TextureManager::LoadTexture("Asset/Texture/ItemSlot.tga");
	TextureManager::LoadTexture("Asset/Texture/MapBG.tga");
	TextureManager::LoadTexture("Asset/Texture/Compass.tga");
	TextureManager::LoadTexture("Asset/Texture/ItemSlotBG.tga");
	TextureManager::LoadTexture("Asset/Texture/PressMessage.tga");

	//Model
	ModelManager::LoadAssetForAssimp("Asset/Model/Miku/miku_01.obj");
	ModelManager::LoadAssetForAssimp("Asset/Model/Miku02/miku_02.obj");
	ModelManager::LoadAssetForAssimp("Asset/Model/Monster/monster.obj");
	ModelManager::LoadAssetForAssimp("Asset/Model/Rock/rock.obj");
	ModelManager::LoadAssetForAssimp("Asset/Model/Cube/cube.obj");
	ModelManager::LoadAssetForAssimp("Asset/Model/Ball/ball.obj");

	//Audio
	AudioManager::CreateDevice();
	AudioManager::LoadAsset(AudioAsset("Experimental","Experimental_Model_long .wav"));

	//Shader
	ShaderManager::Create();

	//Scene
	D3DRenderer::SetRenderStatus(D3DRenderer::GetRenderStatus(hWnd));
	SceneManager::CreateScene<TestScene>();
	SceneManager::CreateScene<TestScene2>();
	SceneManager::CreateScene<TestScene3>();
	SceneManager::CreateScene<BallTest>();

	SceneManager::CreateScene<TitleScene>();
	SceneManager::CreateScene<GameMain>();


	SphereCollider::SetRenderBuffer();
	BoxCollider::SetRenderBuffer();

	SceneManager::LoadScene(SceneManager::GetSceneByName("BallTest"));

	ComponentManager::SendComponentMessage("Start");
}

void CManager::SetFrame()
{
	TimeManager::Update();

	_IsUpdate = TimeManager::IsUpdate();
	_IsFixedUpdate = TimeManager::IsFixedUpdate();
}

bool CManager::IsProcess()
{
	return _IsUpdate || _IsFixedUpdate;
}

void CManager::Update()
{
	if (!_IsUpdate) return;
	Input::Update();

	ComponentManager::SendComponentMessage("Update");
}

void CManager::FixedUpdate()
{
	if (!_IsFixedUpdate) return;
	//Collider::IsHitReset();
	ComponentManager::SendComponentMessage("FixedUpdate");
	Rigidbody::ApplyRigidbody();
	Rigidbody::CollisionRigidbody();
}

void CManager::Render(RenderStatus* renderstatus)
{
	D3DRenderer::SetRenderStatus(renderstatus);
	Camera::Render(Renderer::BeginRender,renderstatus);

	//Renderer2D::BeginRender();
}

void CManager::EndFrame()
{
	SceneManager::ChangeScene();
	ObjectManager::ClearnUp();
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

	TextureManager::EditorWindow();
	ObjectManager::EditorWindow();

	Camera::EditorWindow();

	GUI::guiImGui::Render();
}

void CManager::Finalize()
{
	BoxCollider::ReleaseRenderBuffer();
	SphereCollider::ReleaseRenderBuffer();

	ShaderManager::Release();
	AudioManager::Release();
	ModelManager::Release();
	TextureManager::Release();

	ObjectManager::Release();

	ComponentManager::Release();
	EntityManager::Release();
}