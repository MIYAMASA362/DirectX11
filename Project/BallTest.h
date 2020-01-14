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
	BallTest() :Scene("BallTest") {};
public:
	void Load()
	{
		//Camera
		{
			GameObject* camera = this->AddSceneObject("MainCamera",TagName::Default);
			camera->transform().lock()->position(Vector3::up()*2.0f);
			camera->AddComponent<Camera>().lock()->SetPriority(1);
		}
		//Ball
		//	TODO : ‚±‚±‚Ìˆ—‚ðComponent–ˆ‚É®—‚·‚é
		/*GameObject* ball = this->AddSceneObject("Ball", TagName::Default);
		{
			
			ball->transform().lock()->position(Vector3::up() + Vector3::forward()*6.0f);
			ball->AddComponent<BallScript>();
			ball->AddComponent<SceneChange>().lock()->nextScene = "BallTest";
		}

		{
			GameObject* miku = ModelManager::AddSceneModel("miku_01", this);
			miku->transform().lock()->SetParent(ball->transform());
		}
*/
		GameObject* plane = this->AddSceneObject("Plane",TagName::Default);
		{
			plane->transform().lock()->position(Vector3::up() + Vector3::forward() * 6.0f);
			plane->transform().lock()->localScale( Vector3::one() * 1.0f);
			auto material = &plane->AddComponent<MeshRender>().lock()->_Material;
			material->_Shader = ShaderManager::GetShader();
			material->_Texture = TextureManager::GetTexture("field004");
			plane->AddComponent<MeshFilter>().lock()->SetMesh(MeshManager::GetMesh("Sphere"));

			plane->AddComponent<BallScript>();
		}
	}
};