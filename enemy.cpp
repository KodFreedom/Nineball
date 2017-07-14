//--------------------------------------------------------------------------------
//
//　ウインドウ表示プログラム
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "enemy.h"
#include "KF_Precision.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define ENEMY_WIDTH (30.0f)//ポリゴンの幅
#define ENEMY_HEIGHT (30.0f)//ポリゴンの高さ
#define ENEMY_TEXTURENAME "data/TEXTURE/bullet.png"//ファイル名
#define ENEMY_POS_X (150.0f)//ポリゴンの表示位置X
#define ENEMY_POS_Y (SCREEN_HEIGHT * 0.5f)//ポリゴンの表示位置Y
#define ENEMY_ANGLE (atan2f(ENEMY_HEIGHT, ENEMY_WIDTH))
#define MOVE_SPEED_X (D3DXVECTOR3(1.0f,0.0f,0.0f))//四方向移動速度
#define MOVE_SPEED_Y (D3DXVECTOR3(0.0f,1.0f,0.0f))//四方向移動速度

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//頂点バッファ管理インターフェースポインタ
CBodyCicle *g_apEnemy[NUM_ENEMY] = {};

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	int nCnt = 0;
	
	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		g_apEnemy[nCnt] = new CBodyCicle;
		g_apEnemy[nCnt]->Init();
		g_apEnemy[nCnt]->SetRadius(ENEMY_HEIGHT * 0.5f);
		g_apEnemy[nCnt]->cBody.SetPosCenter(D3DXVECTOR3(ENEMY_POS_X + nCnt * 30.0f, ENEMY_POS_Y - nCnt * 30.0f, 0.0f));
		g_apEnemy[nCnt]->cBody.SetGravityCenter(D3DXVECTOR3(ENEMY_POS_X, ENEMY_POS_Y, 0.0f));
		g_apEnemy[nCnt]->cBody.SetMass(10.0f * nCnt + 10.0f);
		g_apEnemy[nCnt]->cBody.SetGravity(GRAVITY);
		g_apEnemy[nCnt]->cBody.SetAcceleration(D3DXVECTOR3(1.0f * nCnt + 10.0f, 5.0f, 0.0f));
	}

	if (FAILED(MakeVerTexEnemy(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		ENEMY_TEXTURENAME,
		&g_pTextureEnemy)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		if (g_apEnemy[nCnt] != NULL)//アドレスの中身確認
		{
			g_apEnemy[nCnt]->Uninit();//終了処理
			delete g_apEnemy[nCnt];//中身を削除
			g_apEnemy[nCnt] = NULL;//アドレスを空にする
		}
	}

	if (g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	//for test
	VERTEX_2D *pVtx = NULL;
	int nCnt = 0;
	float fLength = 0.0f;
	float fRotation = 0.0f;
	D3DXVECTOR3 vEnemyPos;

	g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		vEnemyPos = g_apEnemy[nCnt]->cBody.GetPosCenter();
		fRotation = g_apEnemy[nCnt]->cBody.GetRotation();
		fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) * 0.5f;

		//pos
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + ENEMY_ANGLE) * fLength, 0.0f);
	}
	
	g_pVtxBufferEnemy->Unlock();

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		g_apEnemy[nCnt]->Update();
		AreaContact(&g_apEnemy[nCnt]->cBody, g_apEnemy[nCnt]->GetRadius());
	}

	//check contact
	int nCntTarget;
	for (nCnt = 0; nCnt < NUM_ENEMY - 1; nCnt++)
	{
		for (nCntTarget = nCnt + 1; nCntTarget < NUM_ENEMY; nCntTarget++)
		{
			CheckContactCTC(g_apEnemy[nCnt], g_apEnemy[nCntTarget]);
		}
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferEnemy,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureEnemy);

	for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
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
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_ENEMY,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferEnemy,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;
	int nCnt = 0;
	float fLength = 0.0f;
	float fRotation = 0.0f;
	D3DXVECTOR3 vEnemyPos;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferEnemy->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		vEnemyPos = g_apEnemy[nCnt]->cBody.GetPosCenter();
		fRotation = g_apEnemy[nCnt]->cBody.GetRotation();
		fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) * 0.5f;

		//頂点座標の設定（2D座標、右回り）
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + ENEMY_ANGLE) * fLength, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//texture頂点
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//仮想アドレス解放
	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  敵アドレス取得
//--------------------------------------------------------------------------------
//void GetEnemy(CBodyCicle *pEnemy, int nNumEnemy)
//{
//	pEnemy = g_apEnemy[nNumEnemy];
//}

CBodyCicle *GetEnemy(int nNumEnemy)
{
	return g_apEnemy[nNumEnemy];
}