#ifndef SPMINIMAX_H_
#define SPMINIMAX_H_

#include "SPFIARGame.h"
#include <limits.h>
#include <assert.h>
/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @return
 * -1 if either currentGame is NULL or maxDepth <= 0.
 * On success the function returns a number between [0,SP_FIAR_GAME_N_COLUMNS -1]
 * which is the best move for the current player.
 */
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth);
int spRecursiveMiniMax(SPFiarGame* currentGame,unsigned int maxDepth,bool flag,char whoCalledMe);
/**
 * Given a game state, this function evaluate and return of the utility of the state, according to
 * the player that called her.
 * @param currentGame - The current game state
 * @param whoCalledMe - the function return positive integer if this state is "good" to the user
 * that called to the function.
 * @return
 * INT_MAX if the player that called the function won the game.
 * INT_Min if the player that called the function has lost the game the game.
 * the utility according to the vector {-5,-2,-1,0,1,2,5}. return the scalar multiply of this vector
 * and the vector {a1,...,a7} s.t. ai = |{s | there is i more disc of whoCalled me than the other player in 4-span}|
 */
int utilityFunction(SPFiarGame* currentGame, char whoCalledMe);
/**
 * multiply 2 vectors of size "size"
 * @param spans - array 1
 * @param vector - array 2
 * @param size - the size of the arrays.
 *
 * @return the scalar multiply between the 2 vactors.
 */
int multiplyVector(int spans[], int vector[], int size);
#endif
