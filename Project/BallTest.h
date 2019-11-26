#pragma once

class DirectX::GameObject;

class BallTest final :public Scene 
{
public:
	BallTest() :Scene("BallTest") {};
public:
	void Load()
	{
		//Camera
		{
			GameObject* camera = this->AddSceneObject("MainCamera",TagName::Default);
			camera->transform()->position(Vector3::up()*2.0f);
			camera->AddComponent<Camera>()->SetPriority(1);
		}
		//Ball
		{
			GameObject* ball = this->AddSceneObject("Ball",TagName::Default);
			ball->transform()->position(Vector3::up() + Vector3::forward()*6.0f);
			ball->AddComponent<MeshRender>()->SetMesh<Model>()->GetAsset("Ball");
			ball->AddComponent<BallScript>();
			ball->AddComponent<SceneChange>()->nextScene = "BallTest";
		}
		//Field
		{
			GameObject* field = this->AddSceneObject("Field",TagName::Default);
			field->transform()->localScale({50.0f,50.0f,50.0f});
			field->AddComponent<MeshRender>()->SetMesh<MeshField>()->m_Texture 
				= TextureManager::GetTexture("field004");
		}
	}
};