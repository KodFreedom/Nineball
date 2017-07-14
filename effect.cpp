//--------------------------------------------------------------------------------
//
//�@effect
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "effect.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define EFFECT_TEXTURENAME "data/TEXTURE/circle.png"//�t�@�C����
#define EFFECT_ANGLE (D3DX_PI * 0.25f)
#define GOAL_EFFECT_NUM (10)
#define GOAL_EFFECT_ANGLE (D3DX_PI * 2.0f / GOAL_EFFECT_NUM)
#define GOAL_EFFECT_SPEED (6.0f)

#define COLOR_R (0.968f)
#define COLOR_G (0.313f)
#define COLOR_B (0.698f)
#define NUM_EFFECT (100)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct {
	D3DXVECTOR3 vPosCenter;//���S���W
	D3DXVECTOR3 vSpeed;//���x
	float fLength;//�Ίp������
	float fLengthSpeed;//�Ίp�������ϊ����x
	float fRotation;//�]��
	int nLife;//����
	bool bUsed;
}EFFECT;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexEffect(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEffect = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
EFFECT g_aEffect[NUM_EFFECT];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	//effect������
	for (int nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		g_aEffect[nCnt].vPosCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCnt].vSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCnt].fLength = 0.0f;
		g_aEffect[nCnt].fLengthSpeed = 0.0f;
		g_aEffect[nCnt].fRotation = 0.0f;
		g_aEffect[nCnt].nLife = 0;
		g_aEffect[nCnt].bUsed = false;
	}

	if (FAILED(MakeVerTexEffect(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		EFFECT_TEXTURENAME,
		&g_pTextureEffect)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitEffect(void)
{
	if (g_pVtxBufferEffect != NULL)
	{
		g_pVtxBufferEffect->Release();
		g_pVtxBufferEffect = NULL;
	}

	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateEffect(void)
{
	//for test
	VERTEX_2D *pVtx = NULL;

	g_pVtxBufferEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUsed)
		{
			//pos
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation + D3DX_PI + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation + D3DX_PI + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation - D3DX_PI - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation - D3DX_PI - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);

			//color
			if (g_aEffect[nCnt].nLife < 10)
			{
				pVtx[nCnt * 4 + 0].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, g_aEffect[nCnt].nLife * 0.08f);
				pVtx[nCnt * 4 + 1].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, g_aEffect[nCnt].nLife * 0.08f);
				pVtx[nCnt * 4 + 2].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, g_aEffect[nCnt].nLife * 0.08f);
				pVtx[nCnt * 4 + 3].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, g_aEffect[nCnt].nLife * 0.08f);
			}
			else
			{
				pVtx[nCnt * 4 + 0].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 0.8f);
				pVtx[nCnt * 4 + 1].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 0.8f);
				pVtx[nCnt * 4 + 2].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 0.8f);
				pVtx[nCnt * 4 + 3].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 0.8f);
			}
			
			//update
			g_aEffect[nCnt].nLife--;

			if (g_aEffect[nCnt].nLife >= 10)
			{
				g_aEffect[nCnt].fLength += g_aEffect[nCnt].fLengthSpeed;
				g_aEffect[nCnt].vPosCenter += g_aEffect[nCnt].vSpeed;
				g_aEffect[nCnt].fLengthSpeed *= 0.8f;
				g_aEffect[nCnt].vSpeed *= 0.8f;
			}

			if (g_aEffect[nCnt].nLife == 0)
			{
				g_aEffect[nCnt].bUsed = false;
			}
		}
	}



	g_pVtxBufferEffect->Unlock();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferEffect,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

						   //���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	for (int nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUsed)
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
HRESULT MakeVerTexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_EFFECT,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferEffect,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferEffect->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (int nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation + D3DX_PI + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation + D3DX_PI + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation - D3DX_PI - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation - D3DX_PI - EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCnt].vPosCenter.x + cosf(g_aEffect[nCnt].fRotation + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, g_aEffect[nCnt].vPosCenter.y + sinf(g_aEffect[nCnt].fRotation + EFFECT_ANGLE) * g_aEffect[nCnt].fLength, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[nCnt * 4 + 0].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 1.0f);
		pVtx[nCnt * 4 + 1].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 1.0f);
		pVtx[nCnt * 4 + 2].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 1.0f);
		pVtx[nCnt * 4 + 3].color = D3DXCOLOR(COLOR_R, COLOR_G, COLOR_B, 1.0f);

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferEffect->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  goal effect�ݒ�
//--------------------------------------------------------------------------------
void SetGoalEffect(D3DXVECTOR3 vSetPos)
{
	for (int nCnt = 0;nCnt < GOAL_EFFECT_NUM;nCnt++)
	{
		for (int nCntEffect = 0;nCntEffect < NUM_EFFECT;nCntEffect++)
		{
			if (!g_aEffect[nCntEffect].bUsed)
			{
				g_aEffect[nCntEffect].bUsed = true;
				g_aEffect[nCntEffect].nLife = 30;
				g_aEffect[nCntEffect].fLength = 1.0f;
				g_aEffect[nCntEffect].fLengthSpeed = 2.0f;
				g_aEffect[nCntEffect].fRotation = GOAL_EFFECT_ANGLE * nCnt;
				g_aEffect[nCntEffect].vPosCenter = vSetPos;
				g_aEffect[nCntEffect].vSpeed = D3DXVECTOR3(GOAL_EFFECT_SPEED * cosf(g_aEffect[nCntEffect].fRotation),
					GOAL_EFFECT_SPEED * sinf(g_aEffect[nCntEffect].fRotation),
					0.0f);
				break;
			}
		}
	}
}