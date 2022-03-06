#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Config.h"
namespace Component {


	class OptionSettingUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;

		Vector2 m_size;

		void PosUpdate();

	public:
		OptionSettingUI(GameObject* _obj);
		~OptionSettingUI();

		void Start();
		void Update();

		void ChengeReverse() 
		{
			auto _mana = Config::Instance();
			_mana->SetReverseX(!_mana->GetReverseX());
		}
	};

}