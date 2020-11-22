
#include "Puissance4.h"

using namespace Imagine;
using namespace std;


#include <chrono>
using namespace std::chrono;


void printResult(ConnectFourBoard& b){
    int m = b.getLig();
    int n = b.getCol();
    int value;
    for (int i = m-1; i>-1; i--){
        for (int j=0; j<n; j++){
            value = b.getPiecePlayer(i,j);
            value = (value<0)? 2:value;
            cout << value  << " ";
        }
        cout << " " << endl;
    }
}
// =================== PUISSANCE 4 CHIP ======================

//ConnectFourBoardChip::ConnectFourBoardChip(int player): Piece(player) {}


// ====================== CLASS GAME ============================


Puissance4::Puissance4(Agent* agentplayer1, Agent* agentplayer2, int nligcol):
    Game(agentplayer1,agentplayer2), B(nligcol){}

Puissance4::Puissance4(Agent* agentplayer1, Agent* agentplayer2,  int nlig, int ncol):
    Game(agentplayer1,agentplayer2), B(nlig,ncol){}


int Puissance4::play(){
    int player1 = 1;
    int player2 = -1;

    pair<int,int> move1;
    pair<int,int> move2;
    while (!B.getEndGameValue()){
        auto start = high_resolution_clock::now();
        move1 = agent1->getMove(B, player1);
        auto stop = high_resolution_clock::now();
//        auto duration = duration_cast<microseconds>(stop - start);
//        cout << "Duration : " << duration.count() << endl;
//        cout << "player 1" << endl;
//        cout << move1.first << " ; " << move1.second << endl;
        B.playMove(move1,1);
        if(B.getEndGameValue()){
            cout << "AFFICHAGE" << endl;
            printResult(B);
            return B.getEndGameValue();
        }

//        cout << "AFFICHAGE" << endl;
//        printResult(B);
        move2 = agent2->getMove(B, player2);
//        cout << "PLayer 2" << endl;
//        cout << move2.first  << " ; "  << move2.second << endl;
        B.playMove(move2,-1);
//        cout << "AFFICHAGE" << endl;
//        printResult(B);
    }
    cout << "AFFICHAGE" << endl;
    printResult(B);
    return B.getEndGameValue();
}


void Puissance4::reset(){
    B.reset();

}



// ====================== CLASS BOARD ===========================


ConnectFourBoard::ConnectFourBoard(int nligcol): Board(nligcol){
    tab = new int[m*n];
    for(int i=0; i<m*n; i++){
        tab[i]= 0;
    }
}

void ConnectFourBoard::reset(){
    for(int i=0; i<m*n; i++){
        tab[i]= 0;
    }
    endGameValue = 0;
}

void ConnectFourBoard::addPiece(int x, int y, int player){
    int index = getIndex(x,y);
    tab[index] = player;
}

ConnectFourBoard::ConnectFourBoard(int nlig, int ncol) : Board(nlig,ncol){
    tab = new int[m*n];
    for(int i=0; i<m*n; i++){
        tab[i]= 0;
    }

}

ConnectFourBoard::ConnectFourBoard():ConnectFourBoard(0){}

ConnectFourBoard::~ConnectFourBoard(){
    delete [] tab;
}



ConnectFourBoard::ConnectFourBoard(const ConnectFourBoard& b){
    m = b.m;
    n = b.n;
    tab = new int[m*n];
    for(int i=0; i<m*n; i++){

        tab[i] = b.tab[i]; // Vraiment besoin de passer par la fonction dans l'environnement ?

    }
    endGameValue = b.endGameValue;
}

void ConnectFourBoard::operator=(const ConnectFourBoard& b){
    delete [] tab;
    m = b.m;
    n = b.n;
    tab = new int[m*n];
    for(int ligne=0; ligne<m; ligne++){
        for(int col=0; col<n; col++){
            int index = getIndex(ligne,col);
            tab[index] = b.tab[index]; // Vraiment besoin de passer par la fonction dans l'environnement ?
        }
    }
    endGameValue = b.endGameValue;

}




