#include "Mario.h"
#include "GameDefine.h"
#include <math.h>
#include "GUI.h"
#include "ObjectManager.h"

Mario* Mario::_mario = nullptr;
Mario::Mario()
{
}

Mario* Mario::GetInstance()
{
	if (nullptr == _mario) {
		_mario = new Mario();
		_mario->_life = StartLive;
		_mario->Init();
	}
	return _mario;
}

Mario::~Mario()
{
	delete _marioController;
	delete _marioCollision;
	delete _mario;
}

void Mario::Init()
{
	//Tạo Animation
	Animation::DataAnimMap data;
	//Small
	data[Mario::Small + Object::Standing] = { 19, 19 };
	data[Mario::Small + Object::Running] = { 2,	3 };
	data[Mario::Small + Object::Running + 1] = { 2, 3, 5 };	//Chạy nhanh
	data[Mario::Small + Object::Running + 2] = { 6, 7, 1 };	//Trượt
	data[Mario::Small + Object::Running + 3] = { 4, 4 };	//Thắng
	data[Mario::Small + Object::Jumping] = { 5, 5 };
	data[Mario::Small + Object::Jumping + 1] = { 5, 5 };	//Rơi
	data[Mario::Small + Object::Jumping + 2] = { 8, 8 };	//Anim bay nhanh
	data[Mario::Small + Object::Dying] = { 10, 10 };	//Chết
	//Big
	data[Mario::Big + Object::Standing] = { 47, 47 };
	data[Mario::Big + Object::Running] = { 25,	27 };
	data[Mario::Big + Object::Running + 1] = { 25, 27, 5 };	//Chạy nhanh
	data[Mario::Big + Object::Running + 2] = { 31, 33, 1 };	//Trượt
	data[Mario::Big + Object::Running + 3] = { 28, 28 };	//Thắng
	data[Mario::Big + Object::Jumping] = { 29, 29 };
	data[Mario::Big + Object::Jumping + 1] = { 30, 30 };	//rơi
	data[Mario::Big + Object::Jumping + 2] = { 34, 34 };	//Anim bay nhanh
	data[Mario::Big + Object::Sitting] = { 35, 35 };

	//Raccoon
	data[Mario::Raccoon + Object::Standing] = { 130, 130 };
	data[Mario::Raccoon + Object::Running] = { 96,	98 };
	data[Mario::Raccoon + Object::Running + 1] = { 96, 98, 5 };	//Chạy nhanh
	data[Mario::Raccoon + Object::Running + 2] = { 108, 110, 1 };	//Trượt
	data[Mario::Raccoon + Object::Running + 3] = { 99, 99 };	//Thắng
	data[Mario::Raccoon + Object::Jumping] = { 106, 106 };
	data[Mario::Raccoon + Object::Jumping + 1] = { 107, 107 };		//rơi
	data[Mario::Raccoon + Object::Jumping + 2] = { 114, 114 };	//Anim bay nhanh
	data[Mario::Raccoon + Object::Jumping + 3] = { 114, 116, 5 };	//Fly
	data[Mario::Raccoon + Object::Attacking] = { 100, 105, 5 };	//Attack
	data[Mario::Raccoon + Object::Sitting] = { 35, 35 };

	//data[Mario::Small + Object::Standing] = { 114, 116};

	//_anim = new Animation(EnemyXML, EnemyPNG);
	//_anim = new Animation(MiscXML, MiscPNG);
	_anim = new Animation(MarioXML, MarioPNG);

	_anim->SetDataAnimation(data);

	//Tạo class xử lý va chạm
	_marioController = new MarioController();
	_marioCollision = new MarioCollision();

	AllowDraw = true;
	Tag = Object::Player;
	_marioType = Mario::Small;
	ChangeMarioType(_marioType);
	position = positionStart;
	velocity = D3DXVECTOR2(0, 0);
	SetState(Object::Standing);
	HP = 1;
}

void Mario::BeforeUpdate(float gameTime, Keyboard* key)
{
	_marioCollision->isGround = false;
	_marioCollision->isCollisionTop = false;

	this->SetBound(Width, Height);
	//Check handler controller
	_marioController->Update(gameTime, key);
}

void Mario::OnCollision(Object* obj, float gameTime)
{
	Object::OnCollision(obj, gameTime);
	if (obj->State == Object::Dying || this->State == Object::Dying)
		return;
	if (obj->Tag == Object::Enemy && _marioController->isAttack)
	{
		_marioCollision->AttackCollision(obj);
	}
}

