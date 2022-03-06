#pragma once
#include <string>
#include "../../DX11System/ImageResourceManager.h"
#include	<DirectXMath.h>
class SkyBox
{
public:
	SkyBox(const SkyBox&) = delete;
	SkyBox& operator=(const SkyBox&) = delete;
	SkyBox(SkyBox&&) = delete;
	SkyBox& operator=(SkyBox&&) = delete;

	static SkyBox* GetInstance() {
		static SkyBox instance;
		return &instance;
	}

	void Init(const char* _filename, const char* _vsfile, const char* _psfile, const char* _texfolder);
	void Exit();
	void Draw();

	bool GetDrawState() { return m_drawExists; }
	void SetDrawState(bool _newState) { m_drawExists = _newState; }

	float GetScale() { return m_scale; }
	void SetScale(float _scale) {
		if (_scale < 1.0f) return;
		m_scale = _scale;
	}
bool  InitTex2D(const char* _filename,
	const char* _vsfile,
	const char* _psfile,
	const char* _texfolder);

private:
	SkyBox() {};
	~SkyBox() {};
	
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT4	Color;
		DirectX::XMFLOAT3	Normal;
	};
	struct Face {
		unsigned int idx[3];
	};
	
	void CreateSphere(int LatLines, int LongLines);

	int NumSphereVertices;
	int NumSphereFaces;
	
	enum FILETYPE {
		eDATFILE,
		eASSIMPFILE
	};
	FILETYPE	m_filetype = eASSIMPFILE;	// �t�@�C���^�C�v


	std::string	m_assimpfileName;			// assimpfile��
	std::string m_VertexShaderName;			// ���_�V�F�[�_�[�t�@�C����
	std::string m_PixelShaderName;			// �s�N�Z���V�F�[�_�[�t�@�C����

	ID3D11ShaderResourceView* _tex;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ID3D11RasterizerState* RSCullNone;


	bool m_drawExists;	//�`�悷�邩�ۂ�
	float m_scale;



};

