#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"
#include "../../DX11System/RandomClass.h"
#include "SmogPlayer.h"
#include "EnemyWingSmog.h"
#include "WingSmog.h"
#include "AirSmog.h"
#include "AirCloud.h"

namespace Component {

	class SmogManager : public ComponentBase
	{

	private:
		Component::QuadRender* ren;
		Transform* targetTra;

		std::vector< SmogPlayer*> smoglist;		// �_�̃��X�g
		std::vector< WingSmog*> winglist;		// �_�̃��X�g
		std::vector< EnemyWingSmog*> ewinglist;	// �_�̃��X�g
		std::vector< AirSmog*> airsmoglist;		// �_�̃��X�g
		std::vector< AirCloud*> aircloudlist;		// �_�̃��X�g
		const int smognum = 500;
		const int wingnum = 20;
		const int enenum = 60;
		const int cloudnum = 15;
		
		const int cloudPosLimit=3000;			// �_�̐������E�l

		int enesmognum = 0;						// �G�̐��ɉ����Ēǉ�

		void InstanceCloud(Vector3 _vec);		//	�_�̐���
	public:
		SmogManager(GameObject* _obj);

		void SetActive(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);			// �����m�F
		void SetWingSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce);							// �H�̉_
		void SetEnemyDeadSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);	// �G�̎��S���̉_
		void SetEnemySmog(Vector3 _vec,XMFLOAT4 _qt, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);		// �G�̉H�_
		void SetAirSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce);							// ��s���̎��͂̉�
	};

}