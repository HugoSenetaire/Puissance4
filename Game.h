#pragma once
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <limits>
#include <vector>
#include <sstream>
#include <cmath>
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <Imagine/LinAlg.h>
#include "Agent.h"
using namespace std;

class Game{
protected :
    Agent* agent1;
    Agent* agent2;
public :
    Game(Agent* agentplayer1, Agent* agentplayer2);
    virtual ~Game();
    virtual int play() = 0 ;
    virtual void reset() = 0;
};

