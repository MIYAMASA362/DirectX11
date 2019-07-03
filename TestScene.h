#pragma once

using namespace DirectX;

class DirectX::GameObject;

class TestScene final :public Scene
{
public:
	TestScene():Scene("TestScene")
	{
		//pCamera
		{
			GameObject* pCamera;
			pCamera = this->AddSceneObject("MainCamera",TagName::MainCamera);
			pCamera->transform.get()->position(Vector3(0.0f, 2.0f, -5.0f));
			pCamera->transform.get()->rotation(Quaternion::AngleAxis(0.0f, Vector3::up()));
			pCamera->transform.get()->scale(Vector3::one());
			Camera* camera = pCamera->AddComponent<Camera>();
			camera->SetPriority(1);
			pCamera->SetActive(false);
		}
		//pWallField
		{
			GameObject* pWallField;
			pWallField = this->AddSceneObject("WallField");
			pWallField->AddComponent<MeshRender>()->SetMesh<CWallField>();
			pWallField->transform.get()->position(Vector3::zero());
			pWallField->transform.get()->rotation(Vector3::zero());
			pWallField->transform.get()->scale(Vector3::one()*10.0f);
		}
		//pModel
		{
			GameObject* pModel;
			pModel = this->AddSceneObject("Miku");
			pModel->transform.get()->scale(Vector3::one() * 2.0f);
			pModel->AddComponent<MikuMove>();
			pModel->AddComponent<MeshRender>()->SetMesh<CModel>()->Load("asset/miku_01.obj");
			{
				auto pCamera = this->AddSceneObject("ModelCamera",TagName::MainCamera);
				pCamera->AddComponent<Camera>();
				pCamera->transform->SetParent(pModel);
				pCamera->transform->position(Vector3(0.0f, 1.0f, -3.0f));
				pCamera->transform->rotation(Vector3::zero());
				pCamera->transform->scale(Vector3::one());
				pCamera->AddComponent<KeyMove>();
			}
		}
	};
};