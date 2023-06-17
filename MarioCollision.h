#pragma once
#include "Collision.h"
#include "Object.h"
#include "GameDefine.h"

using namespace Define;
class Mario;
class MarioCollision : public Collision
{
private:
	typedef void(MarioCollision::* marioCollisionF)(void);
	typedef map<int, marioCollisionF> FunctionMap;
	FunctionMap _functionMap;
public:
	Mario* mario;
	Object* _obj;
	D3DXVECTOR2 _side;
	bool isGround, isCollisionTop;

public:
	MarioCollision();
	~MarioCollision();

	void CheckCollisionEnemy();
	void CheckCollisionItem();
	void CheckCollisionBlock();
	void OnCollision();
	void PlayCollisionF();

	void StandCollision();
	void RunCollision();
	void JumpCollision();
	void AttackCollision(Object* _obj);
	void DeadCollision();
};

