//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "UI_ball.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define UI_BALL_RADIUS (12.0f)//�{�[���̔��a
#define UI_BALL_TEXTURENAME "data/TEXTURE/circle.png"//�t�@�C����
#define UI_BALL_ALPHA_MAX (0.8f)
#define UI_BALL_COUNT_MAX (30)
#define NUM_BALL (9)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;//���S�_
	float fAlpha;//�A���t�@�l
	int nCnt;//����J�E���^�[
}UI_BALL;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIBall = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIBall = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
UI_BALL g_aUIBall[NUM_BALL];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitUIBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

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

	if (FAILED(MakeVerTexUIBall(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexUI ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		UI_BALL_TEXTURENAME,
		&g_pTextureUIBall)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateUITextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
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
//  �X�V����
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

		//���_�J���[�̐ݒ�
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
	}

	g_pVtxBufferUIBall->Unlock();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawUIBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferUIBall,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

						   //���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureUIBall);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		//�v���~�e�B�u�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCnt * 4,//�I�t�Z�b�g�i���_���j
			NUM_POLYGON);
	}
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BALL,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferUIBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferUIBall->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x - UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y - UI_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x + UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y - UI_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x - UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y + UI_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_aUIBall[nCnt].vPos.x + UI_BALL_RADIUS, g_aUIBall[nCnt].vPos.y + UI_BALL_RADIUS, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIBall[nCnt].fAlpha);

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferUIBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  UI Cnt�ݒ�
//--------------------------------------------------------------------------------
void SetUIBallCnt(int nNumBall)
{
	g_aUIBall[nNumBall].nCnt = UI_BALL_COUNT_MAX;
}