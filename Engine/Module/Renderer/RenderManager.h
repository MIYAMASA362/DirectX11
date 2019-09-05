#pragma once

namespace DirectX
{
	class GameObject;
	class Renderer;

	//=== RenderManager =======================================================
	class RenderManager
	{
	private:
		static std::list<std::weak_ptr<DirectX::Renderer>> Renderer3D;
		static std::list<std::weak_ptr<DirectX::Renderer>> Renderer2D;
		static std::map<int, std::shared_ptr<DirectX::Renderer>> ComponentIndex;
	public:
		static void Run(int id);
		static void Remove(int id);
		static void Add(int id);
	};
}