#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/AnimationComponent.h"
#include "../../../Component/SceneManager.h"
#include "BulletEffect.h"


namespace Component {

	class BulletEffectManager : public ComponentBase
	{

	private:

		std::vector<Component::BulletEffect*> m_effectList;	// �z�[�~���O�z��
		const int m_effectnum = 3;							// �G�t�F�N�g������

	public:
		BulletEffectManager(GameObject* _obj);
		~BulletEffectManager();

		void Start();
		void Update();

		void Fire(Vector3 _pos,Vector3 _size);		// �G�t�F�N�g����
	};
}