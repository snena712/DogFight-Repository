#include	<Windows.h>
#include	"SimpleMath.h"

using namespace DirectX::SimpleMath;

// ����͈͊O�̎擾
bool InsightCheckXZ(Vector3 eyepos, Vector3 lookat,float fov,Vector3 checkpoint,float length) {

	// �`�F�b�N�ΏۂƎ��_�����ԃx�N�g��
	Vector3 vecobj;
	vecobj = checkpoint - eyepos;

	// �I�u�W�F�N�g�Ƃ̋��������߂�
	float objlength = vecobj.Length();

	// �����͈͊O�H
	if (objlength > length) {
		return false;
	}

	// �����x�N�g��
	Vector3 vecview;
	vecview = lookat - eyepos;

	// ���K��
	vecview.Normalize();
	vecobj.Normalize();

	// ���ς��v�Z
	float dotobj = vecview.Dot(vecobj);

	// ��]
	Matrix mtx;
	mtx = mtx.CreateRotationZ(fov / 2.0f);

	Vector3 vecrotview;
	vecrotview = vecview.Transform(vecview, mtx);

	// ���ς��v�Z
	float dotrotview = vecview.Dot(vecrotview);

	// ����p�͈͓��H
	if (dotrotview <= dotobj) {
		return true;
	}

	return false;
}

