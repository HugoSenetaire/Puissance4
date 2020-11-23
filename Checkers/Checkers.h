#pragma once
#include "../Game.h"


// 1 Case :

const int NW =-6;
const int NE =-5;
const int SE =+5;
const int SW =+4;



// 2 Cases :

const int NW2 = -11;
const int NE2 = -9;
const int SE2 = 11;
const int SW2 = 9;



// All classes:
class Piece;
class Man;
class King;
class BoardCheckers;
class MoveCheckers;


class Piece {
protected:
    int position;
    int player;
public:
    //Defined Function :
    virtual ~Piece(){}
    Piece(int pos, int player): position(pos), player(player){}
    Piece(const Piece& p){
        position = p.position;
        color = p.color;
    }
    int getPosition(){return position;}
    void setPosition(int pos){position = pos;}
    virtual bool isMan() const =0;
    virtual bool isKing() const {return !isMan();}
    virtual void killFreeMove(Board& B, vector<Move> &possibleMoves) =0;
    virtual void killingMove(Board& B, vector<Move> &possibleMoves)=0;
    virtual void select(Board& B,vector<Move> &possibleMoves)=0;
    virtual Piece* clone()=0;
};

class Man : public Piece {
public:
    ~Man(){}
    Man(int pos, std::string col) : Piece(pos,col){}
    Man(const Man& m) : Piece(m.position, m.color){}
    virtual bool isMan() const { return true;}
    virtual void killFreeMove(Board& B,vector<Move> &possibleMoves);
    virtual void killingMove(Board& B, vector<Move> &possibleMoves);
    virtual void select(Board& B, vector<Move> &possibleMoves);
    virtual Piece* clone();
};



class King : public Piece {
public :
    ~King(){}
    virtual bool isMan() const {return false;}
    King(int pos, std::string col) : Piece(pos,col){}
    King(const King& k) : Piece(k.position, k.color){}
    virtual void killFreeMove(Board& B,vector<Move> &possibleMoves);
    virtual void killingMove(Board& B, vector<Move> &possibleMoves);
    virtual Piece* clone();
    virtual void select(Board& b, vector<Move> &possibleMoves);

};


class BoardCheckers : public Board{
    std::vector<Piece*> pieces;
public :
    BoardCheckers();
    ~BoardCheckers();
    BoardCheckers(const BoardCheckers& b);
    void operator=(const BoardCheckers& b);
    int index_man_here(int pos);
    bool isManHere(int pos);
    bool isKingHere(int pos);
    bool isPieceHere(int pos);
    void playMove(const Move& m, bool inSelect=false);
    void killAt(int pos);
    Piece* getPiece(int index){return pieces[index];}
    void turnToKing(int pos);
    int nbPieces() const{return pieces.size();}
    float evaluateBetter(float manWeight, float kingWeight,float nbMoveWeight, float advancementForwardWeight, float centralWeight, string color);
    std::pair<float,Move> bestMoveAlphaBeta2(string color,int depth, float manWeight, float kingWeight,float nbMoveWeight, float centralWeight, float advanceWeight, bool maxNode,float alpha, float beta );
    map<int,vector<Move> > playableMoves(string color);
    int timeMatch();


    void reset();
    bool endGame();
//    bool winGame(int x, int y , int& player);

    // TODO Faire une classe move
    virtual std::vector< Move* > playableMoves(int playerNumber) = 0;
    virtual void playMove(const Move* move, int player) = 0; // Change inplace the state of the board
};



class MoveCheckers : public Move {
private:
    vector<int> path;
    int kills;
public:
    MoveCheckers() {
        kills=-1;
    }
    MoveCheckers(const Move& m);
    MoveCheckers(int s, int a, int k){
        path.push_back(s);
        path.push_back(a);
        kills = k;
    }
    MoveCheckers(vector<int> path0,int nbKills) {
        path=path0;
        kills=nbKills;
    }
    void operator=(const MoveCheckers& m);
    bool operator==(MoveCheckers& m);
    bool operator<(const MoveCheckers& m) const;
    int getStart() {return path[0];}
    int getArrival() {return path[path.size()-1];}
    int getx() const{ return -1;}
    int gety() const{return -1;}
    int getKills() const {return kills;}
    vector<int> getPath() const {return path;}
    MoveCheckers extendMove(MoveCheckers m);

};
