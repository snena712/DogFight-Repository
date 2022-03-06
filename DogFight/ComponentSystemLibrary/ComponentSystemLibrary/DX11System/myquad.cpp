#include "myquad.h"

#include	<vector>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"shaderhashmap.h"
#include	"mystring.h"
#include	"shader.h"
#include	"DX11Settransform.h"
#include	"dx11mathutil.h"
#include	"DX11util.h"
#include    "ImageResourceManager.h"
#include    "../Component/CameraComponent.h"

namespace MyQuad {

	using namespace DirectX;
	using  Microsoft::WRL::ComPtr;

	// 使用する頂点シェーダー名
	static const char* vsfilename[] = {
		"shader/basicvs.hlsl",		// 通常（3D）
		"shader/quad2dvs.hlsl"	,	// 通常（2D）
	};

	// 使用するピクセルシェーダー名
	static const char* psfilename[] = {
		"shader/basicps.hlsl",
	};

	// 頂点構造体
	struct Vertex {
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
		XMFLOAT2 Tex;
	};

	// インデックス構造体
	struct Face {
		unsigned int idx[4];
	};

	// 頂点データ
	static std::vector<Vertex> g_vertices;

	// インデックスデータ
	static std::vector<Face> g_indices;

	// 文字コードと表示するためのＵＶ座標
	struct myquad {
		float	width;			// 幅
		float	height;			// 高さ
		XMFLOAT4 color;			// 色
		XMFLOAT2 uv[4];			// UV座標
		XMFLOAT4X4 _mtx;		// 行列
		std::string texname;	// テクスチャ
		RenderState renderState;//描画の状態
	};

	// 頂点バッファ、インデックスバッファ
	ComPtr<ID3D11Buffer>	g_VertexBuffer;
	ComPtr<ID3D11Buffer>	g_IndexBuffer;

	// 表示文字列(最大１０２４文字)
	std::vector<myquad> g_my3Dquad(MAXMOJICNT);
	std::vector<myquad> g_my2Dquad(MAXMOJICNT);

	// 現在の番号と最大値
	int g_nownum;

	// 描画のモード
	RenderState g_RenderState = RenderState::NONE;

	// 描画のレイアウトフラグ
	bool g_layoutflg = false;


