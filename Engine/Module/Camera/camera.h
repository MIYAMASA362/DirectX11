#pragma once

//*********************************************************************************************************************
//
//	Camera Component
//
//*********************************************************************************************************************

class Camera final :public Component<Camera>
{
private:
	//�A�N�e�B�u�ȃJ���� �`��̃^�[�Q�b�g
	static Camera* pActiveCamera;
	//�`�揇�Ń\�[�g
	static std::vector<Camera*> CameraSortIndex;


	D3D11_VIEWPORT _Viewport;		//�`��̈�
	int _Priority;					//�`��D��x
	bool _IsEnable;

	XMMATRIX _ViewMatrix;
	XMMATRIX _ProjectionMatrix;


private:
	//CameraSortIndex�ɒǉ�
	static void RegisterSortIndex(Camera* camera);
	//CameraSortIndex����폜
	static void RemoveSortIndex(Camera* camera);

public:
	//�R���X�g���N�^
	Camera();
	//�f�X�g���N�^
	virtual ~Camera();

	//�A�N�e�B�u�J�����擾
	static Camera* GetActiveCamera() { return pActiveCamera; };

	//�`��
	static void Render(void(*Draw)(void), RenderStatus* renderState);
	//�X�N���[�����W�ɕϊ�
	static Vector3 ScreenToWorldPosition(Vector3 position);


	//�r���[�|�[�g�ݒ�
	void SetViewPort(float x, float y, float w, float h);

	void Register(std::shared_ptr<Object> instance) override;

	void Release() override;

	//�`��D��x�ݒ�
	void SetPriority(int priority);
	//�`��D��x�擾
	int GetPriority() const { return _Priority; };
	
	XMMATRIX GetViewMatrix() { return _ViewMatrix; };
	XMMATRIX GetProjectionMatrix() { return _ProjectionMatrix; };

	//�`����s
	void Run();

	//������J�����O
	bool GetVisibility(Vector3 position);

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component<Camera>>(this),
			CEREAL_NVP(_Viewport.Height),
			CEREAL_NVP(_Viewport.Width)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component<Camera>>(this),
			CEREAL_NVP(_Viewport.Height),
			CEREAL_NVP(_Viewport.Width)
		);
	}

	//�G�f�B�^�\��
	static void EditorWindow();

protected:
	virtual void OnDebugImGui();
};

CEREAL_REGISTER_TYPE(Component<Camera>)
CEREAL_REGISTER_TYPE(Camera)
