#include "Player.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Utils.h"
#include "../Framework/SoundMgr.h"
#include "VertexArrayObj.h"
#include "Object.h"
#include "HitBox.h"
#include "Item.h"
#include <iostream>

Player::Player()
	: currState(States::None), speed(500.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), timer(1.f), attackTime(0.5f), isHitBox(false), damage(1), hp(10), maxHp(10), isKey(false), isAlive(true)
{
}

Player::~Player()
{
}

void Player::Init()
{
	isAlive = true;
	hp = maxHp;
	sprite.setScale({ 2.f,2.f });
	
	animator.SetTarget(&sprite);

	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ 6.f * maxHp, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	//player hitbox
	playerHitbox = new HitBox();
	playerHitbox->SetHitbox({ 0,0,25.f,25.f });
	playerHitbox->SetPos(GetPos());
	playerHitbox->SetActive(true);
	
	//attack hitbox
	attackHitbox = new HitBox();
	attackHitbox->SetHitbox({ 0,0,80.f,35.f });
	attackHitbox->SetPos({ ((lastDirection.x > 0.f) ? 25 : -25) + GetPos().x, GetPos().y });
	attackHitbox->SetActive(false);

	//animation
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerMove"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerAttack"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerDead"));
	
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerMoveLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerAttackLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("PlayerDeadLeft"));

	//Attack
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
	//Dead
	{
		AnimationEvent ev;
		ev.clipId = "PlayerDead";
		ev.frame = 2;
		ev.onEvent = bind(&Player::OnCompleteDead, this);
		animator.AddEvent(ev);
	}
	{
		AnimationEvent ev;
		ev.clipId = "PlayerDeadLeft";
		ev.frame = 2;
		ev.onEvent = bind(&Player::OnCompleteDead, this);
		animator.AddEvent(ev);
	}
	
	SetState(States::Idle);
}

void Player::SetState(States newState)
{
	if (currState == newState)
		return;

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
	case Player::States::Dead:
		animator.Play((lastDirection.x > 0.f) ? "PlayerDead" : "PlayerDeadLeft");
		break;
	}
}

void Player::SetBackground(VertexArrayObj* bk)
{
	background = bk;
}

void Player::Update(float dt)
{
	if ( isAlive )
	{
		direction.x = InputMgr::GetAxisRaw(Axis::Horizontal);
		direction.y = InputMgr::GetAxisRaw(Axis::Vertical);
	}
	
	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Move:
		UpdateMove(dt);
		break;
	case Player::States::Attack:
		break;
	}

	//죽음
	if ( hp <= 0 )
	{
		SetState(States::Dead);
	}

	//가속
	velocity = direction * speed;

	//감속
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

	prevPosition = GetPos();
	Translate(velocity * dt);

	//dash
	if ( Keyboard::isKeyPressed(Keyboard::Key::LControl) )
	{
		Dash(dt);
	}
	else
	{
		speed = 500.f;
	}

	//attack
	timer += dt;
	if ( isAlive && timer > attackTime && Keyboard::isKeyPressed(Keyboard::Key::Space) )
	{
		cout << "attack" << endl;
		attackHitbox->SetActive(true);
		PlayAttack();
		timer = 0.f;
	}
	
	//positions
	playerHitbox->SetPos(GetPos());
	attackHitbox->SetPos({ ((lastDirection.x > 0.f) ? 25 : -25) + GetPos().x , GetPos().y });
	
	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);

	//wall bound
	for ( const auto& hb: background->GetHitBoxList() )
	{
		if ( Utils::OBB(hb->GetHitbox(), playerHitbox->GetHitbox()) )
		{
			std::cout << "wall" << std::endl;
			SetPlayerPos();
		}
	}

	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
 		isHitBox = !isHitBox;
	}
	
	if (!EqualFloat(direction.x, 0.f))
	{
		lastDirection = direction;
	}
}

void Player::Draw(RenderWindow& window)
{
	if ( isHitBox )
	{
		playerHitbox->Draw(window);
		attackHitbox->Draw(window);
	}
	SpriteObj::Draw(window);
	window.draw(healthBar);
	
}

void Player::PlayAttack()
{
	SOUND_MGR->Play("sound/sword.wav");
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

void Player::Dash(float dt)
{
	Translate(direction * speed * dt);
}

void Player::OnCompleteAttack()
{
	SetState(States::Idle);
	attackHitbox->SetActive(false);
}

void Player::OnCompleteDead()
{
	isAlive = false;
	playerHitbox->SetActive(false);
	attackHitbox->SetActive(false);
}

void Player::UpdateIdle(float dt)
{
	attackHitbox->SetActive(false);
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
		attackHitbox->SetActive(false);
		animator.Play((direction.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		lastDirection.x = direction.x;
	}

}

bool Player::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

HitBox* Player::GetPlayerHitBox()
{
	return playerHitbox;
}

HitBox* Player::GetAttackHitbox()
{
	return attackHitbox;
}

void Player::SetHp(int num)
{
	if ( hp > 0 )
	{
		hp -= num;
	}
	else
	{
		hp = 0;
	}
}

void Player::SetHpBar()
{
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	healthBar.setSize({ 6.f * hp, 15.f });
	if ( hp > 5 )
	{
		healthBar.setFillColor(Color::Green);
	}
	else if ( hp <= 5 && hp > 2 )
	{
		healthBar.setFillColor(Color::Yellow);
	}
	else if ( hp <= 2 )
	{
		healthBar.setFillColor(Color::Red);
	}
	if ( hp <= 0 )
	{
		healthBar.setOutlineThickness(0.f);
	}
	else
	{
		healthBar.setOutlineThickness(2.f);
	}
}

void Player::OnPickupItem(Item* item)
{
	switch ( item->GetType() )
	{
	case Item::Types::Potion:
		cout << "potion" << endl;
		hp += item->GetValue();
		if ( hp >= maxHp )
			hp = maxHp;
		break;
	case Item::Types::Coin:
		cout << "coin" << endl;
		//exp += item->GetValue();
		break;
	case Item::Types::Key:
		cout << "key" << endl;
		item->SetIsKey();
		isKey = true;
		break;
	}
}

void Player::SetPlayerPos()
{
	SetPos(prevPosition);
	playerHitbox->SetPos(prevPosition);
	healthBar.setPosition({ prevPosition.x, prevPosition.y - 15.f });
}
