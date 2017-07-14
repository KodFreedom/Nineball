//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
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
//  �萔��`
//--------------------------------------------------------------------------------
#define SHAODW_RADIUS (8.0f * 0.5f * PX_PER_CM)//�{�[���̔��a
#define SHAODW_TEXTURENAME "data/TEXTURE/shadow.png"//�t�@�C����
#define SHADOW_NUM (10)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexShadow(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
bool g_abShadow[SHADOW_NUM];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		g_abShadow[nCnt] = true;
	}

	if (FAILED(MakeVerTexShadow(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		SHAODW_TEXTURENAME,
		&g_pTextureShadow)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
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
//  �X�V����
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
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferShadow,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

						   //���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureShadow);

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		if (g_abShadow[nCnt])
		{
			//�v���~�e�B�u�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCnt * 4,//�I�t�Z�b�g�i���_���j
				NUM_POLYGON);
		}
	}
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * SHADOW_NUM,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferShadow,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;
	D3DXVECTOR3 vShadowPos;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferShadow->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (int nCnt = 0; nCnt < SHADOW_NUM; nCnt++)
	{
		
		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferShadow->Unlock();

	return S_OK;
}
