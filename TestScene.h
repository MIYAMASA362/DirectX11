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
		pWallField->transform.position = Vector3(0.0f,0.0f,0.0f);
		pWallField->transform.scale = Vector3(10.0f,10.0f,10.0f);

		pModel = this->AddSceneObject("Miku");
		pModel->AddComponent<CModel>();

		//pPolygon = this->AddSceneObject("Polygon");
		//pPolygon->AddComponent<CPolygon>();
	};
};