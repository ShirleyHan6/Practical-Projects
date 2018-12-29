#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include <vector>
#include <cmath>
#include <queue>
#include <unistd.h>
struct Point{
    int x;
    int y;
};

struct queueNode{
    Point point;
    int distance;
};

//class StudentWorld;
//Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor
bool Actor::near(Actor* anotherActor, int distance){
    return sqrt(pow(anotherActor->getX() - getX(),2) + pow(anotherActor->getY() - getY(),2)) <= distance;
}

bool Actor::go_up() {
    if (getY() >= 60)
        return false;
    for (int i = getX(); i < getX()+4; i++)
        if (getWorld()->m_earth[i][getY()+4] != nullptr)
            return false;
    return true;
}

bool Actor::go_down() {
    if (getY() <= 0)
        return false;
    for (int i = getX(); i < getX()+3; i++)
        if (getWorld()->m_earth[i][getY()-1] != nullptr)
            return false;
    return true;
}

bool Actor::go_right() {
    if (getX()>= 60)
        return false;
    for (int j = getY(); j <getY()+4; j++)
        if (getWorld()->m_earth[getX()+4][j] != nullptr)
            return false;
    return true;
}

bool Actor::go_left() {
    if (getX()<= 0)
        return false;
    for (int j = getY(); j < getY()+4; j++)
        if (getWorld()->m_earth[getX()-1][j] != nullptr)
            return false;
    return true;
}

bool Actor::go(){
    return go_left()||go_right()||go_up()||go_down();
}

bool Actor::notBlocked()
{
    for (int i=0; i<getWorld()->actors.size(); i++) {
        Actor* actor = getWorld()->actors[i];
        int x = actor->getX();
        int y = actor->getY();
        int dir = getDirection();
        if (actor->m_type == EARTH) {
            if (dir == up && y>getY() && y-getY() <= 4 && abs(x-getX())<4)
                return false;
            if (dir == down && getY()>y && getY()-y <= 4 && abs(x-getX()) < 4)
                return false;
            if (dir == right && x>getX() && x-getX() <= 4 && abs(y-getY()) < 4)
                return false;
            if (dir == left && getX()>x && getX()-x <= 4 && abs(y-getY()) < 4)
                return false;
        }
        if(actor->m_type == BOULDER){
            if(near(actor, 3))
                return false;
        }
    }
    
    return true;
}

//For protesters moving
bool Actor::go_up(int x, int y) {
    if (y >= 60)
        return false;
    for (int i = x; i < x+4; i++)
        if (getWorld()->m_earth[i][y+4] != nullptr)
            return false;
    return true;
}

bool Actor::go_down(int x, int y) {
    if (y <= 0)
        return false;
    for (int i = x; i < x+4; i++)
        if (getWorld()->m_earth[i][y-1] != nullptr)
            return false;
    return true;
}

bool Actor::go_right(int x, int y) {
    if (x>= 60)
        return false;
    for (int j = y; j <y+4; j++)
        if (getWorld()->m_earth[x+4][j] != nullptr)
            return false;
    return true;
}

bool Actor::go_left(int x, int y) {
    if (x<= 0)
        return false;
    for (int j = y; j < y+4; j++)
        if (getWorld()->m_earth[x-1][j] != nullptr)
            return false;
    return true;
}

void Actor::moveInDirection()
{
    if (getDirection() == up) moveTo(getX(), getY()+1);
    if (getDirection() == down) moveTo(getX(), getY()-1);
    if (getDirection() == right) moveTo(getX()+1, getY());
    if (getDirection() == left) moveTo(getX()-1, getY());
}

