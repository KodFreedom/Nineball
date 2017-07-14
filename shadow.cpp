//--------------------------------------------------------------------------------
//
//　ウインドウ表示プログラム
//	Author : Xu Wenjie
//	Date   : 2016-04-26SHAODW_
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "shadow.h"
#include "mainball.h"
#include "ball.h"
#include "main.h"
#include "KF_Precision.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define SHAODW_RADIUS (8.0f * 0.5f * PX_PER_CM)//ボールの半径
#define SHAODW_TEXTURENAME "data/TEXTURE/shadow.png"//ファイル名
#define SHADOW_NUM (10)

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexShadow(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL;//頂点バッファ管理インターフェースポインタ
bool g_abShadow[SHADOW_NUM];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		g_abShadow[nCnt] = true;
	}

	if (FAILED(MakeVerTexShadow(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		SHAODW_TEXTURENAME,
		&g_pTextureShadow)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitShadow(void)
{
	if (g_pVtxBufferShadow != NULL)
	{
		g_pVtxBufferShadow->Release();
		g_pVtxBufferShadow = NULL;
	}

	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateShadow(void)
{
	//for test
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vShadowPos;

	g_pVtxBufferShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		//mainball
		if (nCnt == 0)
		{
			if (GetMainBallCnt() != 0)
			{
				g_abShadow[nCnt] = false;
				continue;
			}
			g_abShadow[nCnt] = true;
			vShadowPos = GetMainBallPos();
		}
		else
		{
			if (!GetBallUsing(nCnt - 1)) 
			{
				g_abShadow[nCnt] = false;
				continue; 
			}

			g_abShadow[nCnt] = true;
			vShadowPos = GetBallPos(nCnt - 1);
		}

		if (g_abShadow[nCnt])
		{
			//pos
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vShadowPos.x - SHAODW_RADIUS, vShadowPos.y - SHAODW_RADIUS, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vShadowPos.x + SHAODW_RADIUS, vShadowPos.y - SHAODW_RADIUS, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vShadowPos.x - SHAODW_RADIUS, vShadowPos.y + SHAODW_RADIUS, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vShadowPos.x + SHAODW_RADIUS, vShadowPos.y + SHAODW_RADIUS, 0.0f);
		}
	}

	g_pVtxBufferShadow->Unlock();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferShadow,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

						   //頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureShadow);

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		if (g_abShadow[nCnt])
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
HRESULT MakeVerTexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * SHADOW_NUM,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferShadow,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;
	D3DXVECTOR3 vShadowPos;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferShadow->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		
		//頂点座標の設定（2D座標、右回り）
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//texture頂点
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//仮想アドレス解放
	g_pVtxBufferShadow->Unlock();

	return S_OK;
}
