#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	enum Enemytype
	{
		Goomba = 100, //Con nấm
		Koopa = 200, //Con rùa
		Plant = 300,	//Cây trong tube
		FireBalls = 400,
	} _enemyType;
	float timeDead;
public:
	Enemy();
	~Enemy();

	void Init(D3DXVECTOR2 pos, int _type = Enemytype::Goomba, int kind = 0);
	void BeforeUpdate(float gameTime, Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void UpdateAnimation(float gameTime);
	void EnemyController();
	D3DXVECTOR2 OnCollision(Object* obj, D3DXVECTOR2 side);
	void SetBound(float width, float height);
	void Render(Viewport* viewport);
	static Animation* GetAnimationEnemy();
};