//Check if there is no boulder
bool Tunnelman::noBoulder(Direction dir) {
    for (int i=0; i<getWorld()->actors.size(); i++) {
        Actor* actor = getWorld()->actors[i];
        int x = actor->getX();
        int y = actor->getY();
        
        if (actor->m_type == BOULDER) {
            if (dir == up && y>getY() && y-getY() <= 4 && abs(x-getX())<4)
                return false;
            if (dir == down && getY()>y && getY()-y <= 4 && abs(x-getX()) < 4)
                return false;
            if (dir == right && x>getX() && x-getX() <= 4 && abs(y-getY()) < 4)
                return false;
            if (dir == left && getX()>x && getX()-x <= 4 && abs(y-getY()) < 4)
                return false;
        }
    }
    return true;
}

/*
void Tunnelman::annoyed(){
    m_health-=2;
}
*/

//Earth
void Earth::doSomething() {

    //Find out if Tunnelman is alive
    if (getWorld()->player_health == 0)
        return;
    
   int x = getWorld()->getPlayer()->getX();
   int y = getWorld()->getPlayer()->getY();
    
    if (x <= getX() && getX() <= x + 3)
        if (y <= getY() && getY() <=y+3) {
            state = DEAD;
        }
    return;
}
//Tunnelman
void Tunnelman::doSomething() {
    if (getWorld()-> player_health== 0)
        return;
    int ch = 0;
    //Get a key and move the tunnelman according to the key.
    if (getWorld()->getKey(ch) == true) {
        switch(ch) {
            //Moving in different directions
            case KEY_PRESS_UP:
                if (getDirection() == up){
                    if(getY()<60&&noBoulder(up)){
                        moveTo(getX(), getY()+1);
                        getWorld()->playSound(SOUND_DIG);
                    }
                    else{
                        getWorld()->playSound(SOUND_DIG);
                        moveTo(getX(), getY());
                    }
                    
                }
                else
                    setDirection(up);
                break;
                
            case KEY_PRESS_DOWN:
                if (getDirection() == down){
                    if(getY()>0&&noBoulder(down)){
                        moveTo(getX(), getY()-1);
                        getWorld()->playSound(SOUND_DIG);
                    }
                    else{
                        moveTo(getX(), getY());
                    }
                    
                }
                else
                    setDirection(down);
                break;
                
            case KEY_PRESS_RIGHT:
                if (getDirection() == right){
                    if(getX()<60&&noBoulder(right)){
                        moveTo(getX()+1, getY());
                        getWorld()->playSound(SOUND_DIG);
                    }
                }
                else
                    setDirection(right);
                break;
                
            case KEY_PRESS_LEFT:
                if (getDirection() == left){
                    if(getX()>0&&noBoulder(left)){
                        moveTo(getX()-1, getY());
                        getWorld()->playSound(SOUND_DIG);
                    }
                }
                else
                    setDirection(left);
                break;
                
            //squirt the gun
            case KEY_PRESS_SPACE:
                if(getWorld()->getWater()==0)
                    break;
                getWorld()->getLeftWater();
                if(getDirection()==up){
                    Squirt *squirt = new Squirt(getX(),getY()+4,getWorld(),up);
                    getWorld()->actors.push_back(squirt);
                }
                else if(getDirection()==down){
                    Squirt *squirt = new Squirt(getX(),getY()-4,getWorld(),up);
                    getWorld()->actors.push_back(squirt);
                }
                    
                else if(getDirection()==right)
                {
                    Squirt *squirt = new Squirt(getX()+4,getY(),getWorld(),up);
                    getWorld()->actors.push_back(squirt);
                }
                else{
                    Squirt *squirt = new Squirt(getX()-4,getY(),getWorld(),up);
                    getWorld()->actors.push_back(squirt);
                }
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                break;
            //Use the sonar
            case 'z':
                if(getWorld()->getSonar()==0)
                    break;
                getWorld()->getSonarLeft();
                for(int i=0;i<getWorld()->actors.size();i++){
                    if(near(getWorld()->actors[i], 12))
                        getWorld()->actors[i]->setVisible(true);
                }
                getWorld()->playSound(SOUND_SONAR);
                break;
            case KEY_PRESS_TAB:
                getWorld()->actors.push_back(new Gold(getX(), getY(), true, TEMPORARY,P_PICK,getWorld()));
                getWorld()->dropGold();
                for(int i=0;i<getWorld()->actors.size();i++){
                    if(near(getWorld()->actors[i], 3)){
                        if(getWorld()->actors[i]->m_type == PROTESTER){
                            getWorld()->actors[i]->state=DEAD;
                        }
                        else if(getWorld()->actors[i]->m_type == HCPROTESTER){
                            getWorld()->actors[i]->state = REST;
                        }
                    }
                }
                break;
            //Replay the game
            case KEY_PRESS_ESCAPE:
                getWorld()->player_health = 0;
    
                
        }
    }
    return;
}

