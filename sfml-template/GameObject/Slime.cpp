#include "Slime.h"
#include "Player.h"
#include "../Framework/ResourceMgr.h"
#include <iostream>

void Slime::Init(Player* player)
{
	this->player = player;

	sprite.setPosition(80.f, (720.f * 0.5f) + 60.f);
	sprite.setScale({ 3.f,3.f });
	animator.SetTarget(&sprite);

	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeMove"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeMoveLeft"));
	
	animator.Play("SlimeIdle");

	SpriteObj::Init();
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
	SpriteObj::Update(dt);

	dir = Utils::Normalize(player->GetPos() - GetPos());

	Translate(dir * this->speed * dt);

	//float distance = Utils::Distance(player->GetPos(), GetPos());
	
	

	animator.Update(dt);

}

void Slime::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	window.draw(sprite);
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
