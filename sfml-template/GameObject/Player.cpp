#include "Player.h"
#include "../Framework/ResourceMgr.h"
#include <iostream>

void Player::Init()
{
	sprite.setPosition(1280.f * 0.5f - 80.f, (720.f * 0.5f) + 60.f);
	sprite.setScale({ 3.f,3.f });
	animator.SetTarget(&sprite);
	
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerMove"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerAttack"));
	
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerMoveLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerAttackLeft"));

	{
		AnimationEvent ev;
		ev.clipId = "PlayerAttack";
		ev.frame = 6;
		ev.onEvent = bind(&Player::OnCompleteJump, this);
		animator.AddEvent(ev);
	}
	{
		AnimationEvent ev;
		ev.clipId = "PlayerAttackLeft";
		ev.frame = 6;
		ev.onEvent = bind(&Player::OnCompleteJump, this);
		animator.AddEvent(ev);
	}
	SetState(States::Idle);
}

void Player::SetState(States newState)
{
	if (currState == newState)
		return;

	prevState = currState;
	currState = newState;
	
	switch (currState)
	{
	case Player::States::Idle:
		animator.Play((lastDirection.x > 0.f) ? "PlayerIdle" : "PlayerIdleLeft");
		break;
	case Player::States::Move:
		animator.Play((direction.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		break;
	case Player::States::Attack:
		animator.Play((lastDirection.x > 0.f) ? "PlayerAttack" : "PlayerAttackLeft");
		break;
	}
}

void Player::Update(float dt)
{
	direction.x = 0.f;
	direction.x += Keyboard::isKeyPressed(Keyboard::Right) ? 1 : 0;
	direction.x += Keyboard::isKeyPressed(Keyboard::Left) ? -1 : 0;

	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Move:
		UpdateMove(dt);
		break;
	case Player::States::Attack:
		UpdateAttack(dt);
		break;
	default:
		break;
	}

	animator.Update(dt);

	if (!EqualFloat(direction.x, 0.f))
	{
		lastDirection = direction;
	}
}

void Player::UpdateInput(Event ev)
{
	switch (ev.type)
	{
	case Event::KeyPressed:
		switch (ev.key.code)
		{
		//case Keyboard::Key::Space:
		//	animator.Play((lastDirection.x > 0.f) ? "PlayerAttack" : "PlayerAttackLeft");
		//	//animator.PlayQueue((direction.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		//	break;
		}
		break;
	}
}

void Player::Draw(RenderWindow& window)
{
	window.draw(sprite);
}

void Player::PlayAttack()
{
	animator.Play((lastDirection.x > 0.f) ? "PlayerAttack" : "PlayerAttackLeft");
	switch ( currState )
	{
	case Player::States::Idle:
		animator.PlayQueue((lastDirection.x > 0.f) ? "PlayerIdle" : "PlayerIdleLeft");
		break;
	case Player::States::Move:
		animator.PlayQueue((lastDirection.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		break;
	default:
		break;
	}
}

void Player::OnCompleteJump()
{
	SetState(States::Idle);
}

void Player::UpdateIdle(float dt)
{
	if (!EqualFloat(direction.x, 0.f))
	{
		SetState(States::Move);
		return;
	}
}

void Player::UpdateMove(float dt)
{
	if ( EqualFloat(direction.x, 0.f) )
	{
		SetState(States::Idle);
		return;
	}
	if ( !EqualFloat(direction.x, lastDirection.x) )
	{
		animator.Play((direction.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
	}
}

void Player::UpdateAttack(float dt)
{
	if ( EqualFloat(direction.x, 0.f) )
	{
		SetState(States::Idle);
		return;
	}
}

bool Player::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}
