#include "Enemy.h"
#include "Mario.h"

Animation* Enemy::GetAnimationEnemy()
{
	//Tạo Animation
	Animation::DataAnimMap data;
	// Goomba
	data[Enemy::Goomba + Object::Standing] = { 0 , 0 };
	data[Enemy::Goomba + Object::Running] = { 1 , 2 };
	data[Enemy::Goomba + Object::Dying] = { 3 , 3 };
	// Red Goomba
	data[Enemy::Goomba + Object::Standing + 1] = { 4 , 5 }; // =idle
	data[Enemy::Goomba + Object::Running + 1] = { 6 , 7 }; // =walk 
	data[Enemy::Goomba + Object::Dying + 1] = { 8 , 8 }; // =die

	// Para Red Goomba
	data[Enemy::Goomba + Object::Standing + 2] = { 14 , 15 };
	data[Enemy::Goomba + Object::Running + 2] = { 15 , 16 };
	data[Enemy::Goomba + Object::Jumping + 2] = { 16 , 17 };
	data[Enemy::Goomba + Object::Dying + 2] = { 8 , 8 }; // khi die chuyen sang red goomba

	// Red koopa
	data[Enemy::Koopa + Object::Standing] = { 26 , 27 };
	data[Enemy::Koopa + Object::Running] = { 26 , 27 }; // = đánh lần 1 thụt đầu (shell)
	data[Enemy::Koopa + Object::Jumping] = { 30 , 31 }; // = đánh lần 2 chạy ( shellrun)

	// Green koopa
	data[Enemy::Koopa + Object::Standing + 1] = { 32 , 33 };
	data[Enemy::Koopa + Object::Running + 1] = { 35 , 35 }; // = đánh lần 1 thụt đầu (shell)
	data[Enemy::Koopa + Object::Jumping + 1] = { 36 , 37 }; // = đánh lần 2 chạy ( shellrun)

	// Para Green koopa *
	data[Enemy::Koopa + Object::Standing + 2] = { 38 , 38 };
	data[Enemy::Koopa + Object::Running + 2] = { 38 , 39 };
	data[Enemy::Koopa + Object::Jumping + 2] = { 39 , 40 };

	// Piranha Plant ( cái cây màu xanh cắn)
	data[Enemy::Plant + Object::Standing] = { 24 , 25 };

	//// Fire piranha plant ( cây bắn đạn)
	// Red
	data[Enemy::Plant + Object::Standing] = { 18 , 18 }; // headdown
	data[Enemy::Plant + Object::Running] = { 19 , 19 }; // headup
	// Greem
	data[Enemy::Plant + Object::Standing + 1] = { 20 , 20 }; // headdown
	data[Enemy::Plant + Object::Running + 1] = { 21 , 21 }; // headup
	// cây đỏ tấn công
	data[Enemy::Plant + Object::Standing + 2] = { 65 , 66 }; // down
	data[Enemy::Plant + Object::Running + 2] = { 67 , 68 }; // up
	// cây xanh tấn công
	data[Enemy::Plant + Object::Standing + 3] = { 70 , 71 }; // down
	data[Enemy::Plant + Object::Running + 3] = { 72 , 73 }; // up

	Animation* _animEnemy = new Animation(EnemyXML, EnemyPNG);
	_animEnemy->SetDataAnimation(data);
	return _animEnemy;
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete _anim;
}

void Enemy::Init(D3DXVECTOR2 pos, int _type, int kind)
{
	AllowDraw = true;
	Tag = Object::Enemy;

	_anim = GetAnimationEnemy();
	_enemyType = (Enemytype)_type;
	_kind = kind;
	position = pos;
	velocity = D3DXVECTOR2(-EnemySpeed, Gravity);
	SetState(Object::Running);
	this->SetBound(14, 15);
	timeDead = 0;
	HP = 1;
}

void Enemy::EnemyController()
{
	velocity.y = Gravity;
}

D3DXVECTOR2 Enemy::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	switch (obj->Tag)
	{
	case Object::Player:
		Mario::GetInstance()->_marioCollision->CheckCollisionEnemy();
		return D3DXVECTOR2(Collision::NONE, side.y);

	default:
		if (side.x != Collision::NONE)
		{
			velocity.x = -velocity.x;
			side.x = Collision::NONE;
		}
		return side;
	}
}

void Enemy::BeforeUpdate(float gameTime, Keyboard* key)
{
	this->SetBound(Width, Height);
	EnemyController();
}
void Enemy::Update(float gameTime, Keyboard* key)
{
	//Update Animation
	if (State == Object::Dying)
	{
		this->SetBound(0, 0);
		this->SetVelocity(0, 0);
		timeDead += gameTime;
		if (timeDead > 1)
			AllowDraw = false;
	}

	UpdateAnimation(gameTime);

	Object::Update(gameTime, key);
}

void Enemy::UpdateAnimation(float gameTime)
{
	_anim->NewAnimationByIndex(_enemyType + this->State + _kind);
	_anim->SetPosition(D3DXVECTOR2(position.x, position.y + Height / 2));
	_anim->SetFlipFlag(velocity.x > 0);
	_anim->Update(gameTime);
}

void Enemy::SetBound(float width, float height)
{
	InfoSprite::Infoframe info = _anim->GetCurrentFrameInfo();

	Width = info.w;
	Height = info.h;
	bound.left = position.x - Width / 2;
	bound.right = bound.left + Width;
	bound.top = position.y + Height;
	bound.bottom = position.y;
}

void Enemy::Render(Viewport* viewport)
{
	//Vẽ Enemy
	if (AllowDraw)
	{
		_anim->Render(viewport);
	}
	DrawLine::GetInstance()->DrawRect(bound);
}