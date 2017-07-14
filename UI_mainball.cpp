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
#include "UI_mainball.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define UI_MAIN_BALL_RADIUS (13.0f)//�{�[���̔��a
#define UI_MAIN_BALL_TEXTURENAME "data/TEXTURE/circle.png"//�t�@�C����
#define UI_MAIN_BALL_ALPHA_MAX (1.0f)
#define UI_MAIN_BALL_COUNT_MAX (30)
#define NUM_LIFE (3)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;//���S�_
	float fAlpha;//�A���t�@�l
	int nCnt;//����J�E���^�[
}UI_MAIN_BALL;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIMainBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIMainBall = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIMainBall = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
UI_MAIN_BALL g_aUIMainBall[NUM_LIFE];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitUIMainBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		g_aUIMainBall[nCnt].fAlpha = 0.0f;
		g_aUIMainBall[nCnt].nCnt = 0;
	}

	//set ball pos
	g_aUIMainBall[0].vPos = D3DXVECTOR3(552.0f, 97.0f, 0.0f);//one
	g_aUIMainBall[1].vPos = D3DXVECTOR3(597.0f, 97.0f, 0.0f);//two
	g_aUIMainBall[2].vPos = D3DXVECTOR3(642.0f, 97.0f, 0.0f);//three

	if (FAILED(MakeVerTexUIMainBall(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexUI ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		UI_MAIN_BALL_TEXTURENAME,
		&g_pTextureUIMainBall)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateUITextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitUIMainBall(void)
{
	if (g_pVtxBufferUIMainBall != NULL)
	{
		g_pVtxBufferUIMainBall->Release();
		g_pVtxBufferUIMainBall = NULL;
	}

	if (g_pTextureUIMainBall != NULL)
	{
		g_pTextureUIMainBall->Release();
		g_pTextureUIMainBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateUIMainBall(void)
{
	VERTEX_2D *pVtx = NULL;

	g_pVtxBufferUIMainBall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		if (g_aUIMainBall[nCnt].nCnt != 0)
		{
			g_aUIMainBall[nCnt].nCnt--;
			g_aUIMainBall[nCnt].fAlpha = (float)(UI_MAIN_BALL_COUNT_MAX - g_aUIMainBall[nCnt].nCnt) / (float)UI_MAIN_BALL_COUNT_MAX * UI_MAIN_BALL_ALPHA_MAX;
		}

		//���_�J���[�̐ݒ�
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
	}

	g_pVtxBufferUIMainBall->Unlock();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawUIMainBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferUIMainBall,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

						   //���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureUIMainBall);

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
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
HRESULT MakeVerTexUIMainBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_LIFE,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferUIMainBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferUIMainBall->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x - UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y - UI_MAIN_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x + UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y - UI_MAIN_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x - UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y + UI_MAIN_BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_aUIMainBall[nCnt].vPos.x + UI_MAIN_BALL_RADIUS, g_aUIMainBall[nCnt].vPos.y + UI_MAIN_BALL_RADIUS, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(0.1f, 0.1f, 0.1f, g_aUIMainBall[nCnt].fAlpha);

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferUIMainBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  UI Cnt�ݒ�
//--------------------------------------------------------------------------------
void SetUIMainBallCnt(int nNumBall)
{
	g_aUIMainBall[nNumBall].nCnt = UI_MAIN_BALL_COUNT_MAX;
}