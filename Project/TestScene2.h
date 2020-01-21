#pragma once

class GameObject;

class TestScene2 final:public Scene
{
public:
	TestScene2() :Scene("TestScene2","") {};
public:
	void Load()
	{
	//
	//	//pCamera
	//	{
	//		GameObject* pCamera;
	//		pCamera = this->AddSceneObject("MainCamera", TagName::MainCamera);
	//		pCamera->transform.get()->position(Vector3(0.0f, 10.0f, -10.0f));
	//		pCamera->transform.get()->rotation(Quaternion::Identity());
	//		pCamera->transform.get()->localScale(Vector3::one());
	//		Camera* camera = pCamera->AddComponent<Camera>();
	//		camera->SetPriority(1);
	//	}
	//	//SceneChange
	//	{
	//		auto SceneChanger = this->AddSceneObject("SceneChanger",TagName::Default);
	//		auto sceneChange = SceneChanger->AddComponent<SceneChange>();
	//		sceneChange->nextScene = "TestScene";
	//	}
	}
};
