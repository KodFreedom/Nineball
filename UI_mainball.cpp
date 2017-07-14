//--------------------------------------------------------------------------------
//
//　ウインドウ表示プログラム
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "UI_mainball.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define UI_MAIN_BALL_RADIUS (13.0f)//ボールの半径
#define UI_MAIN_BALL_TEXTURENAME "data/TEXTURE/circle.png"//ファイル名
#define UI_MAIN_BALL_ALPHA_MAX (1.0f)
#define UI_MAIN_BALL_COUNT_MAX (30)
#define NUM_LIFE (3)

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;//中心点
	float fAlpha;//アルファ値
	int nCnt;//動画カウンター
}UI_MAIN_BALL;

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIMainBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIMainBall = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIMainBall = NULL;//頂点バッファ管理インターフェースポインタ
UI_MAIN_BALL g_aUIMainBall[NUM_LIFE];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitUIMainBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		g_aUIMainBall[nCnt].fAlpha = 0.0f;
		g_aUIMainBall[nCnt].nCnt = 0;
	}

	//set ball pos
	g_aUIMainBall[0].vPos = D3DXVECTOR3(552.0f, 97.0f, 0.0f);//one
	g_aUIMainBall[1].vPos = D3DXVECTOR3(597.0f, 97.0f, 0.0f);//two
	g_aUIMainBall[2].vPos = D3DXVECTOR3(642.0f, 97.0f, 0.0f);//three

	if (FAILED(MakeVerTexUIMainBall(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexUI ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		UI_MAIN_BALL_TEXTURENAME,
		&g_pTextureUIMainBall)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateUITextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitUIMainBall(void)
{
	if (g_pVtxBufferUIMainBall != NULL)
	{
		g_pVtxBufferUIMainBall->Release();
		g_pVtxBufferUIMainBall = NULL;
	}

	if (g_pTextureUIMainBall != NULL)
	{
		g_pTextureUIMainBall->Release();
		g_pTextureUIMainBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateUIMainBall(void)
{
	VERTEX_2D *pVtx = NULL;

	g_pVtxBufferUIMainBall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		if (g_aUIMainBall[nCnt].nCnt != 0)
		{
			g_aUIMainBall[nCnt].nCnt--;
			g_aUIMainBall[nCnt].fAlpha = (float)(UI_MAIN_BALL_COUNT_MAX - g_aUIMainBall[nCnt].nCnt) / (float)UI_MAIN_BALL_COUNT_MAX * UI_MAIN_BALL_ALPHA_MAX;
		}

		//頂点カラーの設定
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
	}

	g_pVtxBufferUIMainBall->Unlock();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawUIMainBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferUIMainBall,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

						   //頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureUIMainBall);

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		//プリミティブ描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCnt * 4,//オフセット（頂点数）
			NUM_POLYGON);
	}
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIMainBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_LIFE,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferUIMainBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferUIMainBall->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		//頂点座標の設定（2D座標、右回り）
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x - UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y - UI_MAIN_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x + UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y - UI_MAIN_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x - UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y + UI_MAIN_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x + UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y + UI_MAIN_BALL_RADIUS, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);

		//texture頂点
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//仮想アドレス解放
	g_pVtxBufferUIMainBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  UI Cnt設定
//--------------------------------------------------------------------------------
void SetUIMainBallCnt(int nNumBall)
{
	g_aUIMainBall[nNumBall].nCnt = UI_MAIN_BALL_COUNT_MAX;
}