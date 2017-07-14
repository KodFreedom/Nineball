//--------------------------------------------------------------------------------
//
//　ウインドウ表示プログラム
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "input.h"
#include "player.h"
#include "fade.h"
#include "mainball.h"
#include "UI_mainball.h"
#include "sound.h"
#include "KF_Precision.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define PLAYER_WIDTH (300.0f)//ポリゴンの幅
#define PLAYER_HEIGHT (8.0f)//ポリゴンの高さ
#define PLAYER_TEXTURENAME "data/TEXTURE/stick.png"//ファイル名
#define DISTANCE_MAX (100.0f)
#define FORCE_TO_BALL_MAX (-50.0f)
#define STRENGTH_ADD_SPEED (0.01f)
#define STRENGTH_REDUCE_SPEED (-0.2f)
#define ROTATION_SPEED (D3DX_PI / 180.0f)//回転速度

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct {
	LPDIRECT3DTEXTURE9 pTexture;//textureインターフェース
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;//頂点バッファ管理インターフェースポインタ
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVecToPos[4];
	D3DXVECTOR3 vDistanceToMainBall;//力によって距離が大きくなる
	int nLife;
	float fRotation;
	float fPushStrength;
	float fStrength;//ボールを打つ力
	bool bPress;
	bool bPush;
	bool bTurn;
}PLAYER;

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
PLAYER g_player;

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	g_player.pTexture = NULL;
	g_player.pVtxBuffer = NULL;

	g_player.vDistanceToMainBall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.nLife = 3;
	g_player.fStrength = 0.0f;
	g_player.fPushStrength = 0.0f;
	g_player.fRotation = 0.0f;
	g_player.vPos = MAIN_BALL_POS;
	g_player.bTurn = true;
	g_player.bPush = false;
	g_player.bPress = false;

	g_player.vVecToPos[0] = D3DXVECTOR3(MAIN_BALL_RADIUS, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[1] = D3DXVECTOR3(MAIN_BALL_RADIUS + PLAYER_WIDTH * 0.5f, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[2] = D3DXVECTOR3(MAIN_BALL_RADIUS, PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[3] = D3DXVECTOR3(MAIN_BALL_RADIUS + PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT * 0.5f, 0.0f);


	if (FAILED(MakeVerTexPlayer(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEXTURENAME,
		&g_player.pTexture)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitPlayer(void)
{
	if (g_player.pVtxBuffer != NULL)
	{
		g_player.pVtxBuffer->Release();
		g_player.pVtxBuffer = NULL;
	}

	if (g_player.pTexture != NULL)
	{
		g_player.pTexture->Release();
		g_player.pTexture = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vVertex;
	g_player.vPos = GetMainBallPos();
	g_player.vDistanceToMainBall = D3DXVECTOR3(DISTANCE_MAX * cosf(g_player.fRotation) * g_player.fStrength,
		DISTANCE_MAX * sinf(g_player.fRotation) * g_player.fStrength,
		0.0f);
	g_player.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//pos
	for (int nCnt = 0;nCnt < 4;nCnt++)
	{
		vVertex = g_player.vVecToPos[nCnt];

		//AFFIN転換
		vVertex = D3DXVECTOR3(
			g_player.vVecToPos[nCnt].x * cosf(g_player.fRotation) - g_player.vVecToPos[nCnt].y * sinf(g_player.fRotation),
			g_player.vVecToPos[nCnt].x * sinf(g_player.fRotation) + g_player.vVecToPos[nCnt].y * cosf(g_player.fRotation),
			0.0f);

		pVtx[nCnt].pos = g_player.vPos + vVertex + g_player.vDistanceToMainBall;
	}

	//texture
	g_player.pVtxBuffer->Unlock();

	if (g_player.bTurn)
	{
		//key入力
		if (GetKeyboardPress(DIK_UP))//
		{
			g_player.fStrength += STRENGTH_ADD_SPEED;
			g_player.fStrength = g_player.fStrength >= 1.0f ? 1.0f : g_player.fStrength;
			g_player.fPushStrength = g_player.fStrength;
			g_player.bPress = true;
		}
		else
		{
			g_player.bPress = false;
			g_player.bTurn = false;
		}

		if (GetKeyboardPress(DIK_LEFT))//左回転
		{
			g_player.fRotation -= ROTATION_SPEED;
		}

		if (GetKeyboardPress(DIK_RIGHT))//右回転
		{
			g_player.fRotation += ROTATION_SPEED;
		}

		if (g_player.fStrength > 0.0f && g_player.bPress == false)
		{
			g_player.bPush = true;
		}
	}

	if (g_player.bPush == true)
	{
		g_player.fStrength += STRENGTH_REDUCE_SPEED;
		g_player.fStrength = g_player.fStrength <= 0.0f ? 0.0f : g_player.fStrength;

		if (g_player.fStrength == 0.0f)
		{
			CBodyCicle *pMainBall = GetMainBall();
			pMainBall->cBody.AddForce(D3DXVECTOR3(FORCE_TO_BALL_MAX * cosf(g_player.fRotation) * g_player.fPushStrength,
				FORCE_TO_BALL_MAX * sinf(g_player.fRotation) * g_player.fPushStrength,
				0.0f));

			g_player.fPushStrength = 0.0f;
			g_player.bPush = false;
			PlaySound(SOUND_LABEL_SE_SHOT);
		}
	}
	
	
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	//player
	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_player.pVtxBuffer,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_player.pTexture);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	//player
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_player.pVtxBuffer,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_player.pVtxBuffer->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = g_player.vPos + g_player.vVecToPos[0];
	pVtx[1].pos = g_player.vPos + g_player.vVecToPos[1];
	pVtx[2].pos = g_player.vPos + g_player.vVecToPos[2];
	pVtx[3].pos = g_player.vPos + g_player.vVecToPos[3];


	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//texture頂点
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//仮想アドレス解放
	g_player.pVtxBuffer->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  flag get
//--------------------------------------------------------------------------------
bool *GetPlayerTurnFlag(void)
{
	return &g_player.bTurn;
}

//--------------------------------------------------------------------------------
//  get strength
//--------------------------------------------------------------------------------
float GetPlayerStrength(void)
{
	return g_player.fStrength;
}

void ReducePlayerLife(void)
{
	g_player.nLife--;

	SetUIMainBallCnt(g_player.nLife);

	if (g_player.nLife == 0)
	{
		//game over
		SetFade(FADE_OUT, MODE_ENDING);
	}
}

float GetPlayerRotation(void)
{
	return g_player.fRotation;
}