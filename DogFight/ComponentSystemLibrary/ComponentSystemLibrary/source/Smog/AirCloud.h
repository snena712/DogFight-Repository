#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"
#include "../../DX11System/RandomClass.h"
#include "../../Component/SphereColliderComponent.h"
namespace Component
{
	class AirCloud :public ComponentBase
	{
	public:
		// 生存状態
		enum class Status
		{
			LIVE,
			DEAD
		};

	public:
		enum class ObjType
		{
			Player,
			Enemy
		};

	private:
		Component::QuadRender* ren;
		Component::Rigidbody* rb;
		Component::SphereCollider* col;

		Transform* targetTra;
		GameObject* player;

		XMFLOAT4 m_color;		// 色
		float m_height;			// 減少量
		float m_width;			// 横
		float m_reduce = 0;		// α値減少量
		Vector3 m_plussize;		// 広がる大きさ
		Status status = Status::DEAD;
		ObjType m_Type;			// タイプ

		int m_markalpha;		// 目標値
		int m_alphamaxcnt;		// 最大カウント
		int m_alphanowcnt;		// 今のカウント

		int m_collisionCnt = 0;	// 当たったカウント
		int m_collisionMaxCnt = 10;	// 当たった最大カウント

		void AlphaReduce();		// α値減少
		void AlphaChanger();	// α値の変更
		void Collision();		// 当たり判定

	public:
		AirCloud(GameObject* _obj);
		~AirCloud();
		void Start();
		void Update();

		Status GetStatus() { return status; }
		void SetStatus(Status _sta) { status = _sta; };
		void SetAlpha(XMFLOAT4 _hoge) { m_color = _hoge; }
		void SetSize(float _width, float _height) { m_width = _width; m_height = _height; }
		void SetReduce(float _num) { m_reduce = _num; }
		void SetPlusSize(Vector3 _plussize) { m_plussize = _plussize; }
		void SetType(ObjType _type);

	};

}