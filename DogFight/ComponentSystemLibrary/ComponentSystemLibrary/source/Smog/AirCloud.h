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
		// �������
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

		XMFLOAT4 m_color;		// �F
		float m_height;			// ������
		float m_width;			// ��
		float m_reduce = 0;		// ���l������
		Vector3 m_plussize;		// �L����傫��
		Status status = Status::DEAD;
		ObjType m_Type;			// �^�C�v

		int m_markalpha;		// �ڕW�l
		int m_alphamaxcnt;		// �ő�J�E���g
		int m_alphanowcnt;		// ���̃J�E���g

		int m_collisionCnt = 0;	// ���������J�E���g
		int m_collisionMaxCnt = 10;	// ���������ő�J�E���g

		void AlphaReduce();		// ���l����
		void AlphaChanger();	// ���l�̕ύX
		void Collision();		// �����蔻��

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