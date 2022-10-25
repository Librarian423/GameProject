#include "Player.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Utils.h"
#include "Object.h"
#include "HitBox.h"
#include <iostream>

void Player::Init()
{
	Object::Reset();
	SpriteObj::SetPos({ 1280.f * 0.5f - 80.f, (720.f * 0.5f) + 60.f });
	sprite.setScale({ 3.f,3.f });
	Utils::SetOrigin(sprite, Origins::TC);
	animator.SetTarget(&sprite);

	playerHitbox = new HitBox();
	playerHitbox->SetHitbox({ 0,0,48.f,48.f });
	playerHitbox->SetPos(GetPos());

	attackHitbox = new HitBox();
	attackHitbox->SetHitbox({ 0,0,48.f,48.f });
	attackHitbox->SetPos(GetPos());
	attackHitbox->SetActive(false);

	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerMove"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerAttack"));
	
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerMoveLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerAttackLeft"));

	{
		AnimationEvent ev;
		ev.clipId = "PlayerAttack";
		ev.frame = 3;
		ev.onEvent = bind(&Player::OnCompleteAttack, this);
		animator.AddEvent(ev);
	}
	{
		AnimationEvent ev;
		ev.clipId = "PlayerAttackLeft";
		ev.frame = 3;
		ev.onEvent = bind(&Player::OnCompleteAttack, this);
		animator.AddEvent(ev);
	}
	SpriteObj::Init();
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
		lastDirection = direction;
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
	direction.y = 0.f;
	direction.y += Keyboard::isKeyPressed(Keyboard::Down) ? 1 : 0;
	direction.y += Keyboard::isKeyPressed(Keyboard::Up) ? -1 : 0;
	//direction.y = InputMgr::GetAxisRaw(Axis::Vertical);

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
	}

	//����
	velocity += direction * speed * dt;
	if ( Utils::Magnitude(velocity) > speed )
	{
		velocity = Utils::Normalize(velocity) * speed;
	}

	//����
	if ( Utils::Magnitude(direction) == 0.f )
	{
		velocity = { 0.f, 0.f };
	}
	if ( direction.x == 0.f )
	{
		velocity.x = 0.f;
	}
	if ( direction.y == 0.f )
	{
		velocity.y = 0.f;
	}

	Translate(velocity*dt);
	playerHitbox->SetPos(GetPos());
	attackHitbox->SetPos(GetPos());
	animator.Update(dt);
	
	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
 		isHitBox = !isHitBox;
	}

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
	playerHitbox->Draw(window);
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

void Player::OnCompleteAttack()
{
	SetState(States::Idle);
}

void Player::UpdateIdle(float dt)
{
	if ( !EqualFloat(direction.x, 0.f) )
	{
		SetState(States::Move);
	}
	else if ( !EqualFloat(direction.y, 0.f) )
	{
		SetState(States::Move);
	}
}

void Player::UpdateMove(float dt)
{
	if ( EqualFloat(direction.x, 0.f) && EqualFloat(direction.y, 0.f) )
	{
		SetState(States::Idle);
		return;
	}

	if ( !EqualFloat(direction.x, lastDirection.x))
	{
		animator.Play((direction.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		lastDirection.x = direction.x;
	}

}

void Player::UpdateAttack(float dt)
{
	if (EqualFloat(direction.x, 0.f) )
	{
		SetState(States::Idle);
		return;
	}
}


bool Player::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}
