#include "MarioController.h"
#include "Mario.h"

MarioController::MarioController()
{
	mario = Mario::GetInstance();
	mario->SetState(Object::Standing);
	isAllowJump = false;
	isFall = true;
	isAttack = false;

	_functionMap[Object::Standing] = &MarioController::StandState;
	_functionMap[Object::Running] = &MarioController::RunState;
	_functionMap[Object::Jumping] = &MarioController::JumpState;
	_functionMap[Object::Attacking] = &MarioController::AttackState;
	_functionMap[Object::Dying] = &MarioController::DeadState;
	_functionMap[Object::Sitting] = &MarioController::SitState;
}

MarioController::~MarioController()
{
}

//Trạng thái Đứng
void MarioController::StandState() //reset all state
{
	mario->ChangeMarioType(mario->_marioType);
	timeFlyDown = 0;
	isFly = false;
	isShortJump = false;
	isAllowJump = true;//Cho nhảy khi đứng hoặc chạy
	isFall = false;//cho nhảy thì không rơi
	mario->SetVelocityY(Gravity);
	if (key->IsKeyDown(Dik_JUMP))
	{
		mario->SetState(Object::Jumping);
	}
	else if (key->IsKeyDown(DIK_DOWN) && mario->_marioType != Mario::Small)
	{
		mario->SetState(Object::Sitting);
	}
	else if (key->IsKeyDown(Dik_LEFT) || key->IsKeyDown(Dik_RIGHT))
	{
		mario->SetState(Object::Running);
	}
	else
	{
		mario->SetState(Object::Standing);
	}
}

//Trang thái chạy
void MarioController::RunState()
{
	isShortJump = false;
	isAllowJump = true;
	isFall = false;
	mario->SetVelocityY(Gravity);
	if (key->IsKeyDown(Dik_DOWN) && mario->_marioType != Mario::Small)
	{
		mario->SetState(Object::Sitting);
	}
	else if (key->IsKeyDown(Dik_LEFT) || key->IsKeyDown(Dik_RIGHT))
	{
		if (key->IsKeyDown(Dik_JUMP))
		{
			mario->SetState(Object::Jumping);
		}
		if (key->IsKeyDown(Dik_ATTACK))
		{
			if (timeAc > MaxTimeAcc)
				isCount = true;
		}
	}
}

void MarioController::MoveX()
{
	//nếu chết không di chuyển
	if (mario->State == Object::Dying)
	{
		mario->SetVelocityX(0);
		return;
	}

	float speed = mario->GetVelocity().x;
	float speedRun = key->IsKeyDown(Dik_ATTACK) ? MaxRun : RunSpeed;
	speedRun = isMaxSpeed ? MaxSpeed : speedRun;
	isBake = false;
	if (key->IsKeyDown(Dik_RIGHT) && mario->State != Object::Sitting)
	{
		isBake = speed < 0 && mario->State == Object::Running; //kiểm tra thắng
		speed = speed < RunSpeed ? speed + accDown : speedRun; //nếu giữ X thì tăng tốc dần còn ko thì chạy bình thường
		mario->SetFlipFlag(false);
	}
	else if (key->IsKeyDown(Dik_LEFT) && mario->State != Object::Sitting)
	{
		isBake = speed > 0 && mario->State == Object::Running;
		speed = speed > -RunSpeed ? speed - accDown : -speedRun;
		mario->SetFlipFlag(true);
	}
	else
	{
		if (speed > 0)
		{
			speed = speed - accDown;
			speed = speed < 0 ? 0 : speed;
		}
		else
		{
			speed = speed + accDown;
			speed = speed > 0 ? 0 : speed;
		}
	}
	mario->SetVelocityX(speed);
}

void MarioController::ShortJump()
{
	StandState();
	isShortJump = true;
	mario->SetState(Object::Jumping);
}

void MarioController::Fall()
{
	mario->SetState(Object::Jumping);
	isAllowJump = false;
	isFall = true;
}

