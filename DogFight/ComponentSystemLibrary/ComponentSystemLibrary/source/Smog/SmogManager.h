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

		std::vector< SmogPlayer*> smoglist;		// ‰_‚ÌƒŠƒXƒg
		std::vector< WingSmog*> winglist;		// ‰_‚ÌƒŠƒXƒg
		std::vector< EnemyWingSmog*> ewinglist;	// ‰_‚ÌƒŠƒXƒg
		std::vector< AirSmog*> airsmoglist;		// ‰_‚ÌƒŠƒXƒg
		std::vector< AirCloud*> aircloudlist;		// ‰_‚ÌƒŠƒXƒg
		const int smognum = 500;
		const int wingnum = 20;
		const int enenum = 60;
		const int cloudnum = 15;
		
		const int cloudPosLimit=3000;			// ‰_‚Ì¶¬ŒÀŠE’l

		int enesmognum = 0;						// “G‚Ì”‚É‰‚¶‚Ä’Ç‰Á

		void InstanceCloud(Vector3 _vec);		//	‰_‚Ì¶¬
	public:
		SmogManager(GameObject* _obj);

		void SetActive(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);			// ¶‘¶Šm”F
		void SetWingSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce);							// ‰H‚Ì‰_
		void SetEnemyDeadSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);	// “G‚Ì€–S‚Ì‰_
		void SetEnemySmog(Vector3 _vec,XMFLOAT4 _qt, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce);		// “G‚Ì‰H‰_
		void SetAirSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce);							// ”òs‚ÌüˆÍ‚Ì‰Œ
	};

}