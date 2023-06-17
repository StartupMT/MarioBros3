#pragma once
#include <d3dx9.h>
#include "Keyboard.h"
#include "Object.h"
#include "GameDefine.h"
using namespace Define;

class Mario;
class MarioController
{
private:
	typedef void(MarioController::* MarioControllerF)(void);
	typedef map<int, MarioControllerF> FunctionMap;
	FunctionMap _functionMap;

public:
	//Run
	bool isBake, isCount, isMaxSpeed;
	float accDown;
	int accCount = 0;
	//Jump
	bool isAllowJump, isFall, isFallDown, isSpeedJump, isShortJump, isFly;
	float posYStartJump, velYStartFall, maxJump, fallAc, timeFlyDown;
	//Attack
	bool isAttack, isAllowAttack;
	float timeAc;


	Mario* mario;
	Keyboard* key;
public:
	MarioController();
	~MarioController();

	void StandState();
	void RunState();
	void MoveX();
	void ShortJump();
	void Fall();
	void JumpState();
	void AttackState();
	void DeadState();
	void SitState();
	void PlayControllerF();
	void Update(float gameTime, Keyboard* key);
};