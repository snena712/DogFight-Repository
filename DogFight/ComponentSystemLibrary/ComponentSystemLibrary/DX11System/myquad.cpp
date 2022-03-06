#include "myquad.h"

#include	<vector>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"shaderhashmap.h"
#include	"mystring.h"
#include	"shader.h"
#include	"DX11Settransform.h"
#include	"dx11mathutil.h"
#include	"DX11util.h"
#include    "ImageResourceManager.h"
#include    "../Component/CameraComponent.h"

namespace MyQuad {

	using namespace DirectX;
	using  Microsoft::WRL::ComPtr;

	// �g�p���钸�_�V�F�[�_�[��
	static const char* vsfilename[] = {
		"shader/basicvs.hlsl",		// �ʏ�i3D�j
		"shader/quad2dvs.hlsl"	,	// �ʏ�i2D�j
	};

	// �g�p����s�N�Z���V�F�[�_�[��
	static const char* psfilename[] = {
		"shader/basicps.hlsl",
	};

	// ���_�\����
	struct Vertex {
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
		XMFLOAT2 Tex;
	};

	// �C���f�b�N�X�\����
	struct Face {
		unsigned int idx[4];
	};

	// ���_�f�[�^
	static std::vector<Vertex> g_vertices;

	// �C���f�b�N�X�f�[�^
	static std::vector<Face> g_indices;

	// �����R�[�h�ƕ\�����邽�߂̂t�u���W
	struct myquad {
		float	width;			// ��
		float	height;			// ����
		XMFLOAT4 color;			// �F
		XMFLOAT2 uv[4];			// UV���W
		XMFLOAT4X4 _mtx;		// �s��
		std::string texname;	// �e�N�X�`��
		RenderState renderState;//�`��̏��
	};

	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11Buffer>	g_VertexBuffer;
	ComPtr<ID3D11Buffer>	g_IndexBuffer;

	// �\��������(�ő�P�O�Q�S����)
	std::vector<myquad> g_my3Dquad(MAXMOJICNT);
	std::vector<myquad> g_my2Dquad(MAXMOJICNT);

	// ���݂̔ԍ��ƍő�l
	int g_nownum;

	// �`��̃��[�h
	RenderState g_RenderState = RenderState::NONE;

	// �`��̃��C�A�E�g�t���O
	bool g_layoutflg = false;


