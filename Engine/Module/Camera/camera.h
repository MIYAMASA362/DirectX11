#pragma once

//*********************************************************************************************************************
//
//	Camera Component
//
//*********************************************************************************************************************

class Camera final :public Behaviour<Camera>
{
private:
	//アクティブなカメラ 描画のターゲット
	static Camera* pActiveCamera;
	//描画順でソート
	static std::vector<Camera*> CameraSortIndex;


	D3D11_VIEWPORT _Viewport;		//描画領域
	int _Priority;					//描画優先度

	XMMATRIX _ViewMatrix;
	XMMATRIX _ProjectionMatrix;


private:
	//CameraSortIndexに追加
	static void RegisterSortIndex(Camera* camera);
	//CameraSortIndexから削除
	static void RemoveSortIndex(Camera* camera);

public:
	//コンストラクタ
	Camera() :Camera(0){};
	Camera(EntityID OwnerID);
	//デストラクタ
	virtual ~Camera();

	//アクティブカメラ取得
	static Camera* GetActiveCamera() { return pActiveCamera; };

	//描画
	static void Render(void(*Draw)(void), RenderStatus* renderState);
	//スクリーン座標に変換
	static Vector3 ScreenToWorldPosition(Vector3 position);


	//ビューポート設定
	void SetViewPort(float x, float y, float w, float h);

	//描画優先度設定
	void SetPriority(int priority);
	//描画優先度取得
	int GetPriority() const { return _Priority; };
	
	XMMATRIX GetViewMatrix() { return _ViewMatrix; };
	XMMATRIX GetProjectionMatrix() { return _ProjectionMatrix; };

	//描画実行
	void Run();

	//視錐台カリング
	bool GetVisibility(Vector3 position);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Behaviour<Camera>>(this));
		archive(
			CEREAL_NVP(_Viewport.Height),
			CEREAL_NVP(_Viewport.Width)
		);
	}

	//エディタ表示
	static void EditorWindow();

protected:
	virtual void OnDebugImGui();
};

CEREAL_REGISTER_TYPE(Component<Camera>)
CEREAL_REGISTER_TYPE(Behaviour<Camera>)
CEREAL_REGISTER_TYPE(Camera)
