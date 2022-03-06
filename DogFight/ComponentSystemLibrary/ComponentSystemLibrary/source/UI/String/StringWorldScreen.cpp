#include "StringWorldScreen.h"

// ビュー座標に変換
Vector3  WorldScreenToViewScreen(XMVECTOR World_Pos, Vector3 _pos)
{
	auto cam = Component::Camera::GetMainCamera();
	// ビューとスクリーン変換
	XMMATRIX g_View = XMLoadFloat4x4(&cam->GetCameraMatrix());
	XMMATRIX g_Projection = XMLoadFloat4x4(&cam->GetProjectionMatrix());

	// 行列作成
	float w = Application::Instance()->CLIENT_WIDTH / 2.0f;
	float h = Application::Instance()->CLIENT_HEIGHT / 2.0f;
	XMMATRIX viewport = {
	w, 0, 0, 0,
	0, -h, 0, 0,
	0, 0, 1, 0,
	w, h, 0, 1
	};

	// ビュー変換とプロジェクション変換
	World_Pos = XMVector3Transform(World_Pos, g_View);
	World_Pos = XMVector3Transform(World_Pos, g_Projection);

	// float3に変換
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);

	// 初期化
	XMVECTOR view_vec = XMVECTOR{ 0, 0, 0 };

	// zで割って-1~1の範囲に収める

	XMFLOAT4X4 a;

	DX11MtxMultiply(a, Component::Camera::GetMainCamera()->GetCameraMatrix(), Component::Camera::GetMainCamera()->GetProjectionMatrix());

	view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, -1.0f, 1.0f);
	auto xm= XMVector3Transform(view_vec, viewport);
	XMFLOAT3* c = new XMFLOAT3;
	XMStoreFloat3(c, xm);

	Vector3 hoge;
	 hoge.x = c->x;
	 hoge.y = c->y;
	 hoge.z = c->z;
	return hoge;
}


