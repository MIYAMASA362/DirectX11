#pragma once

using namespace DirectX;

class DirectX::GameObject;

class TestScene:public Scene
{
private:
	GameObject* pCamera;
	GameObject* pPolygon;
	GameObject* pWallField;
	GameObject* pModel;
public:
	TestScene():Scene("TestScene")
	{
		pCamera = this->AddSceneObject("MainCamera");
		pCamera->AddComponent<CCamera>();

		pWallField = this->AddSceneObject("WallField");
		pWallField->AddComponent<CWallField>();
		pWallField->transform = 
			Transform( Vector3::zero(), Vector3::one() * 10.0f, Vector3::zero());

		pModel = this->AddSceneObject("Miku");
		pModel->AddComponent<CModel>();

		//pPolygon = this->AddSceneObject("Polygon");
		//pPolygon->AddComponent<CPolygon>();
	};
};