	// 頂点バッファ更新
	static void UpdateVertexBuffer() {
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		HRESULT hr = devcontext->Map(g_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(g_vertices.data()), sizeof(Vertex) * g_vertices.size());
			devcontext->Unmap(g_VertexBuffer.Get(), 0);
		}
	}

	// インデックスバッファ更新
	static void UpdateIndexBuffer() {
		D3D11_MAPPED_SUBRESOURCE pData;
		ID3D11DeviceContext* devcontext;
		devcontext = GetDX11DeviceContext();

		HRESULT hr = devcontext->Map(g_IndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(g_indices.data()), sizeof(Face) * g_indices.size());
			devcontext->Unmap(g_IndexBuffer.Get(), 0);
		}
	}

	// 戻り値　FACE数
	int CreateVertexIndexDataFirst(int length) {
		int quadnum = 0;			// 四角形の数

		// 頂点初期化
		for (int idx = 0; idx < length; idx++) {
			Vertex v;
			v.Color = XMFLOAT4(0, 0, 0, 0);
			v.Pos.x = 0.0f;
			v.Pos.y = 0.0f;
			v.Pos.z = 0.0f;
			v.Tex.x = 0.0f;
			v.Tex.y = 0.0f;

			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);
			g_vertices.emplace_back(v);

			Face f;

			f.idx[0] = idx * 4;
			f.idx[1] = idx * 4 + 1;
			f.idx[2] = idx * 4 + 2;
			f.idx[3] = idx * 4 + 3;

			g_indices.emplace_back(f);
		}

		return g_indices.size();
	}

	// 戻り値　FACE数
	int Create3DVertexIndexData(int length) {
		int quadnum = 0;			// 四角形の数

		std::string _str = g_my3Dquad[g_nownum].texname;
		RenderState _state = g_my3Dquad[g_nownum].renderState;
		// 頂点初期化
		for (int idx = g_nownum; idx < g_my3Dquad.size(); idx++) {
			
			// 前と同じ文字列＆状態が同じかの確認
			if (_str == g_my3Dquad[idx].texname&& _state== g_my3Dquad[idx].renderState)
			{

				g_RenderState = g_my3Dquad[idx].renderState;

				Vertex v;
				// 1
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x =  static_cast<float>(-g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[0].x;
				v.Tex.y = g_my3Dquad[idx].uv[0].y;
				g_vertices.push_back(v);

				// 2
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[1].x;
				v.Tex.y = g_my3Dquad[idx].uv[1].y;
				g_vertices.push_back(v);

				// 3 
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x = static_cast<float>(-g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[2].x;
				v.Tex.y = g_my3Dquad[idx].uv[2].y;
				g_vertices.push_back(v);

				// 4
				v.Color = g_my3Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my3Dquad[idx].uv[3].x;
				v.Tex.y = g_my3Dquad[idx].uv[3].y;
				g_vertices.push_back(v);

				// インデックス
				Face f;

				f.idx[0] = quadnum * 4;
				f.idx[1] = quadnum * 4 + 1;
				f.idx[2] = quadnum * 4 + 2;
				f.idx[3] = quadnum * 4 + 3;

				g_indices.push_back(f);

				// 現在の個数の更新
				g_nownum++;
				quadnum++;
			}
			else {
				break;
			}
		}

		// srvの設定
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(_str)->GetSRV() };
		devcontext->PSSetShaderResources(0, 1, srv);

		return quadnum;
	}

	// 戻り値　FACE数
	int Create2DVertexIndexData(int length) {
	
		int quadnum = 0;			// 四角形の数

		std::string _str = g_my2Dquad[g_nownum].texname;
		RenderState _state = g_my2Dquad[g_nownum].renderState;
		
		// 頂点初期化
		for (int idx = g_nownum; idx < g_my2Dquad.size(); idx++) {

			// 前と同じ文字列＆状態が同じかの確認
			if (_str == g_my2Dquad[idx].texname && _state == g_my2Dquad[idx].renderState)
			{

				g_RenderState = g_my2Dquad[idx].renderState;

				Vertex v;
				// 1
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(-g_my2Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my2Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[0].x;
				v.Tex.y = g_my2Dquad[idx].uv[0].y;
				g_vertices.push_back(v);

				// 2
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my3Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(g_my3Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[1].x;
				v.Tex.y = g_my2Dquad[idx].uv[1].y;
				g_vertices.push_back(v);

				// 3 
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(-g_my2Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my2Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[2].x;
				v.Tex.y = g_my2Dquad[idx].uv[2].y;
				g_vertices.push_back(v);

				// 4
				v.Color = g_my2Dquad[idx].color;
				v.Pos.x = static_cast<float>(g_my2Dquad[idx].width / 2);
				v.Pos.y = static_cast<float>(-g_my2Dquad[idx].height / 2);
				v.Pos.z = 0;
				v.Tex.x = g_my2Dquad[idx].uv[3].x;
				v.Tex.y = g_my2Dquad[idx].uv[3].y;
				g_vertices.push_back(v);

				// インデックス
				Face f;

				f.idx[0] = quadnum * 4;
				f.idx[1] = quadnum * 4 + 1;
				f.idx[2] = quadnum * 4 + 2;
				f.idx[3] = quadnum * 4 + 3;

				g_indices.push_back(f);

				// 現在の個数の更新
				g_nownum++;
				quadnum++;
			}
			else {
				break;
			}
		}

		// srvの設定
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(_str)->GetSRV() };
		devcontext->PSSetShaderResources(0, 1, srv);

		return quadnum;
	}

	// 初期化
	bool InitMyQuad() {

		int cnt = 0;

		// デバイスを取得
		ID3D11Device* device;
		device = GetDX11Device();

		// デバイスコンテキストを取得
		ID3D11DeviceContext* devicecontext;
		devicecontext = GetDX11DeviceContext();

		// 文字列サイズ初期化
		g_my3Dquad.clear();
		g_my2Dquad.clear();

		// 頂点バッファとインデックスバッファの初期化データ生成
		CreateVertexIndexDataFirst(MAXMOJICNT);

		// 頂点バッファ生成
		bool sts = CreateVertexBufferWrite(device, sizeof(Vertex), MAXMOJICNT * 4, g_vertices.data(), &g_VertexBuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateVerteBuffer error", "error", MB_OK);
			return false;
		}

		// インデックスバッファ生成
		sts = CreateIndexBufferWrite(device, MAXMOJICNT * 4 * 2, g_indices.data(), &g_IndexBuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
			return false;
		}

		// ピクセルシェーダーを生成
		sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
		if (!sts) {
			MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
			return false;
		}

		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// エレメント数
		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーを生成
		sts = ShaderHashmap::GetInstance()->SetVertexShader(
			vsfilename[0],			// 頂点シェーダーファイル名
			layout,					// 頂点レイアウト
			numElements);			// エレメント数

		// 頂点シェーダーを生成
		sts = ShaderHashmap::GetInstance()->SetVertexShader(
			vsfilename[1],			// 頂点シェーダーファイル名
			layout,					// 頂点レイアウト
			numElements);			// エレメント数

		// VECTOR初期化
		ClearMyQuad3D();
		ClearMyQuad2D();

		return true;
	}

	// 3Dオブジェクト描画
	void DrawMyQuad3D() {

		XMFLOAT4X4 mtx;

		// ビュー変換行列セット
		mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

		// プロジェクション変換行列セット
		mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);


		// 文字数が登録吸うより少ない間
		while (g_my3Dquad.size() > g_nownum)
		{
			ID3D11VertexShader* vs;
			ID3D11PixelShader* ps;

			// 画像の文字列が同じ間描画する
			auto num = Create3DVertexIndexData(static_cast<int>(g_my3Dquad.size()));

			// 頂点バッファ更新
			UpdateVertexBuffer();

			// インデックスバッファ更新
			UpdateIndexBuffer();

			// デバイスコンテキストを取得
			ID3D11DeviceContext* devcontext;
			devcontext = GetDX11DeviceContext();

			unsigned int stride = sizeof(Vertex);	// ストライドをセット（１頂点当たりのバイト数）
			unsigned  offset = 0;					// オフセット値をセット

			//------------------------------------------------
			// 頂点バッファをデバイスコンテキストへセット
			//------------------------------------------------
			devcontext->IASetVertexBuffers(
				0,									// スタートスロット
				1,									// 頂点バッファ個数
				g_VertexBuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
				&stride,							// ストライド
				&offset);							// オフセット

			devcontext->IASetIndexBuffer(			// インデックスバッファをデバイスコンテキストへセット
				g_IndexBuffer.Get(),				// インデックスバッファ
				DXGI_FORMAT_R32_UINT,				// フォーマット
				0);									// オフセット			


				// 新しい描画状態
			switch (g_RenderState)
			{
			case RenderState::Basic3D:
				// 頂点シェーダー、ピクセルシェーダー取得					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// 頂点シェーダーとピクセルシェーダーをセット
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				break;

			case RenderState::Basic2D:
				// 頂点シェーダー、ピクセルシェーダー取得					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[1]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// 頂点シェーダーとピクセルシェーダーをセット
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				// トポロジーをセット
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				break;
			default:
				break;
			}

			// 頂点レイアウト取得
			if (!g_layoutflg)
			{
				g_layoutflg = true;

				ID3D11InputLayout* layout;
				layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);
				// 頂点フォーマットをセット
				devcontext->IASetInputLayout(layout);
				// トポロジーをセット
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			// 描画処理
			for (int i = 0; i < num; i++) {
				// 各々のリストの行列取得
				DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_my3Dquad[g_nownum - num + i]._mtx);
				// ドローコール発行
				devcontext->DrawIndexed(
					4,			// 個数
					i * 4,		// 開始インデックス					
					0);			// 基準頂点インデックス				
			}

			// 次の描画用にインデックスと頂点のリスト初期化
			g_indices.clear();
			g_vertices.clear();
		}
		// 初期化
		ClearMyQuad3D();
	}

	// 3Dオブジェクト描画
	void DrawMyQuad2D() {

		XMFLOAT4X4 mtx;

		// ビュー変換行列セット
		mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

		// プロジェクション変換行列セット
		mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);


		// 文字数が登録吸うより少ない間
		while (g_my2Dquad.size() > g_nownum)
		{
			ID3D11VertexShader* vs;
			ID3D11PixelShader* ps;

			// 画像の文字列が同じ間描画する
			auto num = Create2DVertexIndexData(static_cast<int>(g_my2Dquad.size()));

			// 頂点バッファ更新
			UpdateVertexBuffer();

			// インデックスバッファ更新
			UpdateIndexBuffer();

			// デバイスコンテキストを取得
			ID3D11DeviceContext* devcontext;
			devcontext = GetDX11DeviceContext();

			unsigned int stride = sizeof(Vertex);	// ストライドをセット（１頂点当たりのバイト数）
			unsigned  offset = 0;					// オフセット値をセット

			//------------------------------------------------
			// 頂点バッファをデバイスコンテキストへセット
			//------------------------------------------------
			devcontext->IASetVertexBuffers(
				0,									// スタートスロット
				1,									// 頂点バッファ個数
				g_VertexBuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
				&stride,							// ストライド
				&offset);							// オフセット

			devcontext->IASetIndexBuffer(			// インデックスバッファをデバイスコンテキストへセット
				g_IndexBuffer.Get(),				// インデックスバッファ
				DXGI_FORMAT_R32_UINT,				// フォーマット
				0);									// オフセット			


				// 新しい描画状態
			switch (g_RenderState)
			{
			case RenderState::Basic3D:
				// 頂点シェーダー、ピクセルシェーダー取得					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// 頂点シェーダーとピクセルシェーダーをセット
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				break;

			case RenderState::Basic2D:
				// 頂点シェーダー、ピクセルシェーダー取得					
				vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[1]);
				ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

				// 頂点シェーダーとピクセルシェーダーをセット
				devcontext->VSSetShader(vs, nullptr, 0);
				devcontext->PSSetShader(ps, nullptr, 0);

				// トポロジーをセット
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				break;
			default:
				break;
			}

			// 頂点レイアウト取得
			if (!g_layoutflg)
			{
				g_layoutflg = true;

				ID3D11InputLayout* layout;
				layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);
				// 頂点フォーマットをセット
				devcontext->IASetInputLayout(layout);
				// トポロジーをセット
				devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			// 描画処理
			for (int i = 0; i < num; i++) {
				// 各々のリストの行列取得
				DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_my2Dquad[g_nownum - num + i]._mtx);
				// ドローコール発行
				devcontext->DrawIndexed(
					4,			// 個数
					i * 4,		// 開始インデックス					
					0);			// 基準頂点インデックス				
			}

			// 次の描画用にインデックスと頂点のリスト初期化
			g_indices.clear();
			g_vertices.clear();
		}
		// 初期化
		ClearMyQuad2D();
	}

	// 描画する文字列をクリア
	void ClearMyQuad3D() {
		
		// 描画文字列をクリア
		g_my3Dquad.clear();

		// インデックスクリア
		g_indices.clear();

		// 頂点クリア
		g_vertices.clear();

		// 描画個数の初期化
		g_nownum = 0;

		// レイアウトの更新フラグ初期化
		g_layoutflg = false;
	}

	// 描画する文字列をクリア
	void ClearMyQuad2D() {

		// 描画文字列をクリア
		g_my2Dquad.clear();


		// インデックスクリア
		g_indices.clear();

		// 頂点クリア
		g_vertices.clear();

		// 描画個数の初期化
		g_nownum = 0;
		

		// レイアウトの更新フラグ初期化
		g_layoutflg = false;
	}

	// アスキーフォント描画用のＵＶ座標取得
	void GetAsciiFontUV(char code, XMFLOAT2* uv) {

		int codex;
		int codey;

		codex = code & 0x0f;
		codey = code >> 4;

		codey = codey - 2;

		float tu, tv;

		tu = codex * 0.0625f;
		tv = codey * 0.0625f;

		uv[0].x = tu;
		uv[0].y = tv;

		uv[1].x = tu + 0.0625f;
		uv[1].y = tv;

		uv[2].x = tu;
		uv[2].y = tv + 0.0625f;

		uv[3].x = tu + 0.0625f;
		uv[3].y = tv + 0.0625f;
	}

	//　文字列描画
	void DrawQuad(RenderMode _mode,RenderState _state, DirectX::XMFLOAT2 _uv[4], float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4X4 _mtx, std::string _texstr)
	{
		myquad	m;

		m.width = width;
		m.height = height;
		m.color = color;
		m._mtx = _mtx;

		for (int i = 0; i < 4; i++) {
			m.uv[i] = _uv[i];
		}

		// 画像の名前
		m.texname = _texstr;

		// 状態の保存
		m.renderState = _state;

		switch (_mode)
		{
		
		case MyQuad::RenderMode::Rendr3D:
			// 表示文字列情報を蓄える
			g_my3Dquad.emplace_back(m);
			break;
		case MyQuad::RenderMode::Rendr2D:
			// 表示文字列情報を蓄える
			g_my2Dquad.emplace_back(m);
			break;
		default:
			break;
		}
	}
}


// todo

// 描画のラインを２つに分ける必要あり
// リスト２つ
// 関数を２つ用意する　わかりやすいから
// 変数もそれぞれ用意する　現在数や最大数
// リスト　　更新の初期状態と同じか？ 異なるならブレイク　保存