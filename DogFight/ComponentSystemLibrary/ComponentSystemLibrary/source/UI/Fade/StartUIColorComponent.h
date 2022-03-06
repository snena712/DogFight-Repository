#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../MainGameManager.h"

namespace Component {
	class  StartUIColorComponent :public ComponentBase
	{
	public:
		enum class Fade {
			FadeIn,
			FadeOut,
			Normal,
			None,
		};
		enum class RnderState
		{
			NORMAL,
			TWO
		};
	private:
		Component::QuadRender*ren;
		Component::Quad2DRender* ren2d;

		float m_fadeCnt = 0;			// フェード時のカウント
		const float m_fadeMaxCnt = 20;	// フェード時のカウント
		float m_alpha = 1.0f;			// 文字透過色
		float m_stockAlpha = 1.0f;		// 文字透過色
		bool m_fadeFlg = false;			// フラグ入ると自動変換なくなる
		std::string m_stackString;		// 描画文字列
		Vector3 m_pos;					// 大きさ
		Vector3 m_size;					// サイズ
		Vector3 m_color;				// 色
		Fade m_fadeStatus = Fade::None;// フェードの状態

	public:
		StartUIColorComponent(GameObject *_owner, float _alpha, RnderState _state);
		~StartUIColorComponent();

		// 初期の関数
		void Update();

		// 描画設定用の関数
		void SetString(std::string _str) { m_stackString = _str; }
		void SetColor(Vector3 _col) { m_color = _col; }
		void SetBaseAlpha(float _alpha) { m_stockAlpha = _alpha; }
		void SetPosSize(Vector3 _pos, Vector3 _size) { m_pos = _pos; m_size = _size; }
		void SetFadeStatus(Fade _fade) {
			if (_fade == Fade::FadeIn) {
				if (m_fadeStatus != Fade::Normal&&m_fadeStatus != Fade::FadeIn) {
					m_fadeStatus = _fade;
					m_fadeFlg=true;
				}
			}
			if (_fade == Fade::FadeOut) {
				if (m_fadeStatus != Fade::None&&m_fadeStatus != Fade::FadeOut) {
					m_fadeStatus = _fade;
					m_fadeFlg = true;
				}
			}
		}
		Fade GetFadeStatus() { return m_fadeStatus; }
		void ColorSet();
		void GameStateUpdate();
	};
}


