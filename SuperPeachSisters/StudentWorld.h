#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    std::vector<Actor*>* getActorVector(){ return &actorVector; }
    bool isBlockingObjectAt(double x, double y) ;
    void bonkObjectAt(double x, double y) ;
    bool hasOverlap(double x1, double y1, double x2, double y2) ;
    Peach* getPeachPointer() const { return peachPointer; }
    
    void endLevel(bool isGameWon);
    
    // If a can move to (destx,desty), move it and return true; otherwise
    // bonk the object that's blocking it and return false.
    bool moveOrBonk(Actor *a, int destx, int desty) ;
    
    // If a can move to (destx,desty), move it and return true; otherwise,
    // return false.
    bool moveIfPossible(Actor* a, int destx, int desty) ;
    
    // If a can move to (destx,destx), return true (but don't move it);
    // otherwise (it would be blocked), return false.
    bool isMovePossible(Actor* a, int destx, int desty) ;
    
    // If Peach overlaps bonker, bonk 'er and return true; otherwise,
    // return false.
    bool bonkOverlappingPeach(Actor* bonker) ;

    // If a non-Peach actor overlaps bonker, bonk that non-Peach actor and
    // return true; otherwise, return false.  (The bonker will be Peach.)
    bool bonkOverlappingActor(Actor* bonker) ;

    // If Peach overlaps damager, damage her and return true; otherwise,
    // return false.
    bool damageOverlappingPeach(Actor* damager) ;
    
    // If a non-Peach actor overlaps damager, damage that non-Peach actor
    // and return true; otherwise, return false.
    bool damageOverlappingActor(Actor* damager) ;

    // Return true if a overlaps Peach; otherwise, return false.
    bool overlapsPeach(Actor* a) ;
    
    // If the y cooodinates of a and Peach are at least yDeltaLimit apart,
    // return false; otherwise, set xDeltaFromActor to the difference
    // between Peach's and a's x coordinates (positive means Peach is to
    // the right of a) and return true.
    bool getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor);

private:
    Peach* peachPointer;
    std::vector<Actor*> actorVector;
    int status = GWSTATUS_CONTINUE_GAME;
};

#endif // STUDENTWORLD_H_