//Trạng thái nhảy
void MarioController::JumpState()
{
	if (isAllowJump)
	{
		isSpeedJump = isMaxSpeed;
		posYStartJump = mario->GetPosition().y;
		maxJump = isShortJump ? MaxEnemyJump : isFly ? 0 : MaxJump;
		isFall = false;
	}

	isAllowJump = false;
	isCount = isSpeedJump; //đếm tăng tốc
	float jumpSpeed = isSpeedJump ? JumpSpeed * 1.2 : JumpSpeed; // nếu mà chạy nhanh thì nhảy cao hơn xí

	//Fall
	if (!isFall && ((key->GIsKeyUp(Dik_JUMP) && maxJump >= MaxJump) || (mario->GetPosition().y - posYStartJump) >= maxJump))
	{
		isFall = true;
		velYStartFall = jumpSpeed;
	}

	if (isFall)
	{
		isFallDown = velYStartFall < -0.5;//rơi tự do
		float gravity = Gravity;

		//Check Fly
		if (isFallDown && mario->_marioType == Mario::Raccoon)
		{
			isCount = false; //rơi khi là chồn không giữ speed
			if (key->GIsKeyDown(Dik_JUMP))
			{
				if (isMaxSpeed)
					ShortJump();
				else if (timeFlyDown >= 0)
					timeFlyDown = 3;

				isFly = true;
			}
			//Nếu bay không đủ tốc độ thì rơi chậm
			if (timeFlyDown > 0)
			{
				gravity = gravity / 3;
				timeFlyDown -= fallAc;
			}
			else
			{
				timeFlyDown = 0;
				isFly = isMaxSpeed && isFly;
			}
		}
		else isFly = false;

		velYStartFall -= fallAc; //trừ vận tốc sẽ nhảy nhẹ 1 đoạn
		velYStartFall = isFallDown ? gravity : velYStartFall;

		mario->SetVelocityY(velYStartFall);
		return;
	}

	//JumpUP
	mario->SetVelocityY(jumpSpeed);
}

//Trạng thái đánh
void MarioController::AttackState()
{
}

//Trạng thái chêt
void MarioController::DeadState()
{
	velYStartFall -= fallAc;
	mario->SetVelocity(0, velYStartFall);
	mario->SetState(Object::Dying);
	mario->SetBound(0, 0);
}

//Trạng thái ngồi
void MarioController::SitState()
{
	StandState();
	mario->SetBound(15, 15);
}

void MarioController::PlayControllerF()
{
	//Lấy Function từ mario state
	FunctionMap::iterator it = _functionMap.find(mario->State);
	(this->*(it->second))();
}

void MarioController::Update(float gameTime, Keyboard* key)
{
	this->key = key;
	isCount = false;

	if (key->IsKeyDown(DIK_A))
	{
		mario->SetBound(15, 15);
		mario->_marioType = Mario::Small;
	}
	if (key->IsKeyDown(DIK_S))
	{
		mario->_marioType = Mario::Big;
		mario->SetBound(17, 25);
	}
	if (key->IsKeyDown(DIK_D))
	{
		mario->_marioType = Mario::Raccoon;
		mario->SetBound(17, 25);
	}

	this->PlayControllerF();

	//CheckAttack
	if (key->GIsKeyUp(Dik_ATTACK))
		isAllowAttack = true;

	if (isAllowAttack && !isAttack && key->GIsKeyDown(Dik_ATTACK) && mario->_marioType == Mario::Raccoon)
	{
		isAllowAttack = false;
		isAttack = true;
	}

	//Check acceleration
	MoveX();
	//Giảm tốc độ khi thắng hoặc thả phím tấn công
	accCount = ((isBake || key->GIsKeyUp(Dik_ATTACK)) && accCount > MaxSpeedCount) ? MaxSpeedCount - 10 : accCount;
	if (timeAc > MaxTimeAcc)
	{
		fallAc = FallAc;
		accDown = SlowDown;
		if (isCount && !isBake)
			accCount++;
		else
			accCount--;

		accCount = accCount < 0 ? 0 : (accCount > MaxSpeedCount && isCount) ? MaxSpeedCount + 10 : accCount;
		timeAc = 0.0f;
	}
	else
	{
		accDown = 0;
		fallAc = 0;
		timeAc += gameTime;
	}
	isMaxSpeed = accCount >= MaxSpeedCount;
}