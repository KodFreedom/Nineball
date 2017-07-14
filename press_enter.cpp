//--------------------------------------------------------------------------------
//
//　Result.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "press_enter.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define PRESS_ENTER_POS_X (SCREEN_WIDTH * 0.5f)//ポリゴンの表示位置X
#define PRESS_ENTER_POS_Y (SCREEN_HEIGHT * 0.9f)//ポリゴンの表示位置Y
#define PRESS_ENTER_WIDTH (298.0f)//ポリゴンの表示位置Y
#define PRESS_ENTER_HEIGHT (45.0f)//ポリゴンの表示位置Y
#define PRESS_ENTER_TEXTURENAME "data/TEXTURE/press_enter.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPressEnter(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePressEnter = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPressEnter = NULL;//頂点バッファ管理インターフェースポインタ
float g_fAlpha;
float g_fChange;

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得
	g_fAlpha = 0.0f;
	g_fChange = 1.0f;

	if (FAILED(MakeVerTexPressEnter(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPressEnter ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, PRESS_ENTER_TEXTURENAME, &g_pTexturePressEnter)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitPressEnter(void)
{
	//safe release
	if (g_pVtxBufferPressEnter != NULL)
	{
		g_pVtxBufferPressEnter->Release();
		g_pVtxBufferPressEnter = NULL;
	}

	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdatePressEnter(void)
{
	g_fAlpha += 0.05f * g_fChange;

	if (g_fAlpha >= 1.0f)
	{
		g_fAlpha = 1.0f;
		g_fChange = -1.0f;
	}

	if (g_fAlpha <= 0.0f)
	{
		g_fAlpha = 0.0f;
		g_fChange = 1.0f;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[1].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[2].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[3].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);

	//仮想アドレス解放
	g_pVtxBufferPressEnter->Unlock();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferPressEnter,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTexturePressEnter);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPressEnter(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferPressEnter,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferPressEnter->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(PRESS_ENTER_POS_X - PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y - PRESS_ENTER_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PRESS_ENTER_POS_X + PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y - PRESS_ENTER_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PRESS_ENTER_POS_X - PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y + PRESS_ENTER_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PRESS_ENTER_POS_X + PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y + PRESS_ENTER_HEIGHT * 0.5f, 0.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[1].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[2].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[3].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);

	//texture頂点
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//仮想アドレス解放
	g_pVtxBufferPressEnter->Unlock();

	return S_OK;
}