//Boulder
void Boulder::doSomething()
{
    //Alive?
    if (state==DEAD)
        return;
    
    if(state==STABLE){
        for (int i = 0; i < 4; i++) {
            if (getWorld()->m_earth[getX()+i][getY()-1] != nullptr)
                return;
        }
        state = 2;
        return;
    }
    
    if(state<32){
        state++;
        return;
    }
    //and the earth is nullptr
    if(state==FALLING){
        getWorld()->playSound(SOUND_FALLING_ROCK);
        state++;
        return;
    }
    //Falling?
    if(state==33){
        for(int i=0;i<getWorld()->actors.size();i++){
            Actor* actor = getWorld()->actors[i];
            
            if(actor->m_type==PROTESTER||
               actor->m_type==HCPROTESTER||actor->m_type==TUNNELMAN){
                if(near(actor,3)){
                    if(actor->m_type==PROTESTER){
                        getWorld()->increaseScore(100);
                        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
                        actor->state = DEAD;
                    }
                    if(actor->m_type==HCPROTESTER){
                        getWorld()->increaseScore(250);
                        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
                        actor->state = DEAD;
                    }
                    if(actor->m_type==TUNNELMAN){
                        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                        getWorld()->player_health = 0;
                        //actor->state = DEAD;
                    }
                }
            }
    }
    }
    
    if(getY()==0)
        state = DEAD;
    
    for (int i=0; i<4; i++)
        if (getWorld()->m_earth[getX()+i][getY()-1] != nullptr)
            state = DEAD;
    
    if(state==33)
        moveTo(getX(),getY()-1);
    return;
}

void Squirt::doSomething()
{
    if(state==4){
        state = DEAD;
        return;
    }
    else
        state++;
    
    for(int i=0;i<getWorld()->actors.size();i++){
        Actor* actor = getWorld()->actors[i];
        if(actor->m_type==BOULDER&&near(actor,3)){
            state = DEAD;
            return;
        }
    }
    for(int i=0;i<getWorld()->actors.size();i++){
        Actor* actor = getWorld()->actors[i];
        if(near(actor, 3)){
            if(actor->m_type==PROTESTER||actor->m_type==HCPROTESTER){
                actor->m_health-=2;
                actor->state = LEAVE;
                break;
        }
    }
    }
    setDirection(getWorld()->getPlayer()->getDirection());
    if(getDirection()==up && go_up())
        moveTo(getX(),getY()+1);
    else if(getDirection()==down && go_down())
        moveTo(getX(),getY()-1);
    else if(getDirection()==right && go_right())
        moveTo(getX()+1,getY());
    else if(getDirection()==left && go_left())
        moveTo(getX()-1,getY());
    else{
        state= DEAD;
        return;
    }
    
    return;
    
}


    
void Barrel::doSomething()
{
    if(state==DEAD)
        return;
    else if(near(getWorld()->getPlayer(),3)){
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->getLeftBarrel();
        state = DEAD;
    }
    if(near(getWorld()->getPlayer(),4)){
        setVisible(true);
        m_visible = true;
        return;
    }

}

