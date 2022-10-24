#include "Slime.h"
#include "../Framework/ResourceMgr.h"
#include <iostream>

void Slime::Init()
{
	sprite.setPosition(80.f, (720.f * 0.5f) + 60.f);
	sprite.setScale({ 3.f,3.f });
	animator.SetTarget(&sprite);

	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeMove"));

	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeMoveLeft"));
	
	//animator.Play("SlimeMove");
	animator.Play("SlimeIdle");
	//SetState(States::Idle);
}

void Slime::SetState(States newState)
{
	if ( currState == newState )
	{
		return;
	}

	prevState = currState;
	currState = newState;

	switch ( currState )
	{
	case Slime::States::Idle:
		animator.Play((direction.x > 0.f) ? "SlimeIdle" : "SlimeIdleLeft");
		break;
	case Slime::States::Move:
		animator.Play((direction.x > 0.f) ? "SlimeMove" : "SlimeMoveLeft");
		break;
	}
}

void Slime::Update(float dt)
{
	//direction.x = 0.f;
	//direction.x += Keyboard::isKeyPressed(Keyboard::Right) ? 1 : 0;
	//direction.x += Keyboard::isKeyPressed(Keyboard::Left) ? -1 : 0;

	switch ( currState )
	{
	case Slime::States::Idle:
		UpdateIdle(dt);
		break;
	case Slime::States::Move:
		UpdateMove(dt);
		break;
	}

	animator.Update(dt);

}

void Slime::Draw(RenderWindow& window)
{
	window.draw(sprite);
}

void Slime::UpdateIdle(float dt)
{
	if ( !EqualFloat(direction.x, 0.f) )
	{
		//SetState(States::Move);
		return;
	}
}

void Slime::UpdateMove(float dt)
{
	if ( !EqualFloat(direction.x, lastDirection.x) )
	{
		animator.Play((direction.x > 0.f) ? "SlimeMove" : "SlimeMoveLeft");
	}
	
}

void Slime::PlayIdle()
{
	SetState(States::Idle);
}

void Slime::PlayMove()
{
	SetState(States::Move);
}

bool Slime::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}
