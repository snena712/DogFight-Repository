#pragma once
#include	<DirectXMath.h>
#include    "Vector.h"

namespace MyQuad {

#define		MAXMOJICNT		1024

	// 2dと3dの状態
	enum class RenderMode
	{
		Rendr3D,
		Rendr2D
	};

	// シェーダーの状態
	enum class RenderState
	{
		NONE,
		Basic3D,		// 3D
		Basic2D			// 2D
	};

	// 頂点バッファ更新
	static void UpdateVertexBuffer();

	// インデックスバッファ更新
	static void UpdateIndexBuffer();

	// 戻り値　インデックス数
	int Create3DVertexIndexData(int length);

	// 初期化
	bool InitMyQuad();

	// 文字列描画
	void DrawMyQuad3D();

	// 描画する文字列をクリア
	void ClearMyQuad2D();
	void ClearMyQuad3D();

	// アスキーフォント描画用のＵＶ座標取得
	void GetAsciiFontUV(char code, DirectX::XMFLOAT2* uv);

	//　文字列描画
	void DrawQuad(RenderMode _mode, RenderState _state, DirectX::XMFLOAT2 _uv[4], float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4X4 _mtx, std::string _texstr);
}

