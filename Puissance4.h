#pragma once
#include "Game.h"



const pair<int,int> N (0,1);
const pair<int,int> NE (1,1);
const pair<int,int> E (1,0);
const pair<int,int> SE(1,-1);
const pair<int,int> S(0,-1);
const pair<int,int> SW(-1,-1);
const pair<int,int> W(-1,0);
const pair<int,int> NW(-1,1);

const pair<int,int> direction[8]= {N,NE,E,SE,S,SW,W,NW};


class ConnectFourBoard: public Board {
private:
    int* tab;

public:
    ConnectFourBoard();
    ConnectFourBoard(int nligcol); // Constructeur carré
    ConnectFourBoard(int nlig, int ncol); // Constructeur rectangle
    ~ConnectFourBoard(); //Destructeur
    ConnectFourBoard(const ConnectFourBoard& b); // Constructeur copie
    ConnectFourBoard* clone();
    void operator=(const ConnectFourBoard& b);

    void addPiece(int x, int y, int player);
    bool colComplete(int colNumber);
    int getEndGameValue() const;
    int getPiecePlayer(int x, int y) const;


    void reset();
    bool endGame();
    bool winGame(int x, int y , int& player);
    std::vector<std::pair<int,int>> playableMoves(int playerNumber);
    void playMove(const std::pair<int, int>& move, int player); // Change inplace the state of the board

};



class Puissance4: public Game {
protected:
    ConnectFourBoard B;
public :
    Puissance4(Agent* agentplayer1, Agent* agentplayer2, int nligcol);
    Puissance4(Agent* agentplayer1, Agent* agentplayer2, int nlig, int ncol);
    ~Puissance4(){}
    virtual int play();
    virtual void reset();

};




