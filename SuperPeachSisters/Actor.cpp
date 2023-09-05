#include "Actor.h"
#include "StudentWorld.h"
#include <vector>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR CLASS IMPLEMENTATIONS
// ===========================
void Actor::fallIfPossible(int dist){
    bool fall = true;
    for(int y = getY()-1; y > getY()-(dist+1); y--){
        if (getWorld()->isBlockingObjectAt(getX(), y))
            fall = false;
    }
    if (fall)
        this->moveTo(getX(), getY()-dist);
}

void Actor::moveOrReverseDirection(int dist){
    if (this->getDirection() == 180){
        if (getWorld()->isBlockingObjectAt(getX()-dist, getY())){
            setDirection(0);
            return;
        }
        this->moveTo(getX()-dist, getY());
    }
    
    if (this->getDirection() == 0){
        if (getWorld()->isBlockingObjectAt(getX()+dist, getY())){
            setDirection(180);
            return;
        }
        this->moveTo(getX()+dist, getY());
    }
}

// BLOCK CLASS IMPLEMENTATIONS
// ===========================
void Block::bonk(){
    switch (this->m_goodie){
        case none:
            getWorld()->playSound(SOUND_PLAYER_BONK);
            break;
        case flower:
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            getWorld()->getActorVector()->push_back(new Flower(getWorld(), getX()/SPRITE_WIDTH, (getY()+8)/SPRITE_HEIGHT));
            m_goodie = none;
            break;
        case mushroom:
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            getWorld()->getActorVector()->push_back(new Mushroom(getWorld(), getX()/SPRITE_WIDTH, (getY()+8)/SPRITE_HEIGHT));
            m_goodie = none;
            break;
        case star:
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            getWorld()->getActorVector()->push_back(new Star(getWorld(), getX()/SPRITE_WIDTH, (getY()+8)/SPRITE_HEIGHT));
            m_goodie = none;
            break;
    }
}

// FLAG CLASS IMPLEMENTATIONS
// ===========================
void Flag::doSomething(){
    if (!isAlive())
        return;
    
    if (getWorld()->overlapsPeach(this)){
        getWorld()->increaseScore(1000);
        this->setDead();
        getWorld()->endLevel(false);
    }
}

// MARIO CLASS IMPLEMENTATIONS
// ===========================
void Mario::doSomething(){
    if (!isAlive())
        return;
    
    if (getWorld()->overlapsPeach(this)){
        getWorld()->increaseScore(1000);
        this->setDead();
        getWorld()->endLevel(true);
    }
}

