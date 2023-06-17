#pragma once
#include "Object.h"
#include "MarioCollision.h"
#include "MarioController.h"

using namespace Define;

class Mario : public Object
{
private:
	static Mario* _mario;
public:
	enum MarioType
	{
		Small = 100,
		Big = 200,
		Raccoon = 300,
	} _marioType;

	MarioController* _marioController;
	MarioCollision* _marioCollision;
	int _state = 0;
	RECT rect_attack;
	int _life, _coin = 0, _score = 0;
	bool isImmortal = false;
	float immortalTime = 0;

public:
	Mario();
	~Mario();

	void Init();
	void BeforeUpdate(float gameTime, Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void UpdateAnimation(float gameTime);
	void OnCollision(Object* obj, float gameTime);
	D3DXVECTOR2 OnCollision(Object* obj, D3DXVECTOR2 side);
	void OnCollision(Object* obj);
	void SetImmortal(float time);
	void ChangeMarioType(MarioType marioType, float time = 0);
	void SetBound(float width, float height);
	void Render(Viewport* viewport);
	static Mario* GetInstance();
};
