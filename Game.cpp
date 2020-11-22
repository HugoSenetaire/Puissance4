
#include "Game.h"
using namespace std;

Game::Game(Agent* agentplayer1, Agent* agentplayer2){
    agent1 = agentplayer1;
    agent2 = agentplayer2;
}


Game::~Game(){
    delete agent1;
    delete agent2;
}
