#pragma once

namespace DirectX
{
	struct XMMATRIX;

	//=== Mesh ================================================================
	class Mesh
	{
	protected:
		Mesh() = default;
		virtual ~Mesh() = default;
	public:
		virtual void Render(XMMATRIX& worldMatrix) = 0;
	};
}