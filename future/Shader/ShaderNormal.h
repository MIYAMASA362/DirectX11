#pragma once
class CLight;

class CShaderNormal:public CShaderBase<CONSTANT>
{
public:
	void Init(const char* VertexShader,const char* PixelShader) override;
	void Uninit() override;
	void Set() override;
	void SetLight(CLight* light);

	void SetProjectionMatrix(XMFLOAT4X4* ProjectionMatrix) override { m_Constant.ProjectionMatrix = Transpose(ProjectionMatrix); };
	void SetWorldMatrix(XMFLOAT4X4* WorldMatrix) override { m_Constant.WorldMatrix = Transpose(WorldMatrix); };
	void SetViewMatrix(XMFLOAT4X4* ViewMatrix) override { m_Constant.ViewMatrix = Transpose(ViewMatrix); };
};