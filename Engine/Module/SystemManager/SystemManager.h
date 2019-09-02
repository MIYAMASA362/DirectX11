#pragma once

namespace DirectX
{
	class SystemManager
	{
	public:
		static std::list<std::shared_ptr<SystemManager>> ManagerIndex;
	public:
		template<typename Type>
		static void CreateManager() {
			ManagerIndex.push_back(std::shared_ptr<SystemManager>(new Type()));
		}
		static void Initialize()
		{
			for (auto manager : ManagerIndex)
				manager->initialize();
		}
		static void Update() {
			for (auto manager : ManagerIndex)
				manager->update();
		}
		static void FixedUpdate(){
			for (auto manager : ManagerIndex)
				manager->fixedUpdate();
		}
		static void Render() {
			for (auto manager : ManagerIndex)
				manager->render();
		}
		static void Finalize() {
			for (auto manager : ManagerIndex)
				manager->finalize();
		}
		static void Release()
		{
			ManagerIndex.clear();
		}
	public:
		virtual void initialize() {};
		virtual void update() {};
		virtual void fixedUpdate() {};
		virtual void render() {};
		virtual void finalize() {};
	};
}