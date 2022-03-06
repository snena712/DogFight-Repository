#pragma once
#include "../../Component/ComponentBaseClass.h"

namespace Component {
	class StageCollider : public ComponentBase {
		// メッシュの各面のデータ
		struct PlaneInfo {
			PLANE plane;		// 平面の方程式
			Vector3 p0, p1, p2;	// 法線
		};
		PlaneInfo* info;		// 
		int FaceCount;			// 面の数

		void CreateStagePlaneInfoAll(std::vector<XMFLOAT3>& vertices, std::vector<int>& indices);
		void CreatePlaneInfo(const XMFLOAT3& p1,
			const XMFLOAT3& p2,
			const XMFLOAT3& p3,
			PLANE& plane);
		bool LinetoPlaneCross(const PLANE& plane,	//平面の方程式
			const Vector3&	p0,						//直線の起点
			const Vector3&	wv,						//直線の方向ベクトル
			Vector3& ans);							//交点座標
		bool CheckInTriangle(const Vector3&	a,
			const Vector3&	b,
			const Vector3&	c,
			const Vector3&	p);


		static StageCollider* StageCol;
		std::string m_stagename;
	public:
		StageCollider(GameObject* Owner, std::string _str, std::string _texstr);
		~StageCollider() {}


		static StageCollider* GetInstans() { return StageCol; };


		bool CrossPoint(Vector3 pos, Vector3& ans);	// posとのy軸上での交点を返す(ans)

		void Start();
	};
}
