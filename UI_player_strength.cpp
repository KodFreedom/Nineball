//--------------------------------------------------------------------------------
//
//　UI.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-07
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "UI_player_strength.h"
#include "player.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define UI_PLAYER_STRENGTH_POS_X (498.0f - 0.5f)//ポリゴンの表示位置X
#define UI_PLAYER_STRENGTH_POS_Y (26.0f - 0.5f)//ポリゴンの表示位置Y
#define UI_PLAYER_STRENGTH_WIDTH (269.0f)
#define UI_PLAYER_STRENGTH_HEIGHT (82.0f)
#define UI_PLAYER_STRENGTH_TEXTURENAME "data/TEXTURE/UI_strength.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIPlayerStrength(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIPlayerStrength = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIPlayerStrength = NULL;//頂点バッファ管理インターフェースポインタ

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitUIPlayerStrength(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	if (FAILED(MakeVerTexUIPlayerStrength(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexUI ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, UI_PLAYER_STRENGTH_TEXTURENAME, &g_pTextureUIPlayerStrength)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitUIPlayerStrength(void)
{
	//safe release
	if (g_pVtxBufferUIPlayerStrength != NULL)
	{
		g_pVtxBufferUIPlayerStrength->Release();
		g_pVtxBufferUIPlayerStrength = NULL;
	}

	if (g_pTextureUIPlayerStrength != NULL)
	{
		g_pTextureUIPlayerStrength->Release();
		g_pTextureUIPlayerStrength = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateUIPlayerStrength(void)
{
	float fTextureX = GetPlayerStrength();

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferUIPlayerStrength->Lock(0, 0, (void**)&pVtx, 0);

	//texture頂点
	pVtx[0].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X + UI_PLAYER_STRENGTH_WIDTH * fTextureX, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X + UI_PLAYER_STRENGTH_WIDTH * fTextureX, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTextureX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTextureX, 1.0f);

	//仮想アドレス解放
	g_pVtxBufferUIPlayerStrength->Unlock();

}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawUIPlayerStrength(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferUIPlayerStrength,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureUIPlayerStrength);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);

}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIPlayerStrength(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferUIPlayerStrength,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferUIPlayerStrength->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0〜255の整数値)
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//texture頂点
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);

	//仮想アドレス解放
	g_pVtxBufferUIPlayerStrength->Unlock();

	return S_OK;
}