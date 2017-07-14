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
#include "UI_ball.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define UI_BALL_RADIUS (12.0f)//ボールの半径
#define UI_BALL_TEXTURENAME "data/TEXTURE/circle.png"//ファイル名
#define UI_BALL_ALPHA_MAX (0.8f)
#define UI_BALL_COUNT_MAX (30)
#define NUM_BALL (9)

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;//中心点
	float fAlpha;//アルファ値
	int nCnt;//動画カウンター
}UI_BALL;

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIBall = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIBall = NULL;//頂点バッファ管理インターフェースポインタ
UI_BALL g_aUIBall[NUM_BALL];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitUIBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		g_aUIBall[nCnt].fAlpha = 0.0f;
		g_aUIBall[nCnt].nCnt = 0;
	}

	//set ball pos
	g_aUIBall[0].vPos = D3DXVECTOR3(75.0f, 52.0f, 0.0f);//one
	g_aUIBall[1].vPos = D3DXVECTOR3(120.0f, 52.0f, 0.0f);//two
	g_aUIBall[2].vPos = D3DXVECTOR3(165.0f, 52.0f, 0.0f);//three
	g_aUIBall[3].vPos = D3DXVECTOR3(210.0f, 52.0f, 0.0f);//four
	g_aUIBall[4].vPos = D3DXVECTOR3(75.0f, 88.0f, 0.0f);//five
	g_aUIBall[5].vPos = D3DXVECTOR3(120.0f, 88.0f, 0.0f);//six
	g_aUIBall[6].vPos = D3DXVECTOR3(165.0f, 88.0f, 0.0f);//seven
	g_aUIBall[7].vPos = D3DXVECTOR3(210.0f, 88.0f, 0.0f);//eight
	g_aUIBall[8].vPos = D3DXVECTOR3(255.0f, 70.0f, 0.0f);//nine

	if (FAILED(MakeVerTexUIBall(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexUI ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		UI_BALL_TEXTURENAME,
		&g_pTextureUIBall)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateUITextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitUIBall(void)
{
	if (g_pVtxBufferUIBall != NULL)
	{
		g_pVtxBufferUIBall->Release();
		g_pVtxBufferUIBall = NULL;
	}

	if (g_pTextureUIBall != NULL)
	{
		g_pTextureUIBall->Release();
		g_pTextureUIBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateUIBall(void)
{
	VERTEX_2D *pVtx = NULL;

	g_pVtxBufferUIBall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aUIBall[nCnt].nCnt != 0)
		{
			g_aUIBall[nCnt].nCnt--;
			g_aUIBall[nCnt].fAlpha = (float)(UI_BALL_COUNT_MAX - g_aUIBall[nCnt].nCnt) / (float)UI_BALL_COUNT_MAX * UI_BALL_ALPHA_MAX;
		}

		//頂点カラーの設定
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
	}

	g_pVtxBufferUIBall->Unlock();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawUIBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferUIBall,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

						   //頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureUIBall);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
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
HRESULT MakeVerTexUIBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BALL,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferUIBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferUIBall->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		//頂点座標の設定（2D座標、右回り）
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x - UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y - UI_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x + UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y - UI_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x - UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y + UI_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x + UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y + UI_BALL_RADIUS, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);

		//texture頂点
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//仮想アドレス解放
	g_pVtxBufferUIBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  UI Cnt設定
//--------------------------------------------------------------------------------
void SetUIBallCnt(int nNumBall)
{
	g_aUIBall[nNumBall].nCnt = UI_BALL_COUNT_MAX;
}