#include "RedKoopa.h"
#include "Mario.h"

RedKoopa::RedKoopa()
{
}

RedKoopa::~RedKoopa()
{
	delete _anim;
}

void RedKoopa::EnemyController()
{
	velocity.y = Gravity;
}

D3DXVECTOR2 RedKoopa::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	switch (obj->Tag)
	{
	case Object::Player:
		Mario::GetInstance()->_marioCollision->CheckCollisionEnemy();
		return D3DXVECTOR2(Collision::NONE, side.y);

	case Object::Block:
		if (side.y == Collision::BOTTOM)
		{
			if (position.x < obj->GetBound().left || position.x > obj->GetBound().right)
			{
				velocity.x = -velocity.x;
				side.x = Collision::NONE;
			}
		}
		return side;
	default:
		return side;
	}
}
