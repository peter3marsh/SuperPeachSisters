#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// ACTOR CLASS DECLARATIONS
// ===========================
class Actor : public GraphObject{
public:
    Actor(StudentWorld *world, int imageID, int startX, int startY, int startDirection, int depth, double size) : GraphObject(imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size), m_world(world), aliveBool(true){};
    
    virtual void sufferDamageIfDamageable() { return; }
    
    virtual void doSomething() { return; } // by default, this function does nothing
    
    bool isAlive() { return aliveBool; }
    
    void setDead() { aliveBool = false; }
    
    virtual void bonk() { return; }; // by default, this function does nothing
    
    virtual bool isBlockingObject() { return false; };
    
    void fallIfPossible(int dist);
    
    virtual void moveOrReverseDirection(int dist);
    
    StudentWorld* getWorld(){ return m_world; }
    
private:
    StudentWorld *m_world;
    bool aliveBool;
    
};

// BLOCK CLASS DECLARATIONS
// ===========================
class Block : public Actor{
public:
    enum GoodieType { none, flower, mushroom, star };
    Block(StudentWorld *world, int startX, int startY, GoodieType g) : Actor(world, IID_BLOCK, startX, startY, 0, 2, 1), m_goodie(g){};
    virtual void bonk();
    virtual bool isBlockingObject(){ return true; }
    
private:
    GoodieType m_goodie;
};

// GOODIE CLASS DECLARATIONS
// ===========================
class Goodie : public Actor{
public:
    Goodie(StudentWorld *world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, 0, 1 , 1){};
    void doSomethingGoodie(int score);
};

// FLOWER CLASS DECLARATIONS
// ===========================
class Flower : public Goodie{
public:
    Flower(StudentWorld *world, int startX, int startY) : Goodie(world, IID_FLOWER, startX, startY){};
    virtual void doSomething();
};

// MUSHROOM CLASS DECLARATIONS
// ===========================
class Mushroom : public Goodie{
public:
    Mushroom(StudentWorld *world, int startX, int startY) : Goodie(world, IID_MUSHROOM, startX, startY){};
    virtual void doSomething();
};

// STAR CLASS DECLARATIONS
// ===========================
class Star : public Goodie{
public:
    Star(StudentWorld *world, int startX, int startY) : Goodie(world, IID_STAR, startX, startY){};
    virtual void doSomething();
};

// PROJECTILE CLASS DECLARATIONS
// ===========================
class Projectile : public Actor{
public:
    Projectile(StudentWorld *world, int imageID, int startX, int startY, int startDirection) : Actor(world, imageID, startX, startY, startDirection, 1, 1){};
    virtual void doSomething();
    virtual void moveOrReverseDirection(int dist);
};

// PIRAHNA FIREBALL CLASS DECLARATIONS
// ===========================
class PiranhaFireball : public Projectile{
public:
    PiranhaFireball(StudentWorld *world, int startX, int startY, int startDirection) : Projectile(world, IID_PIRANHA_FIRE, startX, startY, startDirection){};
    virtual void doSomething();
};

// PEACH FIREBALL CLASS DECLARATIONS
// ===========================
class PeachFireball : public Projectile{
public:
    PeachFireball(StudentWorld *world, int startX, int startY, int startDirection) : Projectile(world, IID_PEACH_FIRE, startX, startY, startDirection){};
};

// SHELL CLASS DECLARATIONS
// ===========================
class Shell : public Projectile{
public:
    Shell(StudentWorld *world, int startX, int startY, int startDirection) : Projectile(world, IID_SHELL, startX, startY, startDirection){};
};

// ENEMY CLASS DECLARATIONS
// ===========================
class Enemy : public Actor{
public:
    Enemy(StudentWorld *world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, randInt(0,1)*180, 1, 0){};
    virtual void doSomething();
    virtual void moveOrReverseDirection(int dist);
    virtual void sufferDamageIfDamageable();
    virtual void bonk();
};

// GOOMBA CLASS DECLARATIONS
// ===========================
class Goomba : public Enemy{
public:
    Goomba(StudentWorld *world, int startX, int startY) : Enemy(world, IID_GOOMBA, startX, startY){};
};

// KOOPA CLASS DECLARATIONS
// ===========================
class Koopa : public Enemy{
public:
    Koopa(StudentWorld *world, int startX, int startY) : Enemy(world, IID_KOOPA, startX, startY){};
    virtual void sufferDamageIfDamageable();
    virtual void bonk();
};

// PIRANHA CLASS DECLARATIONS
// ===========================
class Piranha : public Enemy{
public:
    Piranha(StudentWorld *world, int startX, int startY) : Enemy(world, IID_PIRANHA, startX, startY), firingdelay(0){};
    virtual void doSomething();
    void setFiringDelay(int toWhat) { firingdelay = toWhat; }
    
private:
    int firingdelay;
};

// PIPE CLASS DECLARATIONS
// ===========================
class Pipe : public Actor{
public:
    Pipe(StudentWorld *world, int startX, int startY) : Actor(world, IID_PIPE, startX, startY, 0, 2, 1){};
    virtual bool isBlockingObject(){ return true; }
};

// FLAG CLASS DECLARATIONS
// ===========================
class Flag : public Actor{
public:
    Flag(StudentWorld *world, int startX, int startY) : Actor(world, IID_FLAG, startX, startY, 0, 1, 1){}
    
    virtual void doSomething();
};

// MARIO CLASS DECLARATIONS
// ===========================
class Mario : public Actor{
public:
    Mario(StudentWorld *world, int startX, int startY) : Actor(world, IID_MARIO, startX, startY, 0, 1, 1){}
    
    virtual void doSomething();
};

// PEACH CLASS DECLARATIONS
// ===========================
class Peach : public Actor{
public:
    Peach(StudentWorld *world, int startX, int startY) : Actor(world, IID_PEACH, startX, startY, 0, 0, 1.0), shootPower(false), jumpPower(false), invincibilityCounter(0), tempInvincibility(0), time_to_recharge_before_next_fire(0), remaining_jump_distance(0), hitpoints(1){};
    
    virtual void bonk();
    
    virtual void sufferDamageIfDamageable() { this->bonk(); }
    
    virtual void doSomething();
    
    void incrementHitpoints() { hitpoints++; }
    
    void decrementHitpoints() { hitpoints--; }
    
    void setHitpoints(int whatNumber);
    
    void setShootPower(bool onoff) { shootPower = onoff; }
    
    void setJumpPower(bool onoff) { jumpPower = onoff; }
    
    void setInvincibilityCounter(int ticks) { invincibilityCounter = ticks; }
    
    bool isInvincible();
    
    bool hasShootPower(){ return shootPower; }
    
    bool hasJumpPower(){ return jumpPower; }
    
    bool hasStarPower();
    
private:
    bool shootPower;
    bool jumpPower;
    int invincibilityCounter;
    int tempInvincibility;
    int time_to_recharge_before_next_fire;
    int remaining_jump_distance;
    int hitpoints;
};

#endif // ACTOR_H_
