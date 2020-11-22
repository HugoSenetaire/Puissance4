
#include "Agent.h"
using namespace std;



//Piece::Piece(int player): player(player){}

//int Piece::getPlayer() const{return player;}


// ====================== CLASS BOARD ==========================


Board::Board(int nligcol):m(nligcol), n(nligcol) {
    if (n>0 && m>0){
        endGameValue=0;
    }
    else {
        endGameValue=1;
    }

//    Piece* tab = new Piece[m*n];
}
Board::Board(int nlig, int ncol):m(nlig), n(ncol){
    if (n>0 && m>0){
        endGameValue=false;
    }
    else {
        endGameValue=true;
    }

//    Piece* tab = new Piece[m*n];
}

Board::Board():Board(0){}

Board::~Board(){}

Board::Board(const Board& b): m(b.m), n(b.n), endGameValue(b.endGameValue){
//    Piece* tab = new Piece[m*n];
//    for(int i=0; i<m*n; i++){
//        tab[i] = b.tab[i];
//    }
}

int Board::getLig() const{return m;}
int Board::getCol() const{return n;}
int Board::getIndex(int x, int y) const{
    if (y>=n){
        cout << y << n << endl;
    }
    if (x>=m){
        cout << x << m << endl;
    }
    assert(x<m);
    assert(y<n);
    return x*n + y;
}

int Board::getEndGameValue() const {return endGameValue;}

//Board* Board::clone(){}
/*int Board::getPiecePlayer(int x, int y) const{
    int index = getIndex(x,y);
    return tab[index]->getPlayer();
}// Return player index : (0, no player), (-1 player 2), (1 player 1)
*/

//Piece* Board::getPiece(int x, int y) const{
//    int index = getIndex(x,y);
//    return tab[index];
//}

//void Board::addPiece(int x, int y, Piece& Piece){
//    int index = getIndex(x,y);
//    tab[index] = Piece;
//}

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


// ====================== CLASS UCTNODE =========================
// TODO : CHECKER LA PARALLELISATION (PLUSIEURS METHODES: plusieurs arbres (avec enfants différents), bagging (juste lancement aléatoire).
UCTNode::UCTNode(Board& currentB, int current_player, std::pair<int,int> current_last_move /*=std::pair<-1,-1> */, bool current_isRoot /*=false*/){
    B = currentB.clone();
    player = current_player;
    isRoot = current_isRoot;
    if(isRoot){
        visits = 1;
    }
    else {visits=0;}
    wins = 0;
    remainingMoves = B->playableMoves(player);
    lastMove = current_last_move;
    if (B->playableMoves(player).size() == 0){
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
        Board* newBoard = B->clone();
        newBoard->playMove(randomMove, player);
        UCTNode* newChildNode = new UCTNode(*newBoard, -player, randomMove, false);
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
    Board* newBoard = B->clone();
    int auxPlayer = player;
    int valueWins;
    while (!newBoard->getEndGameValue()){
        std::vector<std::pair<int,int>> playableMove = newBoard->playableMoves(auxPlayer);
        int movesSize = playableMove.size();
        int randomChoice = rand() % movesSize;
        pair<int,int> randomMove = playableMove[randomChoice];
        newBoard->playMove(randomMove, auxPlayer);
        auxPlayer *=-1;
    }
    if (newBoard->getEndGameValue()== globalPlayer){
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

UCTNode::UCTNode(const UCTNode &node) : isRoot(node.isRoot), isTerminal(node.isTerminal),
parent(node.parent), lastMove(node.lastMove), remainingMoves(node.remainingMoves),
childNodes(node.childNodes), player(node.player), visits(node.visits), wins(node.wins){
    Board* B = node.B->clone();
}


std::pair<int,int> UCTNode::getLastMove() const{
    return lastMove;
}

void UCTNode::operator=(const UCTNode& node){
    B->~Board();
    Board* B = node.B->clone();
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

