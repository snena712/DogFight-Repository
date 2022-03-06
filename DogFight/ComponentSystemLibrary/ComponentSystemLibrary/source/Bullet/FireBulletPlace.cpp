#include "FireBulletPlace.h"

using namespace Component;

FireBulletPlace::FireBulletPlace(GameObject* _owner) :ComponentBase(_owner)
{
	efemana =
		owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "assets/EfeFile/BulletFire.EFE");

}

FireBulletPlace::~FireBulletPlace()
{
}

void FireBulletPlace::Start()
{
}

void FireBulletPlace::Update()
{	
	Vector3 _pos = owner->transform->position;
	efemana->SetSpawnPos(_pos);
}
