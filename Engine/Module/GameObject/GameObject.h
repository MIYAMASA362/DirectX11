#pragma once

class GameObject;
class Scene;

class GameObject final:public Entity<GameObject>
{
	friend Scene;
private:
	Scene* const scene;

	const std::string name;
	const Tag tag;

	bool IsDestroy = false;
	bool IsActive  = true;

	std::weak_ptr<GameObject> _gameObject;
	std::weak_ptr<Transform> _transform;
public:
	GameObject(std::string name,Scene* scene, TagName tagName);
	virtual ~GameObject();

	void SetActive(bool IsActive);
	bool CompareTag(TagName tag);
	bool GetIsDestroy();
	bool GetActive();			//e‚Ì‰e‹¿‚ğó‚¯‚é‚©
	bool GetActiveSelf();		//©g‚Ì‚İ
	Scene* GetScene();

	std::weak_ptr<Transform> transform();
	std::weak_ptr<GameObject> gameObject();

	void OnDebugGUI();
	std::string GetName();
};