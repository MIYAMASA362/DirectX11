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
		GameObject* ball = this->AddSceneObject("Ball", TagName::Default);
		{
			
			ball->transform().lock()->position(Vector3::up() + Vector3::forward()*6.0f);
			/*auto renderer = ball->AddComponent<MeshRender>().lock();

			renderer->SetModel(ModelManager::GetModel("miku_01").lock());

			Material* material = new Material();
			material->_Shader = ShaderManager::GetShader();
			material->_Texture = TextureManager::GetTexture("miku_01");

			renderer->SetMaterial(material);*/

			ball->AddComponent<BallScript>();
			ball->AddComponent<SceneChange>().lock()->nextScene = "BallTest";

		}

		{
			GameObject* miku = ModelManager::AddSceneModel("miku_01", this);
			miku->transform().lock()->SetParent(ball->transform());
			
			//miku->transform().lock()->position(Vector3::up() + Vector3::forward()*6.0f);
			//miku->AddComponent<BallScript>();
		}

		////Field
		//{
		//	GameObject* field = this->AddSceneObject("Field",TagName::Default);
		//	field->transform().lock()->localScale({50.0f,50.0f,50.0f});
		//	field->AddComponent<MeshRender>().lock()->SetMesh<MeshField>()->m_Texture 
		//		= TextureManager::GetTexture("field004");
		//}
	}
};