#pragma once

class GameObject;

class TitleScene final :public Scene
{
public:
	//Font* font;
public:
	TitleScene() :Scene("TitleScene") {
		//this->font = new Font();
		//this->font->Load("Asset/cinecaption226.ttf", "cinecaption");
	};
	~TitleScene()
	{
		//delete font;
	};
public:
	void Load(){

		GameObject* MainCamera = this->AddSceneObject("MainCamera", TagName::Default);
		{
			MainCamera->transform().lock()->position(Vector3(0.0f,2.0f,-10.0f));
			MainCamera->transform().lock()->rotation(Quaternion::Identity());
			MainCamera->transform().lock()->localScale(Vector3::one());
			MainCamera->AddComponent<CameraMouse>();
			auto camera = MainCamera->AddComponent<Camera>();
			camera.lock()->SetPriority(1);
		}

		//GameObject* Sky = this->AddSceneObject("Sky",TagName::Default);
		//{
		//	Sky->transform().lock()->localScale(Vector3::one()*50.0f);
		//	Sky->AddComponent<MeshRender>().lock()->SetMesh<SkySphere>()
		//		->m_Texture = TextureManager::GetTexture("sky");
		//}

		//GameObject* bg = this->AddSceneObject("bg", TagName::Default);
		//{
		//	bg->transform().lock()->localScale(Vector3(40.0f, 30.0f, 40.0f));
		//	bg->transform().lock()->localRotation(Quaternion::AngleAxisToRadian(Mathf::PI,Vector3::up()));
		//	//Mesh
		//	{
		//		auto meshRender = bg->AddComponent<MeshRender>();
		//		auto mesh = meshRender.lock()->SetMesh<MeshCircle>();
		//		mesh->m_Texture = TextureManager::GetTexture("bg");
		//	}
		//}

		//GameObject* enemy = this->AddSceneObject("Enemy", TagName::Enemy);
		//{
		//	enemy->transform().lock()->position(Vector3(3.0f, 2.0f, 0.0f));
		//	enemy->transform().lock()->localScale(Vector3(1.5f, 1.5f, 1.5f));
		//	enemy->transform().lock()->localRotation(Quaternion::AngleAxisToEuler(-90.0f, Vector3::up()));
		//	//Mesh
		//	{
		//		auto meshRender = enemy->AddComponent<MeshRender>();
		//		auto mesh = meshRender.lock()->SetMesh<Model>();
		//		mesh->GetAsset("Monster");
		//		mesh->SetCulling(false);
		//		mesh->SetDepth(false);
		//		mesh->SetoffsetMatrix(XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(0.0f, -1.0f, 0.0f));
		//	}
		//}

		////Rock
		//{
		//	{
		//		GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
		//		rock->transform().lock()->position(Vector3(4.0f, 0.0f, -5.0f));
		//		rock->transform().lock()->localScale(Vector3(1.0f, 1.0f, 1.0f));
		//		rock->AddComponent<MeshRender>().lock()->SetMesh<Model>()->GetAsset("Rock");
		//	}
		//	{
		//		GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
		//		rock->transform().lock()->position(Vector3(5.0f, 0.0f, -1.0f));
		//		rock->transform().lock()->localScale(Vector3(3.0f, 3.0f, 3.0f));
		//		rock->transform().lock()->localRotation(Quaternion::AngleAxisToEuler(30.0f,Vector3::up()));
		//		rock->AddComponent<MeshRender>().lock()->SetMesh<Model>()->GetAsset("Rock");
		//	}
		//	{
		//		GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
		//		rock->transform().lock()->position(Vector3(-5.0f, 0.0f, 5.0f));
		//		rock->transform().lock()->localScale(Vector3(3.0f, 3.0f, 3.0f));
		//		rock->transform().lock()->localRotation(Quaternion::AngleAxisToEuler(30.0f, Vector3::up()));
		//		auto mesh = rock->AddComponent<MeshRender>().lock()->SetMesh<Model>();
		//		mesh->GetAsset("Rock");
		//		mesh->SetDepth(false);
		//	}
		//}

		////Field
		//GameObject* Ground = this->AddSceneObject("Ground", TagName::Default);
		//{
		//	Ground->transform().lock()->localScale(Vector3(50.0f, 0.5f, 50.0f));
		//	//Mesh
		//	{
		//		auto meshRender = Ground->AddComponent<MeshRender>();
		//		auto mesh = meshRender.lock()->SetMesh<MeshField>();
		//		mesh->m_Texture = TextureManager::GetTexture("field004");
		//	}
		//	//Collider
		//	{
		//		auto boxCollider = Ground->AddComponent<BoxCollider>();
		//		boxCollider.lock()->Center(Vector3(0.0f, 0.0f, 0.0f));
		//		boxCollider.lock()->SetSize(Vector3(1.0f, 1.0f, 1.0f));
		//	}
		//}
	}
};