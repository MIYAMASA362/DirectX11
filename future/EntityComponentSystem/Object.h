#pragma once

class Object
{
private:
	unsigned int m_InstanceID;

public:
	Object(unsigned int id);

	void SetInstanceID(unsigned int id) { m_InstanceID = id; };
	unsigned int GetInstanceID() { return m_InstanceID; };
};