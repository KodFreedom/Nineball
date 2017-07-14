//--------------------------------------------------------------------------------
//
//　ウインドウ表示プログラム
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "ball.h"
#include "UI_ball.h"
#include "goal.h"
#include "table.h"
#include "fade.h"
#include "KF_Precision.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define BALL_RADIUS (5.75f * 0.5f * PX_PER_CM)//ボールの半径
#define BALL_MASS (1.0f)//ボールの重さ
#define BALL_TEXTURENAME "data/TEXTURE/ball.png"//ファイル名
#define BALL_POS (D3DXVECTOR3(238.0f,365.0f,0.0f))//ボールの開始位置

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct {
	CBodyCicle *pBody;
	bool bUsed;
}BALL;

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBall = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBall = NULL;//頂点バッファ管理インターフェースポインタ
BALL g_aBall[NUM_BALL];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		g_aBall[nCnt].bUsed = true;
		g_aBall[nCnt].pBody = new CBodyCicle;
		g_aBall[nCnt].pBody->Init();
		g_aBall[nCnt].pBody->SetRadius(BALL_RADIUS);
		g_aBall[nCnt].pBody->cBody.SetMass(BALL_MASS);
		g_aBall[nCnt].pBody->cBody.SetGravity(GRAVITY);
	}

	//set ball pos
	g_aBall[0].pBody->cBody.SetPosCenter(BALL_POS);//one
	g_aBall[1].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 2.0f * sinf(D3DX_PI / 3), -BALL_RADIUS, 0.0f));//two
	g_aBall[2].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 2.0f * sinf(D3DX_PI / 3), BALL_RADIUS, 0.0f));//three
	g_aBall[3].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 4.0f * sinf(D3DX_PI / 3), -BALL_RADIUS * 2.0f, 0.0f));//four
	g_aBall[4].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 4.0f * sinf(D3DX_PI / 3), BALL_RADIUS * 2.0f, 0.0f));//five
	g_aBall[5].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 6.0f * sinf(D3DX_PI / 3), -BALL_RADIUS, 0.0f));//six
	g_aBall[6].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 6.0f * sinf(D3DX_PI / 3), BALL_RADIUS, 0.0f));//seven
	g_aBall[7].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 8.0f * sinf(D3DX_PI / 3), 0.0f, 0.0f));//eight
	g_aBall[8].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 4.0f * sinf(D3DX_PI / 3), 0.0f, 0.0f));//nine

	if (FAILED(MakeVerTexBall(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		BALL_TEXTURENAME,
		&g_pTextureBall)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitBall(void)
{
	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].pBody != NULL)//アドレスの中身確認
		{
			g_aBall[nCnt].pBody->Uninit();//終了処理
			delete g_aBall[nCnt].pBody;//中身を削除
			g_aBall[nCnt].pBody = NULL;//アドレスを空にする
		}
	}

	if (g_pVtxBufferBall != NULL)
	{
		g_pVtxBufferBall->Release();
		g_pVtxBufferBall = NULL;
	}

	if (g_pTextureBall != NULL)
	{
		g_pTextureBall->Release();
		g_pTextureBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateBall(void)
{
	//for test
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vBallPos;

	g_pVtxBufferBall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		vBallPos = g_aBall[nCnt].pBody->cBody.GetPosCenter();

		//pos
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);
	}

	g_pVtxBufferBall->Unlock();

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			g_aBall[nCnt].pBody->Update();
		}
	}

	//衝突検出
	UpdateBallContact();

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			//check goal
			g_aBall[nCnt].bUsed = CheckGoal(g_aBall[nCnt].pBody->cBody.GetPosCenter());

			//ゴールに辿り着いたらUIの動画カウンターを設定
			if (!g_aBall[nCnt].bUsed)
			{
				SetUIBallCnt(nCnt);

				if (nCnt == 8)//nine
				{
					for (int nCntOther = 0;nCntOther < nCnt;nCntOther++)
					{
						if (g_aBall[nCntOther].bUsed)
						{
							//game over
							SetFade(FADE_OUT, MODE_ENDING);
							return;
						}
					}
					
					//game clear
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferBall,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureBall);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			//プリミティブ描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCnt * 4,//オフセット（頂点数）
				NUM_POLYGON);
		}
	}
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BALL,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;
	D3DXVECTOR3 vBallPos;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferBall->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//ball color
	D3DXCOLOR aBallColor[9] = {
		D3DXCOLOR(0.99f,0.96f,0.01f,1.0f),//one
		D3DXCOLOR(0.01f,0.29f,0.99f,1.0f),//two
		D3DXCOLOR(0.99f,0.09f,0.01f,1.0f),//three
		D3DXCOLOR(0.58f,0.01f,0.67f,1.0f),//four
		D3DXCOLOR(0.99f,0.4f,0.01f,1.0f),//five
		D3DXCOLOR(0.04f,0.99f,0.01f,1.0f),//six
		D3DXCOLOR(0.64f,0.12f,0.01f,1.0f),//seven
		D3DXCOLOR(0.1f,0.1f,0.1f,1.0f),//eight
		D3DXCOLOR(0.99f,0.99f,0.4f,1.0f)//nine
	};

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		vBallPos = g_aBall[nCnt].pBody->cBody.GetPosCenter();
		//頂点座標の設定（2D座標、右回り）
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[nCnt * 4 + 0].color = aBallColor[nCnt];
		pVtx[nCnt * 4 + 1].color = aBallColor[nCnt];
		pVtx[nCnt * 4 + 2].color = aBallColor[nCnt];
		pVtx[nCnt * 4 + 3].color = aBallColor[nCnt];

		//texture頂点
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//仮想アドレス解放
	g_pVtxBufferBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  ボール衝突検出
//--------------------------------------------------------------------------------
void UpdateBallContact(void)
{
	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			//check table contact
			CBodyQuadrangle *pTable = NULL;

			for (int nCntTable = 0;nCntTable < NUM_TABLE;nCntTable++)
			{
				pTable = GetTable(nCntTable);
				CheckContactCTQ(g_aBall[nCnt].pBody, pTable);
				pTable = NULL;
			}

			//check area contact
			AreaContact(&g_aBall[nCnt].pBody->cBody, g_aBall[nCnt].pBody->GetRadius());
		}
	}

	//check ball contact
	for (int nCnt = 0; nCnt < NUM_BALL - 1; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			for (int nCntTarget = nCnt + 1; nCntTarget < NUM_BALL; nCntTarget++)
			{
				if (g_aBall[nCntTarget].bUsed)
				{
					CheckContactCTC(g_aBall[nCnt].pBody, g_aBall[nCntTarget].pBody);
				}

			}
		}
	}
}

//--------------------------------------------------------------------------------
//  敵アドレス取得
//--------------------------------------------------------------------------------
CBodyCicle *GetBall(int nNumBall)
{
	return g_aBall[nNumBall].pBody;
}

//--------------------------------------------------------------------------------
//  ボールの位置取得
//--------------------------------------------------------------------------------
D3DXVECTOR3 GetBallPos(int nNumBall)
{
	return g_aBall[nNumBall].pBody->cBody.GetPosCenter();
}

//--------------------------------------------------------------------------------
//  ボールのFLAG取得
//--------------------------------------------------------------------------------
bool GetBallUsing(int nNumBall)
{
	return g_aBall[nNumBall].bUsed;
}

//--------------------------------------------------------------------------------
//  ボールの速度取得
//--------------------------------------------------------------------------------
D3DXVECTOR3 GetBallVelocity(int nNumBall)
{
	return g_aBall[nNumBall].pBody->cBody.GetVelocity();
}