/*
 * SPMainAux.c
 *
 *  Created on: 3 αιεπι 2017
 *      Author: Dolev and Ido
 */

#include "SPMainAux.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPArrayList.h"
#include "SPMiniMax.h"

void printErrorLevel(){
	printf("Error: invalid level (should be between 1 to 7)\n");
}
void printEnterLevel(){
	printf("Please enter the difficulty level between [1-7]:\n");
}
void printNextMove(){
	printf("Please make the next move:\n");
}
void printErrorUndo(){
	printf("Error: cannot undo previous move!\n");
}
void printErrorColumnRange(){
	printf("Error: column number must be in range 1-7\n");
}
void printWinner(char winner){
	if(winner == SP_FIAR_GAME_PLAYER_1_SYMBOL) printf("Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
	if(winner == SP_FIAR_GAME_PLAYER_2_SYMBOL) printf("Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
	if(winner == SP_FIAR_GAME_TIE_SYMBOL) printf("Game over: it’s a tie\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
}
SPCommand parseCommand(SPFiarGame* game){
	char* str = (char*) malloc((Max_len+1)*sizeof(char));
	if(str == NULL) {
		printMallocError();
		spFiarGameDestroy(game);
		exit(42);
	}
	SPCommand spCmd;
	if (fgets(str, Max_len+1, stdin) == NULL) {
		spFiarGameDestroy(game);
		free(str);
		exit(42);
	}
	spCmd = spParserPraseLine(str);
	while(spCmd.cmd == SP_INVALID_LINE){
		printf("Error: invalid command\n");
		if (fgets(str, Max_len+1, stdin) == NULL) {
			spFiarGameDestroy(game);
			free(str);
			exit(42);
		}
		spCmd = spParserPraseLine(str);
	}
	free(str);
	return spCmd;
}

SP_COMMAND playturn(SPFiarGame* game,int difficultyLevel,bool win){
	SPCommand spCmd = parseCommand(game);
	if(spCmd.cmd == SP_QUIT) {
		printQuit();
		return 	SP_QUIT;
	}
	else if(spCmd.cmd == SP_RESTART){
		printf("Game restarted!\n");
		return SP_RESTART;
	}
	else if(spCmd.cmd == SP_ADD_DISC){
		if(spCmd.validArg == true){
			if(win == true){
				printf("Error: the game is over\n");
				return playturn(game,difficultyLevel,win);
			}
			return playAddDisc(game, spCmd);
		}
		else{
			printErrorColumnRange();
			return playturn(game,difficultyLevel,win);
		}
	}
	else if(spCmd.cmd == SP_UNDO_MOVE){
		int check = spFiarGameUndoPrevMove1(game);
		switch(check){
		case 0: return SP_INVALID_LINE;
		case SP_FIAR_GAME_PLAYER_1_SYMBOL: return SP_UNDO_MOVE;
		case SP_FIAR_GAME_PLAYER_2_SYMBOL: spFiarGameUndoPrevMove1(game);
		}
		return SP_UNDO_MOVE;
	}
	else if(spCmd.cmd == SP_SUGGEST_MOVE){
		if(win == true){
			printf("Error: the game is over\n");
			return playturn(game,difficultyLevel,win);
		}
		return playSuggestedMove(game,difficultyLevel,win);
	}
	else {
		printf("Error: invalid command\n");    //until we get a valid command
		return playturn(game,difficultyLevel,win);
	}


}

SP_COMMAND playAddDisc(SPFiarGame* game,SPCommand spCmd) {
	if (spFiarGameIsValidMove(game, spCmd.arg-1)) {
		spFiarGameSetMove(game, spCmd.arg - 1);
		return SP_ADD_DISC;
	} else {
		if (spCmd.arg > 7 || spCmd.arg < 1)
			printf("Error: column number must be in range 1-7\n");
		else if (game->tops[spCmd.arg - 1] == SP_FIAR_GAME_N_ROWS)
			printf("Error: column %d is full\n", spCmd.arg);
		return SP_INVALID_LINE;
	}
}


SP_COMMAND playSuggestedMove(SPFiarGame* game,int difficultyLevel, bool win) {
	SPFiarGame* currentGame = spFiarGameCopy(game);
	int col = spMinimaxSuggestMove(currentGame, difficultyLevel);
	spFiarGameDestroy(currentGame);
	printf("Suggested move: drop a disc to column %d\n", col+1);
	return playturn(game, difficultyLevel, win);
}

void printQuit() {
	printf("Exiting...\n");
}
void comPlay(SPFiarGame* game,int difficultyLevel){
	SPFiarGame* currentGame = spFiarGameCopy(game);
	int col = spMinimaxSuggestMove(currentGame,difficultyLevel);
	spFiarGameDestroy(currentGame);
	printf("Computer move: add disc to column %d\n",col+1);
	spFiarGameSetMove(game,col);
}
int initializationGame(){
	int difficultyLevel = 0;
	char* line = (char*) malloc((Max_len+1)*sizeof(char));
	if(line == NULL) {
		printMallocError();
		exit(42);
	}
	if (fgets(line, Max_len+1, stdin) == NULL) {
		free(line);
		exit(42);
	}
	line[strlen(line)-1] = '\0';
	if(strcmp(line, "quit") == 0){
		printf("Exiting...\n");
		free(line);
		return -1;
	}
	else if(!spParserIsInt(line)){
		printErrorLevel();
		free(line);
		return initializationGame();
	}
	difficultyLevel = atoi(line);
	free(line);
	if(difficultyLevel > 7 || difficultyLevel < 1){
		printErrorLevel();
		return initializationGame();
	}
	return difficultyLevel;
}
int spFiarGameUndoPrevMove1(SPFiarGame* src){
	if(src == NULL) {
		printErrorUndo();
		return 0;
	}
	int col = 0;
	if (spFiarGameGetCurrentPlayer(src) == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		if(spArrayListIsEmpty(src->player2Moves)) {
			printErrorUndo();
			return 0;
		}
		col = spArrayListGetFirst(src->player2Moves);
		spArrayListRemoveFirst(src->player2Moves);
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	else{
		if(spArrayListIsEmpty(src->player1Moves)) {
			printErrorUndo();
			return 0;
		}
		col = spArrayListGetFirst(src->player1Moves);
		spArrayListRemoveFirst(src->player1Moves);
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	src->gameBoard[src->tops[col]-1][col] = SP_FIAR_GAME_EMPTY_ENTRY;
	src->tops[col] -= 1;
	if(spFiarGameGetCurrentPlayer(src) == SP_FIAR_GAME_PLAYER_2_SYMBOL){
		printf("Remove disc: remove computer's disc at column %d\n",col+1);
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}
	else if (spFiarGameGetCurrentPlayer(src) == SP_FIAR_GAME_PLAYER_1_SYMBOL){
		printf("Remove disc: remove user's disc at column %d\n",col+1);
		return SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	return 32;
}
