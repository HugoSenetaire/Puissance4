#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <limits>
using namespace std;




const pair<int,int> N (0,1);
const pair<int,int> NE (1,1);
const pair<int,int> E (1,0);
const pair<int,int> SE(1,-1);
const pair<int,int> S(0,-1);
const pair<int,int> SW(-1,-1);
const pair<int,int> W(-1,0);
const pair<int,int> NW(-1,1);

const pair<int,int> direction[8]= {N,NE,E,SE,S,SW,W,NW};



// Pour plus tard, transformer cette classe en virtuelle
class Board {
private:
    int m, n;
    int* tab;
    int endGameValue;

public:
    Board();
    Board(int nligcol); // Constructeur carré
    Board(int nlig, int ncol); // Constructeur rectangle
    ~Board(); //Destructeur
    Board(const Board& b); // Constructeur copie
    void operator=(const Board& b);
    void addPiece(int x, int y, int player);
    void reset();

    int getLig() const;
    int getCol() const;
    int getIndex(int x, int y) const;
    int getPiece(int x, int y) const; // Return player index : (0, no player), (-1 player 2), (1 player 1)
    bool colComplete(int colNumber);
    int getEndGameValue() const;
    bool endGame();
    bool winGame(int x, int y , int& player);

    std::vector<std::pair<int,int>> playableMoves(int playerNumber);
    void playMove(const std::pair<int, int>& move, int player); // Change inplace the state of the board

};


class UCTNode{
private :
    Board B;
    bool isRoot;
    bool isTerminal;
    UCTNode* parent;
    std::pair<int,int> lastMove;
    vector<pair<int,int>> remainingMoves;
    vector<UCTNode*> childNodes;
    int player;
    int visits;
    int wins;


public :
    UCTNode(Board currentB,  int player, std::pair<int,int> last_move = std::pair<int,int>(-1,-1), bool isRoot = false);
//    UCTNode(Board currentB, int current_player, bool current_isRoot = false);
    ~UCTNode();

    int getPlayer() const;
    int getWins() const;
    int getVisits() const;
    void addWins(int newWins);
    bool getIsRoot() const;
    bool getIsTerminal() const;
    std::pair<int,int> getLastMove() const;
    UCTNode* selection(bool verbose = false);
    UCTNode* selectionFinal();
    bool isFullyExpanded() const;
    void operator=(const UCTNode& node);// Copie
    UCTNode(const UCTNode& node); // Constructeur par copie
    UCTNode* selectExpandedPath();
    UCTNode* expand();
    void backPropagation(int newWins = 0);
    int rollout(int globalPlayer);

};



class Agent{
public:
    Agent();
    virtual ~Agent(){}
    virtual pair<int,int> getMove(Board& B, int& player) = 0;
};


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

class Puissance4: public Game{
private :
    Board B;
public :
    Puissance4(Agent* agentplayer1, Agent* agentplayer2, int nligcol);
    Puissance4(Agent* agentplayer1, Agent* agentplayer2, int nlig, int ncol);
    ~Puissance4(){}
    virtual int play();
    virtual void reset();

};


class UCTAgent : public Agent{
private :
    int max_iter;
public:
    UCTAgent(int iter_max = 1500);
    ~UCTAgent(){}
    virtual pair<int,int> getMove(Board& B, int& player);
};


class randomAgent: public Agent{
public:
    randomAgent();
    ~randomAgent(){}
    virtual pair<int,int> getMove(Board& B, int& player);
};


