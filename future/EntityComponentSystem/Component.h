#pragma once

class IComponent: public Object
{
private:

public:
	IComponent(unsigned int id);

	virtual size_t GetComponentType() = 0;

};


struct Vector3
{
	float x;
	float y;
	float z;	
};


class Transform : public IComponent
{
private:
	Vector3 m_Position;

public:
	Transform(unsigned int id);

	Vector3 GetPosition() { return m_Position; };
	void SetPosition(Vector3 position) { m_Position = position; };

	virtual size_t GetComponentType() override { return typeid(*this).hash_code(); };
	static size_t ComponentType() { return typeid(Transform).hash_code(); }

	void Print();
};