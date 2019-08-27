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
		GameObject* pCamera = this->AddSceneObject("MainCamera", TagName::MainCamera);
		{
			pCamera->transform.get()->position(Vector3(0.0f, 2.0f, -10.0f));
			pCamera->transform.get()->rotation(Quaternion::Identity());
			pCamera->transform.get()->localScale(Vector3::one());
			Camera* camera = pCamera->AddComponent<Camera>();
			camera->SetPriority(1);
		}

		//Sky
		GameObject* Sky = this->AddSceneObject("Sky", TagName::Default);
		{
			Sky->transform->localScale(Vector3::one()*50.0f);
			Sky->AddComponent<MeshRender>()->SetMesh<SkySphere>()
				->m_Texture = TextureManager::GetTexture("sky");
		}

		GameObject* bg = this->AddSceneObject("bg", TagName::Default);
		{
			bg->transform->localScale(Vector3(40.0f, 30.0f, 40.0f));
			bg->transform->localRotation(Quaternion::AngleAxisToRadian(Mathf::PI,Vector3::up()));
			//Mesh
			{
				auto meshRender = bg->AddComponent<MeshRender>();
				auto mesh = meshRender->SetMesh<MeshCircle>();
				mesh->m_Texture = TextureManager::GetTexture("bg");
			}
		}

		GameObject* enemy = this->AddSceneObject("Enemy", TagName::Enemy);
		{
			enemy->transform->position(Vector3(3.0f, 2.0f, 0.0f));
			enemy->transform->localScale(Vector3(1.5f, 1.5f, 1.5f));
			enemy->transform->localRotation(Quaternion::AngleAxisToEuler(-90.0f, Vector3::up()));
			//Mesh
			{
				auto meshRender = enemy->AddComponent<MeshRender>();
				auto mesh = meshRender->SetMesh<Model>();
				mesh->GetAsset("Monster");
				mesh->SetCulling(false);
				mesh->SetDepth(false);
				mesh->SetoffsetMatrix(XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(0.0f, -1.0f, 0.0f));
			}
		}

		//Rock
		{
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(4.0f, 0.0f, -5.0f));
				rock->transform->localScale(Vector3(1.0f, 1.0f, 1.0f));
				auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
				mesh->GetAsset("Rock");
			}
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(5.0f, 0.0f, -1.0f));
				rock->transform->localScale(Vector3(3.0f, 3.0f, 3.0f));
				rock->transform->localRotation(Quaternion::AngleAxisToEuler(30.0f,Vector3::up()));
				auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
				mesh->GetAsset("Rock");
			}
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(-5.0f, 0.0f, 5.0f));
				rock->transform->localScale(Vector3(3.0f, 3.0f, 3.0f));
				rock->transform->localRotation(Quaternion::AngleAxisToEuler(30.0f, Vector3::up()));
				auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
				mesh->GetAsset("Rock");
				mesh->SetDepth(false);
			}
		}

		//Field
		GameObject* Ground = this->AddSceneObject("Ground", TagName::Default);
		{
			Ground->transform->localScale(Vector3(50.0f, 0.5f, 50.0f));
			//Mesh
			{
				auto meshRender = Ground->AddComponent<MeshRender>();
				auto mesh = meshRender->SetMesh<MeshField>();
				mesh->m_Texture = TextureManager::GetTexture("field004");
			}
			//Collider
			{
				auto boxCollider = Ground->AddComponent<BoxCollider>();
				boxCollider->Center(Vector3(0.0f, 0.0f, 0.0f));
				boxCollider->SetSize(Vector3(1.0f, 1.0f, 1.0f));
			}
		}

		

		//PressMessage
		{
			GameObject* PressMessage = this->AddSceneObject("PressMessage",TagName::Default);
			PressMessage->transform->position(Vector3(D3DApp::GetScreenWidth()*0.5f,D3DApp::GetScreenHeight()*0.7f,0.0f));
			PressMessage->transform->localScale(Vector3(50.0f,5.0f,1.0f));
			PressMessage->AddComponent<Image>()->texture = TextureManager::GetTexture("PressMessage");
			PressMessage->AddComponent<PressMessageScript>()->nextScene ="GameMain";
		}

		//SceneChange
		{
			auto SceneChanger = this->AddSceneObject("SceneChanger",TagName::Default);
			SceneChanger->transform->position(Vector3(487.0f, 264.0f, 0.0f));
			SceneChanger->transform->localScale(Vector3(70.0f,50.0f,0.0f));
			SceneChanger->AddComponent<Move>();
			auto sceneChange = SceneChanger->AddComponent<SceneChange>();
			sceneChange->nextScene = "GameMain";
		}
	}
};