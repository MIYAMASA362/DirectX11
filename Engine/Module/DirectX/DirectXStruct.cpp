#include<d3d11.h>
#include<DirectXMath.h>
#include"DirectXStruct.h"

const float Mathf::kEpsilon = 0.000001f;
const float Mathf::PI = DirectX::XM_PI;
const float Mathf::PI_2 = DirectX::XM_PIDIV2;
const float Mathf::PI_4 = DirectX::XM_PIDIV4;
const float Mathf::PI2 = DirectX::XM_2PI;

const float Mathf::Deg2Rad = Mathf::PI * 2.0f /360.0f;
const float Mathf::Rad2Deg = 1.0f / Mathf::Deg2Rad;