D3DXVECTOR2 Mario::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	_marioCollision->_obj = obj;
	_marioCollision->_side = side;
	_marioCollision->OnCollision();
	return _marioCollision->_side;
}

void Mario::OnCollision(Object* obj)
{

}

void Mario::Update(float gameTime, Keyboard* key)
{
	//Update Animation
	UpdateAnimation(gameTime);

	Object::Update(gameTime, key);
}

void Mario::UpdateAnimation(float gameTime)
{
	//check changeType
	if (isImmortal)
	{
		if (immortalTime > 0)
		{
			AllowDraw = (int)(immortalTime / 0.1) % 2;
			immortalTime -= gameTime;

			//gamePause
			if (ObjectManager::GetInstance()->isPause)
			{
				if ((int)(immortalTime / 0.1) % 2 == 1)
				{
					_anim->NewAnimationByIndex(Mario::Big + Object::Standing);
					_anim->SetPosition(D3DXVECTOR2(position.x, position.y + 15 / 2));
				}
				else
				{
					_anim->NewAnimationByIndex(Mario::Small + Object::Standing);
					_anim->SetPosition(D3DXVECTOR2(position.x, position.y + 15 / 2));
				}
				_anim->SetFlipFlag(FlipFlag);
				_anim->Update(gameTime);
				return;
			}
		}
		else
		{
			isImmortal = false;
			AllowDraw = true;
		}
	}

	_state = 0;
	if (State == Object::Running)
	{
		_state = _marioController->isBake ? 3 : abs(velocity.x) >= MaxSpeed ? 2 : abs(velocity.x) >= MaxRun ? 1 : 0;
		State = velocity.x == 0 ? Object::Standing : State;
	}
	else if (State == Object::Jumping)
	{
		_state = _marioController->isFly ? 3 : _marioController->isSpeedJump ? 2 : _marioController->isFallDown;
	}

	//nếu đang trên không mà không có bay hoặc đụng đầu thì mario rơi
	if (!_marioCollision->isGround && velocity.y < 0 && !_marioController->isFly || _marioCollision->isCollisionTop)
	{
		if (State != Object::Dying)
			_marioController->Fall();
	}

	//mếu đang tấn công thì đổi trạng thái
	State = _marioController->isAttack ? Object::Attacking : State;	//check anim attack
	_anim->NewAnimationByIndex(_marioType + this->State + _state);

	if (_marioController->isAttack && _anim->GetIndex() == _anim->GetEnd()) //check end anim attack
	{
		_marioController->isAttack = false;
		if (State != Object::Dying)
			_marioController->Fall();
	}

	_anim->SetPosition(D3DXVECTOR2(position.x, position.y + Height / 2));
	_anim->SetFlipFlag(FlipFlag);
	_anim->Update(gameTime);
}

void Mario::SetImmortal(float time)
{
	immortalTime = time;
	isImmortal = true;
}

void Mario::ChangeMarioType(MarioType newMarioType, float time)
{
	switch (_marioType)
	{
	case Mario::Small:
		if (newMarioType > _marioType)
			ObjectManager::GetInstance()->StartPause(1.0f);
		SetBound(12, 15);
		break;
	case Mario::Big:
		if (newMarioType < _marioType)
			ObjectManager::GetInstance()->StartPause(1.0f);
		SetBound(17, 25);
		break;
	case Mario::Raccoon:
		SetBound(17, 25);
		break;
	default:
		SetBound(12, 15);
		break;
	}

	if (_marioType == newMarioType) return;
	_marioType = newMarioType;
	SetImmortal(time);
	State = Object::Standing;
}

void Mario::SetBound(float width, float height)
{
	//InfoSprite::Infoframe info = _anim->GetCurrentFrameInfo();
	Width = width;
	Height = height;
	bound.left = position.x - Width / 2;
	bound.right = bound.left + Width;
	bound.top = position.y + Height;
	bound.bottom = position.y;
}

void Mario::Render(Viewport* viewport)
{
	//Vẽ Mario
	if (AllowDraw)
	{
		_anim->Render(viewport);
	}
	DrawLine::GetInstance()->DrawRect(bound);

	GUI::GetInstance()->Render(_marioController->accCount, { 5, 200, 40, 235 });

	GUI::GetInstance()->Render("Coin: ", { 90, 200, 130, 235 });
	GUI::GetInstance()->Render(_coin, { 100, 200, 150, 235 });

	GUI::GetInstance()->Render("Life: ", { 150, 200, 200, 235 });
	GUI::GetInstance()->Render(_life, { 210, 200, 220, 235 });
}
