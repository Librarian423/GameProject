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
	
	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	Utils::SetOrigin(healthBar, Origins::MC);

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
	//SetState(States::Move);
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
				}
				getAttackTime = 0.f;
			}
		}
	}
	

	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
		isHitBox = !isHitBox;
	}

	slimeHitbox->SetPos({ GetPos().x,GetPos().y + 30.f });
	SetHpBar();

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
	window.draw(healthBar);
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
