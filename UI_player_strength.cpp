//--------------------------------------------------------------------------------
//
//�@UI.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-07
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "UI_player_strength.h"
#include "player.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define UI_PLAYER_STRENGTH_POS_X (498.0f - 0.5f)//�|���S���̕\���ʒuX
#define UI_PLAYER_STRENGTH_POS_Y (26.0f - 0.5f)//�|���S���̕\���ʒuY
#define UI_PLAYER_STRENGTH_WIDTH (269.0f)
#define UI_PLAYER_STRENGTH_HEIGHT (82.0f)
#define UI_PLAYER_STRENGTH_TEXTURENAME "data/TEXTURE/UI_strength.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIPlayerStrength(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIPlayerStrength = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIPlayerStrength = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitUIPlayerStrength(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	if (FAILED(MakeVerTexUIPlayerStrength(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexUI ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, UI_PLAYER_STRENGTH_TEXTURENAME, &g_pTextureUIPlayerStrength)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitUIPlayerStrength(void)
{
	//safe release
	if (g_pVtxBufferUIPlayerStrength != NULL)
	{
		g_pVtxBufferUIPlayerStrength->Release();
		g_pVtxBufferUIPlayerStrength = NULL;
	}

	if (g_pTextureUIPlayerStrength != NULL)
	{
		g_pTextureUIPlayerStrength->Release();
		g_pTextureUIPlayerStrength = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateUIPlayerStrength(void)
{
	float fTextureX = GetPlayerStrength();

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferUIPlayerStrength->Lock(0, 0, (void**)&pVtx, 0);

	//texture���_
	pVtx[0].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X + UI_PLAYER_STRENGTH_WIDTH * fTextureX, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X + UI_PLAYER_STRENGTH_WIDTH * fTextureX, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTextureX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTextureX, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferUIPlayerStrength->Unlock();

}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawUIPlayerStrength(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferUIPlayerStrength,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureUIPlayerStrength);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);

}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIPlayerStrength(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferUIPlayerStrength,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferUIPlayerStrength->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_PLAYER_STRENGTH_POS_X, UI_PLAYER_STRENGTH_POS_Y + UI_PLAYER_STRENGTH_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferUIPlayerStrength->Unlock();

	return S_OK;
}