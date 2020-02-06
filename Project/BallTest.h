#pragma once

class GameObject;
//*********************************************************************************************************************
//
//	BallTest Scene
//
//*********************************************************************************************************************
class BallTest final :public Scene
{
public:
	BallTest() :Scene("BallTest","") {};
public:
	void Load()
	{
		//Camera
		auto camera = this->AddSceneObject("MainCamera", TagName::Default);
		{
			camera->transform().lock()->position(Vector3::up()*2.0f);
			camera->AddComponent<Camera>().lock()->SetPriority(1);

		}

		////Sky
		//GameObject* SkyShpere = this->AddSceneObject("SkySphere", TagName::Default);
		//{
		//	SkyShpere->transform().lock()->localScale(Vector3::one() * 10.0f);

		//	auto renderer = SkyShpere->AddComponent<MeshRender>().lock();
		//	renderer->_Material._Shader = ShaderManager::GetShader();
		//	renderer->_Material._Texture = TextureManager::GetTexture("sky");
		//	SkyShpere->AddComponent<MeshFilter>().lock()->SetMesh(MeshManager::GetMesh("Sphere"));
		//}

		////Plane
		//GameObject* plane = this->AddSceneObject("Plane", TagName::Default);
		//{
		//	plane->transform().lock()->position(Vector3::up() + Vector3::forward() * 6.0f);
		//	plane->transform().lock()->localScale(Vector3::one() * 1.0f);
		//	auto material = &plane->AddComponent<MeshRender>().lock()->_Material;
		//	material->_Shader = ShaderManager::GetShader();
		//	material->_Texture = TextureManager::GetTexture("field004");
		//	plane->AddComponent<MeshFilter>().lock()->SetMesh(MeshManager::GetMesh("Sphere"));

		//	plane->AddComponent<BallScript>();
		//}
	}
};