#pragma once

using namespace DirectX;

class DirectX::GameObject;

class TestScene:public Scene
{
private:
	GameObject* pCamera;
	GameObject* pWallField;
	GameObject* pModel;
public:
	TestScene():Scene("TestScene")
	{
		//pCamera
		{
			pCamera = this->AddSceneObject("MainCamera",TagManager::MainCamera);
			pCamera->AddComponent<CCamera>();
			pCamera->transform =
				Transform(Vector3(0.0f,1.0f,5.0f),Vector3::one(),Vector3::zero());
		}
		//pWallField
		{
			pWallField = this->AddSceneObject("WallField");
			pWallField->AddComponent<CWallField>();
			pWallField->transform =
				Transform(Vector3::zero(), Vector3::one() * 10.0f, Vector3::zero());
		}
		//pModel
		{
			pModel = this->AddSceneObject("Miku");
			pModel->AddComponent<CModel>();
		}
	};
};