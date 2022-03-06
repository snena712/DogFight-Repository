#include "StageCollider.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/ModelResourceManager.h"
#include "../../Component/SceneManager.h"
#include "../../Component/RigidbodyComponent.h"
#include "../../DX11System/Mesh.h"

using namespace Component;

StageCollider* StageCollider::StageCol;

StageCollider::StageCollider(GameObject* _owner, std::string _str, std::string _texstr) :ComponentBase(_owner)
{
	StageCol = this;
	m_stagename = _str;

	owner->AddComponent<Component::ModelRender>(
		m_stagename.c_str(),
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		_texstr.c_str());			// テクスチャの存在フォルダ);

	owner->transform->position.y += 10;
}



void StageCollider::Start() {

	std::vector<XMFLOAT3> vertices;		// 頂点データ
	std::vector<int> indices;			// インデックスデータ


	const ModelData* md =
		ModelResourceManager::GetInstance()->GetResource(m_stagename);		// 

	const std::vector<Mesh>& meshes = md->GetMeshes();

	// 頂点データを取り出す
	for (auto m : meshes) {
		for (auto v : m.m_vertices) {
			vertices.emplace_back(v.m_Pos);
		}
	}

	// インデックスデータを取り出す
	for (auto m : meshes) {
		for (auto idx : m.m_indices) {
			indices.emplace_back(idx);
		}
	}

	// 平面の方程式を作成
	CreateStagePlaneInfoAll(vertices, indices);
}

bool StageCollider::CrossPoint(Vector3 pos, Vector3& ans) {

	Vector3	point;	// 交点
	bool status;

	// 全ポリゴン数分ループ
	for (int i = 0; i < FaceCount; i++) {
		// 平面と直線の交点を求める
		status = LinetoPlaneCross(info[i].plane,	//平面の方程式
			pos,									//直線の起点（球の現在位置）
			Vector3(0, 1, 0),						//直線の方向ベクトル
			point);									//交点座標
		if (status &&
			CheckInTriangle(info[i].p0, info[i].p1, info[i].p2, point)) {
			// 交点が３角形の内部にあるか？
			ans.y = point.y+owner->transform->position.y;
			return true;
		}
	}
	return false;
}

void StageCollider::CreateStagePlaneInfoAll(std::vector<XMFLOAT3>& vertices, std::vector<int>& indices) {

	FaceCount = indices.size() / 3;				// ３角形数計算

	info = new PlaneInfo[FaceCount];		// 面数分確保


	// 面数分ループ
	for (int facecnt = 0; facecnt < FaceCount; facecnt++) {

		info[facecnt].p0.x = vertices[indices[facecnt * 3]].x;
		info[facecnt].p0.y = vertices[indices[facecnt * 3]].y;
		info[facecnt].p0.z = vertices[indices[facecnt * 3]].z;

		info[facecnt].p1.x = vertices[indices[facecnt * 3 + 1]].x;
		info[facecnt].p1.y = vertices[indices[facecnt * 3 + 1]].y;
		info[facecnt].p1.z = vertices[indices[facecnt * 3 + 1]].z;

		info[facecnt].p2.x = vertices[indices[facecnt * 3 + 2]].x;
		info[facecnt].p2.y = vertices[indices[facecnt * 3 + 2]].y;
		info[facecnt].p2.z = vertices[indices[facecnt * 3 + 2]].z;

		//平面の方程式を作成する
		CreatePlaneInfo(info[facecnt].p0, info[facecnt].p1, info[facecnt].p2, info[facecnt].plane);
	}
}



bool StageCollider::LinetoPlaneCross(const PLANE&		plane,		//平面の方程式
	const Vector3&	p0,						//直線の起点
	const Vector3&	wv,						//直線の方向ベクトル
	Vector3&			ans)					//交点座標
{
	float t = 0;

	Vector3 normal;				// 平面の法線ベクトル（正規化済み）
	normal.x = plane.a;
	normal.y = plane.b;
	normal.z = plane.c;

	float dot;			// 分母
	// 平行チェック(内積を計算する)
	DX11Vec3Dot(dot, normal, wv);
	if (fabsf(dot) < FLT_EPSILON) {
		//MessageBox(nullptr, "平行です。", "平行です。", MB_OK);
		return false;
	}

	float dot2;			// 分子
	DX11Vec3Dot(dot2, normal, p0);

	t = -(dot2 + plane.d) / dot;

	// ここにｔを求める式を入れる
	ans.x = p0.x + wv.x*t;
	ans.y = p0.y + wv.y*t;
	ans.z = p0.z + wv.z*t;
	return true;
}




void StageCollider::CreatePlaneInfo(const XMFLOAT3& p1,
	const XMFLOAT3& p2,
	const XMFLOAT3& p3,
	PLANE& plane)
{
	XMFLOAT3		p1p2;
	XMFLOAT3		p2p3;
	XMFLOAT3		normal;

	p1p2.x = p2.x - p1.x;
	p1p2.y = p2.y - p1.y;
	p1p2.z = p2.z - p1.z;

	p2p3.x = p3.x - p2.x;
	p2p3.y = p3.y - p2.y;
	p2p3.z = p3.z - p2.z;

	DX11Vec3Cross(normal, p1p2, p2p3);

	DX11Vec3Normalize(normal, normal);

	plane.a = normal.x;
	plane.b = normal.y;
	plane.c = normal.z;

	plane.d = -(plane.a * p1.x + plane.b * p1.y + plane.c * p1.z);
}



bool StageCollider::CheckInTriangle(const Vector3&	a,
	const Vector3&	b,
	const Vector3&	c,
	const Vector3&	p) {
	XMFLOAT3	ab, bc, ca;			// ３辺のベクトル
	XMFLOAT3	ap, bp, cp;			// 内部の点とのベクトル
	XMFLOAT3	normal;				// ３角形の法線ベクトル
	XMFLOAT3	n1, n2, n3;			// ３辺と内部の点との法線ベクトル

	ab.x = b.x - a.x;
	ab.y = b.y - a.y;
	ab.z = b.z - a.z;

	bc.x = c.x - b.x;
	bc.y = c.y - b.y;
	bc.z = c.z - b.z;

	ca.x = a.x - c.x;
	ca.y = a.y - c.y;
	ca.z = a.z - c.z;

	ap.x = p.x - a.x;
	ap.y = p.y - a.y;
	ap.z = p.z - a.z;

	bp.x = p.x - b.x;
	bp.y = p.y - b.y;
	bp.z = p.z - b.z;

	cp.x = p.x - c.x;
	cp.y = p.y - c.y;
	cp.z = p.z - c.z;

	DX11Vec3Cross(normal, ab, bc);			// 外積を計算

	DX11Vec3Cross(n1, ab, ap);				// 外積を計算

	DX11Vec3Cross(n2, bc, bp);				// 外積を計算

	DX11Vec3Cross(n3, ca, cp);				// 外積を計算

	float dot;
	DX11Vec3Dot(dot, n1, normal);
	if (dot < 0) return false;			// 為す角度が鈍角
	DX11Vec3Dot(dot, n2, normal);
	if (dot < 0) return false;			// 為す角度が鈍角
	DX11Vec3Dot(dot, n3, normal);
	if (dot < 0) return false;			// 為す角度が鈍角

	return true;
}