bool ConnectFourBoard::colComplete(int colNumber){
    int index = getIndex(m-1, colNumber);
    if (tab[index]==0){
        return false;
    }
    else {
        return true;
    }
}

std::vector<std::pair<int,int>> ConnectFourBoard::playableMoves(int playerNumber){
    std::vector<std::pair<int,int>> playMoves;
    if (endGameValue){
        return playMoves;
    }

    for(int col =0; col<n; col++){
        if (!colComplete(col)){
            int ligne = m-2;
            while(ligne>-1 && getPiecePlayer(ligne,col)==0){
                ligne--;
            }
            std::pair<int,int> move(ligne+1,col);
//            cout << move.first << " ; " << move.second << endl;
            playMoves.push_back(move);
        }
    }
    return playMoves;
}

void ConnectFourBoard::playMove(const std::pair<int,int>& move, int player){
    int x = move.first;
    int y = move.second;
    int index = getIndex(x,y);
    tab[index] = player;
    if(winGame(x,y,player)){
        endGameValue = player;
    }
    else if (endGame()){
        endGameValue = 2;
    }
    else {
        endGameValue = 0;
    }
}

int ConnectFourBoard::getEndGameValue() const{
    return endGameValue;
}

int ConnectFourBoard::getPiecePlayer(int x, int y) const{
    int index = getIndex(x,y);
    return tab[index];
}

ConnectFourBoard* ConnectFourBoard::clone(){
    return new ConnectFourBoard(*this);
}

bool ConnectFourBoard::endGame(){
    for(int col =0; col<n; col++){
        if (!colComplete(col)){
            return false;
        }
    }
    return true;
}


bool ConnectFourBoard::winGame(int x, int y, int& player){
    int compteur;
    pair<int,int> delta;
    int deltax;
    int deltay;
    int aux_x = x;
    int aux_y = y;


    for(int i =0; i<4;i++){
        compteur = 1;
        delta = direction[i];

        deltax = delta.first;
        deltay = delta.second;
        bool here = false;
        int next_x = aux_x+deltax;
        int next_y = aux_y+deltay;
        while(compteur<4  && (next_x< m && next_x>=0 && next_y>=0 && next_y<n) && tab[getIndex(next_x,next_y)]==player){
            aux_x = next_x;
            next_x = aux_x+deltax;
            aux_y = next_y;
            next_y = aux_y+deltay;
            compteur+=1;
        }
        aux_x = x;
        aux_y = y;
        deltax = -deltax;
        deltay = -deltay;
        next_x = aux_x+deltax;
        next_y = aux_y+deltay;

        while(compteur<4  && (next_x< m && next_x>=0 && next_y>=0 && next_y<n) && tab[getIndex(next_x,next_y)]==player){
            x = next_x;
            next_x = x+deltax;
            y = next_y;
            next_y = y+deltay;
            compteur+=1;
        }

        if (compteur==4){
            return true;
        }
    }
    return false;
}

// ====================== Main function =========================
int main(int argc, char* argv[]) {
//    UCTAgent agent1();
//    randomAgent agent2();
    UCTAgent* agent1 = new UCTAgent(10000);
    UCTAgent* agent2 = new UCTAgent(10000);
////    randomAgent* agent1 = new randomAgent();
////    randomAgent* agent2 = new randomAgent();
//    int result;
//    Puissance4 puissance4(agent1, agent2, 6, 7);
//    for(int i =0; i<10; i++){
//        cout << "Play" << endl;
//        result = puissance4.play();
//        cout << "result" << endl;
//        cout << result << endl;
//        puissance4.reset();
//    }
    ConnectFourBoard b(8,6);
    for(int i =0; i<3; i++){
        b.addPiece(0,i, 1);
    }
    int player = 1;
    pair<int,int> move = agent2->getMove(b,player);
    cout << move.first << " ; " << move.second << endl;
    player = -1;
    move = agent1->getMove(b,player);
    cout << move.first << " ; " << move.second << endl;


    return 1;
}
