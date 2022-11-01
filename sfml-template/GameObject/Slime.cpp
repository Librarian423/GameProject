#include "Slime.h"
#include "Player.h"
#include "HitBox.h"
#include "ItemGenerator.h"
#include "VertexArrayObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundMgr.h"
#include <iostream>

Slime::Slime()
	: currState(States::None), speed(50.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), slimeState(0), moveTime(0.8f), hitTime(0.f), getAttackTime(1.f), attack(true), damage(1), hp(5), maxHp(5), barScaleX(60.f), isHitBox(false)
{
}

Slime::~Slime()
{
}

void Slime::Init(Player* player)
{
	SetPos({ 200.f,200.f });
	this->player = player;
	
	hp = maxHp;
	
	sprite.setScale({ 2.f,2.f });
	
	animator.SetTarget(&sprite);
	
	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	//hit box
	slimeHitbox = new HitBox();
	slimeHitbox->SetHitbox({ 0,0,30.f,20.f });
	slimeHitbox->SetPos({ GetPos().x,GetPos().y + 20.f });
	slimeHitbox->SetActive(true);

	//animation
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
		SOUND_MGR->Play("sound/slime_die.wav");
		slimeHitbox->SetActive(false);
		break;
	}

}

Slime::States Slime::GetState()
{
	return currState;
}

void Slime::SetBackground(VertexArrayObj* bk)
{
	background = bk;
}

void Slime::Update(float dt)
{
	prevPosition = GetPos();

	SpriteObj::Update(dt);
	direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;

	moveTime -= dt;
	if ( moveTime < 0.f )
	{
		cout << "timer" << endl;
		if ( slimeState % 2 == 0 )
		{
			SetState(Slime::States::Idle);
			slimeState = 1;
		}
		else
		{
			SetState(Slime::States::Move);
			slimeState = 0;
		}
		moveTime = 5.f;
	}

	//move
	if ( currState == States::Move )
	{
		dir = Utils::Normalize(player->GetPos() - GetPos());

		Translate(dir * this->speed * dt);
	}

	//attack motion
	if ( currState == States::Move && (Utils::Distance(player->GetPos(), GetPos()) < 150.f) && attack )
	{
		Translate(dir * 200.f * dt);
	}
	if ( currState == States::Move && Utils::Distance(player->GetPos(), GetPos()) < 15.f )
	{
		Translate(dir * 50.f * dt);
		attack = false;
	}

	hitTime += dt;
	getAttackTime += dt;

	//slime hits player
	if ( slimeHitbox->GetActive() )
	{
		if ( hitTime > 1.f && Utils::OBB(slimeHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
		{
			cout << "slime hit player" << endl;
			player->SetHp(damage);
			hitTime = 0.f;
		}
	}
	
	//player attack hitbox hits slime
	if ( getAttackTime > 0.2f )
	{
		if ( slimeHitbox->GetActive() && player->GetAttackHitbox()->GetActive() )
		{
			if ( Utils::OBB(slimeHitbox->GetHitbox(), player->GetAttackHitbox()->GetHitbox()) )
			{
				cout << "player hit slime" << endl;
				SetHp(player->GetDamage());
				if ( hp <= 0 )
				{
					SetState(States::Dead);
					ITEM_GEN->Generate(GetPos());
				}
				getAttackTime = 0.f;
			}
		}
	}

	//position
	slimeHitbox->SetPos(GetPos());
	SetHpBar();
	
	//animation
	animator.Update(dt);

	//wall bound
	for ( const auto& hb : background->GetHitBoxList() )
	{
		if ( Utils::OBB(hb->GetHitbox(), slimeHitbox->GetHitbox()) )
		{
			std::cout << "wall" << std::endl;
			SetSlimePos();
		}
	}

	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
		isHitBox = !isHitBox;
	}
}

void Slime::Draw(RenderWindow& window)
{
	if ( isHitBox )
	{
		slimeHitbox->Draw(window);
	}
	if ( GetActive() )
	{
		SpriteObj::Draw(window);
	}
	
	window.draw(healthBar);
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

void Slime::SetHp(int num)
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

void Slime::SetHpBar()
{
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	healthBar.setSize({ (barScaleX / maxHp) * hp, 15.f });
	if ( hp > (maxHp / 2) )
	{
		healthBar.setFillColor(Color::Green);
	}
	else if ( hp <= (maxHp / 2) && hp > (maxHp / 5) )
	{
		healthBar.setFillColor(Color::Yellow);
	}
	else
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

void Slime::SetSlimePos()
{
	SetPos(prevPosition);
	slimeHitbox->SetPos(prevPosition);
	healthBar.setPosition({ prevPosition.x, prevPosition.y - 15.f });
}

HitBox* Slime::GetSlimeHitBox()
{
	return slimeHitbox;
}
