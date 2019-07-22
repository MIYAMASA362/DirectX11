#pragma once

namespace DirectX
{
	enum class TagName
	{
		Default =0,
		MainCamera,
	};

	class Tag
	{
	public:
		const TagName name;
	public:
		Tag(TagName tag) :name(tag) {};
	};
}