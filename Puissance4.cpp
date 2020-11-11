

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <Imagine/LinAlg.h>
#include <vector>
#include <sstream>
#include <cmath>
#include "Puissance4.h"

using namespace Imagine;
using namespace std;


#include <chrono>
using namespace std::chrono;


void printResult(Board& b){
    int m = b.getLig();
    int n = b.getCol();
    int value;
    for (int i = m-1; i>-1; i--){
        for (int j=0; j<n; j++){
            value = b.getPiece(i,j);
            value = (value<0)? 2:value;
            cout << value  << " ";
        }
        cout << " " << endl;
    }
}


// ====================== CLASS GAME ============================

Game::Game(Agent* agentplayer1, Agent* agentplayer2){
    agent1 = agentplayer1;
    agent2 = agentplayer2;
}


Game::~Game(){
    delete agent1;
    delete agent2;
}

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
// ====================== CLASS AGENT ===========================

Agent::Agent(){}

randomAgent::randomAgent(): Agent(){}

pair<int,int> randomAgent::getMove(Board& B, int& player){
    std::vector<std::pair<int,int>> playableMove = B.playableMoves(player);
    int playableMoveSize = playableMove.size();
    int randomChoice = rand() % playableMoveSize;
    pair<int,int> randomMove = playableMove[randomChoice];
    return randomMove;
}


UCTAgent::UCTAgent(int iter_max) : Agent(){
    max_iter = iter_max;
}



pair<int,int> UCTAgent::getMove(Board& B, int& player){
    UCTNode root(B, player,pair<int,int>(-1,-1),true);
    pair<int,int> best_move;
    int newWins;
    for(int i =0; i<max_iter; i++){
        UCTNode* beforeLeaf = root.selectExpandedPath();
        UCTNode* leaf = beforeLeaf->expand();
        newWins = leaf->rollout(player);
        leaf->backPropagation(newWins);
    }
    best_move = root.selectionFinal()->getLastMove();
    return best_move;
}



// ====================== CLASS BOARD ===========================


Board::Board(int nligcol){
    assert(nligcol>=0);
    m=n=nligcol;
    tab = new int[m*n];
    for(int i=0; i<m*n; i++){
        tab[i]= 0;
    }
    if (n>0 && m>0){
        endGameValue=false;
    }
    else {
        endGameValue=true;
    }

}

void Board::reset(){
    for(int i=0; i<m*n; i++){
        tab[i]= 0;
    }
    endGameValue = 0;

}

void Board::addPiece(int x, int y, int player){
    int index = getIndex(x,y);
    tab[index] = player;
}

Board::Board(int nlig, int ncol){
    assert(nlig>=0);
    assert(ncol>=0);
    m=nlig;
    n=ncol;
    tab = new int[m*n];
    for(int i=0; i<m*n; i++){
        tab[i]= 0;
    }
    if (n>0 && m>0){
        endGameValue=false;
    }
    else {
        endGameValue=true;
    }

}

Board::~Board(){
    delete [] tab;
}

int Board::getLig() const{
    return m;
}

int Board::getCol() const{
    return n;
}

int Board::getIndex(int x, int y) const{
//    assert(x<m);
//    if(x>=m){
//        cout << "ERREUR" << endl;
//        cout << x << ";" << m << endl;
//    }
//    else {
//        cout << "NO ERREUR" << endl;
//        cout << x << ";" << m << endl;
//    }

    assert(x<m);
    assert(y<n);
    return x*n + y;
}

