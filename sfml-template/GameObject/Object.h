#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Object
{
protected:
	int id;
	string name;
	bool enabled;

	Vector2f position;
	float rotation;
	Vector2f scale;

	static int objCount;
	
	//vector<Shape> hitbox;
	RectangleShape hitbox;

	bool isDevMod;

public:
	Object();
	virtual ~Object();

	int GetObjId()const;
	const string& GetName() const { return name; }
	void SetName(const string& n) { name = n; }

	virtual void SetActive(bool active);
	virtual bool GetActive() const;

	virtual void Init();
	virtual void Release();

	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	virtual const Vector2f& GetPos() const;
	virtual void Translate(const Vector2f delta);

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	virtual void SetHitbox(const FloatRect rect);

	virtual void SetDevMode(bool dev)
	{
		isDevMod = dev;
	}
};

