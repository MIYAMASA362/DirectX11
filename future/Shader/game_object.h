#pragma once

#include <list>
#include "main.h"
#include "renderer.h"

class CScene;

class CGameObject
{

protected:

	XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMFLOAT3					m_Scale;

	CScene* m_Scene;

public:

	XMFLOAT3 GetPosition(){ return m_Position; }
	XMFLOAT3 GetRotation(){ return m_Rotation; }
	XMFLOAT3 GetScale(){ return m_Scale; }

	void SetPosition( XMFLOAT3 Position ){ m_Position = Position; }
	void SetRotation( XMFLOAT3 Rotation ){ m_Rotation = Rotation; }
	void SetScale( XMFLOAT3 Scale ){ m_Scale = Scale; }

	void SetScene(CScene* scene) { m_Scene = scene; }

public:

	CGameObject(){}

	virtual ~CGameObject()
	{
	}

	virtual void Init()
	{
		
	}

	virtual void Uninit()
	{
	}


	virtual void Update()
	{
	}

	virtual void DrawShadow()
	{
		
	}

	virtual void Draw()
	{
	}



};