//--------------------------------------------------------------------------------
//
//　game.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "input.h"
#include "game.h"
#include "KF_BodyContact.h"
#include "player.h"
#include "ball.h"
#include "mainball.h"
#include "table.h"
#include "goal.h"
#include "timer.h"
#include "bg.h"
#include "UI.h"
#include "UI_player_strength.h"
#include "UI_ball.h"
#include "UI_mainball.h"
//#include "subline.h"
#include "shadow.h"
#include "effect.h"
#include "fade.h"

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
bool CheckTurn(void);

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitGame(void)
{
	InitPlayer();
	InitMainBall();
	InitBall();
	InitBodyContact();
	InitTable();
	InitGoal();
	InitBG();

	//UI
	InitUI();
	InitUIBall();
	InitUIMainBall();
	InitUIPlayerStrength();
	//InitSubline();

	InitTimer();
	InitEffect();
	InitShadow();
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitGame(void)
{
	UninitPlayer();
	UninitMainBall();
	UninitBall();
	UninitBodyContact();
	UninitTable();
	UninitGoal();
	UninitBG();

	//UI
	UninitUI();
	UninitUIBall();
	UninitUIMainBall();
	UninitUIPlayerStrength();
	//UninitSubline();

	UninitTimer();
	UninitEffect();
	UninitShadow();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateGame(void)
{
	UpdateShadow();
	UpdatePlayer();
	UpdateMainBall();
	UpdateBall();
	UpdateBodyContact();
	

	//二回目衝突処理
	for (int nCnt = 0;nCnt < 3;nCnt++)
	{
		UpdateMainBallContact();
		UpdateBallContact();
		UpdateBodyContact();
	}
	
	
	UpdateTable();
	UpdateBG();

	//UI
	UpdateUI();
	UpdateUIBall();
	UpdateUIMainBall();
	UpdateUIPlayerStrength();
	//UpdateSubline();

	UpdateTimer();
	UpdateEffect();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawGame(void)
{
	bool bPlayer = CheckTurn();

	DrawBG();
	DrawTable();

	//UI
	DrawUI();
	DrawUIBall();
	DrawUIMainBall();
	DrawUIPlayerStrength();
	//DrawSubline();

	DrawShadow();

	if (bPlayer)
	{
		DrawPlayer();
	}
	
	DrawTimer();
	DrawMainBall();
	DrawBall();
	DrawEffect();
}

bool CheckTurn(void)
{
	D3DXVECTOR3 vVelocity;
	bool *bTurn = GetPlayerTurnFlag();

	//mainball
	vVelocity = GetMainBallVelocity();
	if (GetMainBallCnt() != 0)
	{
		return false;
	}
	else
	{
		if (Vector3Magnitude(vVelocity) <= 0.01f)
		{
			*bTurn = true;
		}
		else
		{
			return false;
		}
	}

	//ball
	for (int nCnt = 0;nCnt < NUM_BALL;nCnt++)
	{
		if (GetBallUsing(nCnt))
		{
			vVelocity = GetBallVelocity(nCnt);
			if (Vector3Magnitude(vVelocity) <= 0.01f)
			{
				*bTurn = true;
			}
			else
			{
				return false;
			}
		}
	}

	return *bTurn;
}