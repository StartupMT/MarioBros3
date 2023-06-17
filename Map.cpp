#include "Map.h"
#include"TinyXML\tinyxml.h"
#include "Mario.h"

Map::Map()
{
	//load tileset lên
	info = new InfoMap(MapXML);
	this->tileset = new TileSet(info->tileCount, info->tileColumns, info->tileWidth, info->tileHeight);
	position = D3DXVECTOR2(0, 0);

	objectTag["Goomba"] = Enemy::Goomba;
	objectTag["Wall"] = Block::Wall;
	objectTag["Koopa"] = Enemy::Koopa;

	objectTag["Coin"] = Item::Coin;

	for (int i = 0; i < info->numObjectGroups; i++)
	{
		for (int j = 0; j < info->ObjectGroups.at(i)->NumOnjects; j++)
		{
			ListObject.push_back(CreateObject(info->ObjectGroups.at(i)->Objects.at(j)));
		}
	}
}

Map::~Map()
{
	delete tileset;
	delete info;
	int size = ListObject.size();
	for (size_t i = 0; i < size; i++)
	{
		Object* obj = ListObject.at(i);
		if (obj->Tag == Object::Player) 
			continue;  // không xóa mario
		delete ListObject.at(i);
		obj = NULL;
	}
}

void Map::Update(float gameTime)
{

}

Object* Map::CreateObject(MapObject* _mapobject)
{
	D3DXVECTOR2 pos;
	pos.x = _mapobject->x + _mapobject->width / 2;
	pos.y = info->height * info->tileHeight - _mapobject->y - _mapobject->height / 2;

	Object* obj;
	Object::tag tagg = Object::GetTag(_mapobject->name);
	switch (tagg)
	{
	case Object::Player:
		obj = Mario::GetInstance();
		obj->SetPositionStart(pos);
		break;
	case Object::Enemy:
		switch (objectTag[_mapobject->name])
		{
		case Enemy::Goomba:
			if (_mapobject->kind == 1) //ParaGoomba
			{
				//obj = new ParaGoomba();
				obj = new Enemy();
				break;
			}
			obj = new Enemy();// Enemy = Goomba
			break;
		case Enemy::Koopa:
			if (_mapobject->kind == 1) //GreenKoopa
			{
				//obj = new GreenKoopa();
				obj = new Enemy();
				break;
			}
			obj = new RedKoopa(); //RedKoopa
			break;
		case Enemy::Plant:
		default:
			obj = new Enemy();
			break;
		}
		break;
	case Object::Item:
		switch (objectTag[_mapobject->name])
		{
		case Item::Coin:
		default:
			obj = new Item();
			break;
		}
		break;
	case Object::Block:
		switch (objectTag[_mapobject->name])
		{
		case Block::Wall:
		default:
			obj = new Block();
			break;
		}
		break;
	default:
		obj = new Object();
		break;
	}
	obj->id = _mapobject->id;
	obj->Init(pos, objectTag[_mapobject->name], _mapobject->kind);
	obj->SetPositionStart(pos);
	obj->GetBound(_mapobject->width, _mapobject->height);
	return obj;
}

void Map::Render(Viewport* view)
{
	int startH, startW, endH, endW;
	RECT r = view->GetBoundViewport();
	startH = r.bottom / info->tileHeight;
	endH = r.top / info->tileHeight + 1;
	startW = r.left / info->tileWidth;
	endW = r.right / info->tileWidth + 1;

	for (int h = startH; h < endH; h++)
		for (int w = startW; w < endW; w++)
		{
			int id = info->GetData(w, h);
			if (id > 0)
			{
				D3DXVECTOR2 position(w * info->tileWidth, (h + 1) * info->tileHeight);
				tileset->Render(view, id, position);
			}
		}
}
