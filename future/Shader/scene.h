#pragma once

#include <list>
#include "main.h"

#include "game_object.h"

#include"texture.h"

#include"Camera.h"
#include"Field.h"
#include "polygon.h"
#include"model.h"

#include"CLight.h"

class CScene
{
protected:
	std::list<CGameObject*>	m_GameObject;

public:
	CScene(){}
	virtual ~CScene(){}


	virtual void Init()
	{
		auto camera = AddGameObject<CCamera>();
		auto light = AddGameObject<CLight>();

		{
			auto polygon = AddGameObject<CPolygon>();
			polygon->GetShader()->SetLight(light);
		}
		{
			auto field = AddGameObject<CField>();
			field->SetCamera(camera);
			field->SetPosition({ 0.0f,-2.0f,0.0f });
		}

		{
			auto model = AddGameObject<CModel>();
			model->Load("data/MODEL/sphere_smooth.obj");
			model->GetShader()->Init("EnvironmentMappingVS.cso", "EnvironmentMappingPS.cso");
			model->GetShader()->SetLight(light);
			{
				auto texture = new CTexture[2];
				texture[0].Load("data/TEXTURE/earthenvmap.tga");
				texture[1].Load("data/TEXTURE/field004.tga");
				model->SetTexture(texture);
			}
			model->SetCamera(camera);
			model->SetPosition({ -5.0f,0.0f,0.0f });
			model->SetScale({ 2.0f,2.0f,2.0f });
		}

		{
			auto model = AddGameObject<CModel>();
			model->Load("data/MODEL/sphere_smooth.obj");
			model->GetShader()->Init("ToonShader3DVS.cso", "ToonShader3DPS.cso");
			model->GetShader()->SetLight(light);
			{
				auto texture = new CTexture[2];
				texture[0].Load("data/TEXTURE/toon.tga");
				texture[1].Load("data/TEXTURE/field004.tga");
				model->SetTexture(texture);
			}
			model->SetCamera(camera);
			model->SetPosition({ 5.0f,0.0f,0.0f });
			model->SetScale({ 2.0f,2.0f,2.0f });
		}

		
		

	}

	virtual void Uninit()
	{
		for (CGameObject* object : m_GameObject)
		{
			object->Uninit();
			delete object;
		}

		m_GameObject.clear();
	}


	virtual void Update()
	{
		for( CGameObject* object : m_GameObject )
			object->Update();
	}


	virtual void Draw()
	{
		for (CGameObject* object : m_GameObject)
			object->Draw();
	}


	template <typename T>
	T* AddGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject.push_back( gameObject );

		return gameObject;
	}
};