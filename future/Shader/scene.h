#pragma once

#include <list>
#include "main.h"

#include "game_object.h"

#include"texture.h"

#include"Camera.h"
#include"Field.h"
#include "polygon.h"
#include"model.h"

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
		AddGameObject<CPolygon>();
		/*auto field = AddGameObject<CField>();
		field->SetCamera(camera);*/
		auto model = AddGameObject<CModel>();
		model->SetCamera(camera);
		model->SetPosition({0.0f,0.0f,0.0f});
		model->SetScale({2.0f,2.0f,2.0f});

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