// GOODIE CLASS IMPLEMENTATIONS
// ===========================
void Goodie::doSomethingGoodie(int score){
    if (getWorld()->overlapsPeach(this)){
        getWorld()->increaseScore(score);
        getWorld()->getPeachPointer()->setHitpoints(2);
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    // Make the goodie fall
    fallIfPossible(2);
    // Make the goodie move or change direction
    moveOrReverseDirection(2);
}

// FLOWER CLASS IMPLEMENTATIONS
// ===========================
void Flower::doSomething(){
    Goodie::doSomethingGoodie(50);
    if (getWorld()->overlapsPeach(this))
        getWorld()->getPeachPointer()->setShootPower(true);
}

// MUSHROOM CLASS IMPLEMENTATIONS
// ===========================
void Mushroom::doSomething(){
    Goodie::doSomethingGoodie(75);
    if (getWorld()->overlapsPeach(this))
        getWorld()->getPeachPointer()->setJumpPower(true);
}

// STAR CLASS IMPLEMENTATIONS
// ===========================
void Star::doSomething(){
    Goodie::doSomethingGoodie(100);
    if (getWorld()->overlapsPeach(this))
        getWorld()->getPeachPointer()->setInvincibilityCounter(150);
}

// PROJECTILE CLASS IMPLEMENTATIONS
// ===========================
void Projectile::moveOrReverseDirection(int dist){
    if (getDirection() == 180 && !getWorld()->moveIfPossible(this, getX()-dist, getY())){
        setDead();
        return;
    }
    
    if (getDirection() == 0 && !getWorld()->moveIfPossible(this, getX()+dist, getY())){
        setDead();
        return;
    }
}

void Projectile::doSomething(){
    if (getWorld()->damageOverlappingActor(this)){
        setDead();
        return;
    }
    // Make the projectile fall
    fallIfPossible(2);
    // Make the projectile move or change direction
    Projectile::moveOrReverseDirection(2);
}

// PIRANHAFIREBALL CLASS IMPLEMENTATIONS
// ===========================
void PiranhaFireball::doSomething(){
    if (getWorld()->damageOverlappingPeach(this)){
        setDead();
        return;
    }
    // Make the projectile fall
    fallIfPossible(2);
    // Make the projectile move
    Projectile::moveOrReverseDirection(2);
}

// PEACHFIREBALL CLASS IMPLEMENTATIONS
// ===========================

// SHELL CLASS IMPLEMENTATIONS
// ===========================

// ENEMY CLASS IMPLEMENTATIONS
// ===========================
void Enemy::doSomething(){
    if (!isAlive())
        return;
    if(getWorld()->overlapsPeach(this)){
        getWorld()->bonkOverlappingPeach(this);
        return;
    }
     Enemy::moveOrReverseDirection(1);
}

void Enemy::moveOrReverseDirection(int dist){
    if (this->getDirection() == 180){
        if (getWorld()->isBlockingObjectAt(getX()-dist, getY()) || !getWorld()->isBlockingObjectAt(getX()-SPRITE_WIDTH, getY()-1)){
            setDirection(0);
            return;
        }
    }
    
    if (this->getDirection() == 0){
        if (getWorld()->isBlockingObjectAt(getX()+dist, getY()) || !getWorld()->isBlockingObjectAt(getX()+SPRITE_WIDTH, getY()-1)){
            setDirection(180);
            return;
        }
    }
    
    Actor::moveOrReverseDirection(dist);
}

void Enemy::sufferDamageIfDamageable(){
    if (!isAlive())
        return;
    getWorld()->increaseScore(100);
    this->setDead();
}

void Enemy::bonk(){
    if (!isAlive())
        return;
    if (getWorld()->getPeachPointer()->isInvincible()){
        this->sufferDamageIfDamageable();
        getWorld()->playSound(SOUND_PLAYER_KICK);
    }
}

// GOOMBA CLASS IMPLEMENTATIONS
// ===========================

// KOOPA CLASS IMPLEMENTATIONS
// ===========================
void Koopa::sufferDamageIfDamageable(){
    if (!isAlive())
        return;
    setDead();
    getWorld()->increaseScore(100);
    Actor* newShell = new Shell(getWorld(), getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, getDirection());
    getWorld()->getActorVector()->push_back(newShell);
}

void Koopa::bonk(){
    
    if(!isAlive())
        return;
    
    if (getWorld()->getPeachPointer()->isInvincible()){
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setDead();
        Actor* newShell = new Shell(getWorld(), getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, getDirection());
        getWorld()->getActorVector()->push_back(newShell);
    }
}

// PIRANHA CLASS IMPLEMENTATIONS
// ===========================
void Piranha::doSomething(){
    if (!isAlive())
        return;
    
    increaseAnimationNumber();
    
    // Bonks Peach if Peach is overlapping, then returns
    if (getWorld()->bonkOverlappingPeach(this))
        return;
    
    // Piranha get's Peach's targeting info
    int xDeltaFromActor;
    getWorld()->getPeachTargetingInfo(this, 1.5*SPRITE_HEIGHT, xDeltaFromActor);
    
    // Piranha changes direction
    if (xDeltaFromActor < 0)
        setDirection(180);
    if (xDeltaFromActor > 0)
        setDirection(0);
    
    // Piranha decrements firing delay if > 0
    if (firingdelay > 0)
        firingdelay--;
    
    // Use targeting info to decide whether or not to shoot a fireball
    if (std::abs(xDeltaFromActor) < 8*SPRITE_WIDTH && firingdelay == 0 && getWorld()->getPeachTargetingInfo(this, 1.5*SPRITE_HEIGHT, xDeltaFromActor)){
        getWorld()->getActorVector()->push_back(new PiranhaFireball(getWorld(), getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, getDirection()));
        getWorld()->playSound(SOUND_PIRANHA_FIRE);
        setFiringDelay(40);
    }
}

// PEACH CLASS IMPLEMENTATIONS
// ===========================
bool Peach::hasStarPower(){
    if (invincibilityCounter > 0)
        return true;
    else
        return false;
}

bool Peach::isInvincible(){
    if (invincibilityCounter > 0 || tempInvincibility > 0)
        return true;
    else
        return false;
}

void Peach::bonk(){
    if (invincibilityCounter > 0 || tempInvincibility > 0){
        return;
    }
    
    hitpoints--;
    tempInvincibility = 10;
    shootPower = false;
    jumpPower = false;
        
    if (hitpoints > 0)
        getWorld()->playSound(SOUND_PLAYER_HURT);
    else
        this->setDead();
}

void Peach::setHitpoints(int whatNumber){
    hitpoints = whatNumber;
}

void Peach::doSomething(){
    // Make sure that Peach is currently alive
    if (!isAlive())
        return;
    
    // Checks to see if Peach is invincible, if so, decrement invincibility time
    // Also, if she overlaps with an Actor, she bonks it
    // NOTE: this is basically just the same thing as Star Power, but can be temporariy
    // applied to the other powers if needed
    if (invincibilityCounter > 0){
        getWorld()->bonkOverlappingActor(this);
        invincibilityCounter--;
    }

    if (tempInvincibility > 0)
        tempInvincibility--;
    
    // Checks to see if the recharge time is zero, if not, decrement it
    if (time_to_recharge_before_next_fire > 0)
        time_to_recharge_before_next_fire--;
    
    
    // Checking the remaining_jump_distance, and take the necessary steps to continue/finish jump
    if (remaining_jump_distance > 0){
        // If there's an object in the way, bonk it and cancel the jump
        if (!getWorld()->moveOrBonk(this, getX(), getY()+4)){
            remaining_jump_distance = 0;
        }
        
        // If there's not an object in the way then just continue the jump
        else{
            remaining_jump_distance--;
        }
    }
    
    // Implementing the movement for falling
    if (remaining_jump_distance == 0){
        bool fall = true;
        for(int y = getY()-1; y > getY()-4; y--){
            if (getWorld()->isBlockingObjectAt(getX(), y))
                fall = false;
        }
        if (fall)
            this->moveTo(getX(), getY()-4);
    }
        
    
    // if the user is pressing a key during this tick
    int ch;
    if (getWorld() -> getKey(ch)){
        switch (ch){
            // move Peach to the left
            case KEY_PRESS_LEFT:
                setDirection(180);
                getWorld()->moveIfPossible(this, this->getX()-4, this->getY());
                break;
                
            // move Peach to the right
            case KEY_PRESS_RIGHT:
                setDirection(0);
                getWorld()->moveIfPossible(this, this->getX()+4, this->getY());
                break;
                
            // make Peach jump
            case KEY_PRESS_UP:
                if (getWorld()->isBlockingObjectAt(getX(), getY()-1)){
                    if (jumpPower == true)
                        remaining_jump_distance = 12;
                    else
                        remaining_jump_distance = 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
                
            // make Peach shoot a fireball
            case KEY_PRESS_SPACE:
                if (shootPower == false || time_to_recharge_before_next_fire > 0)
                    break;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                time_to_recharge_before_next_fire = 8;
                if (getDirection() == 0){
                    getWorld()->getActorVector()->push_back(new PeachFireball(getWorld(), (getX()+SPRITE_WIDTH)/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, 0));
                }
                if (getDirection() == 180)
                    getWorld()->getActorVector()->push_back(new PeachFireball(getWorld(), (getX()-SPRITE_HEIGHT)/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, 180));
                break;
        }
    }
}
