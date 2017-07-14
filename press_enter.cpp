//--------------------------------------------------------------------------------
//
//�@Result.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "press_enter.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define PRESS_ENTER_POS_X (SCREEN_WIDTH * 0.5f)//�|���S���̕\���ʒuX
#define PRESS_ENTER_POS_Y (SCREEN_HEIGHT * 0.9f)//�|���S���̕\���ʒuY
#define PRESS_ENTER_WIDTH (298.0f)//�|���S���̕\���ʒuY
#define PRESS_ENTER_HEIGHT (45.0f)//�|���S���̕\���ʒuY
#define PRESS_ENTER_TEXTURENAME "data/TEXTURE/press_enter.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPressEnter(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePressEnter = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPressEnter = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
float g_fAlpha;
float g_fChange;

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾
	g_fAlpha = 0.0f;
	g_fChange = 1.0f;

	if (FAILED(MakeVerTexPressEnter(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPressEnter ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, PRESS_ENTER_TEXTURENAME, &g_pTexturePressEnter)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitPressEnter(void)
{
	//safe release
	if (g_pVtxBufferPressEnter != NULL)
	{
		g_pVtxBufferPressEnter->Release();
		g_pVtxBufferPressEnter = NULL;
	}

	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdatePressEnter(void)
{
	g_fAlpha += 0.05f * g_fChange;

	if (g_fAlpha >= 1.0f)
	{
		g_fAlpha = 1.0f;
		g_fChange = -1.0f;
	}

	if (g_fAlpha <= 0.0f)
	{
		g_fAlpha = 0.0f;
		g_fChange = 1.0f;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[1].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[2].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[3].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);

	//���z�A�h���X���
	g_pVtxBufferPressEnter->Unlock();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferPressEnter,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTexturePressEnter);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPressEnter(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferPressEnter,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferPressEnter->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(PRESS_ENTER_POS_X - PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y - PRESS_ENTER_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PRESS_ENTER_POS_X + PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y - PRESS_ENTER_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PRESS_ENTER_POS_X - PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y + PRESS_ENTER_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PRESS_ENTER_POS_X + PRESS_ENTER_WIDTH * 0.5f, PRESS_ENTER_POS_Y + PRESS_ENTER_HEIGHT * 0.5f, 0.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[1].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[2].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);
	pVtx[3].color = D3DXCOLOR(0.9f, 0.9f, 0.9f, g_fAlpha);

	//texture���_
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferPressEnter->Unlock();

	return S_OK;
}