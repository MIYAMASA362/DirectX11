#pragma once

class GameMain final :public Scene
{
public:
	GameMain() :Scene("GameMain") {};
	~GameMain() {};
public:
	void Load() override
	{
		//Sky
		GameObject* Sky = this->AddSceneObject("Sky", TagName::Default);
		{
			Sky->transform->localScale(Vector3::one()*100.0f);
			Sky->AddComponent<MeshRender>()->SetMesh<SkySphere>()
				->m_Texture = TextureManager::GetTexture("sky");
		}

		//background
		GameObject* bg = this->AddSceneObject("bg",TagName::Default);
		{
			bg->transform->localScale(Vector3(80.0f,30.0f,80.0f));
			//Mesh
			{
				auto meshRender = bg->AddComponent<MeshRender>();
				auto mesh = meshRender->SetMesh<MeshCircle>();
				mesh->m_Texture = TextureManager::GetTexture("bg");
			}
		}

		//Rocks
		{
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(10.0f, 0.0f, 20.0f));
				rock->transform->localScale(Vector3(2.0f, 2.0f, 2.0f));
				{
					auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
					mesh->GetAsset("Rock");
				}
				{
					auto collider = rock->AddComponent<SphereCollider>();
					collider->SetRadius(0.7f);
				}
			}
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(-20.0f, 0.0f, 20.0f));
				rock->transform->localScale(Vector3(4.0f, 4.0f, 4.0f));
				{
					auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
					mesh->GetAsset("Rock");
				}
				{
					auto collider = rock->AddComponent<SphereCollider>();
					collider->SetRadius(0.7f);
				}
			}
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(40.0f, 0.0f, -20.0f));
				rock->transform->localScale(Vector3(2.0f, 2.0f, 2.0f));
				rock->transform->localRotation(Quaternion::AngleAxisToRadian(Mathf::PI_2,Vector3::forward()));
				{
					auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
					mesh->GetAsset("Rock");
				}
				{
					auto collider = rock->AddComponent<SphereCollider>();
					collider->SetRadius(0.7f);
				}
			}
			{
				GameObject* rock = this->AddSceneObject("Rock", TagName::Default);
				rock->transform->position(Vector3(30.0f, 0.0f, -20.0f));
				rock->transform->localScale(Vector3(3.0f, 3.0f, 3.0f));
				{
					auto mesh = rock->AddComponent<MeshRender>()->SetMesh<Model>();
					mesh->GetAsset("Rock");
				}
				{
					auto collider = rock->AddComponent<SphereCollider>();
					collider->SetRadius(0.7f);
				}
			}
		}

		//Field
		GameObject* Ground = this->AddSceneObject("Ground",TagName::Default);
		{
			Ground->transform->localScale(Vector3(50.0f,0.5f,50.0f));
			//Mesh
			{
				auto meshRender = Ground->AddComponent<MeshRender>();
				auto mesh = meshRender->SetMesh<MeshField>();
				mesh->m_Texture = TextureManager::GetTexture("field004");
			}
			//Collider
			{
				auto boxCollider = Ground->AddComponent<BoxCollider>();
				boxCollider->Center(Vector3(0.0f,0.0f,0.0f));
				boxCollider->SetSize(Vector3(1.0f,1.0f,1.0f));
			}
		}

		//Enemy
		GameObject* enemy = this->AddSceneObject("Enemy",TagName::Enemy);
		{
			enemy->transform->position(Vector3(0.0f,5.0f,10.0f));
			enemy->transform->localScale(Vector3(1.0f,1.0f,1.0f));
			//Mesh
			{
				auto meshRender = enemy->AddComponent<MeshRender>();
				auto mesh = meshRender->SetMesh<Model>();
				mesh->GetAsset("Monster");
				mesh->SetCulling(false);
				mesh->SetoffsetMatrix(XMMatrixScaling(0.5f,0.5f,0.5f) * XMMatrixTranslation(0.0f,-1.0f,0.0f));
			}
			//Collider
			{
				auto boxCollider = enemy->AddComponent<BoxCollider>();
				boxCollider->SetSize(Vector3(1.0f,1.0f,1.0f));
				auto sphereCollider = enemy->AddComponent<SphereCollider>();
				sphereCollider->Center(Vector3(0.0f,0.0f,0.0f));
				sphereCollider->SetRadius(2.0f);
				sphereCollider->IsTrigger = true;
			}
			//Rigidbody
			{
				auto rigidbody = enemy->AddComponent<Rigidbody>();
			}
			auto enemyScript = enemy->AddComponent<Enemy>();

			GameObject* billboard = this->AddSceneObject("Billboard",TagName::Default);
			{
				billboard->transform->SetParent(enemy);
				billboard->transform->localPosition(Vector3(0.0f,2.0f,0.0f));
				billboard->transform->localScale(Vector3(1.0f,0.2f,1.0f));
				{
					auto component = billboard->AddComponent<MeshRender>()->SetMesh<Billboard>();
					component->m_Texture = TextureManager::GetTexture("sky");
				}
				enemyScript->hpBoard = billboard;
			}
		}

		//Player
		GameObject* player = this->AddSceneObject("Player",TagName::Player);
		{
			player->transform->position(Vector3(0.0f,4.0f,0.0f));
			player->transform->localScale(Vector3(1.0f,1.0f,1.0f));
			//Mesh
			{
				auto meshRender = player->AddComponent<MeshRender>();
				auto model = meshRender->SetMesh<Model>();
				model->GetAsset("Miku02");
				model->SetCulling(true);
			}
			//Collider
			{
				auto boxCollider = player->AddComponent<BoxCollider>();
				boxCollider->SetSize(Vector3(0.5f,0.5f,0.5f));
				auto sphereCollider = player->AddComponent<SphereCollider>();
				sphereCollider->Center(Vector3(0.0f,0.0f,0.5f));
				sphereCollider->IsTrigger = true;
				auto sphereCollider2 = player->AddComponent<SphereCollider>();
				sphereCollider2->SetRadius(0.25f);
			}
			//Rigidbody
			{
				auto rigidbody = player->AddComponent<Rigidbody>();
				rigidbody->SetMass(0.5f);
			}
			player->AddComponent<PlayerMove>();
			player->AddComponent<Player>();
		}
		
		//Pivot
		GameObject* HorizontalPivot = this->AddSceneObject("HorizontalPivot",TagName::Default);
		{
			HorizontalPivot->transform->localPosition(Vector3(0.0f,0.0f,0.0f));
			HorizontalPivot->AddComponent<PivotBehaviour>()->target = player;
		}
		GameObject* VerticalPivot = this->AddSceneObject("VerticalPivot",TagName::Default);
		{
			VerticalPivot->transform->localPosition(Vector3(0.0f,0.0f,0.0f));
			VerticalPivot->transform->SetParent(HorizontalPivot);
		}

		//MainCamera
		GameObject* MainCamera = this->AddSceneObject("MainCamera",TagName::MainCamera);
		{
			MainCamera->transform->SetParent(VerticalPivot);
			MainCamera->transform->localPosition(Vector3(0.0f,0.5f,-4.0f));
			//Camera
			{
				auto camera = MainCamera->AddComponent<Camera>();
				camera->SetPriority(0);
				player->GetComponent<PlayerMove>().lock()->camera = camera;
			}
			{
				auto playerCamera = MainCamera->AddComponent<PlayerCamera>();
				playerCamera->target = enemy;
				playerCamera->HorizonPivot = HorizontalPivot;
				playerCamera->VerticalPivot = VerticalPivot;
				playerCamera->player = player;
			}
		}

		

		//Canvas
		GameObject* canvas = this->AddSceneObject("Canvas",TagName::Default);
		{
			canvas->transform->position(Vector3((float)D3DApp::GetScreenWidth(),(float)D3DApp::GetScreenHeight(),0.0f));
			canvas->AddComponent<Canvas>();

			GameObject* ItemSlot = this->AddSceneObject("ItemSlot",TagName::Default);
			{
				ItemSlot->transform->SetParent(canvas);
				ItemSlot->transform->localPosition(Vector3(-150.0f,-80.0f,0.0f));
				ItemSlot->transform->localScale(Vector3(10.0f,10.0f,10.0f));
				{
					auto script = ItemSlot->AddComponent<ItemSlotScript>();
				}

				GameObject* ItemSlotBG = this->AddSceneObject("ItemSlotBG", TagName::Default);
				{
					ItemSlotBG->transform->SetParent(ItemSlot);
					ItemSlotBG->transform->localPosition(Vector3(0.0f, 0.0f, 0.0f));
					ItemSlotBG->transform->localScale(Vector3(2.25f, 1.5f, 1.0f));
					{
						auto image = ItemSlotBG->AddComponent<Image>();
						image->texture = TextureManager::GetTexture("ItemSlotBG");
					}
				}

				GameObject* ItemSlotCenter = this->AddSceneObject("ItemCenter",TagName::Default);
				{
					ItemSlotCenter->transform->SetParent(ItemSlot);
					ItemSlotCenter->transform->localPosition(Vector3(0.0f, 0.0f, 0.0f));
					ItemSlotCenter->transform->localScale(Vector3(1.0f, 1.0f, 1.0f));
					{
						auto image = ItemSlotCenter->AddComponent<Image>();
						image->texture = TextureManager::GetTexture("ItemSlot");
					}
				}

				GameObject* ItemLeft = this->AddSceneObject("ItemLeft", TagName::Default);
				{
					ItemLeft->transform->SetParent(ItemSlot);
					ItemLeft->transform->localPosition(Vector3(-8.0f, 0.0f, 0.0f));
					ItemLeft->transform->localScale(Vector3(0.5f,0.5f,0.5f));
					{
						auto image = ItemLeft->AddComponent<Image>();
						image->texture = TextureManager::GetTexture("ItemSlot");
					}
				}

				GameObject* ItemRight = this->AddSceneObject("ItemRight", TagName::Default);
				{
					ItemRight->transform->SetParent(ItemSlot);
					ItemRight->transform->localPosition(Vector3(8.0f, 0.0f, 0.0f));
					ItemRight->transform->localScale(Vector3(0.5f, 0.5f, 0.5f));
					{
						auto image = ItemRight->AddComponent<Image>();
						image->texture = TextureManager::GetTexture("ItemSlot");
					}
				}
			}
		}

		GameObject* canvas2 = this->AddSceneObject("Canvas2",TagName::Default);
		{
			canvas2->transform->position(Vector3(0.0f, D3DApp::GetScreenHeight(), 0.0f));
			canvas2->AddComponent<Canvas>();

			GameObject* mapBG = this->AddSceneObject("mapBG",TagName::Default);
			{
				mapBG->transform->SetParent(canvas2);
				mapBG->transform->localPosition(Vector3(120.0f,-120.0f,0.0f));
				mapBG->transform->localScale(Vector3(20.0f,20.0f,0.0f));
				mapBG->transform->localRotation(Quaternion::Identity());
				mapBG->AddComponent<Image>()->texture = TextureManager::GetTexture("MapBG");

				GameObject* compass = this->AddSceneObject("Compass",TagName::Default);
				{
					compass->transform->SetParent(mapBG);
					compass->transform->localPosition(Vector3(0.0f,0.0f,0.0f));
					compass->transform->localScale(Vector3(1.0f,1.0f,1.0f));
					compass->transform->localRotation(Quaternion::Identity());
					compass->AddComponent<Image>()->texture = TextureManager::GetTexture("Compass");
					compass->AddComponent<CompassScript>()->dirObject = HorizontalPivot;
				}
			}
		}

		//SceneChange
		GameObject* SceneChanger = this->AddSceneObject("SceneChanger", TagName::Default);
		{
			auto sceneChange = SceneChanger->AddComponent<SceneChange>();
			sceneChange->nextScene = "TitleScene";
		}
	};
};