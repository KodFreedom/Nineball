//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "enemy.h"
#include "KF_Precision.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define ENEMY_WIDTH (30.0f)//�|���S���̕�
#define ENEMY_HEIGHT (30.0f)//�|���S���̍���
#define ENEMY_TEXTURENAME "data/TEXTURE/bullet.png"//�t�@�C����
#define ENEMY_POS_X (150.0f)//�|���S���̕\���ʒuX
#define ENEMY_POS_Y (SCREEN_HEIGHT * 0.5f)//�|���S���̕\���ʒuY
#define ENEMY_ANGLE (atan2f(ENEMY_HEIGHT, ENEMY_WIDTH))
#define MOVE_SPEED_X (D3DXVECTOR3(1.0f,0.0f,0.0f))//�l�����ړ����x
#define MOVE_SPEED_Y (D3DXVECTOR3(0.0f,1.0f,0.0f))//�l�����ړ����x

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
CBodyCicle *g_apEnemy[NUM_ENEMY] = {};

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	int nCnt = 0;
	
	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		g_apEnemy[nCnt] = new CBodyCicle;
		g_apEnemy[nCnt]->Init();
		g_apEnemy[nCnt]->SetRadius(ENEMY_HEIGHT * 0.5f);
		g_apEnemy[nCnt]->cBody.SetPosCenter(D3DXVECTOR3(ENEMY_POS_X + nCnt * 30.0f, ENEMY_POS_Y - nCnt * 30.0f, 0.0f));
		g_apEnemy[nCnt]->cBody.SetGravityCenter(D3DXVECTOR3(ENEMY_POS_X, ENEMY_POS_Y, 0.0f));
		g_apEnemy[nCnt]->cBody.SetMass(10.0f * nCnt + 10.0f);
		g_apEnemy[nCnt]->cBody.SetGravity(GRAVITY);
		g_apEnemy[nCnt]->cBody.SetAcceleration(D3DXVECTOR3(1.0f * nCnt + 10.0f, 5.0f, 0.0f));
	}

	if (FAILED(MakeVerTexEnemy(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		ENEMY_TEXTURENAME,
		&g_pTextureEnemy)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		if (g_apEnemy[nCnt] != NULL)//�A�h���X�̒��g�m�F
		{
			g_apEnemy[nCnt]->Uninit();//�I������
			delete g_apEnemy[nCnt];//���g���폜
			g_apEnemy[nCnt] = NULL;//�A�h���X����ɂ���
		}
	}

	if (g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	//for test
	VERTEX_2D *pVtx = NULL;
	int nCnt = 0;
	float fLength = 0.0f;
	float fRotation = 0.0f;
	D3DXVECTOR3 vEnemyPos;

	g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		vEnemyPos = g_apEnemy[nCnt]->cBody.GetPosCenter();
		fRotation = g_apEnemy[nCnt]->cBody.GetRotation();
		fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) * 0.5f;

		//pos
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + ENEMY_ANGLE) * fLength, 0.0f);
	}
	
	g_pVtxBufferEnemy->Unlock();

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		g_apEnemy[nCnt]->Update();
		AreaContact(&g_apEnemy[nCnt]->cBody, g_apEnemy[nCnt]->GetRadius());
	}

	//check contact
	int nCntTarget;
	for (nCnt = 0; nCnt < NUM_ENEMY - 1; nCnt++)
	{
		for (nCntTarget = nCnt + 1; nCntTarget < NUM_ENEMY; nCntTarget++)
		{
			CheckContactCTC(g_apEnemy[nCnt], g_apEnemy[nCntTarget]);
		}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferEnemy,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureEnemy);

	for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
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
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_ENEMY,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferEnemy,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;
	int nCnt = 0;
	float fLength = 0.0f;
	float fRotation = 0.0f;
	D3DXVECTOR3 vEnemyPos;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferEnemy->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		vEnemyPos = g_apEnemy[nCnt]->cBody.GetPosCenter();
		fRotation = g_apEnemy[nCnt]->cBody.GetRotation();
		fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) * 0.5f;

		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + D3DX_PI + ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation - D3DX_PI - ENEMY_ANGLE) * fLength, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(vEnemyPos.x + cosf(fRotation + ENEMY_ANGLE) * fLength, vEnemyPos.y + sinf(fRotation + ENEMY_ANGLE) * fLength, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �G�A�h���X�擾
//--------------------------------------------------------------------------------
//void GetEnemy(CBodyCicle *pEnemy, int nNumEnemy)
//{
//	pEnemy = g_apEnemy[nNumEnemy];
//}

CBodyCicle *GetEnemy(int nNumEnemy)
{
	return g_apEnemy[nNumEnemy];
}