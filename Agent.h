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

using namespace std;


//class Piece{
//protected:
//    int player;
//public:
//    Piece(int player);
//    virtual ~Piece();
//    virtual Piece* clone();
//    int getPlayer() const;
//};


class Move{
protected:
    int player;
public:
    Move(int player):player(player){}
    virtual ~Move(){}
    virtual Move* clone() = 0;
    virtual int getx() const=0 ;
    virtual int gety() const =0;
    int getPlayer() const;


};


class Board{
protected:
    int m, n;
//    Piece* tab;
    int endGameValue;

public:
    Board();
    Board(int nligcol); // Constructeur carré
    Board(int nlig, int ncol); // Constructeur rectangle
    Board(const Board& b); // Constructeur copie
    virtual Board* clone()=0;

    int getLig() const;
    int getCol() const;
    int getIndex(int x, int y) const;
    int getEndGameValue() const;


    virtual ~Board(); //Destructeur
    virtual int getPiecePlayer(int x, int y) const =0;// Return player index : (0, no player), (-1 player 2), (1 player 1)
//    virtual Piece* getPiece(int x, int y) const =0;
//    virtual void addPiece(int x, int y, Piece& Piece) = 0;



    //    void operator=(const Board& b);

    virtual void reset() = 0;
    virtual bool endGame() = 0;
    virtual bool winGame(int x, int y , int& player) = 0;

    // TODO Faire une classe move
    virtual std::vector< Move* > playableMoves(int playerNumber) = 0;
    virtual void playMove(const Move* move, int player) = 0; // Change inplace the state of the board
};


//======================== CLASS AGENT ================================
class Agent{
public:
    Agent();
    virtual ~Agent(){}
    virtual Move* getMove(Board& B, int& player) = 0;
};



class UCTAgent : public Agent{
protected :
    int max_iter;
public:
    UCTAgent(int iter_max = 1500);
    ~UCTAgent(){}
    virtual Move* getMove(Board& B, int& player);
};


class randomAgent: public Agent{
public:
    randomAgent();
    ~randomAgent(){}
    virtual Move* getMove(Board& B, int& player);
};


//======================== CLASS NODE ================================

class UCTNode{
private :
    Board* B;
    bool isRoot;
    bool isTerminal;
    UCTNode* parent;
    Move* lastMove;
    vector< Move* > remainingMoves;
    vector<UCTNode*> childNodes;
    int player;
    int visits;
    int wins;


public :
    UCTNode(Board& currentB,  int player, Move* last_move, bool isRoot = false);
    UCTNode(Board& currentB, int current_player, bool current_isRoot = false);
    ~UCTNode();

    int getPlayer() const;
    int getWins() const;
    int getVisits() const;
    void addWins(int newWins);
    bool getIsRoot() const;
    bool getIsTerminal() const;
    Move* getLastMove() const;
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

