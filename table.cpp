//--------------------------------------------------------------------------------
//
//　Table.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-07
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "table.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define TABLE_POS_X (-0.5f)//ポリゴンの表示位置X
#define TABLE_POS_Y (-0.5f)//ポリゴンの表示位置Y
#define TABLE_TEXTURENAME "data/TEXTURE/biriyard.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTable(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTable = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTable = NULL;//頂点バッファ管理インターフェースポインタ
CBodyQuadrangle *g_apTable[6] = {};

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitTable(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0;nCnt < NUM_TABLE;nCnt++)
	{
		g_apTable[nCnt] = new CBodyQuadrangle;
		g_apTable[nCnt]->Init();
		g_apTable[nCnt]->cBody.SetInverseMass(0.0f);
	}

	//set table
	g_apTable[0]->SetVertex(0, D3DXVECTOR3(17.0f, 173.0f, 0.0f));
	g_apTable[0]->SetVertex(1, D3DXVECTOR3(77.0f, 233.0f, 0.0f));
	g_apTable[0]->SetVertex(2, D3DXVECTOR3(77.0f, 498.0f, 0.0f));
	g_apTable[0]->SetVertex(3, D3DXVECTOR3(17.0f, 559.0f, 0.0f));

	g_apTable[1]->SetVertex(0, D3DXVECTOR3(47.0f, 146.0f, 0.0f));
	g_apTable[1]->SetVertex(1, D3DXVECTOR3(379.0f, 146.0f, 0.0f));
	g_apTable[1]->SetVertex(2, D3DXVECTOR3(379.0f, 204.0f, 0.0f));
	g_apTable[1]->SetVertex(3, D3DXVECTOR3(106.0f, 204.0f, 0.0f));

	g_apTable[2]->SetVertex(0, D3DXVECTOR3(421.0f, 146.0f, 0.0f));
	g_apTable[2]->SetVertex(1, D3DXVECTOR3(751.0f, 146.0f, 0.0f));
	g_apTable[2]->SetVertex(2, D3DXVECTOR3(694.0f, 204.0f, 0.0f));
	g_apTable[2]->SetVertex(3, D3DXVECTOR3(421.0f, 204.0f, 0.0f));

	g_apTable[3]->SetVertex(0, D3DXVECTOR3(723.0f, 233.0f, 0.0f));
	g_apTable[3]->SetVertex(1, D3DXVECTOR3(783.0f, 172.0f, 0.0f));
	g_apTable[3]->SetVertex(2, D3DXVECTOR3(783.0f, 557.0f, 0.0f));
	g_apTable[3]->SetVertex(3, D3DXVECTOR3(723.0f, 498.0f, 0.0f));

	g_apTable[4]->SetVertex(0, D3DXVECTOR3(421.0f, 527.0f, 0.0f));
	g_apTable[4]->SetVertex(1, D3DXVECTOR3(694.0f, 527.0f, 0.0f));
	g_apTable[4]->SetVertex(2, D3DXVECTOR3(757.0f, 589.0f, 0.0f));
	g_apTable[4]->SetVertex(3, D3DXVECTOR3(421.0f, 589.0f, 0.0f));

	g_apTable[5]->SetVertex(0, D3DXVECTOR3(106.0f, 527.0f, 0.0f));
	g_apTable[5]->SetVertex(1, D3DXVECTOR3(379.0f, 527.0f, 0.0f));
	g_apTable[5]->SetVertex(2, D3DXVECTOR3(379.0f, 589.0f, 0.0f));
	g_apTable[5]->SetVertex(3, D3DXVECTOR3(45.0f, 589.0f, 0.0f));

	if (FAILED(MakeVerTexTable(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexTable ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TABLE_TEXTURENAME, &g_pTextureTable)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitTable(void)
{
	for (int nCnt = 0;nCnt < NUM_TABLE;nCnt++)
	{
		if (g_apTable[nCnt] != NULL)
		{
			g_apTable[nCnt]->Uninit();
			delete g_apTable[nCnt];
			g_apTable[nCnt] = NULL;
		}
	}

	//safe release
	if (g_pVtxBufferTable != NULL)
	{
		g_pVtxBufferTable->Release();
		g_pVtxBufferTable = NULL;
	}

	if (g_pTextureTable != NULL)
	{
		g_pTextureTable->Release();
		g_pTextureTable = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateTable(void)
{
	for (int nCnt = 0;nCnt < NUM_TABLE;nCnt++)
	{
		g_apTable[nCnt]->Update();
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawTable(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferTable,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//Textureの設定
	pDevice->SetTexture(0, g_pTextureTable);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);

}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTable(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferTable,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferTable->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(TABLE_POS_X, TABLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TABLE_POS_X + SCREEN_WIDTH, TABLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TABLE_POS_X, TABLE_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TABLE_POS_X + SCREEN_WIDTH, TABLE_POS_Y + SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBufferTable->Unlock();

	return S_OK;
}

CBodyQuadrangle *GetTable(int nNumTable)
{
	if (nNumTable < 0 || nNumTable >= NUM_TABLE) { return NULL; }

	return g_apTable[nNumTable];
}