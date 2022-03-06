#pragma once
#include	<DirectXMath.h>
#include    "Vector.h"

namespace MyQuad {

#define		MAXMOJICNT		1024

	// 2d��3d�̏��
	enum class RenderMode
	{
		Rendr3D,
		Rendr2D
	};

	// �V�F�[�_�[�̏��
	enum class RenderState
	{
		NONE,
		Basic3D,		// 3D
		Basic2D			// 2D
	};

	// ���_�o�b�t�@�X�V
	static void UpdateVertexBuffer();

	// �C���f�b�N�X�o�b�t�@�X�V
	static void UpdateIndexBuffer();

	// �߂�l�@�C���f�b�N�X��
	int Create3DVertexIndexData(int length);

	// ������
	bool InitMyQuad();

	// ������`��
	void DrawMyQuad3D();

	// �`�悷�镶������N���A
	void ClearMyQuad2D();
	void ClearMyQuad3D();

	// �A�X�L�[�t�H���g�`��p�̂t�u���W�擾
	void GetAsciiFontUV(char code, DirectX::XMFLOAT2* uv);

	//�@������`��
	void DrawQuad(RenderMode _mode, RenderState _state, DirectX::XMFLOAT2 _uv[4], float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4X4 _mtx, std::string _texstr);
}

