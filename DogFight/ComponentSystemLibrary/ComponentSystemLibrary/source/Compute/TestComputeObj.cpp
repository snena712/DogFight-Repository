#include "TestComputeObj.h"
#include "../../DX11System\DX11Settransform.h"
#include "../../GameObjects/MainCamera.h"
#include "../../Component/SceneManager.h"
#include <random>


using namespace Component;

TestComputeObj::TestComputeObj(GameObject* _owner) : ComponentBase(_owner)
{
	/*owner->SetName("test");
	comCompo = 
		owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png","assets/EfeFile/splashes.EFE");
	*/
}

TestComputeObj::~TestComputeObj()
{}

// �R���s���[�g
void TestComputeObj::DrawObject()
{	
	
}

void TestComputeObj::Update()
{
	// ���ɏ������ރf�[�^�̏�������X�V������

	// �R�s�[��dispatch�@�폜�̏���������
}

