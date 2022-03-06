#include "EnemyPosFrame.h"
#define FrameSize (70)

using namespace Component;

EnemyPosFrame::EnemyPosFrame(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("EnemyPosFrame");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/square.png");
	ren->SetAnchorType(AnchorType::LeftTop);
	ren->SetHeight(FrameSize);
	ren->SetWidth(FrameSize);

	start = owner->AddComponent<Component::StartUIColorComponent>(1.0f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));
}

EnemyPosFrame::~EnemyPosFrame()
{

}

void EnemyPosFrame::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	cameraCompo = m_camera->GetComponent<Component::Camera>();
}

void EnemyPosFrame::Update()
{
}


// �r���[���W�ɕϊ�
XMVECTOR EnemyPosFrame::Screenpos(XMVECTOR World_Pos, Vector3 _pos) {


	// �r���[�ƃX�N���[���ϊ�
	XMMATRIX g_View = XMLoadFloat4x4(&cameraCompo->GetCameraMatrix());
	XMMATRIX g_Projection = XMLoadFloat4x4(&cameraCompo->GetProjectionMatrix());

	// �s��쐬
	float w = Application::Instance()->CLIENT_WIDTH / 2.0f;
	float h = Application::Instance()->CLIENT_HEIGHT / 2.0f;
	XMMATRIX viewport = {
	w, 0, 0, 0,
	0, -h, 0, 0,
	0, 0, 1, 0,
	w, h, 0, 1
	};

	// �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	World_Pos = XMVector3Transform(World_Pos, g_View);
	World_Pos = XMVector3Transform(World_Pos, g_Projection);

	// float3�ɕϊ�
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);

	// ������
	XMVECTOR view_vec = XMVECTOR{ 0, 0, 0 };

	// z�Ŋ�����-1~1�͈̔͂Ɏ��߂�

	XMFLOAT4X4 a;

	DX11MtxMultiply(a, Component::Camera::GetMainCamera()->GetCameraMatrix(), Component::Camera::GetMainCamera()->GetProjectionMatrix());

	

	// ��ʓ����H
	if (IsInFrustum(_pos, a))
	{
		m_outside = false;
	}
	else {
		m_outside = true;
	}
	
	view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, -1.0f, 1.0f);


	return XMVector3Transform(view_vec, viewport);
}


void EnemyPosFrame::CheckScreen()
{
	XMVECTOR a = XMVectorSet(Ene->transform->position.x, Ene->transform->position.y, Ene->transform->position.z, 0);
	XMVECTOR b = Screenpos(a, Ene->transform->position);

	auto h = WorldScreenToViewScreen(a, Ene->transform->position);	// �ʒu����@�v���C���[�Ɣ�r���Ċp�x�擾�ł�����
	m_screenPos = h;

	if (Ene->GetExistState() == false)
	{
		return;
	}
	XMFLOAT3* c = new XMFLOAT3;
	XMStoreFloat3(c, b);

	if (m_outside)
	{
		bool inside = true;
		bool Xflg = false, Yflg = false;
		auto _vec = cameraCompo->GetOwner()->transform->position.Direction(Ene->transform->position);
		//printf("%f%f%f\n", _vec.x, _vec.y, _vec.z);

		owner->transform->position.z = 3;


		// ��
		if (c->x > Application::CLIENT_WIDTH)
		{
			inside = false;
			Xflg = true;
			owner->transform->position.x = Application::CLIENT_WIDTH;
		}
		if (c->x < 0)
		{
			inside = false; Xflg = true;
			owner->transform->position.x = 0;
		}

		// ����
		if (c->y > Application::CLIENT_HEIGHT)
		{
			inside = false; Yflg = true;
			float h = Application::CLIENT_HEIGHT;

			owner->transform->position.y = -h;
		}
		if (c->y < 0)
		{
			inside = false; Yflg = true;
			owner->transform->position.y = 0;
		}

		if (!Xflg)
		{
			owner->transform->position.x = c->x;
		}
		if (!Yflg)
		{
			owner->transform->position.y = -c->y;
		}

		// ����
		if (inside)
		{
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0));
		}
		else {
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
		}
	}
	else {

		// �e���W�ɕϊ�
		owner->transform->position.x = c->x;
		owner->transform->position.y = -c->y;
		owner->transform->position.z = 3;
	}
}
