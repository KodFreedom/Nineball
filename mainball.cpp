//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "mainball.h"
#include "ball.h"
#include "table.h"
#include "goal.h"
#include "player.h"
#include "KF_Precision.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define MAIN_BALL_MASS (1.0f)//�{�[���̏d��
#define MAIN_BALL_TEXTURENAME "data/TEXTURE/ball.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexMainBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureMainBall = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMainBall = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
CBodyCicle *g_pMainBall = NULL;
int g_nCntMainBall;

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitMainBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	g_pMainBall = new CBodyCicle;
	g_pMainBall->Init();
	g_pMainBall->SetRadius(MAIN_BALL_RADIUS);
	g_pMainBall->cBody.SetPosCenter(MAIN_BALL_POS);
	g_pMainBall->cBody.SetMass(MAIN_BALL_MASS);
	g_pMainBall->cBody.SetGravity(GRAVITY);

	g_nCntMainBall = 0;

	if (FAILED(MakeVerTexMainBall(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		MAIN_BALL_TEXTURENAME,
		&g_pTextureMainBall)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitMainBall(void)
{
	if (g_pMainBall != NULL)//�A�h���X�̒��g�m�F
	{
		g_pMainBall->Uninit();//�I������
		delete g_pMainBall;//���g���폜
		g_pMainBall = NULL;//�A�h���X����ɂ���
	}

	if (g_pVtxBufferMainBall != NULL)
	{
		g_pVtxBufferMainBall->Release();
		g_pVtxBufferMainBall = NULL;
	}

	if (g_pTextureMainBall != NULL)
	{
		g_pTextureMainBall->Release();
		g_pTextureMainBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateMainBall(void)
{
	if (g_nCntMainBall != 0)
	{
		g_nCntMainBall--;
		return;
	}

	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vBallPos;

	g_pVtxBufferMainBall->Lock(0, 0, (void**)&pVtx, 0);

	vBallPos = g_pMainBall->cBody.GetPosCenter();

	//pos
	pVtx[0].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);

	g_pVtxBufferMainBall->Unlock();

	g_pMainBall->Update();

	UpdateMainBallContact();

	if (CheckGoal(vBallPos) == false)
	{
		g_nCntMainBall = 300;
		g_pMainBall->cBody.SetPosCenter(MAIN_BALL_POS);
		g_pMainBall->cBody.SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		g_pMainBall->cBody.SetAcceleration(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		ReducePlayerLife();
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawMainBall(void)
{
	if (g_nCntMainBall != 0) { return; }

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferMainBall,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureMainBall);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexMainBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferMainBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;
	D3DXVECTOR3 vBallPos;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferMainBall->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	vBallPos = g_pMainBall->cBody.GetPosCenter();
	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y - MAIN_BALL_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(vBallPos.x - MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(vBallPos.x + MAIN_BALL_RADIUS, vBallPos.y + MAIN_BALL_RADIUS, 0.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	pVtx[1].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	pVtx[2].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	pVtx[3].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	//texture���_
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferMainBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �{�[���Փˌ��o
//--------------------------------------------------------------------------------
void UpdateMainBallContact(void)
{
	//check table contact
	CBodyQuadrangle *pTable = NULL;

	for (int nCntTable = 0;nCntTable < NUM_TABLE;nCntTable++)
	{
		pTable = GetTable(nCntTable);
		CheckContactCTQ(g_pMainBall, pTable);
		pTable = NULL;
	}

	AreaContact(&g_pMainBall->cBody, g_pMainBall->GetRadius());

	//check contact
	CBodyCicle *pBall = NULL;

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (!GetBallUsing(nCnt)) { continue; }

		pBall = GetBall(nCnt);
		CheckContactCTC(pBall, g_pMainBall);
		pBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �G�A�h���X�擾
//--------------------------------------------------------------------------------
CBodyCicle *GetMainBall(void)
{
	return g_pMainBall;
}

D3DXVECTOR3 GetMainBallPos(void)
{
	return g_pMainBall->cBody.GetPosCenter();
}

D3DXVECTOR3 GetMainBallVelocity(void)
{
	return g_pMainBall->cBody.GetVelocity();
}

int GetMainBallCnt(void)
{
	return g_nCntMainBall;
}