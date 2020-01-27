#include <iostream>
#include <map>
#include<typeinfo>

#include"Object.h"
#include"Component.h"
#include"Entity.h"

#include"ComponentSystem.h"

int main()
{
	//エンティティの作成
	Entity* entity = new Entity(10);

	//Trasnformを追加
	entity->AddComponent(new Transform(100));

	//システムがエンティティを動かす
	MoveSystem* moveSystem;
	moveSystem = new MoveSystem();
	moveSystem->SetEntity(entity);
	moveSystem->Update();

	getchar();

	delete moveSystem;
	delete entity;

	return 0;
}