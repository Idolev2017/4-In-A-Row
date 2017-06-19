/*
 * SPMiniMax.c
 *
 *  Created on: 3 αιεπι 2017
 *      Author: Dolev
 */

#include "SPMiniMax.h"
#include <stdbool.h>
#include <stdio.h>

static int constVector[SP_FIAR_GAME_N_COLUMNS] = {-5,-2,-1,0,1,2,5};
int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth){
	if (currentGame == NULL || maxDepth <= 0) return -1;
	char whoCalledMe = spFiarGameGetCurrentPlayer(currentGame);
	return spRecursiveMiniMax(currentGame,maxDepth,true,whoCalledMe,true);
}

int spRecursiveMiniMax(SPFiarGame* currentGame,unsigned int maxDepth,bool flag,char whoCalledMe, bool Root){
	char winner = spFiarCheckWinner(currentGame);
	if(winner != '\0'){
		if(winner == SP_FIAR_GAME_PLAYER_1_SYMBOL || winner == SP_FIAR_GAME_PLAYER_2_SYMBOL){
			return (winner == whoCalledMe) ? INT_MAX : INT_MIN;
		}
		else return utilityFunction(currentGame,whoCalledMe);
	}
	if (maxDepth == 0) return utilityFunction(currentGame,whoCalledMe);
	int childvalue = 0;
	int value = flag ? INT_MIN : INT_MAX;
	int bestCol = -1;
	for(int col = 0; col < SP_FIAR_GAME_N_COLUMNS; ++col){
		if (spFiarGameIsValidMove(currentGame,col)){
			spFiarGameSetMove(currentGame,col);
			childvalue = spRecursiveMiniMax(currentGame,maxDepth-1,!flag,whoCalledMe, false);
			if ((flag && value < childvalue) || bestCol == -1){
				value = childvalue;
				bestCol = col;
			}
			else if(!flag && childvalue < value){
				value = childvalue;
				bestCol = col;
			}
			spFiarGameUndoPrevMove(currentGame);
		}
	}
	return Root ? bestCol : value;
}

int utilityFunction(SPFiarGame* currentGame, char whoCalledMe){
	int spans[2*SP_FIAR_GAME_SPAN-1] = {0,0,0,0,0,0,0};
	int cnt1 = 0;
	int cnt2 = 0;
	int cnt3 = 0;
	int cnt4 = 0;
	char entry;
	for(int row = 0; row < SP_FIAR_GAME_N_ROWS; ++row){
		for(int col = 0; col < SP_FIAR_GAME_N_COLUMNS; ++col){
			for(int j = 0; j < 4; ++j){
				//passing rows
				if(col < SP_FIAR_GAME_N_COLUMNS - 3) {
					entry = currentGame->gameBoard[row][col + j];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt1 = (entry == whoCalledMe) ? cnt1 + 1 : cnt1 - 1;
				}
				//passing columns
				if(row < SP_FIAR_GAME_N_ROWS - 3) {
					entry = currentGame->gameBoard[row + j][col];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt2 = (entry == whoCalledMe) ? cnt2 + 1 : cnt2 - 1;
				}
				//passing diagonal spans
				if(col < SP_FIAR_GAME_N_COLUMNS - 3 &&(row < SP_FIAR_GAME_N_ROWS - 3)){
					//passing ascending diagonal spans
					entry = currentGame->gameBoard[row + j][col + j];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt3 = (entry == whoCalledMe) ? cnt3 + 1 : cnt3 - 1;

					//passing descending diagonal spans
					entry = currentGame->gameBoard[row + 3 - j][col + j];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt4 = (entry == whoCalledMe) ? cnt4 + 1 : cnt4 - 1;
				}
			}
			spans[cnt1 + 3] += 1;
			spans[cnt2 + 3] += 1;
			spans[cnt3 + 3] += 1;
			spans[cnt4 + 3] += 1;
			cnt1 = 0;
			cnt2 = 0;
			cnt3 = 0;
			cnt4 = 0;
		}
	}
	int d = multiplyVector(spans,constVector,2*SP_FIAR_GAME_SPAN-1);
	return d;
}

int multiplyVector(int spans[],int vector[], int size){
	int sum = 0;
	for (int i = 0; i < size; ++i) sum += spans[i]*vector[i];
	return sum;
}
