//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "input.h"
#include "player.h"
#include "fade.h"
#include "mainball.h"
#include "UI_mainball.h"
#include "sound.h"
#include "KF_Precision.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define PLAYER_WIDTH (300.0f)//�|���S���̕�
#define PLAYER_HEIGHT (8.0f)//�|���S���̍���
#define PLAYER_TEXTURENAME "data/TEXTURE/stick.png"//�t�@�C����
#define DISTANCE_MAX (100.0f)
#define FORCE_TO_BALL_MAX (-50.0f)
#define STRENGTH_ADD_SPEED (0.01f)
#define STRENGTH_REDUCE_SPEED (-0.2f)
#define ROTATION_SPEED (D3DX_PI / 180.0f)//��]���x

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct {
	LPDIRECT3DTEXTURE9 pTexture;//texture�C���^�[�t�F�[�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVecToPos[4];
	D3DXVECTOR3 vDistanceToMainBall;//�͂ɂ���ċ������傫���Ȃ�
	int nLife;
	float fRotation;
	float fPushStrength;
	float fStrength;//�{�[����ł�
	bool bPress;
	bool bPush;
	bool bTurn;
}PLAYER;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
PLAYER g_player;

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	g_player.pTexture = NULL;
	g_player.pVtxBuffer = NULL;

	g_player.vDistanceToMainBall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.nLife = 3;
	g_player.fStrength = 0.0f;
	g_player.fPushStrength = 0.0f;
	g_player.fRotation = 0.0f;
	g_player.vPos = MAIN_BALL_POS;
	g_player.bTurn = true;
	g_player.bPush = false;
	g_player.bPress = false;

	g_player.vVecToPos[0] = D3DXVECTOR3(MAIN_BALL_RADIUS, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[1] = D3DXVECTOR3(MAIN_BALL_RADIUS + PLAYER_WIDTH * 0.5f, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[2] = D3DXVECTOR3(MAIN_BALL_RADIUS, PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[3] = D3DXVECTOR3(MAIN_BALL_RADIUS + PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT * 0.5f, 0.0f);


	if (FAILED(MakeVerTexPlayer(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEXTURENAME,
		&g_player.pTexture)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitPlayer(void)
{
	if (g_player.pVtxBuffer != NULL)
	{
		g_player.pVtxBuffer->Release();
		g_player.pVtxBuffer = NULL;
	}

	if (g_player.pTexture != NULL)
	{
		g_player.pTexture->Release();
		g_player.pTexture = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vVertex;
	g_player.vPos = GetMainBallPos();
	g_player.vDistanceToMainBall = D3DXVECTOR3(DISTANCE_MAX * cosf(g_player.fRotation) * g_player.fStrength,
		DISTANCE_MAX * sinf(g_player.fRotation) * g_player.fStrength,
		0.0f);
	g_player.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//pos
	for (int nCnt = 0;nCnt < 4;nCnt++)
	{
		vVertex = g_player.vVecToPos[nCnt];

		//AFFIN�]��
		vVertex = D3DXVECTOR3(
			g_player.vVecToPos[nCnt].x * cosf(g_player.fRotation) - g_player.vVecToPos[nCnt].y * sinf(g_player.fRotation),
			g_player.vVecToPos[nCnt].x * sinf(g_player.fRotation) + g_player.vVecToPos[nCnt].y * cosf(g_player.fRotation),
			0.0f);

		pVtx[nCnt].pos = g_player.vPos + vVertex + g_player.vDistanceToMainBall;
	}

	//texture
	g_player.pVtxBuffer->Unlock();

	if (g_player.bTurn)
	{
		//key����
		if (GetKeyboardPress(DIK_UP))//
		{
			g_player.fStrength += STRENGTH_ADD_SPEED;
			g_player.fStrength = g_player.fStrength >= 1.0f ? 1.0f : g_player.fStrength;
			g_player.fPushStrength = g_player.fStrength;
			g_player.bPress = true;
		}
		else
		{
			g_player.bPress = false;
			g_player.bTurn = false;
		}

		if (GetKeyboardPress(DIK_LEFT))//����]
		{
			g_player.fRotation -= ROTATION_SPEED;
		}

		if (GetKeyboardPress(DIK_RIGHT))//�E��]
		{
			g_player.fRotation += ROTATION_SPEED;
		}

		if (g_player.fStrength > 0.0f && g_player.bPress == false)
		{
			g_player.bPush = true;
		}
	}

	if (g_player.bPush == true)
	{
		g_player.fStrength += STRENGTH_REDUCE_SPEED;
		g_player.fStrength = g_player.fStrength <= 0.0f ? 0.0f : g_player.fStrength;

		if (g_player.fStrength == 0.0f)
		{
			CBodyCicle *pMainBall = GetMainBall();
			pMainBall->cBody.AddForce(D3DXVECTOR3(FORCE_TO_BALL_MAX * cosf(g_player.fRotation) * g_player.fPushStrength,
				FORCE_TO_BALL_MAX * sinf(g_player.fRotation) * g_player.fPushStrength,
				0.0f));

			g_player.fPushStrength = 0.0f;
			g_player.bPush = false;
			PlaySound(SOUND_LABEL_SE_SHOT);
		}
	}
	
	
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	//player
	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_player.pVtxBuffer,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_player.pTexture);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	//player
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_player.pVtxBuffer,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_player.pVtxBuffer->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = g_player.vPos + g_player.vVecToPos[0];
	pVtx[1].pos = g_player.vPos + g_player.vVecToPos[1];
	pVtx[2].pos = g_player.vPos + g_player.vVecToPos[2];
	pVtx[3].pos = g_player.vPos + g_player.vVecToPos[3];


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
	g_player.pVtxBuffer->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  flag get
//--------------------------------------------------------------------------------
bool *GetPlayerTurnFlag(void)
{
	return &g_player.bTurn;
}

//--------------------------------------------------------------------------------
//  get strength
//--------------------------------------------------------------------------------
float GetPlayerStrength(void)
{
	return g_player.fStrength;
}

void ReducePlayerLife(void)
{
	g_player.nLife--;

	SetUIMainBallCnt(g_player.nLife);

	if (g_player.nLife == 0)
	{
		//game over
		SetFade(FADE_OUT, MODE_ENDING);
	}
}

float GetPlayerRotation(void)
{
	return g_player.fRotation;
}