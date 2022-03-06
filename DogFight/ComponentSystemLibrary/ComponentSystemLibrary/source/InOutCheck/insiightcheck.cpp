#include	<Windows.h>
#include	"SimpleMath.h"

using namespace DirectX::SimpleMath;

// 視野範囲外の取得
bool InsightCheckXZ(Vector3 eyepos, Vector3 lookat,float fov,Vector3 checkpoint,float length) {

	// チェック対象と視点を結ぶベクトル
	Vector3 vecobj;
	vecobj = checkpoint - eyepos;

	// オブジェクトとの距離を求める
	float objlength = vecobj.Length();

	// 距離範囲外？
	if (objlength > length) {
		return false;
	}

	// 視線ベクトル
	Vector3 vecview;
	vecview = lookat - eyepos;

	// 正規化
	vecview.Normalize();
	vecobj.Normalize();

	// 内積を計算
	float dotobj = vecview.Dot(vecobj);

	// 回転
	Matrix mtx;
	mtx = mtx.CreateRotationZ(fov / 2.0f);

	Vector3 vecrotview;
	vecrotview = vecview.Transform(vecview, mtx);

	// 内積を計算
	float dotrotview = vecview.Dot(vecrotview);

	// 視野角範囲内？
	if (dotrotview <= dotobj) {
		return true;
	}

	return false;
}

