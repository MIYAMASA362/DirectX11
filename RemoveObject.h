#pragma once

class RemoveObjectMethod:public Behaviour
{
public:
	void Update() override;
	void Destroy() override;
	void OnDestroy() override;
};	