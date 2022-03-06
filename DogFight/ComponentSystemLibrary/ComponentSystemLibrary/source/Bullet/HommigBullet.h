#pragma once
#include "../../Component\GameObject.h"
#include "../../Component\ModelComponent.h"
#include "../../Component\SphereColliderComponent.h"
#include "../../Component\RigidbodyComponent.h"
#include "../../DX11System\dx11mathutil.h"
#include "BulletBase.h"
#include "../Smog/SmogManager.h"

namespace Component {

	class HommingBullet :public ComponentBase
	{
	private:

		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::BulletBase* bul;
		Component::SmogManager* smog;

		Vector3 m_targetPos;			// ターゲットの位置
		GameObject* m_targetobj;		// ターゲットのオブジェクト
		bool m_hitflg = false;			// ヒットのフラグ
		const float movrRotMax = 0.1f;	// 回転の上限

		void MoveUpdate();				// 移動
		void Collision();				// 当たり判定
		void SmogUpdate();				// 煙の更新

	public:
		HommingBullet(GameObject* _obj);
		~HommingBullet();

		void Start();
		void Update();
		void Draw();

		// 初期位置
		void SetInitialPosition(XMFLOAT3 _pos){ owner->transform->position = _pos; }

		// 角度のセット
		void SetDirection(XMFLOAT4 _qt){ owner->transform->qt = _qt; }

		// ターゲットの位置セット
		void SetTargetPos(Vector3 _pos){ m_targetPos = _pos; };
		
		//ターゲットのオブジェクト
		void SetTargetObj(GameObject* _obj){ m_targetobj = _obj; };

		// 煙のセット
		void SetSmogMana(Component::SmogManager* _smog) { smog = _smog; }

		// ヒット管理
		bool GetHitFlg() { return m_hitflg; }
		void SetHitFlg(bool _flg) { m_hitflg = _flg; }
	};
}


