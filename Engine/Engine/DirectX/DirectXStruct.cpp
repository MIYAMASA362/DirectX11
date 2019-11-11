#include<d3d11.h>
#include<DirectXMath.h>
#include"DirectXStruct.h"

const float DirectX::Mathf::kEpsilon = 0.000001f;
const float DirectX::Mathf::PI = DirectX::XM_PI;
const float DirectX::Mathf::PI_2 = DirectX::XM_PIDIV2;
const float DirectX::Mathf::PI_4 = DirectX::XM_PIDIV4;
const float DirectX::Mathf::PI2 = DirectX::XM_2PI;

const float DirectX::Mathf::Deg2Rad = DirectX::Mathf::PI * 2.0f /360.0f;
const float DirectX::Mathf::Rad2Deg = 1.0f / DirectX::Mathf::Deg2Rad;