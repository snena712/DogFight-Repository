#include "LockOnArrowUIParent.h"

using namespace Component;

LockOnArrowUIParent::LockOnArrowUIParent(GameObject *_owner) :ComponentBase(_owner)
{
	Component::LockOnArrowUI* arrow;
	GameObject* m_centerhit = new GameObject();

	arrow=m_centerhit->AddComponent<Component::LockOnArrowUI>();
	arrow->SetStatus(Component::LockOnArrowUI::Status::RIGHT);
	SceneManager::Instance()->AddObject(m_centerhit, false);
	m_lockArrowList.emplace_back(arrow);

	m_centerhit = new GameObject();
	arrow = m_centerhit->AddComponent<Component::LockOnArrowUI>();
	arrow->SetStatus(Component::LockOnArrowUI::Status::UP);
	SceneManager::Instance()->AddObject(m_centerhit, false);

	m_centerhit = new GameObject();
	arrow = m_centerhit->AddComponent<Component::LockOnArrowUI>();
	arrow->SetStatus(Component::LockOnArrowUI::Status::DOWN);
	SceneManager::Instance()->AddObject(m_centerhit, false);

	m_centerhit = new GameObject();
	arrow = m_centerhit->AddComponent<Component::LockOnArrowUI>();
	arrow->SetStatus(Component::LockOnArrowUI::Status::LEFT);
	SceneManager::Instance()->AddObject(m_centerhit, false);
	m_lockArrowList.emplace_back(arrow);
}


LockOnArrowUIParent::~LockOnArrowUIParent()
{
}

void LockOnArrowUIParent::Start() {}
void LockOnArrowUIParent::Update() {}
void LockOnArrowUIParent::DrawObject()
{
	LockOnUpdate();
}


// ���b�N�I���̍X�V
void LockOnArrowUIParent::LockOnUpdate()
{

}

// �e��4�����J�E���g�ɉ����Ĉړ�������