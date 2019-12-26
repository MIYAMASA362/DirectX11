#pragma once

class GameObject;

class TestScene3 final :public Scene
{
public:
	TestScene3() :Scene("TestScene3") {};
public:
	void Load()
	{
		////pCamera
		//{
		//	GameObject* pCamera;
		//	pCamera = this->AddSceneObject("MainCamera", TagName::MainCamera);
		//	pCamera->transform.get()->position(Vector3(0.0f, 10.0f, -10.0f));
		//	pCamera->transform.get()->rotation(Quaternion::AngleAxisToEuler(30.0f, Vector3::right()));
		//	pCamera->transform.get()->localScale(Vector3::one());
		//	Camera* camera = pCamera->AddComponent<Camera>();
		//	camera->SetPriority(1);
		//}
		////pModel
		//{
		//	GameObject* pModel;
		//	pModel = this->AddSceneObject("Miku",TagName::Default);
		//	pModel->transform.get()->position(Vector3::up()*2.0f);
		//	pModel->transform.get()->localScale(Vector3::one()*2.0f);
		//	pModel->AddComponent<MeshRender>()->SetMesh<Model>()->GetAsset("Miku");
		//	pModel->AddComponent<BoxCollider>();
		//}
		////SceneChange
		//{
		//	auto SceneChanger = this->AddSceneObject("SceneChanger",TagName::Default);
		//	auto sceneChange = SceneChanger->AddComponent<SceneChange>();
		//	sceneChange->nextScene = "TitleScene";
		//}
	}
};
