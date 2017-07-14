//--------------------------------------------------------------------------------
//
//　ウインドウ表示プログラム
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "mainball.h"
#include "ball.h"
#include "table.h"
#include "goal.h"
#include "player.h"
#include "KF_Precision.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define MAIN_BALL_MASS (1.0f)//ボールの重さ
#define MAIN_BALL_TEXTURENAME "data/TEXTURE/ball.png"//ファイル名

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexMainBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureMainBall = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMainBall = NULL;//頂点バッファ管理インターフェースポインタ
CBodyCicle *g_pMainBall = NULL;
int g_nCntMainBall;

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitMainBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	g_pMainBall = new CBodyCicle;
	g_pMainBall->Init();
	g_pMainBall->SetRadius(MAIN_BALL_RADIUS);
	g_pMainBall->cBody.SetPosCenter(MAIN_BALL_POS);
	g_pMainBall->cBody.SetMass(MAIN_BALL_MASS);
	g_pMainBall->cBody.SetGravity(GRAVITY);

	g_nCntMainBall = 0;

	if (FAILED(MakeVerTexMainBall(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		MAIN_BALL_TEXTURENAME,
		&g_pTextureMainBall)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitMainBall(void)
{
	if (g_pMainBall != NULL)//アドレスの中身確認
	{
		g_pMainBall->Uninit();//終了処理
		delete g_pMainBall;//中身を削除
		g_pMainBall = NULL;//アドレスを空にする
	}

	if (g_pVtxBufferMainBall != NULL)
	{
		g_pVtxBufferMainBall->Release();
		g_pVtxBufferMainBall = NULL;
	}

	if (g_pTextureMainBall != NULL)
	{
		g_pTextureMainBall->Release();
		g_pTextureMainBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateMainBall(void)
{
	if (g_nCntMainBall != 0)
	{
		g_nCntMainBall--;
		return;
	}

	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vBallPos;

	g_pVtxBufferMainBall->Lock(0, 0, (void**)&pVtx, 0);

	vBallPos = g_pMainBall->cBody.GetPosCenter();

	//pos
	pVtx[0].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);

	g_pVtxBufferMainBall->Unlock();

	g_pMainBall->Update();

	UpdateMainBallContact();

	if (CheckGoal(vBallPos) == false)
	{
		g_nCntMainBall = 300;
		g_pMainBall->cBody.SetPosCenter(MAIN_BALL_POS);
		g_pMainBall->cBody.SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		g_pMainBall->cBody.SetAcceleration(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		ReducePlayerLife();
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawMainBall(void)
{
	if (g_nCntMainBall != 0) { return; }

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferMainBall,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureMainBall);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexMainBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferMainBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;
	D3DXVECTOR3 vBallPos;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferMainBall->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	vBallPos = g_pMainBall->cBody.GetPosCenter();
	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	pVtx[1].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	pVtx[2].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	pVtx[3].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	//texture頂点
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//仮想アドレス解放
	g_pVtxBufferMainBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  ボール衝突検出
//--------------------------------------------------------------------------------
void UpdateMainBallContact(void)
{
	//check table contact
	CBodyQuadrangle *pTable = NULL;

	for (int nCntTable = 0;nCntTable < NUM_TABLE;nCntTable++)
	{
		pTable = GetTable(nCntTable);
		CheckContactCTQ(g_pMainBall, pTable);
		pTable = NULL;
	}

	AreaContact(&g_pMainBall->cBody, g_pMainBall->GetRadius());

	//check contact
	CBodyCicle *pBall = NULL;

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (!GetBallUsing(nCnt)) { continue; }

		pBall = GetBall(nCnt);
		CheckContactCTC(pBall, g_pMainBall);
		pBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  敵アドレス取得
//--------------------------------------------------------------------------------
CBodyCicle *GetMainBall(void)
{
	return g_pMainBall;
}

D3DXVECTOR3 GetMainBallPos(void)
{
	return g_pMainBall->cBody.GetPosCenter();
}

D3DXVECTOR3 GetMainBallVelocity(void)
{
	return g_pMainBall->cBody.GetVelocity();
}

int GetMainBallCnt(void)
{
	return g_nCntMainBall;
}