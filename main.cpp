#include <iostream>
#include <system_error>
#include "GameState.h"
#include "Graph.h"
#include "Vertex.h"
#include <Queue.h>

using namespace std;


Vec validMove(GameState game){
    for (int i = 0; i < game.size; i++){
        for (int j = 0; j < game.size; j++){
            if (game.grid[i][j] == -1){
                return Vec(i, j);
            }
        }
    }
    return Vec(0,0);
}

int getReward(Vertex<GameState> successor, int depth, int player){
    //takes in a vertex and returns the score using minmax algorithm

    if (depth == 0 || successor.data.done == true){
        //evaluate the final score
        if (successor.data.hasWon(0)){
            return 100;
        }
        if (successor.data.hasWon(1)){
            return -100;
        }
        else{
            return 0;
        }
    }

    if(player == 0){
        //player X is the maximizing player
        int max = -101;
        // for each child of vertex
        for(int i = (successor.neighbors.size()); i > 0; i--){
            int x = getReward(*(successor.neighbors[i - 1])->location, depth - 1 ,1);
            max = (max > x) ? max : x;
        }
        return max;


    }
    else{
        //player O is the minimizing player
        int min = 101;
        //for each child of vertex
        for(int i = (successor.neighbors.size()); i > 0; i--){
            int x = getReward(*(successor.neighbors[i - 1])->location, depth - 1 ,0);
            min = (min < x) ? min : x;
        }
        return min;
    }

}

 Graph<GameState> findGraph(GameState game){
    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(game);

    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> frontier;
    frontier.enqueue(start);

    while (!frontier.isEmpty()){
        Vertex<GameState>* curr = frontier.dequeue();
        if (!curr->data.done){
            for(int i = 0; i < game.size; i++){
                for (int j = 0; j < game.size; j++){
                    if(curr->data.grid[i][j] == -1){
                        GameState next = curr->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);
                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(curr, successor);
                        if (!successor->data.done){
                            frontier.enqueue(successor);
                        }
                    }
                }
            }
        }
    }

    return stateSpace;

    // for every successor of start, call getReward(successor, player1)
    

    //return Vec(0,0);
}

// Vec findBestMove(GameState game){
//     auto startGame = findGraph(game);
//     auto startVertice = startGame
//     auto bestMove = *((starting.vertices)[0]);
//     for(auto move : *(starting.vertices[0]).data.neighbors){
//         if(player == 0){
//             if (getReward(bestMove) < getReward(*move)){
//                 bestMove = move;
//             }
//         }
//         else{
//             if (getReward(bestMove) > getReward(*move)){
//                 bestMove = move;
//             }
//         }
//     }
    
//     for(int i = 0; i < game.size; i++){
//                 for (int j = 0; j < game.size; j++){
//                     if(game.data.grid[i][j] != bestMove.data.grid[i][j]){
//                         return Vec(i,j);
//                     }
//                 }
//             }

// }







int main(){
    GameState game;

    while(!game.done){
        //system("clear");
        cout << game << endl;

        int x, y;

        //if (game.currentTurn){
            //Vec move = findBestMove(game);
           // x = move.x;
            //y = move.y;
        //}
        //else{
            cout << endl;
            auto jelly = findGraph(game);
            std::cout << game.turnCount;
            std::cout << getReward(*(jelly.vertices[0]), 9 - game.turnCount, game.currentTurn);
            cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            cin >> x >> y;
        //}

        game.play(x, y);
    }

    system("clear");
    cout << game << endl;
    cout << endl;
    if (game.hasWon(0)){
        cout << "Player X has won" << endl;
    }
    else if (game.hasWon(1)){
        cout << "Player O has won" << endl;
    }
    else {
        cout << "It's a tie" << endl;
    }
    return 0;
}