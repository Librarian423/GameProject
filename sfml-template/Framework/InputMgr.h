#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <map>

using namespace sf;
using namespace std;

//스태틱 함수를 이요해서 키보드 밋 마우스 입력을 검사할 수 있는 클래스

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;
	list<Keyboard::Key> positives;
	list<Keyboard::Key> negatives;

	float sensi;
	float value;
};

class InputMgr
{
public:
	static void Init();
	static void Update(float dt);
	static void ProcessInput(Event& ev);

	static bool GetKeyDown(Keyboard::Key key);
	static bool GetKey(Keyboard::Key key);
	static bool GetKeyUp(Keyboard::Key key);

	static float GetAxis(Axis axis);
	static float GetAxisRaw(Axis axis);

	static const Vector2f& GetMousePos();
	static bool GetMouseDown(Mouse::Button key);
	static bool GetMouse(Mouse::Button key);
	static bool GetMouseUp(Mouse::Button key);

private:
	static map<Axis, AxisInfo> axisInfoMap;

	//static list<Keyboard::Key>downList;
	//static list<Keyboard::Key>ingList;
	//static list<Keyboard::Key>upList;
	static list<int>downList;
	static list<int>ingList;
	static list<int>upList;
	
	/*static list<Mouse::Button>downListMouse;
	static list<Mouse::Button>ingListMouse;
	static list<Mouse::Button>upListMouse;*/

	static Vector2f mousePos;

	/*static vector<Keyboard::Key>downList;
	static vector<Keyboard::Key>ingList;
	static vector<Keyboard::Key>upList;*/
};

