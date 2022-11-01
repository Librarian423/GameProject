#include "Boss.h"
#include "Player.h"
#include "HitBox.h"
#include "ItemGenerator.h"
#include "VertexArrayObj.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundMgr.h"
#include <iostream>

Boss::Boss()
    : currState(States::None), speed(50.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), bossState(0), moveTime(0.f), hitTime(0.f), getAttackTime(1.f), attack(true), damage(1), hp(15), maxHp(15), barScaleX(60.f), isHitBox(false)
{
}

Boss::~Boss()
{
}

void Boss::Init(Player* player)
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
	healthBar.setSize({ barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	//hit box
	bossHitbox = new HitBox();
	bossHitbox->SetHitbox({ 0,0,32.f,32.f });
	bossHitbox->SetPos({ GetPos().x,GetPos().y + 20.f });
	bossHitbox->SetActive(true);

	//attack hitbox
	attackHitbox = new HitBox();
	attackHitbox->SetHitbox({ 0,0,100.f,100.f });
	attackHitbox->SetPos({ GetPos().x, GetPos().y - 40.f });
	attackHitbox->SetActive(false);

	//animation
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossMove"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossIdleLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossMoveLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossAttack"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossAttackLeft"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossDead"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("BossDeadLeft"));

	{
		AnimationEvent ev;
		ev.clipId = "BossAttack";
		ev.frame = 4;
		ev.onEvent = bind(&Boss::OnCompleteAttack, this);
		animator.AddEvent(ev);
	}
	{
		AnimationEvent ev;
		ev.clipId = "BossAttackLeft";
		ev.frame = 4;
		ev.onEvent = bind(&Boss::OnCompleteAttack, this);
		animator.AddEvent(ev);
	}

	{
		AnimationEvent ev;
		ev.clipId = "BossDead";
		ev.frame = 8;
		ev.onEvent = bind(&Boss::OnCompleteDead, this);
		animator.AddEvent(ev);
	}
	{
		AnimationEvent ev;
		ev.clipId = "BossDeadLeft";
		ev.frame = 8;
		ev.onEvent = bind(&Boss::OnCompleteDead, this);
		animator.AddEvent(ev);
	}

	SpriteObj::Init();
}

void Boss::SetState(States newState)
{
	currState = newState;

	switch ( currState )
	{
	case Boss::States::Idle:
		attackHitbox->SetActive(false);
		animator.PlayQueue((direction.x < 0.f) ? "BossIdle" : "BossIdleLeft");
		break;
	case Boss::States::Move:
		animator.Play((direction.x < 0.f) ? "BossMove" : "BossMoveLeft");
		attackHitbox->SetActive(false);
		break;
	case Boss::States::Dead:
		animator.Play((direction.x < 0.f) ? "BossDead" : "BossDeadLeft");
		SOUND_MGR->Play("sound/boss_die.wav");
		bossHitbox->SetActive(false);
		break;
	case Boss::States::Attack:
		animator.Play((direction.x < 0.f) ? "BossAttack" : "BossAttackLeft");
		SOUND_MGR->Play("sound/boss_bite.wav");
		attackHitbox->SetActive(true);
		break;
	}
}

Boss::States Boss::GetState()
{
    return currState;
}

void Boss::SetBackground(VertexArrayObj* bk)
{
	background = bk;
}

void Boss::Update(float dt)
{
	prevPosition = GetPos();

	SpriteObj::Update(dt);
	direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;

	//boss attack
	if ( currState != States::Dead )
	{
		AttackPattern(dt);
	}

	//player attack hitbox hits boss
	getAttackTime += dt;
	if ( getAttackTime > 0.5f )
	{
		if ( bossHitbox->GetActive() && player->GetAttackHitbox()->GetActive() )
		{
			if ( Utils::OBB(bossHitbox->GetHitbox(), player->GetAttackHitbox()->GetHitbox()) )
			{
				cout << "player hit boss" << endl;
				SetHp(player->GetDamage());
				if ( hp <= 0 )
				{
					SetState(States::Dead);
					bossHitbox->SetActive(false);
					attackHitbox->SetActive(false);
					ITEM_GEN->Generate(GetPos(), true);
				}
				getAttackTime = 0.f;
			}
		}
		if ( attackHitbox->GetActive() && player->GetPlayerHitBox()->GetActive() )
		{
			if ( Utils::OBB(attackHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
			{
				cout << "boss attack player" << endl;
				player->SetHp(damage * 2);
				getAttackTime = 0.f;
			}
		}
	}

	//position
	bossHitbox->SetPos(GetPos());
	attackHitbox->SetPos({ GetPos().x, GetPos().y - 40.f });

	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);

	//wall bound
	for ( const auto& hb : background->GetHitBoxList() )
	{
		if ( Utils::OBB(hb->GetHitbox(), bossHitbox->GetHitbox()) )
		{
			std::cout << "wall" << std::endl;
			SetBossPos();
		}
	}

	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
		isHitBox = !isHitBox;
	}
}

void Boss::Draw(RenderWindow& window)
{
	if ( isHitBox )
	{
		bossHitbox->Draw(window);
		attackHitbox->Draw(window);
	}
	if ( GetActive() )
	{
		SpriteObj::Draw(window);
		window.draw(healthBar);
	}
}

void Boss::OnCompleteDead()
{
	SetActive(false);
	bossHitbox->SetActive(false);
}

void Boss::OnCompleteAttack()
{
	SetState(States::Idle);
	attackHitbox->SetActive(false);
	moveTime = 3.f;
}

bool Boss::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

void Boss::SetHp(int num)
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

void Boss::SetHpBar()
{
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
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

void Boss::SetBossPos()
{
	SetPos(prevPosition);
	bossHitbox->SetPos(prevPosition);
	healthBar.setPosition({ prevPosition.x, prevPosition.y - 15.f });
}

HitBox* Boss::GetBossHitBox()
{
    return bossHitbox;
}

void Boss::AttackPattern(float dt)
{
	moveTime -= dt;
	if ( moveTime <= 0.f )
	{
		SetState(States::Move);
		moveTime = 3.f;
	}
	//move
	if ( currState == States::Move )
	{
		dir = Utils::Normalize(player->GetPos() - GetPos());

		Translate(dir * this->speed * dt);
	}
	//attack motion
	if ( currState == States::Move && Utils::Distance(player->GetPos(), GetPos()) < 85.f )
	{
		SetState(States::Attack);
	}

	//boss hits player
	hitTime += dt;
	if ( bossHitbox->GetActive() )
	{
		if ( hitTime > 1.f && Utils::OBB(bossHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
		{
			cout << "boss hit player" << endl;
			player->SetHp(damage);
			hitTime = 0.f;
		}
	}
}