Board::Board(const Board& b){
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

void Board::operator=(const Board& b){
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

Board::Board(){
    Board(0);
}
int Board::getPiece(int x, int y) const{
    int index = getIndex(x,y);
    return tab[index];
}

bool Board::colComplete(int colNumber){
    int index = getIndex(m-1, colNumber);
    if (tab[index]==0){
        return false;
    }
    else {
        return true;
    }
}

std::vector<std::pair<int,int>> Board::playableMoves(int playerNumber){
    std::vector<std::pair<int,int>> playMoves;
    if (endGameValue){
        return playMoves;
    }

    for(int col =0; col<n; col++){
        if (!colComplete(col)){
            int ligne = m-2;
            while(ligne>-1 && getPiece(ligne,col)==0){
                ligne--;
            }
            std::pair<int,int> move(ligne+1,col);
//            cout << move.first << " ; " << move.second << endl;
            playMoves.push_back(move);
        }
    }
    return playMoves;
}

void Board::playMove(const std::pair<int,int>& move, int player){
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

int Board::getEndGameValue() const{
    return endGameValue;
}

bool Board::endGame(){
    for(int col =0; col<m; col++){
        if (!colComplete(col)){
            return false;
        }
    }
    return true;
}


bool Board::winGame(int x, int y, int& player){
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
//        if(x==1 && y == 5){
//            here = true;
//            cout << "HERE" << endl;
//        }

        int next_x = aux_x+deltax;
        int next_y = aux_y+deltay;
//        if (here) {
//        cout << tab[getIndex(next_x,next_y)] << endl;
//        cout << player << endl;}
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
//        if (here) {
//        cout << tab[getIndex(next_x,next_y)] << endl;
//        cout << player << endl;
//        cout << (next_x<m) << endl;
//        cout << (next_x>=0) << endl;
//        cout << (next_y>=0) << endl;
//        cout << (next_y<n) << endl;
//        }
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
// ====================== CLASS UCTNODE =========================
// TODO : CHECKER LA PARALLELISATION (PLUSIEURS METHODES: plusieurs arbres (avec enfants différents), bagging (juste lancement aléatoire).
UCTNode::UCTNode(Board currentB, int current_player, std::pair<int,int> current_last_move /*=std::pair<-1,-1> */, bool current_isRoot /*=false*/): B(currentB) {
    player = current_player;
    isRoot = current_isRoot;
    if(isRoot){
        visits = 1;
    }
    else {visits=0;}
    wins = 0;
    remainingMoves = B.playableMoves(player);
    lastMove = current_last_move;
    if (B.playableMoves(player).size() == 0){
        isTerminal = true;
    }
    else {
        isTerminal = false;
    }
}

bool UCTNode::getIsRoot() const{
    return isRoot;
}

bool UCTNode::getIsTerminal() const{
    return isTerminal;
}

bool UCTNode::isFullyExpanded() const{
//    cout << remainingMoves.size() << endl;
//    cout << (remainingMoves.size() == 0 )<< endl;
    return (remainingMoves.size()==0);
}

UCTNode::~UCTNode(){
    for(vector<UCTNode*>::iterator it = childNodes.begin(); it < childNodes.end(); it++){ // Garder le sous arbre du noeud choisi.
        (*it)->~UCTNode();
    }
}

int UCTNode::getPlayer() const{
    return player;
}

int UCTNode::getWins() const{
    return wins;
}

int UCTNode::getVisits() const{
    return visits;
}


UCTNode* UCTNode::selection(bool verbose){
    float max = -1000000000000000;
    float value;
    UCTNode* bestChild;
    for(vector<UCTNode*>::iterator it = childNodes.begin(); it < childNodes.end(); it++){
        float childWin = float((*it)->getWins());
        float childVisit = float((*it)->getVisits());

//        if (visits = 1000){
//            cout << "test" << endl;
//        }

        if (childVisit==0){
            value = 0;
        }
        else {
            value = player * (childWin/childVisit) + sqrt(2*log(visits)/childVisit);
        }
        if(value >= max){
            max = value;
            bestChild = *it;
        }

    }
    return bestChild;
}



UCTNode* UCTNode::selectionFinal(){
    float max = -1000000000000000;
    float value;
    UCTNode* bestChild;
    for(vector<UCTNode*>::iterator it = childNodes.begin(); it < childNodes.end(); it++){
        float childWin = float((*it)->getWins());
        float childVisit = float((*it)->getVisits());

        value = abs(childWin/childVisit);


        if(value >= max){
            max = value;
            bestChild = *it;
        }

    }
    return bestChild;
}

UCTNode* UCTNode::selectExpandedPath(){ // Utiliser un while pour accélerer ?
//    cout << this->isFullyExpanded() << endl;
//    cout << this->getIsTerminal() << endl;
    if (this->isFullyExpanded() && !this->getIsTerminal()){
        return this->selection()->selectExpandedPath();
    }
    else{
        return this;
    }
}

UCTNode* UCTNode::expand(){
    int remainingSize = remainingMoves.size();

    if (remainingSize>0){ // Not terminal node (Check terminal better ?)
        int randomChoice = rand() % remainingSize;
        pair<int,int> randomMove = remainingMoves[randomChoice];
        remainingMoves.erase(remainingMoves.begin()+randomChoice);
        Board newBoard(B);
        newBoard.playMove(randomMove, player);
        UCTNode* newChildNode = new UCTNode(newBoard, -player, randomMove, false);
        newChildNode->parent = this;
//        newChildNode->visits+=1;
        childNodes.push_back(newChildNode);

        return newChildNode;
    }
    else{
        return this;
    }
}

void UCTNode::addWins(int newWins){
    wins+=newWins;
}

int UCTNode::rollout(int globalPlayer){
    Board newBoard(B);
    int auxPlayer = player;
    int valueWins;
    while (!newBoard.getEndGameValue()){
        std::vector<std::pair<int,int>> playableMove = newBoard.playableMoves(auxPlayer);
        int movesSize = playableMove.size();
        int randomChoice = rand() % movesSize;
        pair<int,int> randomMove = playableMove[randomChoice];
        newBoard.playMove(randomMove, auxPlayer);
        auxPlayer *=-1;
    }
    if (newBoard.getEndGameValue()== globalPlayer){
        valueWins = 1;
    }
    else {
        valueWins =0;
    }

    return valueWins;
}

void UCTNode::backPropagation(int newWins /*=0*/){
    visits++;
    wins+=newWins;
    if(!isRoot){
        parent->backPropagation(newWins);
    }
}

UCTNode::UCTNode(const UCTNode &node) : B(node.B), isRoot(node.isRoot), isTerminal(node.isTerminal),
parent(node.parent), lastMove(node.lastMove), remainingMoves(node.remainingMoves),
childNodes(node.childNodes), player(node.player), visits(node.visits), wins(node.wins){}


std::pair<int,int> UCTNode::getLastMove() const{
    return lastMove;
}

void UCTNode::operator=(const UCTNode& node){
    B.~Board();
    Board B(node.B);
    isRoot = node.isRoot;
    isTerminal = node.isTerminal;
    parent = node.parent;
    lastMove = node.lastMove;
    remainingMoves = node.remainingMoves;
    childNodes = node.childNodes;
    player = node.player;
    visits = node.visits;
    wins = node.wins;
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
    Board b(8,6);
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
