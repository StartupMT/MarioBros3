#include "GUI.h"
#include "string.h"
#pragma warning(disable : 4996)

GUI* GUI::_gui = nullptr;

GUI* GUI::GetInstance()
{
	if (nullptr == _gui) {
		_gui = new GUI();
	}
	return _gui;
}

GUI::GUI()
{
	gui = new DrawString(Graphic::GetInstance(), 12);
}

GUI::~GUI()
{
	delete gui;
}

void GUI::Render(const char* text, RECT rect)
{
	char _text[10];
	strcpy(_text, text);
	gui->Render(_text, rect, D3DCOLOR_XRGB(255, 255, 255));
}

void GUI::Render(int num, RECT rect)
{
	char _text[10];
	itoa(num, _text, 10);
	gui->Render(_text, rect, D3DCOLOR_XRGB(255, 255, 255));
}

//Vẽ
void GUI::Render()
{
	////Hiển thị Score
	//char speed[5];
	//itoa(Mario::GetInstance()->_marioController->accCount, speed, 10);
	//gui->Render(speed, { 5, 200, 40, 235 }, D3DCOLOR_XRGB(255, 255, 255));

	//char state[5];
	//itoa(Mario::GetInstance()->State, state, 10);
	//gui->Render(state, { 50, 200, 90, 235 }, D3DCOLOR_XRGB(255, 255, 255));
}