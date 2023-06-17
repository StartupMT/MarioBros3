#pragma once
#include "dinput.h"

namespace Define
{
	//Windows
	const float GameWidth = 256.0f;
	const float GameHeight = 240.0f;
	const float WinWidth = 768.0f;
	const float WinHeight = 720.0f;
	const float FPS = 60.0f;

	//Control
	const int Dik_SELECT = DIK_SPACE;
	const int Dik_START = DIK_RETURN;
	const int Dik_LEFT = DIK_LEFT;
	const int Dik_RIGHT = DIK_RIGHT;
	const int Dik_UP = DIK_UP;
	const int Dik_DOWN = DIK_DOWN;
	const int Dik_JUMP = DIK_Z;
	const int Dik_ATTACK = DIK_X;

	//Path
	//Mario
	const char MarioPNG[] = "./Resource Files/mario_png.png";
	const char MarioXML[] = "./Resource Files/mario_info.xml";
	//Enemy
	const char EnemyPNG[] = "./Resource Files/enemy_png.png";
	const char EnemyXML[] = "./Resource Files/enemy_info.xml";
	//Misc
	const char MiscPNG[] = "./Resource Files/misc_png.png";
	const char MiscXML[] = "./Resource Files/misc_info.xml";

	//Map
	const char TileSetPNG[] = "./Resource Files/Tileset_map_1.png";
	const char MapXML[] = "./Resource Files/Map_1.tmx";

	//Mario
	const int StartLive = 3;
	const float timeEnd = 3;

	//MarioRun
	const float RunSpeed = 1.0f;
	const float SlowDown = 0.20f;
	const float MaxRun = 1.5f;
	const float MaxSpeed = 2.0f;
	const float MaxTimeAcc = 0.02f;
	const int MaxSpeedCount = 30;

	//MarioJump
	const float JumpSpeed = 1.5f;
	const float Gravity = -1.75f;
	const float FallAc = 0.3f;
	const float MaxEnemyJump = 16.0f;
	const float MaxJump = 64.0f;

	//EnemyAttributes
	const float EnemySpeed = 0.5f;
	const float EnemyJump = 32.0f;

	//ItemAttributes
	const int MaxCoin = 100;
	const float ItemSpeed = 0.5f;
}