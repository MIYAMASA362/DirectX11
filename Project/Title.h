#pragma once

class DirectX::GameObject;

class TitleScene final :public Scene
{
public:
	Font* font;
public:
	TitleScene() :Scene("TitleScene") {
		this->font = new Font();
		this->font->Load("Asset/cinecaption226.ttf", "cinecaption");
	};
	~TitleScene()
	{
		delete font;
	};
public:
	void Load(){

		//pCamera
		{
			GameObject* pCamera;
			pCamera = this->AddSceneObject("MainCamera", TagName::MainCamera);
			pCamera->transform.get()->position(Vector3(0.0f, 10.0f, -10.0f));
			pCamera->transform.get()->rotation(Quaternion::Identity());
			pCamera->transform.get()->localScale(Vector3::one());
			Camera* camera = pCamera->AddComponent<Camera>();
			camera->SetPriority(1);
		}
		//SceneChange
		{
			auto SceneChanger = this->AddSceneObject("SceneChanger");
			SceneChanger->transform->position(Vector3(487.0f, 264.0f, 0.0f));
			SceneChanger->transform->localScale(Vector3(50.0f,50.0f,0.0f));
			SceneChanger->AddComponent<Move>();
			auto sceneChange = SceneChanger->AddComponent<SceneChange>();
			sceneChange->nextScene = "TestScene";
			auto button = SceneChanger->AddComponent<Button>();
			//button->texture = TextureManager::GetTexture("k-on0664");
			button->texture = font->TransformToTexture("‚ ");
		}
	}
};