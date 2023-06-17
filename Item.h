#include "Object.h"

class Item : public Object
{
public:
	enum Itemtype
	{
		Coin = 100,
		Nấm = 200,
	} _itemtype = Itemtype::Coin;

	Item();
	~Item();

	void Init(D3DXVECTOR2 pos, int _type = Itemtype::Coin, int kind = 0);
	void BeforeUpdate(float gameTime, Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void UpdateAnimation(float gameTime);
	void ItemController();
	D3DXVECTOR2 OnCollision(Object* obj, D3DXVECTOR2 side);
	void SetBound(float width, float height);
	void Render(Viewport* viewport);
	static Animation* GetAnimationItem();
};
