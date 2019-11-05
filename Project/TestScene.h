#pragma once

class DirectX::GameObject;

class TestScene final :public Scene
{
public:
	TestScene():Scene("TestScene"){};
	void Load() override
	{
		GameObject* Sky = this->AddSceneObject("Sky", TagName::Default);
		{
			Sky->transform()->localScale(Vector3::one()*50.0f);
			Sky->AddComponent<MeshRender>()->SetMesh<SkySphere>()
				->m_Texture = TextureManager::GetTexture("sky");
		}

		//wall
		GameObject* wall = this->AddSceneObject("Wall",TagName::Default);
		{
			wall->transform()->localScale(Vector3::one()*50.0f);
			wall->AddComponent<MeshRender>()->SetMesh<MeshCircle>()
				->m_Texture = TextureManager::GetTexture("sky");
		}

		//Field
		GameObject* Field = this->AddSceneObject("Field",TagName::Default);
		{
			Field->transform()->position(Vector3::up() * 3.0f);
			Field->transform()->localScale(Vector3::one()*50.0f);
			Field->AddComponent<MeshRender>()->SetMesh<MeshField>()
				->m_Texture = TextureManager::GetTexture("field004");
			auto boxCollider = Field->AddComponent<BoxCollider>();
			boxCollider->Center(Vector3::zero()-Vector3::down());
			boxCollider->SetSize(Vector3(1.0f,0.1f,1.0f));
		}

		//Field2
		GameObject* Field2 = this->AddSceneObject("Field2", TagName::Default);
		{
			Field2->transform()->position(Vector3::down()*10.0f);
			Field2->transform()->localScale(Vector3::one()*50.0f);
			Field2->AddComponent<MeshRender>()->SetMesh<MeshField>()
				->m_Texture = TextureManager::GetTexture("field004");
			auto boxCollider = Field2->AddComponent<BoxCollider>();
			boxCollider->Center(Vector3::zero());
			boxCollider->SetSize(Vector3(1.0f, 0.1f, 1.0f));
		}

		//block
		GameObject* block = this->AddSceneObject("Block", TagName::Default);
		{
			block->transform()->position(Vector3::up() * 10.0f);
			block->transform()->localScale(Vector3(1.0f,0.0f,1.0f) * 10.0f + Vector3::up());
			block->AddComponent<MeshRender>()->SetMesh<MeshField>()
				->m_Texture = TextureManager::GetTexture("field004");
			auto boxCollider = block->AddComponent<BoxCollider>();
		}

		//pModel
		GameObject* pModel = this->AddSceneObject("Miku", TagName::ChildMiku);
		{
			pModel->transform()->position(Vector3::up()*4.0f);
			pModel->transform()->localScale(Vector3::one());
			pModel->AddComponent<WASDMove>();
			pModel->AddComponent<CameraHorizontal>();
			pModel->AddComponent<MeshRender>()->SetMesh<Model>()->GetAsset("Miku");
			pModel->AddComponent<Rigidbody>()->SetMass(0.5f);
			pModel->AddComponent<Shot>();
			//auto sphereCollider = pModel->AddComponent<SphereCollider>();
			//sphereCollider->SetRadius(2.0f);
			auto boxCollider = pModel->AddComponent<BoxCollider>();
			boxCollider->Center(Vector3::zero());
			boxCollider->SetSize(Vector3::one()*0.5f);
		}

		//MikuCamera
		GameObject* MikuCamera = this->AddSceneObject("MikuCamera", TagName::Default);
		{
			MikuCamera->AddComponent<Camera>()->SetPriority(0);
			MikuCamera->transform()->SetParent(pModel);
			MikuCamera->transform()->localPosition(Vector3::back()*3.0f);
			MikuCamera->transform()->rotation(Quaternion::Euler({ 45.0f,0.0f,0.0f }));
			MikuCamera->AddComponent<CameraMouse>();
		}
		//ChildMiku
		GameObject* ChildMiku = this->AddSceneObject("ChildMiku", TagName::Default);
		{
			ChildMiku->AddComponent<LookAt>()->parent = pModel;
			ChildMiku->AddComponent<MeshRender>()->SetMesh<Model>()->GetAsset("Miku");
			ChildMiku->transform()->position(Vector3::forward()*10.0f + Vector3::up());
			ChildMiku->transform()->rotation(Vector3::zero());
			ChildMiku->transform()->localScale(Vector3::one());
			auto boxCollider = ChildMiku->AddComponent<SphereCollider>();
			boxCollider->Center(Vector3::zero());
			boxCollider->SetRadius(2.0f);
			boxCollider->IsTrigger = true;
		}

		//Billboard
		GameObject* billboard = this->AddSceneObject("Billboard", TagName::Default);
		{
			billboard->transform()->localScale(Vector3::one()*1.0f);
			{
				auto component = billboard->AddComponent<MeshRender>()->SetMesh<Billboard>();
				component->m_Texture = TextureManager::GetTexture("field004");
				component->lockAxis.y = true;
				component->IsPivotBottom = true;
			}
		}

		//SceneChange
		GameObject* SceneChanger = this->AddSceneObject("SceneChanger", TagName::Default);
		{
			auto sceneChange = SceneChanger->AddComponent<SceneChange>();
			sceneChange->nextScene = "TestScene";
		}
	}
};