Gold::Gold(int x, int y,bool vs, int T_P, int PICK, StudentWorld* world):Actor(TID_GOLD,x,y,right,1.0,2,world){
    visibility = vs;
    setVisible(vs);
    m_visible = vs;
    m_health = 1;
    state = ALIVE;
    pick = PICK;
    TemOrPer = T_P;
    tick = max(5-getWorld()->getLevel()/2, 2);
}

void Gold::doSomething()
{
    if(state==DEAD)
        return;
    else if(near(getWorld()->getPlayer(),3)&&pick==M_PICK){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addGold();
        getWorld()->increaseScore(10);;
        state = DEAD;
        return;
    }
    else if(near(getWorld()->getPlayer(),4)){
        setVisible(true);
        m_visible = true;
        return;
    }
    else if(pick==P_PICK){
        if(tick!=0)
            tick--;
        else
            state = DEAD;
        for(int i=0;i<getWorld()->actors.size();i++){
            Actor* actor = getWorld()->actors[i];
            if(actor->m_type==PROTESTER||
               actor->m_type==HCPROTESTER){
                if(near(actor,3)){
                    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
                    getWorld()->increaseScore(25);
                    state = DEAD;
                    actor->state = LEAVE;
                }
            }
        }
    }
    /*
    if(state!=0&&state%getWorld()->T_water==0)
        state = DEAD;
     */
    return;
}

void Water::doSomething()
{
    if(state==DEAD)
        return;
    else if(near(getWorld()->getPlayer(),3)){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addWater();
        getWorld()->getPlayer()->getWorld()->increaseScore(100);
        state = DEAD;
        return;
    }
    else{
        state++;
    }
    /*
    if(state!=0&&state%getWorld()->T_water==0)
        state = DEAD;*/
    return;
}

void Sonar::doSomething()
{
    if(state==DEAD)
        return;
    else if(near(getWorld()->getPlayer(),3)){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addSonar();
        getWorld()->getPlayer()->getWorld()->increaseScore(75);
        state = DEAD;
        return;
    }
    else{
        state++;
    }
    if(state!=0&&state%getWorld()->T_sonar==0)
        state = DEAD;
}

Protester::Protester(StudentWorld* world)
: Actor(TID_PROTESTER, 60, 60, left, 1.0, 0, world) {
    setVisible(true);
    state = NOT_LEAVE;
    tick_count = 0;
    tick_to_wait = max(0, 3 - getWorld()->getLevel()/4);
    stunned_tick = max(50, 100-getWorld()->getLevel()*10);
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
    vertical_turn_count = 0;
    m_health= 5;
    m_type = PROTESTER;
    shout_count = 0;
}

void Protester::doSomething()
{
    
    if(shout_count<100){
        shout_count++;
    }
    //check alive
    if(state == DEAD)
        return;
    tick_count ++;
    if(tick_count%tick_to_wait!=1){
        state = REST;
        return;
    }
    if(state==REST){
        if(m_health <= 0){
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            state = DEAD;
            return;
        }
    }
    if(state==LEAVE){
        if(getX() == 60 && getY() == 60){
            state = DEAD;
        }
        moveAround(60,60);
        return;
    }
    if(near(getWorld()->getPlayer(), 4) && faceTunnelman()){
        if(shout_count==100){
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->player_health-=2;
            shout_count = 0;
            state = REST;

        }
        return;
    }
    else if(!near(getWorld()->getPlayer(),4)&&faceTunnelmanStraight()&&notBlocked()){
        if(getWorld()->getPlayer()->getX()==getX()){
            if(getWorld()->getPlayer()->getY()-getY()>0){
                setDirection(up);
                moveTo(getX(),getY()+1);
            }
            else if(getWorld()->getPlayer()->getY()-getY()<0){
                setDirection(down);
                moveTo(getX(),getY()-1);
            }
        }
        else{
            if(getWorld()->getPlayer()->getX()-getX()>0){
                setDirection(right);
                moveTo(getX()+1,getY());
            }
            else if(getWorld()->getPlayer()->getX()-getX()<0){
                setDirection(left);
                moveTo(getX()-1,getY());
            }
        }
        return;
    }
    
    else if(notDirectlySee()){
        if(go_up()){
                setDirection(up);
                moveTo(getX(),getY()+1);;
            return;
        }
        if(go_down()){
            setDirection(down);
            moveTo(getX(),getY()-1);;
            return;
        }
        if(go_left()){
            setDirection(left);
            moveTo(getX()-1,getY());;
            return;
        }
        if(go_right()){
            setDirection(right);
            moveTo(getX()+1,getY());
            return;
        }
    }
    
    if(getDirection()==up&&go_up()){
            moveTo(getX(),getY()+1);
        return;
    }
    if(getDirection()==down&&go_down()){
        moveTo(getX(),getY()-1);
        return;
    }
    if(getDirection()==right&&go_right()){
        moveTo(getX()+1,getY());
        return;
    }
    if(getDirection()==left&&go_left()){
        moveTo(getX()-1,getY());
        return;
    }
    return;
    
}

