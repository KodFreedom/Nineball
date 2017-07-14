//--------------------------------------------------------------------------------
//
//�@Table.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-07
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "table.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define TABLE_POS_X (-0.5f)//�|���S���̕\���ʒuX
#define TABLE_POS_Y (-0.5f)//�|���S���̕\���ʒuY
#define TABLE_TEXTURENAME "data/TEXTURE/biriyard.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTable(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTable = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTable = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
CBodyQuadrangle *g_apTable[6] = {};

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitTable(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

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

	if (FAILED(MakeVerTexTable(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexTable ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TABLE_TEXTURENAME, &g_pTextureTable)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
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
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateTable(void)
{
	for (int nCnt = 0;nCnt < NUM_TABLE;nCnt++)
	{
		g_apTable[nCnt]->Update();
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawTable(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferTable,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureTable);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);

}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTable(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferTable,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTable->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(TABLE_POS_X, TABLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TABLE_POS_X + SCREEN_WIDTH, TABLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TABLE_POS_X, TABLE_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TABLE_POS_X + SCREEN_WIDTH, TABLE_POS_Y + SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//texture���_
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferTable->Unlock();

	return S_OK;
}

CBodyQuadrangle *GetTable(int nNumTable)
{
	if (nNumTable < 0 || nNumTable >= NUM_TABLE) { return NULL; }

	return g_apTable[nNumTable];
}