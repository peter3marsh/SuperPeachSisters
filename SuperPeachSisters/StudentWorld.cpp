#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    peachPointer = nullptr;
    actorVector.clear();
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    
    status = GWSTATUS_CONTINUE_GAME;
    
    // Creating a string for the level file based off of the level number
    int k = getLevel();
    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << k << ".txt";
    string level_file = oss.str();
    
    // Checking if there is a problem loading the data file
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    // Populating the StudentWorld with necessary Actors
    else if (result == Level::load_success){
        for (int r=0; r<32; r++){
            for (int c=0; c<32; c++){
                Level::GridEntry ge = lev.getContentsOf(c,r);
                switch (ge){
                    case Level::empty:
                        break;
                    case Level::koopa:
                        actorVector.push_back(new Koopa(this, c, r));
                        break;
                    case Level::goomba:
                        actorVector.push_back(new Goomba(this, c, r));
                        break;
                    case Level::peach:
                        peachPointer = new Peach(this, c, r);
                        break;
                    case Level::flag:
                        actorVector.push_back(new Flag(this, c, r));
                        break;
                    case Level::block:
                        actorVector.push_back(new Block(this, c, r, Block::none));
                        break;
                    case Level::star_goodie_block:
                        actorVector.push_back(new Block(this, c, r, Block::star));
                        break;
                    case Level::piranha:
                        actorVector.push_back(new Piranha(this, c, r));
                        break;
                    case Level::mushroom_goodie_block:
                        actorVector.push_back(new Block(this, c, r, Block::mushroom));
                        break;
                    case Level::flower_goodie_block:
                        actorVector.push_back(new Block(this, c, r, Block::flower));
                        break;
                    case Level::pipe:
                        actorVector.push_back(new Pipe(this, c, r));
                        break;
                    case Level::mario:
                        actorVector.push_back(new Mario(this, c, r));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::endLevel(bool isGameWon){
    if (isGameWon){
        playSound(SOUND_GAME_OVER);
        status = GWSTATUS_PLAYER_WON;
    }
    else{
        playSound(SOUND_FINISHED_LEVEL);
        status = GWSTATUS_FINISHED_LEVEL;
    }
}

int StudentWorld::move()
{
    peachPointer -> doSomething();
    for (auto it = actorVector.begin(); it != actorVector.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();

            if (!peachPointer->isAlive()) {
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }

            if (status == GWSTATUS_FINISHED_LEVEL)
                return GWSTATUS_FINISHED_LEVEL;
            
            if (status == GWSTATUS_PLAYER_WON)
                return GWSTATUS_PLAYER_WON;
        }
    }

    for (auto it = actorVector.begin(); it != actorVector.end(); it++) {
        if (!(*it)->isAlive()) {
            delete *it;
            actorVector.erase(it);
            it = actorVector.begin();
        }
    }

        ostringstream oss;

        oss.fill('0');

        oss << "Lives: ";
        oss << getLives() << "  ";
    
        oss << "Level: ";
        oss << getLevel() << "  ";
    
        oss << "Points: ";
        oss << setw(6) << getScore() << "  ";
    
        if (peachPointer->hasStarPower()){
            oss << "StarPower! ";
        }

        if (peachPointer->hasShootPower()){
        oss << "ShootPower! ";
        }

        if (peachPointer->hasJumpPower()){
            oss << "JumpPower! ";
        }

        setGameStatText(oss.str());

        return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isBlockingObjectAt(double x, double y) {
    for (Actor* it : actorVector){
        if (hasOverlap(x, y, it->getX(), it->getY()) && it->isBlockingObject())
            return true;
    }
    return false;
}

void StudentWorld::bonkObjectAt(double x, double y) {
    for (Actor* it : actorVector){
        if (hasOverlap(it->getX(), it->getY(), x, y+1))
            it->bonk();
    }
}

bool StudentWorld::hasOverlap(double x1, double y1, double x2, double y2) {
    if (x1 + SPRITE_WIDTH - 1 >= x2 && x1 <= x2 + SPRITE_WIDTH - 1)
                if (y1 + SPRITE_HEIGHT - 1 >= y2 && y1 <= y2 + SPRITE_HEIGHT - 1)
                    return true;
    return false;
}

bool StudentWorld::moveOrBonk(Actor *a, int destx, int desty) {
    if (!isBlockingObjectAt(destx, desty)){
        a->moveTo(destx, desty);
        return true;
    }
    else{
        bonkObjectAt(destx, desty);
        return false;
    }
}

bool StudentWorld::moveIfPossible(Actor *a, int destx, int desty) {
    if (!isBlockingObjectAt(destx, desty)){
        a->moveTo(destx, desty);
        return true;
    }
    return false;
}

bool StudentWorld::isMovePossible(Actor* a, int destx, int desty) {
    if (!isBlockingObjectAt(destx, desty))
        return true;
    else
        return false;
}

bool StudentWorld::bonkOverlappingPeach(Actor* bonker) {
    
    if (overlapsPeach(bonker)){
        peachPointer->bonk();
        return true;
    }
    else
        return false;
}

bool StudentWorld::bonkOverlappingActor(Actor* bonker) {
    for (Actor* it : actorVector){
        if (hasOverlap(bonker->getX(), bonker->getY(), it->getX(), it->getY()) && bonker != it){
            it->bonk();
            return true;
        }
    }
    return false;
}

bool StudentWorld::damageOverlappingPeach(Actor* damager) {
    if (overlapsPeach(damager)){
        peachPointer->sufferDamageIfDamageable();
        return true;
    }
    else
        return false;
}

bool StudentWorld::damageOverlappingActor(Actor* damager) {
    for (Actor* it : actorVector){
        if (hasOverlap(damager->getX(), damager->getY(), it->getX(), it->getY()) && damager != it && it->isAlive()){
            it->sufferDamageIfDamageable();
            return true;
        }
    }
    return false;
}

bool StudentWorld::overlapsPeach(Actor* a) {
    if (hasOverlap(peachPointer->getX(), peachPointer->getY(), a->getX(), a->getY())){
        return true;
    }
    else
        return false;
}

bool StudentWorld::getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) {
    if (std::abs(a->getY() - getPeachPointer()->getY()) >= yDeltaLimit)
        return false;
    else
        xDeltaFromActor = getPeachPointer()->getX() - a->getX();
    return true;
}

void StudentWorld::cleanUp()
{
    delete peachPointer;
    peachPointer = nullptr;
    for (Actor* it : actorVector)
        delete it;
    actorVector.clear();
}
