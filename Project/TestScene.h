#pragma once

class DirectX::GameObject;

class TestScene final :public Scene
{
public:
	TestScene():Scene("TestScene"){};
	void Load() override
	{
		//Sky
		GameObject* Sky = this->AddSceneObject("Sky", TagName::Default);
		{
			Sky->transform().lock()->localScale(Vector3::one()*80.0f);
			Sky->AddComponent<MeshRender>().lock()->SetMesh<SkySphere>()
				->m_Texture = TextureManager::GetTexture("sky");
		}
		
		//Field
		FieldCollider* fieldCollider;
		GameObject* Field = this->AddSceneObject("Field",TagName::Default);
		{
			Field->transform().lock()->localScale(Vector3(1.0f,0.1f,1.0f) * 50.0f);
			auto mesh = Field->AddComponent<MeshRender>().lock()->SetMesh<MeshField>();
			mesh->m_Texture = TextureManager::GetTexture("field004");
			fieldCollider = Field->AddComponent<FieldCollider>().lock().get();
			fieldCollider->SetMesh(mesh);

			auto render = Field->GetComponent<Renderer>();
			Physics::AddRayCastTarget(std::dynamic_pointer_cast<MeshRender>(render.lock()));
		}

		//pModel
		GameObject* pModel = this->AddSceneObject("Miku", TagName::ChildMiku);
		{
			pModel->transform().lock()->position(Vector3::up()*10.0f);
			pModel->transform().lock()->localScale(Vector3::one());
			pModel->AddComponent<MeshRender>().lock()->SetMesh<Model>()->GetAsset("Miku");
			pModel->AddComponent<Rigidbody>().lock()->IsUseGravity(false);
			//pModel->AddComponent<SphereCollider>().lock()->SetRadius(1.0f);
			auto box = pModel->AddComponent<BoxCollider>().lock();
			box->SetSize({1.0f,1.0f,1.0f});
			auto player = pModel->AddComponent<Player>().lock();
			player->_fieldCollider = fieldCollider;

			/*for(int i = 0; i < 10; i++)
			{
				GameObject* child = this->AddSceneObject("Child" + std::to_string(i),TagName::Default);
				{
					child->AddComponent<MeshRender>().lock()->SetMesh<Model>()->GetAsset("Miku");
					child->AddComponent<BoxCollider>();
					child->transform().lock()->SetParent(pModel->gameObject());
					child->transform().lock()->localPosition(pModel->transform().lock()->right() * (i+1));
				}
			}*/
		}

		//MikuCamera
		GameObject* MikuCamera = this->AddSceneObject("MikuCamera", TagName::Default);
		{
			MikuCamera->AddComponent<Camera>().lock()->SetPriority(0);
			MikuCamera->transform().lock()->SetParent(pModel->gameObject());
			MikuCamera->transform().lock()->localPosition(Vector3::up() + Vector3::back() * 4.0f);
			MikuCamera->transform().lock()->rotation(Quaternion::Euler({ 0.0f,0.0f,0.0f }));
			MikuCamera->AddComponent<CameraMouse>();
		}

		//SceneChange
		GameObject* SceneChanger = this->AddSceneObject("SceneChanger", TagName::Default);
		{
			auto sceneChange = SceneChanger->AddComponent<SceneChange>();
			sceneChange.lock()->nextScene = "TestScene";
		}

		GameObject* sphere = this->AddSceneObject("Sphere",TagName::Default);
		{
			sphere->transform().lock()->position({0.0f,0.0f,10.0f});
			sphere->transform().lock()->localScale(Vector3::one()*2.0f);

			//auto rigidbody = sphere->AddComponent<Rigidbody>().lock();
			//rigidbody->IsUseGravity(false);

			auto collider = sphere->AddComponent<SphereCollider>().lock();
			collider->SetRadius(2.0f);
			collider->IsTrigger = true;
		}

		GameObject* box = this->AddSceneObject("Box",TagName::Default);
		{
			box->transform().lock()->position({10.0f,0.0f,10.0f});
			box->transform().lock()->localScale(Vector3::one() *2.0f);

			auto collider = box->AddComponent<BoxCollider>().lock();
			collider->SetSize({1.0f,1.0f,1.0f});
			collider->IsTrigger = true;

			box->AddComponent<RotationBox>();
		}

		//Canvas
		GameObject* canvas = this->AddSceneObject("Canvas",TagName::Default);
		{
			canvas->transform().lock()->localPosition(Vector3(100.0f,50.0f,0.0f));
			canvas->transform().lock()->localScale(Vector3::one()*10.0f);
			auto image = canvas->AddComponent<Image>();
			image.lock()->texture = TextureManager::GetTexture("field004");
		}

	}
};