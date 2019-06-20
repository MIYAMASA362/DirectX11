#pragma once

namespace DirectX
{
	class TagManager
	{
	public:
		const enum TagName
		{
			Default =0,
			MainCamera,
		};
	};

	class Tag
	{
	public:
		const TagManager::TagName name;
	public:
		Tag(TagManager::TagName tag) :name(tag) {};
	};
}