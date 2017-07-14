//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "ball.h"
#include "UI_ball.h"
#include "goal.h"
#include "table.h"
#include "fade.h"
#include "KF_Precision.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define BALL_RADIUS (5.75f * 0.5f * PX_PER_CM)//�{�[���̔��a
#define BALL_MASS (1.0f)//�{�[���̏d��
#define BALL_TEXTURENAME "data/TEXTURE/ball.png"//�t�@�C����
#define BALL_POS (D3DXVECTOR3(238.0f,365.0f,0.0f))//�{�[���̊J�n�ʒu

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct {
	CBodyCicle *pBody;
	bool bUsed;
}BALL;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBall(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBall = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBall = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
BALL g_aBall[NUM_BALL];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		g_aBall[nCnt].bUsed = true;
		g_aBall[nCnt].pBody = new CBodyCicle;
		g_aBall[nCnt].pBody->Init();
		g_aBall[nCnt].pBody->SetRadius(BALL_RADIUS);
		g_aBall[nCnt].pBody->cBody.SetMass(BALL_MASS);
		g_aBall[nCnt].pBody->cBody.SetGravity(GRAVITY);
	}

	//set ball pos
	g_aBall[0].pBody->cBody.SetPosCenter(BALL_POS);//one
	g_aBall[1].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 2.0f * sinf(D3DX_PI / 3), -BALL_RADIUS, 0.0f));//two
	g_aBall[2].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 2.0f * sinf(D3DX_PI / 3), BALL_RADIUS, 0.0f));//three
	g_aBall[3].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 4.0f * sinf(D3DX_PI / 3), -BALL_RADIUS * 2.0f, 0.0f));//four
	g_aBall[4].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 4.0f * sinf(D3DX_PI / 3), BALL_RADIUS * 2.0f, 0.0f));//five
	g_aBall[5].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 6.0f * sinf(D3DX_PI / 3), -BALL_RADIUS, 0.0f));//six
	g_aBall[6].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 6.0f * sinf(D3DX_PI / 3), BALL_RADIUS, 0.0f));//seven
	g_aBall[7].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 8.0f * sinf(D3DX_PI / 3), 0.0f, 0.0f));//eight
	g_aBall[8].pBody->cBody.SetPosCenter(BALL_POS + D3DXVECTOR3(-BALL_RADIUS * 4.0f * sinf(D3DX_PI / 3), 0.0f, 0.0f));//nine

	if (FAILED(MakeVerTexBall(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		BALL_TEXTURENAME,
		&g_pTextureBall)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitBall(void)
{
	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].pBody != NULL)//�A�h���X�̒��g�m�F
		{
			g_aBall[nCnt].pBody->Uninit();//�I������
			delete g_aBall[nCnt].pBody;//���g���폜
			g_aBall[nCnt].pBody = NULL;//�A�h���X����ɂ���
		}
	}

	if (g_pVtxBufferBall != NULL)
	{
		g_pVtxBufferBall->Release();
		g_pVtxBufferBall = NULL;
	}

	if (g_pTextureBall != NULL)
	{
		g_pTextureBall->Release();
		g_pTextureBall = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateBall(void)
{
	//for test
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vBallPos;

	g_pVtxBufferBall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		vBallPos = g_aBall[nCnt].pBody->cBody.GetPosCenter();

		//pos
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);
	}

	g_pVtxBufferBall->Unlock();

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			g_aBall[nCnt].pBody->Update();
		}
	}

	//�Փˌ��o
	UpdateBallContact();

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			//check goal
			g_aBall[nCnt].bUsed = CheckGoal(g_aBall[nCnt].pBody->cBody.GetPosCenter());

			//�S�[���ɒH�蒅������UI�̓���J�E���^�[��ݒ�
			if (!g_aBall[nCnt].bUsed)
			{
				SetUIBallCnt(nCnt);

				if (nCnt == 8)//nine
				{
					for (int nCntOther = 0;nCntOther < nCnt;nCntOther++)
					{
						if (g_aBall[nCntOther].bUsed)
						{
							//game over
							SetFade(FADE_OUT, MODE_ENDING);
							return;
						}
					}
					
					//game clear
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferBall,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureBall);

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
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
HRESULT MakeVerTexBall(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BALL,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferBall,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;
	D3DXVECTOR3 vBallPos;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferBall->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//ball color
	D3DXCOLOR aBallColor[9] = {
		D3DXCOLOR(0.99f,0.96f,0.01f,1.0f),//one
		D3DXCOLOR(0.01f,0.29f,0.99f,1.0f),//two
		D3DXCOLOR(0.99f,0.09f,0.01f,1.0f),//three
		D3DXCOLOR(0.58f,0.01f,0.67f,1.0f),//four
		D3DXCOLOR(0.99f,0.4f,0.01f,1.0f),//five
		D3DXCOLOR(0.04f,0.99f,0.01f,1.0f),//six
		D3DXCOLOR(0.64f,0.12f,0.01f,1.0f),//seven
		D3DXCOLOR(0.1f,0.1f,0.1f,1.0f),//eight
		D3DXCOLOR(0.99f,0.99f,0.4f,1.0f)//nine
	};

	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		vBallPos = g_aBall[nCnt].pBody->cBody.GetPosCenter();
		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y - BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vBallPos.x - BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vBallPos.x + BALL_RADIUS, vBallPos.y + BALL_RADIUS, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[nCnt * 4 + 0].color = aBallColor[nCnt];
		pVtx[nCnt * 4 + 1].color = aBallColor[nCnt];
		pVtx[nCnt * 4 + 2].color = aBallColor[nCnt];
		pVtx[nCnt * 4 + 3].color = aBallColor[nCnt];

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferBall->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �{�[���Փˌ��o
//--------------------------------------------------------------------------------
void UpdateBallContact(void)
{
	for (int nCnt = 0; nCnt < NUM_BALL; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			//check table contact
			CBodyQuadrangle *pTable = NULL;

			for (int nCntTable = 0;nCntTable < NUM_TABLE;nCntTable++)
			{
				pTable = GetTable(nCntTable);
				CheckContactCTQ(g_aBall[nCnt].pBody, pTable);
				pTable = NULL;
			}

			//check area contact
			AreaContact(&g_aBall[nCnt].pBody->cBody, g_aBall[nCnt].pBody->GetRadius());
		}
	}

	//check ball contact
	for (int nCnt = 0; nCnt < NUM_BALL - 1; nCnt++)
	{
		if (g_aBall[nCnt].bUsed)
		{
			for (int nCntTarget = nCnt + 1; nCntTarget < NUM_BALL; nCntTarget++)
			{
				if (g_aBall[nCntTarget].bUsed)
				{
					CheckContactCTC(g_aBall[nCnt].pBody, g_aBall[nCntTarget].pBody);
				}

			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �G�A�h���X�擾
//--------------------------------------------------------------------------------
CBodyCicle *GetBall(int nNumBall)
{
	return g_aBall[nNumBall].pBody;
}

//--------------------------------------------------------------------------------
//  �{�[���̈ʒu�擾
//--------------------------------------------------------------------------------
D3DXVECTOR3 GetBallPos(int nNumBall)
{
	return g_aBall[nNumBall].pBody->cBody.GetPosCenter();
}

//--------------------------------------------------------------------------------
//  �{�[����FLAG�擾
//--------------------------------------------------------------------------------
bool GetBallUsing(int nNumBall)
{
	return g_aBall[nNumBall].bUsed;
}

//--------------------------------------------------------------------------------
//  �{�[���̑��x�擾
//--------------------------------------------------------------------------------
D3DXVECTOR3 GetBallVelocity(int nNumBall)
{
	return g_aBall[nNumBall].pBody->cBody.GetVelocity();
}