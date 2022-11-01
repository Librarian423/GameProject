#include "Item.h"
#include "HitBox.h"
#include "Player.h"
#include "ItemGenerator.h"
#include "../Framework/ResourceMgr.h"
#include "../Framework/InputMgr.h"

#include <iostream>

Item::Item()
    :currState(Types::None), isHitBox(false), isKey(false), value(0)
{
}

Item::~Item()
{
}

void Item::Init(Types type)
{
    currState = type;

    switch ( type )
    {
    case Item::Types::None:
        break;
    case Item::Types::Coin:
        animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Coin"));
        sprite.setScale({ 1.2f,1.2f });
        animator.SetTarget(&sprite);
        break;
    case Item::Types::Potion:
        animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Potion"));
        sprite.setScale({ 3.f,3.f });
        animator.SetTarget(&sprite);
        break;
    case Item::Types::Key:
        animator.AddClip(*ResourceMgr::GetInstance()->GetAnimationClip("Key"));
        sprite.setScale({ 3.f,3.f });
        animator.SetTarget(&sprite);
        break;
    }
    
    itemHitbox = new HitBox();
    itemHitbox->SetHitbox({ 0,0,20.f,20.f });
   
    SpriteObj::Init();
}

void Item::SetType(Types newTypes)
{
    currState = newTypes;

    switch ( newTypes )
    {
    case Item::Types::Coin:
        animator.Play("Coin");
        break;
    case Item::Types::Potion:
        animator.Play("Potion");
        break;
    case Item::Types::Key:
        animator.Play("Key");
        break;
    }

}

void Item::Update(float dt)
{
    itemHitbox->SetPos(GetPos());
    animator.Update(dt);
    //player Á¢ÃË
    if ( Utils::OBB(itemHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
    {
        player->OnPickupItem(this);
        
        if ( !isKey )
        {
            SetActive(false);
            ITEM_GEN->Erase(this->GetObjId());
        } 
    }

    if ( isKey )
    {
        itemHitbox->SetActive(false);
        SetPos({ player->GetPos().x + (player->GetPlayerLastDir().x > 0.f ? -30.f : 30.f), player->GetPos().y });
    }
   

    if ( InputMgr::GetKeyDown(Keyboard::F1) )
    {
        isHitBox = !isHitBox;
    }
    SpriteObj::Update(dt);
}

void Item::Draw(RenderWindow& window)
{
    SpriteObj::Draw(window);
    if ( isHitBox )
    {
        itemHitbox->Draw(window);
    }
}

void Item::SetPlayer(Player* p)
{
    player = p;
}