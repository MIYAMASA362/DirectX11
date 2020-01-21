#pragma once


enum class TagName
{
	Default =0,
	MainCamera,
	ChildMiku,
	Player,
	Enemy
};

class Tag
{
private:
	const TagName name;
public:
	Tag(TagName tag) :name(tag) {};
	Tag():Tag(TagName::Default) {};

	bool Compare(TagName tag) { return name == tag; }
};