	// ���_�o�b�t�@�X�V
	static void UpdateVertexBuffer() {
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		HRESULT hr = devcontext->Map(g_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(g_vertices.data()), sizeof(Vertex) * g_vertices.size());
			devcontext->Unmap(g_VertexBuffer.Get(), 0);
		}
	}

	// �C���f�b�N�X�o�b�t�@�X�V
	static void UpdateIndexBuffer() {
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		HRESULT hr = devcontext->Map(g_IndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(g_indices.data()), sizeof(Face) * g_indices.size());
			devcontext->Unmap(g_IndexBuffer.Get(), 0);
		}
	}

	// �߂�l�@FACE��
	int CreateVertexIndexDataFirst(int length) {
		int quadnum = 0;			// �l�p�`�̐�

		// ���_������
		for (int idx = 0; idx < length; idx++) {
			Vertex v;
			v.Color = XMFLOAT4(0, 0, 0, 0);
			v.Pos.x = 0.0f;
			v.Pos.y = 0.0f;
			v.Pos.z = 0.0f;
			v.Tex.x = 0.0f;
			v.Tex.y = 0.0f;

			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);

			Face f;

			f.idx[0] = idx * 4;
			f.idx[1] = idx * 4 + 1;
			f.idx[2] = idx * 4 + 2;
			f.idx[3] = idx * 4 + 3;

			g_indices.emplace_back(f);
		}

		return g_indices.size();
	}

	// �߂�l�@FACE��
	int Create3DVertexIndexData(int length) {
		int quadnum = 0;			// �l�p�`�̐�

		std::string _str = g_my3Dquad[g_nownum].texname;
		RenderState _state = g_my3Dquad[g_nownum].renderState;
		// ���_������
		for (int idx = g_nownum; idx < g_my3Dquad.size(); idx++) {
			
			// �O�Ɠ��������񁕏�Ԃ��������̊m�F
			if (_str == g_my3Dquad[idx].texname&& _state== g_my3Dquad[idx].renderState)
			{

				g_RenderState = g_my3Dquad[idx].renderState;

				Vertex v;
				// 1
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x =  static_cast<float>(-g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[0].x;
				v.Tex.y = g_my3Dquad[idx].uv[0].y;
				g_vertices.push_back(v);

				// 2
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[1].x;
				v.Tex.y = g_my3Dquad[idx].uv[1].y;
				g_vertices.push_back(v);

				// 3 
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x = static_cast<float>(-g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[2].x;
				v.Tex.y = g_my3Dquad[idx].uv[2].y;
				g_vertices.push_back(v);

				// 4
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[3].x;
				v.Tex.y = g_my3Dquad[idx].uv[3].y;
				g_vertices.push_back(v);

				// �C���f�b�N�X
				Face f;

				f.idx[0] = quadnum * 4;
				f.idx[1] = quadnum * 4 + 1;
				f.idx[2] = quadnum * 4 + 2;
				f.idx[3] = quadnum * 4 + 3;

				g_indices.push_back(f);

				// ���݂̌��̍X�V
				g_nownum++;
				quadnum++;
			}
			else {
				break;
			}
		}

		// srv�̐ݒ�
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(_str)->GetSRV() };
		devcontext->PSSetShaderResources(0, 1, srv);

		return quadnum;
	}

	// �߂�l�@FACE��
	int Create2DVertexIndexData(int length) {
	
		int quadnum = 0;			// �l�p�`�̐�

		std::string _str = g_my2Dquad[g_nownum].texname;
		RenderState _state = g_my2Dquad[g_nownum].renderState;
		
		// ���_������
		for (int idx = g_nownum; idx < g_my2Dquad.size(); idx++) {

			// �O�Ɠ��������񁕏�Ԃ��������̊m�F
			if (_str == g_my2Dquad[idx].texname && _state == g_my2Dquad[idx].renderState)
			{

				g_RenderState = g_my2Dquad[idx].renderState;

				Vertex v;
				// 1
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(-g_my2Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my2Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[0].x;
				v.Tex.y = g_my2Dquad[idx].uv[0].y;
				g_vertices.push_back(v);

				// 2
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[1].x;
				v.Tex.y = g_my2Dquad[idx].uv[1].y;
				g_vertices.push_back(v);

				// 3 
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(-g_my2Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my2Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[2].x;
				v.Tex.y = g_my2Dquad[idx].uv[2].y;
				g_vertices.push_back(v);

				// 4
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my2Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my2Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[3].x;
				v.Tex.y = g_my2Dquad[idx].uv[3].y;
				g_vertices.push_back(v);

				// �C���f�b�N�X
				Face f;

				f.idx[0] = quadnum * 4;
				f.idx[1] = quadnum * 4 + 1;
				f.idx[2] = quadnum * 4 + 2;
				f.idx[3] = quadnum * 4 + 3;

				g_indices.push_back(f);

				// ���݂̌��̍X�V
				g_nownum++;
				quadnum++;
			}
			else {
				break;
			}
		}

		// srv�̐ݒ�
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(_str)->GetSRV() };
		devcontext->PSSetShaderResources(0, 1, srv);

		return quadnum;
	}

	// ������
	bool InitMyQuad() {

		int cnt = 0;

		// �f�o�C�X���擾
		ID3D11Device* device;
		device = GetDX11Device();

		// �f�o�C�X�R���e�L�X�g���擾
		ID3D11DeviceContext* devicecontext;
		devicecontext = GetDX11DeviceContext();

		// ������T�C�Y������
		g_my3Dquad.clear();
		g_my2Dquad.clear();

		// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̏������f�[�^����
		CreateVertexIndexDataFirst(MAXMOJICNT);

		// ���_�o�b�t�@����
		bool sts = CreateVertexBufferWrite(device, sizeof(Vertex), MAXMOJICNT * 4, g_vertices.data(), &g_VertexBuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateVerteBuffer error", "error", MB_OK);
			return false;
		}

		// �C���f�b�N�X�o�b�t�@����
		sts = CreateIndexBufferWrite(device, MAXMOJICNT * 4 * 2, g_indices.data(), &g_IndexBuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
		if (!sts) {
			MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
			return false;
		}

		// ���_�f�[�^�̒�`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// �G�������g��
		unsigned int numElements = ARRAYSIZE(layout);

		// ���_�V�F�[�_�[�𐶐�
		sts = ShaderHashmap::GetInstance()->SetVertexShader(
			vsfilename[0],			// ���_�V�F�[�_�[�t�@�C����
			layout,					// ���_���C�A�E�g
			numElements);			// �G�������g��

		// ���_�V�F�[�_�[�𐶐�
		sts = ShaderHashmap::GetInstance()->SetVertexShader(
			vsfilename[1],			// ���_�V�F�[�_�[�t�@�C����
			layout,					// ���_���C�A�E�g
			numElements);			// �G�������g��

		// VECTOR������
		ClearMyQuad3D();
		ClearMyQuad2D();

		return true;
	}

	// 3D�I�u�W�F�N�g�`��
	void DrawMyQuad3D() {

		XMFLOAT4X4 mtx;

		// �r���[�ϊ��s��Z�b�g
		mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

		// �v���W�F�N�V�����ϊ��s��Z�b�g
		mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);


		// ���������o�^�z����菭�Ȃ���
		while (g_my3Dquad.size() > g_nownum)
		{
			ID3D11VertexShader* vs;
			ID3D11PixelShader* ps;

			// �摜�̕����񂪓����ԕ`�悷��
			auto num = Create3DVertexIndexData(static_cast<int>(g_my3Dquad.size()));

			// ���_�o�b�t�@�X�V
			UpdateVertexBuffer();

			// �C���f�b�N�X�o�b�t�@�X�V
			UpdateIndexBuffer();

			// �f�o�C�X�R���e�L�X�g���擾
			ID3D11DeviceContext* devcontext;
			devcontext = GetDX11DeviceContext();

			unsigned int stride = sizeof(Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
			unsigned  offset = 0;					// �I�t�Z�b�g�l���Z�b�g

			//------------------------------------------------
			// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
			//------------------------------------------------
			devcontext->IASetVertexBuffers(
				0,									// �X�^�[�g�X���b�g
				1,									// ���_�o�b�t�@��
				g_VertexBuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
				&stride,							// �X�g���C�h
				&offset);							// �I�t�Z�b�g

			devcontext->IASetIndexBuffer(			// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
				g_IndexBuffer.Get(),				// �C���f�b�N�X�o�b�t�@
				DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
				0);									// �I�t�Z�b�g			


				// �V�����`����
			switch (g_RenderState)
			{
			case RenderState::Basic3D:
				// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				break;

			case RenderState::Basic2D:
				// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[1]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				// �g�|���W�[���Z�b�g
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				break;
			default:
				break;
			}

			// ���_���C�A�E�g�擾
			if (!g_layoutflg)
			{
				g_layoutflg = true;

				ID3D11InputLayout* layout;
				layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);
				// ���_�t�H�[�}�b�g���Z�b�g
				devcontext->IASetInputLayout(layout);
				// �g�|���W�[���Z�b�g
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			// �`�揈��
			for (int i = 0; i < num; i++) {
				// �e�X�̃��X�g�̍s��擾
				DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_my3Dquad[g_nownum - num + i]._mtx);
				// �h���[�R�[�����s
				devcontext->DrawIndexed(
					4,			// ��
					i * 4,		// �J�n�C���f�b�N�X					
					0);			// ����_�C���f�b�N�X				
			}

			// ���̕`��p�ɃC���f�b�N�X�ƒ��_�̃��X�g������
			g_indices.clear();
			g_vertices.clear();
		}
		// ������
		ClearMyQuad3D();
	}

	// 3D�I�u�W�F�N�g�`��
	void DrawMyQuad2D() {

		XMFLOAT4X4 mtx;

		// �r���[�ϊ��s��Z�b�g
		mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

		// �v���W�F�N�V�����ϊ��s��Z�b�g
		mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);


		// ���������o�^�z����菭�Ȃ���
		while (g_my2Dquad.size() > g_nownum)
		{
			ID3D11VertexShader* vs;
			ID3D11PixelShader* ps;

			// �摜�̕����񂪓����ԕ`�悷��
			auto num = Create2DVertexIndexData(static_cast<int>(g_my2Dquad.size()));

			// ���_�o�b�t�@�X�V
			UpdateVertexBuffer();

			// �C���f�b�N�X�o�b�t�@�X�V
			UpdateIndexBuffer();

			// �f�o�C�X�R���e�L�X�g���擾
			ID3D11DeviceContext* devcontext;
			devcontext = GetDX11DeviceContext();

			unsigned int stride = sizeof(Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
			unsigned  offset = 0;					// �I�t�Z�b�g�l���Z�b�g

			//------------------------------------------------
			// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
			//------------------------------------------------
			devcontext->IASetVertexBuffers(
				0,									// �X�^�[�g�X���b�g
				1,									// ���_�o�b�t�@��
				g_VertexBuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
				&stride,							// �X�g���C�h
				&offset);							// �I�t�Z�b�g

			devcontext->IASetIndexBuffer(			// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
				g_IndexBuffer.Get(),				// �C���f�b�N�X�o�b�t�@
				DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
				0);									// �I�t�Z�b�g			


				// �V�����`����
			switch (g_RenderState)
			{
			case RenderState::Basic3D:
				// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				break;

			case RenderState::Basic2D:
				// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[1]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				// �g�|���W�[���Z�b�g
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				break;
			default:
				break;
			}

			// ���_���C�A�E�g�擾
			if (!g_layoutflg)
			{
				g_layoutflg = true;

				ID3D11InputLayout* layout;
				layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);
				// ���_�t�H�[�}�b�g���Z�b�g
				devcontext->IASetInputLayout(layout);
				// �g�|���W�[���Z�b�g
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			// �`�揈��
			for (int i = 0; i < num; i++) {
				// �e�X�̃��X�g�̍s��擾
				DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_my2Dquad[g_nownum - num + i]._mtx);
				// �h���[�R�[�����s
				devcontext->DrawIndexed(
					4,			// ��
					i * 4,		// �J�n�C���f�b�N�X					
					0);			// ����_�C���f�b�N�X				
			}

			// ���̕`��p�ɃC���f�b�N�X�ƒ��_�̃��X�g������
			g_indices.clear();
			g_vertices.clear();
		}
		// ������
		ClearMyQuad2D();
	}

	// �`�悷�镶������N���A
	void ClearMyQuad3D() {
		
		// �`�敶������N���A
		g_my3Dquad.clear();

		// �C���f�b�N�X�N���A
		g_indices.clear();

		// ���_�N���A
		g_vertices.clear();

		// �`����̏�����
		g_nownum = 0;

		// ���C�A�E�g�̍X�V�t���O������
		g_layoutflg = false;
	}

	// �`�悷�镶������N���A
	void ClearMyQuad2D() {

		// �`�敶������N���A
		g_my2Dquad.clear();


		// �C���f�b�N�X�N���A
		g_indices.clear();

		// ���_�N���A
		g_vertices.clear();

		// �`����̏�����
		g_nownum = 0;
		

		// ���C�A�E�g�̍X�V�t���O������
		g_layoutflg = false;
	}

	// �A�X�L�[�t�H���g�`��p�̂t�u���W�擾
	void GetAsciiFontUV(char code, XMFLOAT2* uv) {

		int codex;
		int codey;

		codex = code & 0x0f;
		codey = code >> 4;

		codey = codey - 2;

		float tu, tv;

		tu = codex * 0.0625f;
		tv = codey * 0.0625f;

		uv[0].x = tu;
		uv[0].y = tv;

		uv[1].x = tu + 0.0625f;
		uv[1].y = tv;

		uv[2].x = tu;
		uv[2].y = tv + 0.0625f;

		uv[3].x = tu + 0.0625f;
		uv[3].y = tv + 0.0625f;
	}

	//�@������`��
	void DrawQuad(RenderMode _mode,RenderState _state, DirectX::XMFLOAT2 _uv[4], float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4X4 _mtx, std::string _texstr)
	{
		myquad	m;

		m.width = width;
		m.height = height;
		m.color = color;
		m._mtx = _mtx;

		for (int i = 0; i < 4; i++) {
			m.uv[i] = _uv[i];
		}

		// �摜�̖��O
		m.texname = _texstr;

		// ��Ԃ̕ۑ�
		m.renderState = _state;

		switch (_mode)
		{
		
		case MyQuad::RenderMode::Rendr3D:
			// �\�����������~����
			g_my3Dquad.emplace_back(m);
			break;
		case MyQuad::RenderMode::Rendr2D:
			// �\�����������~����
			g_my2Dquad.emplace_back(m);
			break;
		default:
			break;
		}
	}
}


// todo

// �`��̃��C�����Q�ɕ�����K�v����
// ���X�g�Q��
// �֐����Q�p�ӂ���@�킩��₷������
// �ϐ������ꂼ��p�ӂ���@���ݐ���ő吔
// ���X�g�@�@�X�V�̏�����ԂƓ������H �قȂ�Ȃ�u���C�N�@�ۑ