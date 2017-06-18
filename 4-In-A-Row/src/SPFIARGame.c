/*
 * SPFIARGame.c
 *
 *  Created on: 2 αιεπι 2017
 *      Author: Dolev
 */

#include "SPFIARGame.h"

#include <stdio.h>
#include <stdlib.h>

SPFiarGame* spFiarGameCreate(int historySize){
	if(historySize<=0) return NULL;
	SPFiarGame* game= (SPFiarGame*)malloc(sizeof(SPFiarGame));
	if(game == NULL) {
		printMallocError();
		exit(42);
	}
	for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; ++j){
		for(int i = 0; i < SP_FIAR_GAME_N_ROWS; ++i){
			game->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
		}
		game->tops[j] = 0;
	}
	game->player1Moves = spArrayListCreate(historySize/2);
	game->player2Moves = spArrayListCreate(historySize/2);
	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	if(src == NULL) return NULL;
	SPFiarGame* cpy = spFiarGameCreate(2*src->player1Moves->maxSize);
	if(cpy == NULL) {
		free(src);
		printf("Error: spFiarGameCopy has failed");
		exit(42);
	}
	cpy->currentPlayer = src->currentPlayer;
	spArrayListDestroy(cpy->player1Moves);
	spArrayListDestroy(cpy->player2Moves);
	cpy->player1Moves = spArrayListCopy(src->player1Moves);
	cpy->player2Moves = spArrayListCopy(src->player2Moves);
	for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; ++j){
		for(int i = 0; i < SP_FIAR_GAME_N_ROWS; ++i){
			cpy->gameBoard[i][j] = src->gameBoard[i][j];
		}
		cpy->tops[j] = src->tops[j];
	}
	return cpy;
}
void spFiarGameDestroy(SPFiarGame* src){
	if(src != NULL){
		spArrayListDestroy(src->player2Moves);
		spArrayListDestroy(src->player1Moves);
		free(src);
	}
}
// player hasn't changed !!!
SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if(src == NULL || col < 0 || SP_FIAR_GAME_N_COLUMNS <= col){
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if(src->tops[col] == SP_FIAR_GAME_N_ROWS) return SP_FIAR_GAME_INVALID_MOVE;
	src->gameBoard[src->tops[col]][col] = src->currentPlayer;
	src->tops[col] += 1;
	if(src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		if(spArrayListIsFull(src->player1Moves)) spArrayListRemoveLast(src->player1Moves);
		spArrayListAddFirst(src->player1Moves, col);
	}
	else{
		if(spArrayListIsFull(src->player2Moves)) spArrayListRemoveLast(src->player2Moves);
		spArrayListAddFirst(src->player2Moves, col);
	}
	src->currentPlayer = (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) ? SP_FIAR_GAME_PLAYER_2_SYMBOL : SP_FIAR_GAME_PLAYER_1_SYMBOL;
	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if(src == NULL || col < 0 || SP_FIAR_GAME_N_COLUMNS <= col || src->tops[col] == SP_FIAR_GAME_N_ROWS){
		return false;
	}
	return true;
}
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if(src == NULL) return SP_FIAR_GAME_INVALID_ARGUMENT;
	int col = 0;
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		if(spArrayListIsEmpty(src->player2Moves)) return SP_FIAR_GAME_NO_HISTORY;
		col = spArrayListGetFirst(src->player2Moves);
		spArrayListRemoveFirst(src->player2Moves);
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	else{
		if(spArrayListIsEmpty(src->player1Moves)) return SP_FIAR_GAME_NO_HISTORY;
		col = spArrayListGetFirst(src->player1Moves);
		spArrayListRemoveFirst(src->player1Moves);
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	src->gameBoard[src->tops[col]-1][col] = SP_FIAR_GAME_EMPTY_ENTRY;
	src->tops[col] -= 1;
	return SP_FIAR_GAME_SUCCESS;
}
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (src == NULL) return SP_FIAR_GAME_INVALID_ARGUMENT;
	for(int i = SP_FIAR_GAME_N_ROWS-1 ; 0 <= i; --i){
		printf("|");
		for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; ++j){
			printf("%c%c",SP_FIAR_GAME_EMPTY_ENTRY,src->gameBoard[i][j]);
		}
		printf(" |\n");
	}
	for(int j = 0; j < 2*SP_FIAR_GAME_N_COLUMNS+2;++j){
		printf("%c",SP_FIAR_GAME_TIE_SYMBOL);
	}
	printf("%c\n",SP_FIAR_GAME_TIE_SYMBOL);
	for(int j = 0; j < 2*SP_FIAR_GAME_N_COLUMNS+3;++j){
		if(j % 2 == 0 && j != 0 && j != 2*SP_FIAR_GAME_N_COLUMNS+2){
			printf("%d",j/2);
		}
		else printf("%c",SP_FIAR_GAME_EMPTY_ENTRY);
	}
	printf("\n");
	return SP_FIAR_GAME_SUCCESS;
}
char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if(src == NULL) return SP_FIAR_GAME_EMPTY_ENTRY;
	return src->currentPlayer;
}
char spFiarCheckWinner(SPFiarGame* src){
	if(src == NULL) return '\0';
	int col = 0;
	int row = 0;
	int cnt = 0;
	int howUpCanWeGet = 1;
	int howDownCanWeGet = 1;
	if(src->player1Moves->actualSize <= 2 || src->player2Moves->actualSize <= 2){
		return '\0';
	}
	col = (spFiarGameGetCurrentPlayer(src) == SP_FIAR_GAME_PLAYER_1_SYMBOL) ? spArrayListGetFirst(src->player2Moves) : spArrayListGetFirst(src->player1Moves);
	row = src->tops[col] - 1;
	char lastTurn = src->gameBoard[row][col];
	//check rows
	int leftLimit = max(0,col-3);
	int rightLimit = min(SP_FIAR_GAME_N_COLUMNS-1 ,col+3);
	for(int j = leftLimit; j <= rightLimit; ++j){
		cnt = (src->gameBoard[row][j] == lastTurn) ? cnt + 1 : 0;
		if(cnt == 4) return lastTurn;
	}
	cnt = 0;
	//check columns
	if(3 <= row){
		for(int j = 0; j < 4; ++j){
			if(src->gameBoard[row - j][col] == lastTurn) cnt += 1;
		}
		if (cnt == 4) return lastTurn;
	}
	cnt = 0;
	//check descending diagonal
	//up and left
	while(true){
		if(0 <= col-howUpCanWeGet && row+howUpCanWeGet <= SP_FIAR_GAME_N_ROWS-1 &&
				src->gameBoard[row+howUpCanWeGet][col-howUpCanWeGet] == lastTurn) {
			++cnt;
			++howUpCanWeGet;
		}
		else break;
	}
	//down and right
	while(true){
		if(col+howDownCanWeGet<=SP_FIAR_GAME_N_COLUMNS-1 && 0 <= row-howDownCanWeGet &&
				src->gameBoard[row-howDownCanWeGet][col+howDownCanWeGet] == lastTurn) {
			++cnt;
			++howDownCanWeGet;
		}
		else break;
	}

	if(cnt >= 3) return lastTurn;

	cnt = 0;
	//check ascending diagonal
	//right and up
	howUpCanWeGet = 1;
	while(true){
		if(col+howUpCanWeGet <= SP_FIAR_GAME_N_COLUMNS-1 && row+howUpCanWeGet <= SP_FIAR_GAME_N_ROWS-1 &&
				src->gameBoard[row+howUpCanWeGet][col+howUpCanWeGet] == lastTurn) {
			++cnt;
			++howUpCanWeGet;
		}
		else break;
	}
	howDownCanWeGet = 1;
	while(true){
		if(0 <= col-howDownCanWeGet && 0 <= row-howDownCanWeGet &&
				src->gameBoard[row-howDownCanWeGet][col-howDownCanWeGet] == lastTurn) {
			++cnt;
			++howDownCanWeGet;
		}
		else break;
	}
	if(cnt >= 3) return lastTurn;

	//checking if the game is over
	for(int j = 0; j < SP_FIAR_GAME_N_COLUMNS; ++j){
		if(src->tops[j] != SP_FIAR_GAME_N_ROWS) return '\0';
	}
	return SP_FIAR_GAME_TIE_SYMBOL;

}
