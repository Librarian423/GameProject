#pragma once
#include "AnimationClip.h"
#include <map>
#include <queue>
#include <list>

class Animator
{
protected:
	map<string, AnimationClip> clips;
	list<AnimationEvent> events;

	Sprite* target;

	float speed;	//1.0
	bool isPlaying;

	AnimationClip* currClip;
	int currFrame;
	int totalFrame;
	float frameDuration;	//1프레임 시간  1 / fps

	float accumTime;

	queue<AnimationClip*> queue;
public:
	Animator();
	~Animator();

	void AddEvent(const AnimationEvent& ev);
	void ClearEvent();

	void AddClip(const AnimationClip& newClip);
	void SetTarget(Sprite* t);
	
	void Update(float dt);

	int GetCurrentFrame()const;
	void SetFrame(const AnimationFrame& frame);

	void Play(string id, bool clearQueue = true);
	void PlayQueue(string id);

	void Stop();
	bool IsPlaying()const;
	void ClearPlayQueue();
	
	float GetSpeed()const;
	void SetSpeed(float s);
};