void Protester::moveAround(int px, int py)
{
    if (getX() == px && getY() == py) {
        return;
    }
    int pre[64*64];
    bool visited[64*64];
    memset(visited, false, sizeof(visited));
    int a, x, y;
    std::queue<int> Q;
    Q.push(getY() * 64 + getX());
    visited[getY() * 64 + getX()] = true;
    
    while (!Q.empty()) {
        a = Q.front(); Q.pop();
        x = a % 64;
        y = a / 64;
        if (x == px && y == py) break;
        if (go_up(x, y) && !visited[a+64]) {
            Q.push(a + 64);
            pre[a+64] = a;
            visited[a+64] = true;
        }
        if (go_down(x, y) && !visited[a-64]) {
            Q.push(a-64);
            pre[a-64] = a;
            visited[a-64] = true;
        }
        if (go_right(x, y) && !visited[a+1]) {
            Q.push(a + 1);
            pre[a+1] = a;
            visited[a+1] = true;
        }
        if (go_left(x, y) && !visited[a-1]) {
            Q.push(a - 1);
            pre[a-1] = a;
            visited[a-1] = true;
        }
    }
    int curNode = py*64 + px, preNode = pre[curNode];
    while (true) {
        x = preNode % 64;
        y = preNode / 64;
        if (x == getX() && y == getY()) {
            int curx = curNode % 64, cury = curNode / 64;
            if (cury > y) setDirection(up);
            if (cury < y) setDirection(down);
            if (curx > x) setDirection(right);
            if (curx < x) setDirection(left);
            moveTo(curx, cury);
            break;
        }
        preNode = pre[preNode];
        curNode = pre[curNode];
    }
}


bool Protester::notDirectlySee()
{
    return !faceTunnelman()&&!faceTunnelmanStraight();
}

bool Protester::faceTunnelman()
{
    if((getDirection()==up&&getWorld()->getPlayer()->getY()>getY())
       ||(getDirection()==down&&getWorld()->getPlayer()->getY()<getY())
       ||(getDirection()==right&&getWorld()->getPlayer()->getX()>getX())
       ||(getDirection()==left&&getWorld()->getPlayer()->getX()<getX())
       )
        return true;
    return false;
}
bool Protester::faceTunnelmanStraight()
{
    int x = getWorld()->getPlayer()->getX();
    int y = getWorld()->getPlayer()->getY();
    
    if(x!=getX()&&y!=getY())
        return false;
    
    if(x==getX()){
    for(int i = min(x, getX());i<max(x, getX());i++){
        for(int j = y;j<j+4;j++){
            if(getWorld()->m_earth[i][j]!=nullptr)
                return false;
        }
    }
    }
    
    if(y==getY()){
    for(int i = min(y, getY());i<max(y, getY());i++){
        for(int j = x;j<j+4;j++){
            if(getWorld()->m_earth[i][j]!=nullptr)
                return false;
        }
    }
    }
    return true;
}


