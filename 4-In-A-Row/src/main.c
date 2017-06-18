/*
 ============================================================================
 Name        : Connect4.c
 Author      :
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : 4 in a row Game.
 ============================================================================
 */
#include <stdio.h>
#include "SPMainAux.h"
#define historySize 20

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	printEnterLevel();
	int difficultyLevel = initializationGame();
	if(difficultyLevel == -1) return 0;
	SP_COMMAND cmd = 0;
	bool win = false;
	char winner = ' ';
	SPFiarGame* game = spFiarGameCreate(historySize);
	while(true){
		if(win == false && cmd != SP_INVALID_LINE) {
			spFiarGamePrintBoard(game);
			printNextMove();
		}
		cmd = playturn(game,difficultyLevel,win);
		if(cmd == SP_INVALID_LINE) continue;
		if(cmd == SP_QUIT) {
			spFiarGameDestroy(game);
			return 0;            //quit
		}
		if(cmd == SP_RESTART) {           //restart
			printEnterLevel();
			difficultyLevel = initializationGame();
			spFiarGameDestroy(game);
			if(difficultyLevel == -1) return 0;
			game = spFiarGameCreate(historySize);
			win = false;
			continue;
		}
		if(cmd == SP_ADD_DISC){        //add_disc
			win = false;
			winner = spFiarCheckWinner(game);
			if(winner != '\0') {
				spFiarGamePrintBoard(game);
				printWinner(winner);
				win = true;
				continue;
			}
			comPlay(game,difficultyLevel);
			winner = spFiarCheckWinner(game);
			if(winner != '\0') {
				spFiarGamePrintBoard(game);
				printWinner(winner);
				win = true;
				continue;
			}
		}
		if(cmd == SP_UNDO_MOVE) {
			win = false;
			continue;
		}
	}
}
