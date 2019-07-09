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
			pCamera->transform.get()->position(Vector3(0.0f, 10.0f, -10.0f));
			pCamera->transform.get()->rotation(Quaternion::AngleAxisToEuler(30.0f, Vector3::right()));
			pCamera->transform.get()->localScale(Vector3::one());
			Camera* camera = pCamera->AddComponent<Camera>();
			camera->SetPriority(1);
		}
		//pWallField
		{
			GameObject* pWallField;
			pWallField = this->AddSceneObject("WallField");
			pWallField->AddComponent<MeshRender>()->SetMesh<CWallField>();
			pWallField->transform.get()->position(Vector3::zero());
			pWallField->transform.get()->rotation(Vector3::zero());
			pWallField->transform.get()->localScale(Vector3::one()*50.0f);
		}
		//pModel
		{
			GameObject* pModel;
			pModel = this->AddSceneObject("Miku");
			pModel->transform.get()->position(Vector3::up()*2.0f);
			pModel->transform.get()->localScale(Vector3::one()*2.0f);
			pModel->AddComponent<MikuMove>();
			pModel->AddComponent<MeshRender>()->SetMesh<CModel>()->Load("asset/miku_01.obj");
			{
				auto pChild = this->AddSceneObject("ChildMiku");
				pChild->AddComponent<KeyMove>()->parent = pModel;
				pChild->AddComponent<MeshRender>()->SetMesh<CModel>()->Load("asset/miku_01.obj");
				//pChild->transform->SetParent(pModel);
				pChild->transform->position(Vector3::up()*2.0f);
				pChild->transform->rotation(Vector3::zero());
				pChild->transform->localScale(Vector3::one()*2.0f);
			}
		}
	};
};