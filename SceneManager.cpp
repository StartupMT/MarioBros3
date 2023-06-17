#include "SceneManager.h"

SceneManager* SceneManager::_sceneManager;
SceneManager* SceneManager::GetInstance()
{
	if (nullptr == _sceneManager) {
		_sceneManager = new SceneManager();
	}
	return _sceneManager;
}

SceneManager::SceneManager()
{
	isEnd = false;
	endTime = 0;
}
SceneManager::~SceneManager()
{
}

//Load Data Game
void SceneManager::InitDT()
{
	ObjectManager::GetInstance()->InitDT();
}

//Update các scene game Update lớn nhất
void SceneManager::Update(float gameTime, Keyboard* key)
{
	//check end
	if (isEnd)
	{
		//Animation mario
		Mario::GetInstance()->BeforeUpdate(gameTime, key);
		Mario::GetInstance()->Update(gameTime, key);
		if (endTime > 0)
			endTime -= gameTime;
		else
		{
			ObjectManager::GetInstance()->InitDT();
			Mario::GetInstance()->Init();
			isEnd = false;
		}
	}
	ObjectManager::GetInstance()->Update(gameTime, key);
}

void SceneManager::StartEnd(float time)
{
	ObjectManager::GetInstance()->StartPause(time);
	isEnd = true;
	endTime = time;
}

//Vẽ Object lên màn hình
void SceneManager::Render()
{
	ObjectManager::GetInstance()->Render();
}