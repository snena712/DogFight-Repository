#include "PlayerJatPartsParent.h"
#include "../../Component/SceneManager.h"

using namespace Component;
int _rot = 0;float _leng = 0.29f;

PlayerJatPartsParent::PlayerJatPartsParent(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("PlayerJatPartsParent");

	tra = owner->transform;

	// �I�u�W�F�N�g�̃V�[���ւ̓��ꍞ��
	std::vector<GameObject*> _list;	

	Vector3 _center;

	// �e��p�[�c�̃Z�b�g
	for (int i = 0; i < m_jetnum; i++)
	{
		GameObject* _jet = new GameObject();
		Component::PlayerJatParts* pjet = _jet->AddComponent<Component::PlayerJatParts>();
		_jet->transform->SetParent(owner->transform);
		_jet->transform->RotateLocalZaxis(_rot);
		_jet->transform->RotateLocalXaxis(-24.5f);

		
		double radi = (_rot+90.0f) * 3.141592653589793f / 180;
		_jet->transform->position.x = _center.x + _leng * cos(radi);
		_jet->transform->position.y = _center.y + _leng * sin(radi);		// �����}�C�i�X�ɂȂ����̂Ń}�C�i�X����

		m_jetlist.emplace_back(pjet);
		_list.emplace_back(_jet);

		_rot += 40;
	}	

	SceneManager::Instance()->AddList(&_list);
}

PlayerJatPartsParent::~PlayerJatPartsParent()
{

}

void PlayerJatPartsParent::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");

	owner->transform->SetParent(player->transform);
}

void PlayerJatPartsParent::Update()
{
}

// �p�x�̐ݒ�
void PlayerJatPartsParent::SetRot(float _num)
{
	for (auto _list : m_jetlist)
	{
		_list->SetRot(_num);
	}
}




