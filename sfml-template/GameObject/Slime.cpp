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
	
	SpriteObj::Init();
}

void Slime::SetState(States newState)
{
	prevState = currState;
	currState = newState;

	switch ( currState )
	{
	case Slime::States::Idle:
		animator.Play((direction.x > 0.f) ? "SlimeIdle" : "SlimeIdleLeft");
		attack = true;
		break;
	case Slime::States::Move:
		animator.Play((direction.x > 0.f) ? "SlimeMove" : "SlimeMoveLeft");
		break;
	}
}

void Slime::Update(float dt)
{
	SpriteObj::Update(dt);

	direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;


	if ( currState == States::Move )
	{
		dir = Utils::Normalize(player->GetPos() - GetPos());

		Translate(dir * this->speed * dt);
	}
	
	if ( currState == States::Move && (Utils::Distance(player->GetPos(), GetPos()) < 150.f) && attack )
	{
		cout << "attack" << endl;
		Translate(dir * 200.f * dt);
	}
	if ( currState == States::Move && Utils::Distance(player->GetPos(), GetPos()) < 15.f )
	{
		cout << "stop" << endl;
		Translate(dir * 50.f * dt);
		attack = false;
	}
	
	animator.Update(dt);
	
}

void Slime::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	window.draw(sprite);
}

//void Slime::UpdateIdle(float dt)
//{
//	if ( !EqualFloat(direction.x, 0.f) )
//	{
//		animator.PlayQueue((direction.x > 0.f) ? "SlimeIdle" : "SlimeIdleLeft");
//	}
//}
//
//void Slime::UpdateMove(float dt)
//{
//	if ( !EqualFloat(direction.x, 0.f) )
//	{
//		animator.PlayQueue((direction.x > 0.f) ? "SlimeMove" : "SlimeMoveLeft");
//	}
//}

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
