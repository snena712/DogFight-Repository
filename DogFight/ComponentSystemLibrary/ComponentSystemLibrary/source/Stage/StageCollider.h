#pragma once
#include "../../Component/ComponentBaseClass.h"

namespace Component {
	class StageCollider : public ComponentBase {
		// ���b�V���̊e�ʂ̃f�[�^
		struct PlaneInfo {
			PLANE plane;		// ���ʂ̕�����
			Vector3 p0, p1, p2;	// �@��
		};
		PlaneInfo* info;		// 
		int FaceCount;			// �ʂ̐�

		void CreateStagePlaneInfoAll(std::vector<XMFLOAT3>& vertices, std::vector<int>& indices);
		void CreatePlaneInfo(const XMFLOAT3& p1,
			const XMFLOAT3& p2,
			const XMFLOAT3& p3,
			PLANE& plane);
		bool LinetoPlaneCross(const PLANE& plane,	//���ʂ̕�����
			const Vector3&	p0,						//�����̋N�_
			const Vector3&	wv,						//�����̕����x�N�g��
			Vector3& ans);							//��_���W
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


		bool CrossPoint(Vector3 pos, Vector3& ans);	// pos�Ƃ�y����ł̌�_��Ԃ�(ans)

		void Start();
	};
}
