#ifndef ACTOR_H
#define ACTOR_H

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//class Tunnelman;
class StudentWorld;

//For GOLD
const int TEMPORARY = 10;//set to dead after a few Ticks
const int PERMANENT = 11;
const int M_PICK = 40;//pickable by Tunnelman
const int P_PICK = 41;//pickupable by Protesters

//For protestors:
const int LEAVE = 20;
const int NOT_LEAVE = 21;
const int REST = 22;

//For Boulder
const int STABLE = 0;
const int WAITING = 1;
const int FALLING = 32;

//For Tunnelman and protesters and all actors
const int DEAD = 1000001;
const int ALIVE =1000000;

//Check the type of an actor to determine...
const int UNDEFINED = 1000;
const int BOULDER = 1001;
const int PROTESTER = 1002;
const int HCPROTESTER = 1003;
const int TUNNELMAN = 1004;
const int SONAR = 1005;
const int EARTH = 1006;


//Base Class-Actor derived from the GraphObject class
class Actor: public GraphObject {
public:
    
    Actor(int imageid, int posx, int posy, Direction dir, double size,
          unsigned int depth, StudentWorld* world)
    : GraphObject(imageid, posx, posy, dir, size, depth), m_world(world) {
        setVisible(true);
        m_visible = true;
    }
    virtual ~Actor(){}
    virtual void doSomething() = 0;
    
    StudentWorld* getWorld() {return m_world;}
    
    bool near(Actor* anotherActor, int distance);
    bool go();
    bool go_up();
    bool go_down();
    bool go_right();
    bool go_left();
    bool go_up(int x, int y);
    bool go_down(int x, int y);
    bool go_right(int x, int y);
    bool go_left(int x, int y);
    bool notBlocked();
    void moveInDirection();
    void changeDirectionRandomly();
    bool changeDirectionRandomlyVertically();
    int m_visible, m_type, m_time = 0, m_health;
    
    int state;
    int score;
    int pick;
    int visibility;
    int TemOrPer;
    int tick;
    
private:
    StudentWorld* m_world;
};


class Earth: public Actor {
public:
    Earth(int x, int y, StudentWorld* world): Actor(TID_EARTH, x, y, right, 0.25, 3, world){
        m_health = 1;
        state = STABLE;
        m_type = EARTH;
    }
    virtual void doSomething();
};


class Tunnelman: public Actor {
public:
    Tunnelman(StudentWorld* world): Actor(TID_PLAYER, 30, 60, right, 1.0, 0, world){
        setVisible(true);
        m_visible = true;
        m_type = TUNNELMAN;
        m_health = 10;
        state = STABLE;
    }
    virtual void doSomething();
    //void annoyed();
    bool noBoulder(Direction dir);

};

class Boulder: public Actor {
public:
    Boulder(int x, int y,  StudentWorld* world):Actor(TID_BOULDER,x,y,right,1.0,2,world){
        setVisible(true);
        m_visible = true;
        m_health = 1;
        state = STABLE;
        m_type = EARTH;
    }
    virtual void doSomething();
    
    //~Boulder();
};

class Barrel: public Actor {
public:
    Barrel(int x, int y, StudentWorld* world):Actor(TID_BARREL,x,y,down, 1.0, 1, world){
        setVisible(false);
        m_visible = false;
        m_health = 1;
        state = STABLE;
    }
    virtual void doSomething();
   // ~Barrel();
};


class Gold: public Actor {
public:
    Gold(int x, int y,bool vs, int T_P, int PICK, StudentWorld* world);
    virtual void doSomething();
    //~Gold();
};

class Sonar: public Actor {
public:
    Sonar(int x, int y,  StudentWorld* world):Actor(TID_SONAR,x,y,right,1.0,2,world){
        setVisible(true);
        m_visible = true;
        m_health = 1;
        state = STABLE;
        m_type = SONAR;
    }
    virtual void doSomething();
private:
};

class Water: public Actor {
public:
    Water(int x, int y, StudentWorld* world):Actor(TID_WATER_POOL,x,y,right,1.0,2,world){
        setVisible(true);
        m_visible = true;
        m_health = 1;
        state = STABLE;
    }
    virtual void doSomething();
    //~Water();
};

class Squirt: public Actor {
public:
    Squirt(int x, int y, StudentWorld* world, Direction dir):Actor(TID_WATER_SPURT,x,y,dir,1.0,1,world){
        setVisible(true);
        m_visible = true;
        m_health = 1;
        state = 0;
    }
    void doSomething();
   // ~Squirt();
};

class Protester: public Actor {
public:
    Protester(StudentWorld* world);
    virtual void doSomething();
    void moveAround(int posx, int posy);
    bool faceTunnelmanStraight();
    bool faceTunnelman();
    bool notDirectlySee();
    int tick_count, tick_to_wait, stunned_tick;
    int numSquaresToMoveInCurrentDirection;
    int shout_count, vertical_turn_count;
    int hit_points;
    
};

class HcProtester:public Protester{
public:
    HcProtester(StudentWorld* world):Protester(world){
        setVisible(true);
        m_visible = true;
        m_health = 20;
        state = STABLE;
        m_type = HCPROTESTER;
    };
};


#endif // ACTOR_H_






