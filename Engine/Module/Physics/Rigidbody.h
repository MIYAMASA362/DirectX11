#pragma once


class Collider;

//Rigidbody
//	����������ݒ肷��
//
class Rigidbody final :public Component<Rigidbody>
{
private:
	//�d�̗͂L���E����
	bool _UseGravity;
	//�����x
	Vector3 _Velocity;
	//����
	float _Mass;
	//�O�ʒu
	Vector3 _OldPosition;
	//�Փ˔���
	std::list<std::weak_ptr<Collider>> _Colliders;


public:
	//���������K��
	static void ApplyRigidbody();
	//��...
	static void CollisionRigidbody();

	//�R���X�^���g
	Rigidbody(EntityID OwnerID);
	//�f�X�g���N�^
	virtual ~Rigidbody();

	//useGravity�̐؂�ւ�
	void IsUseGravity(bool enable) { _UseGravity = enable; }
	
	//�d�͎擾
	Vector3 GetVelocity() { return _Velocity; };
	//�����x�ݒ�
	void SetVelocity(Vector3 velocity) { _Velocity = velocity; };
	
	//���ʐݒ�
	void SetMass(float mass) { _Mass = mass; };
	//Collider�֒ǉ�
	void RegisterCollider(std::weak_ptr<Collider> collider) { _Colliders.push_back(collider); };

	//�͂�ǉ�
	void AddForce(Vector3 force) { _Velocity = _Mass * force; }
	//�O�ʒu�擾
	Vector3 GetOldPosition() { return this->_OldPosition; }
	//��莞�ԍX�V
	void FixedUpdate();


protected:
	//�폜�����s�֐�
	void OnDestroy() override;

	void OnDebugImGui() override;

	void SendComponentMessage(std::string message) override;
};
