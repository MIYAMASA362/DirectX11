#include"Common.h"
#include<fstream>
#include"Module\DirectX\DirectX.h"

//--- Module ------------------------------------
#include"Module\Module.h"

#include"Module\IMGUI\GUI_ImGui.h"


#include"manager.h"
#include"Module\Shader\Shader.h"
#include"Module\Shader\ShaderManager.h"

#include"Module\Geometry\Geometry.h"

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
//	内部関数
//
//*********************************************************************************************************************

//LoadAssetData_Directory
//	ディレクトリ内のアセットを見つける
//
void LoadAssetData_Directory(std::string directory,std::string AssetDirectory)
{
	HANDLE hFind;
	WIN32_FIND_DATA file;

	TCHAR current[MAX_PATH + 1];
	strcpy_s(current, directory.c_str());
	strcat_s(current, "\\*");
	hFind = FindFirstFile(current, &file);

	if (hFind == INVALID_HANDLE_VALUE) return;

	do
	{
		//下層ディレクトリ探査
		if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((strcmp(file.cFileName, ".") == 0) || strcmp(file.cFileName, "..") == 0) continue;

			TCHAR path[MAX_PATH + 1];
			strcpy_s(path, directory.c_str());
			strcat_s(path, "\\");
			strcat_s(path, file.cFileName);

			LoadAssetData_Directory(path,AssetDirectory + "\\" + file.cFileName);
		}
		//ファイルを発見
		else
		{
			//拡張子の取得
			std::string asset = file.cFileName;
			size_t pos = asset.find_last_of(".");
			std::string extension = asset.substr(pos + 1);

			std::string name = AssetDirectory + "\\" + file.cFileName;

			//テクスチャ
			if (extension == "tga" || extension == "png")
			{
				TextureManager::LoadTexture(name);
			}
			//モデル
			else if(extension == "obj")
			{
				ModelManager::LoadAssetForAssimp(name);
			}
		}
	} while (FindNextFile(hFind, &file));

	FindClose(hFind);
}

//LoadAssetData
//	LoadAssetData_Directoryへの入り口
//
void LoadAssetData(std::string AssetDirectory)
{
	//ディレクトリ取得 現在のディレクトリ
	TCHAR path[MAX_PATH + 1];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\");
	strcat_s(path, AssetDirectory.c_str());

	LoadAssetData_Directory(path,AssetDirectory);
}



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

	TextureManager::Create();

	//Assetフォルダ内のデータを資源として読み込む
	LoadAssetData("Asset");

	//Audio
	AudioManager::CreateDevice();
	AudioManager::LoadAsset(AudioAsset("Experimental","Experimental_Model_long .wav"));

	//Shader
	ShaderManager::Create();
	VERTEX_INPUT_LAYOUT layout[] = 
	{
		VSIL_POSITION,
		VSIL_NORMAL,
		VSIL_COLOR,
		VSIL_TEXCOORD
	};
	ShaderManager::RegisterShader("Shader2D", "Asset/Shader/vertexShader.cso",layout,ARRAYSIZE(layout), "Asset/Shader/pixelShader2D.cso");

	MeshManager::RegisterIndex("Plane",new Geometry::Plane());
	MeshManager::RegisterIndex("Cube",new Geometry::Cube());
	MeshManager::RegisterIndex("Sphere",new Geometry::Sphere());

	D3DRenderer::SetRenderStatus(D3DRenderer::GetRenderStatus(hWnd));

	//Scene
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
	ModelManager::EditorWindow();

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
	MeshManager::Release();

	ObjectManager::Release();

	ComponentManager::Release();
	EntityManager::Release();
}

