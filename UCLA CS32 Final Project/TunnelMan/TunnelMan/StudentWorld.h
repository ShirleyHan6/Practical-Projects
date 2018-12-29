#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "Actor.h"
#include <string>
#include "GameWorld.h"
#include <vector>

int max(int a, int b);
int min(int a, int b);

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

struct cor{
    int x;
    int y;
};
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();

    //Tunnelman, earth, barrels, gold, boulders, and protestors
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    //Sonar
    virtual void init_Sonar();
    //Pool
    virtual void init_Water();
    //New protestors
    virtual void init_Protester();
    
    int T = max(25,200-getLevel());
    int P = min(15,2+getLevel()*1.5);
    int boulder = min(getLevel()/2+2,9);
    int T_sonar = max(100,300-10*getLevel());
    int T_water = max(100,300-10*getLevel());
    int T_gold =  max(5-getLevel()/2, 2);
    int T_barrel = max(2+getLevel(), 21);
    int T_wait = max(0, 3-getLevel()/4);
    int T_protester = max(50, 100-getLevel()*10);
    int T_protester_stare = max(50, 100-getLevel()*10);
    int tick;
    void text();
    
    //Tunnelman
    Tunnelman* getPlayer(){return  player;}
    int player_health = 10;
    
    //Distributing
    void deleteEarth(int x, int y);
    cor distribute();
    
    //Other actors
    int getWater(){return m_water;}
    int getSonar(){return m_sonar;}
    int getGold(){return m_gold;}
    int getBarrel(){return m_barrel;}
    void getLeftBarrel(){m_barrel-=1;}
    void setBarrel(){m_barrel = min(2 + getLevel(),21);}
    void addWater(){m_water+=5;}
    void getLeftWater(){m_water-=1;}
    void addGold(){m_gold+=1;}
    void dropGold(){m_gold>0?m_gold-=1:m_gold+=0;}
    void getSonarLeft(){m_sonar-=1;}
    void addSonar(){m_sonar++;}
    int getProtestor(){return m_protester;}
    
    //Initialize a datastructure of Actors
    std::vector<Actor*>actors;
    
    //Initialize a datastructure of Earth
    Earth* m_earth[128][128];
private:
    Tunnelman * player;
    int m_water, m_gold, m_sonar;
    int m_boulder,m_barrel, m_protester;
};

#endif // STUDENTWORLD_H_
