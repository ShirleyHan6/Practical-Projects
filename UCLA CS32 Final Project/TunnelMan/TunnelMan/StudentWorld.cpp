#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "GameWorld.h"
#include "GameController.h"
#include "StudentWorld.h"

class Actor;
class Tunnelman;
using namespace std;

int max(int a, int b){return a<b?b:a;}
int min(int a, int b){return a>b?b:a;}

bool ifNear(Actor*actor,int x, int y)
{
    int dis = sqrt(pow(actor->getX()-x,2)+pow(actor->getY()-y,2));
    return dis <= 6;
}

cor StudentWorld::distribute()
{
    int x;
    int y;
    int i = rand()%2;
    if(actors.size()!=0){
        x = i==0?rand()%30:34+rand()%26;
        y = rand()%20+36;
        for(int i=0;i<actors.size();i++){
            while(ifNear(actors[i],x,y)){
                x = i==0?rand()%30:34+rand()%26;
                y = rand()%20+36;
            }
        }
    }
    else{
        x = i==0?rand()%30:34+rand()%26;
        y = rand()%20+36;
    }
    cor cdn;
    cdn.x = x;
    cdn.y = y;
    return cdn;
    
}
GameWorld* createStudentWorld(string assetDir)
{
    
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
:GameWorld(assetDir)
{
    player_health = 10;
    m_water = 5 ;
    m_sonar = 1;
    m_gold = 0;
    m_barrel = T_barrel;
    
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    //Initialize an oil field with "Assets".
    GameController controller;
    controller.doSomething();
    StudentWorld world("Assets");
    
    //Initialize a tunnelman and put it at the right starting position
    player = new Tunnelman(this);
    actors.push_back(player);
    player_health = 10;
    
    //Earth
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 60; j++)
            m_earth[i][j] = nullptr;
    
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 60; j++) {
            m_earth[i][j] = new Earth(i, j, this);
            actors.push_back(m_earth[i][j]);
        }
    
    for (int i_shaft = 30; i_shaft <34 ; i_shaft++)
    for (int j = 4; j < 60; j++) {
        m_earth[i_shaft][j]->state = DEAD;
    }
    
    //Boulders
    for (int i=0; i<min(getLevel()/2+2,9); i++){
          cor cdn = distribute();
          deleteEarth(cdn.x,cdn.y);
          actors.push_back(new Boulder(cdn.x, cdn.y, this));
    }
    //Gold Nuggets
    for (int i=0; i<max(5-getLevel()/2,2); i++){
        cor cdn = distribute();
        actors.push_back(new Gold(cdn.x, cdn.y, false, PERMANENT, M_PICK, this));
    }
        
    
    //Barrels
    setBarrel();
    for (int i=0; i< getBarrel(); i++){
        cor cdn = distribute();
        actors.push_back(new Barrel(cdn.x, cdn.y, this));
    }
    
    actors.push_back(new Protester(this));
   // int barrel = getBarrel();
    m_protester = 1;
    
    //Squirt
    
    return GWSTATUS_CONTINUE_GAME;
}



void StudentWorld::deleteEarth(int x, int y)
{
    for(int i=x;i<x+4;i++){
        for(int j=y;j<y+4;j++){
            if(m_earth[i][j])
                m_earth[i][j]->state = DEAD;
        }
    }
}

void StudentWorld::init_Protester()
{
    int p_Hc = min(90,getLevel()*10+30);
    bool protester = (rand()%100)<p_Hc;
    protester?actors.push_back(new Protester(this)):actors.push_back(new HcProtester(this));
    m_protester++;
}

void StudentWorld::init_Sonar()
{
    actors.push_back(new Sonar(0,60,this));
}

void StudentWorld::init_Water()
{
    actors.push_back(new Water(30, 30, this));
}


//Each actor does something during each tick
int StudentWorld::move()
{
    if (player_health == 0) {
        cleanUp();
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    text();
    
    if(tick%T==0&&m_protester<P){
        init_Protester();
    }
    tick++;
    for(int i=0;i<actors.size();i++){
        actors[i]->doSomething();
    }
    
    //Protester steps on a gold, activate the gold, and
    
    //Add a goodie
    int G = getLevel()*25+300;
    bool goodie = (rand()%G)==0;
    bool water = (rand()%5)==1;
    bool sonar = (rand()%5)<4;
    if(goodie&&water)
        init_Water();
    else if(goodie&&sonar)
        init_Sonar();
    for(int i=0; i<actors.size(); i++){
        //if the actor is alive
       if(actors[i]->m_health!=0)
            actors[i]->doSomething();
    }
    
    //Remove dead actors
    for(int i=0;i<actors.size();i++){
        if(actors[i]->state == DEAD){
            if(actors[i]->m_type == PROTESTER||actors[i]->m_type == HCPROTESTER)
                m_protester--;
            if(actors[i]->m_type == EARTH){
                m_earth[actors[i]->getX()][actors[i]->getY()] = nullptr;
            }
            delete actors[i];
            //actors[i] = nullptr;
            actors.erase(actors.begin()+i);
            i--;
        }
    }
    //Finished the level
    if (getBarrel() == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //Tunnelman lost all lives
    if (getLives() == 0) {
        cleanUp();
        playSound(SOUND_PLAYER_GIVE_UP);
        return GWSTATUS_PLAYER_DIED;
    }
    
    return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::cleanUp()
{
    for(int i=0; i<actors.size(); i++){
        delete actors[i];
        //actors.erase(actors.begin()+i);
    }
    actors.clear();
}

void StudentWorld::text()
{
    int level = getLevel();
    int lives = getLives();
    int health = player_health;
    int water = getWater();
    int gold = getGold();
    int barrels = getBarrel();
    int sonar = getSonar();
    int score = getScore();

    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(6) << score << "  ";
    
    oss << "Lvl:";
    oss.fill(' ');
    oss << setw(2) << level << "  ";
    
    oss << "Lives:";
    oss << setw(1) << lives << "  ";
    
    oss << "health:";
    oss.fill(' ');
    oss << setw(3) << health*10 << "%  ";
    
    oss << "water:";
    oss.fill(' ');
    oss << setw(2) << water << "  ";
    
    oss << "gold:";
    oss.fill(' ');
    oss << setw(2) << gold << "  ";
    
    oss << "barrels:";
    oss.fill(' ');
    oss << setw(2) << barrels << "  ";
    
    oss << "sonar:";
    oss.fill(' ');
    oss << setw(2) << sonar << "  ";
    
    
    
    string s = oss.str();
    setGameStatText(s);
    
}

