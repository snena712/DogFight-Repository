#include "FogDistance.h"


using namespace Component;

// �n�ʐ���
FogDistance::FogDistance(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("Ground");

	tra = owner->transform;

	// �eSRV�̐ݒ�
	ImageResourceManager::GetInstance()->Load("assets/Image/Ocean/Ocean2.jpg");

	// Shader������
	InitVertexShader(CDirectXGraphics::GetInstance()->GetDXDevice(),
		"shader/Fog.hlsl",
		"VS",
		"vs_5_0",
		&m_pFogVS);

	InitPixelShader(CDirectXGraphics::GetInstance()->GetDXDevice(),
		"shader/Fog.hlsl",
		"PS",
		"ps_5_0",
		&m_pFogPS);



	RenderingType render = RenderingType::Opaque;
	SceneManager::AddDrawFunction(render, owner);


	m_oceanConBuffer = new ConstBuffer();
	m_oceanConBuffer->CreateConStructuredBuffer<OceanConStruct>(ConstBuffer::Type::Normal,1);

	// �ꏊ�w��@���S�ɂȂ�悤�ɂ���
	tra->position = Vector3(-3000, 0, -3000);
	tra->RotateLocalXaxis(90);
	tra->scale = Vector3(500, 500, 1);
}

FogDistance::~FogDistance()
{

}

void FogDistance::Start()
{
}

void FogDistance::DrawObject() {

	DrawSetting();
}

/// <summary>
/// �f�X�g���N�^�B
/// </summary>
void FogDistance::DrawSetting()
{
	// Shader�̃Z�b�g

	// �e�o�b�t�@�̍X�V

	// �e�o�b�t�@�̐ݒ�i�萔��J�����ށj

	// SRV�̐ݒ�

	// �摜�̐ݒ�


	//// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	//// ���[���h�ϊ��s��Z�b�g

	DirectX::XMFLOAT4X4 make, mtx, scale;
	DX11MtxIdentity(mtx);	//���[���h�ϊ��s���P�ʍs��ɂ���

	DX11MtxScale(tra->scale.x, tra->scale.y, tra->scale.z, scale);
	DX11MtxFromQt(mtx, tra->qt);
	DX11MtxMultiply(make, scale, mtx);

	make._41 = tra->position.x;
	make._42 = tra->position.y;
	make._43 = tra->position.z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);

	// �o�b�t�@�̃Z�b�g
	m_oceanConStruct.size = 200;
	m_oceanConStruct.uv++;
	m_oceanConBuffer->NoPointerUpdate(m_oceanConStruct);
	m_oceanConBuffer->SetVSBuffer(3);
	m_oceanConBuffer->SetGSBuffer(3);

	////�@������
	//ID3D11Buffer* ppCBNULL[2] = { NULL,NULL };
	//devcontext->VSSetConstantBuffers(0, 2, ppCBNULL);

	//// �eSRV�̐ݒ�
	ID3D11ShaderResourceView* srv = ImageResourceManager::GetInstance()->GetResource("assets/Image/Ocean/Ocean2.jpg")->GetSRV();
	devcontext->PSSetShaderResources(0, 1, &srv);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�A���_���C�A�E�g�擾		
	devcontext->VSSetShader(m_pFogVS, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(m_pFogPS, nullptr, 0);

	//// �`��̐ݒ�
	//ID3D11Buffer* const                 g_pNullBuffer = NULL;    // Helper to Clear Buffers
	//UINT                                g_iNullUINT = 0;         // Helper to Clear Buffers
	//devcontext->IASetVertexBuffers(0, 1, &g_pNullBuffer, &g_iNullUINT, &g_iNullUINT);   //NULL
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);             // ���X�g

	// �h���[�R�[�����s
	devcontext->Draw(
		m_oceanConStruct.size*m_oceanConStruct.size,			// �C���f�b�N�X���@�@�i���͌�Ō��߂�j
		0);														// ����_�C���f�b�N�X

	devcontext->GSSetShader(nullptr
		, nullptr, 0);
}

