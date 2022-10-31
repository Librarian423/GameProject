#include "ItemBox.h"
#include "HitBox.h"
#include "Player.h"
#include "ItemGenerator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"


ItemBox::ItemBox()
	:isHitBox(true), eraseTimer(1.f), isBoxOpen(false)
{
}

ItemBox::~ItemBox()
{
}

void ItemBox::Init()
{
	this->player = player;

	SetPos({ 1020.f,100.f });
	
	//animation
	animator.SetTarget(&sprite);
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("ItemBoxIdle"));
	animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("ItemBoxOpen"));

	{
		AnimationEvent ev;
		ev.clipId = "ItemBoxOpen";
		ev.frame = 3;
		ev.onEvent = bind(&ItemBox::OnCompleteOpen, this);
		animator.AddEvent(ev);
	}

	itemboxHitbox = new HitBox();
	itemboxHitbox->SetHitbox({ 0,0,80.f,80.f });
	itemboxHitbox->SetPos({ GetPos().x,GetPos().y - 40 });
	itemboxHitbox->SetActive(false);

	SpriteObj::Init();
}

void ItemBox::SetPlayer(Player* player)
{
	this->player = player;
}

void ItemBox::SetState(States newState)
{
	currState = newState;

	switch ( currState )
	{
	case ItemBox::States::Idle:
		animator.Play("ItemBoxIdle");
		itemboxHitbox->SetActive(true);
		eraseTimer = 1.f;
		isBoxOpen = false;
		break;
	case ItemBox::States::Open:
		animator.Play("ItemBoxOpen");
		itemboxHitbox->SetActive(false);
		break;
	}
}

void ItemBox::Update(float dt)
{
	SpriteObj::Update(dt);
	animator.Update(dt);

	if ( isBoxOpen )
	{
		eraseTimer -= dt;
	}
	if ( eraseTimer < 0.f )
	{
		SetActive(false);
	}
	if ( player->GetIsKey() && Utils::OBB(itemboxHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
	{
		cout << "box" << endl;
		player->SetIsKey();
		ITEM_GEN->EraseKey();
		SetState(States::Open);
		ITEM_GEN->Generate(GetPos());
	}
	if ( InputMgr::GetKeyDown(Keyboard::F1) )
	{
		isHitBox = !isHitBox;
	}
}

void ItemBox::Draw(RenderWindow& window)
{
	if ( isHitBox )
	{
		itemboxHitbox->Draw(window);
	}
	if ( GetActive() )
	{
		SpriteObj::Draw(window);
	}
	
}

void ItemBox::OnCompleteOpen()
{
	isBoxOpen = true;	
}
