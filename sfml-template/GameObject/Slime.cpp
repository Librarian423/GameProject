#include "Slime.h"
#include "Player.h"
#include "HitBox.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include <iostream>

void Slime::Init(Player* player)
{
	this->player = player;

	sprite.setScale({ 3.f,3.f });
	
	animator.SetTarget(&sprite);
	
	//hit box
	slimeHitbox = new HitBox();
	slimeHitbox->SetHitbox({ 0,0,35.f,30.f });
	slimeHitbox->SetPos({ GetPos().x,GetPos().y + 30.f });

	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeMove"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeMoveLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeDead"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("SlimeDeadLeft"));
	
	{
		AnimationEvent ev;
		ev.clipId = "SlimeDead";
		ev.frame = 3;
		ev.onEvent = bind(&Slime::OnCompleteDead, this);
		animator.AddEvent(ev);
	}
	{
		AnimationEvent ev;
		ev.clipId = "SlimeDeadLeft";
		ev.frame = 3;
		ev.onEvent = bind(&Slime::OnCompleteDead, this);
		animator.AddEvent(ev);
	}
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
	case Slime::States::Dead:
		animator.Play((direction.x > 0.f) ? "SlimeDead" : "SlimeDeadLeft");
		slimeHitbox->SetActive(false);
		break;
	}

}

Slime::States Slime::GetState()
{
	return currState;
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
		//cout << "attack" << endl;
		Translate(dir * 200.f * dt);
	}
	if ( currState == States::Move && Utils::Distance(player->GetPos(), GetPos()) < 15.f )
	{
		//cout << "stop" << endl;
		Translate(dir * 50.f * dt);
		attack = false;
	}
	if ( currState == States::Dead )
	{
		deleteTime -= dt;
	}
	hitTime += dt;
	if ( hitTime > 1.f && Utils::OBB(slimeHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
	{
		cout << "slime hit player" << endl;
		hitTime = 0.f;
	}
	if ( player->GetAttackHitbox()->GetActive() && Utils::OBB(slimeHitbox->GetHitbox(), player->GetAttackHitbox()->GetHitbox()) )
	{
		cout << "player hit slime" << endl;
		//SetState(States::Dead);
	}
	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
		isHitBox = !isHitBox;
	}

	slimeHitbox->SetPos({ GetPos().x,GetPos().y + 30.f });
	animator.Update(dt);
	
}

void Slime::Draw(RenderWindow& window)
{
	
	if ( GetActive() )
	{
		SpriteObj::Draw(window);
	}
	if ( isHitBox )
	{
		slimeHitbox->Draw(window);
	}
	//window.draw(sprite);
}

void Slime::PlayIdle()
{
	SetState(States::Idle);
}

void Slime::PlayMove()
{
	SetState(States::Move);
}

void Slime::OnCompleteDead()
{
	SetActive(false);
	slimeHitbox->SetActive(false);
}

bool Slime::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}
