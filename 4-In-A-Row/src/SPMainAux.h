#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdbool.h>
#include "SPFIARGame.h"
#include "SPFIARParser.h"
#define Max_len 1024

#define commandSize 1024

/**
 * print the massage that tell the user to enter the difficulty level of the game
 */
void printEnterLevel();
/**
 * print an error that indicate that the user entered an invalid input for the difficulty of the game.
 */
void printErrorLevel();
/**
 * print the massage that ask the user what his next move.
 */
void printNextMove();
/**
 * print an error that indicate that the user entered an invalid column.
 */
void printErrorColumnRange();
/**
 * print the massage that declare who is the winner of the game.
 * @param char winner.
 */
void printWinner(char winner);
/**
 * play the turn of the user. the function ask the user what he want to do, and act according to
 * his move.
 * @param game - The current game state
 * @param difficultLevel - the difficult of the game, according to the user.
 * @param win boolean variable. true iff in the current game state there is a winner.
 * @return
 * the command that the user entered, one of the SP_COMMAND struct.
 */
SP_COMMAND playturn(SPFiarGame* game,int difficultLevel,bool win);
/**
 * This function ask for the difficulty level that the user want to play.
 * @return the difficulty level.
 */
int initializationGame();
/**
 * This function ask for the user's next move, and parse it to SP_COMMAND.
 * @param game - The current game state
 * @return the command that the user entered, one of the SP_COMMAND struct.
 */
SPCommand parseCommand(SPFiarGame* game);
/**
 * This function undo the last turn, and print the column that
 * she take disc from her.
 * @param game - The current game state
 * @return 0 iff the undo is failed.
 */
int spFiarGameUndoPrevMove1(SPFiarGame* src);
/**
 * play the computer turn, according to the minimax algorithm.
 * @param game - The current game state
 * @param difficultLevel - The higher it is, the higher the "wisdom" of the computer
 * @return 0 iff the undo is failed.
 */
void comPlay(SPFiarGame* game,int difficultLevel);
/**
 * enter the disc according to the game's state and the command
 * @param game - The current game state
 * @param spCmd - the command that the function need to Apply.
 * @ïinvariant - spCmd.cmd == ADD_DISC.
 * @return ADD_DISC if the cmd is legal and the disc has entered. otherwise return INVALID_LINE.
 */
SP_COMMAND playAddDisc(SPFiarGame* game,SPCommand spCmd);
/**
 * Given a game state, this function prints the best move according to
 * the current player.
 * @param game - The current game state
 * @param win - the suggest is allowed only if there hasn't winner in the game's state.
 * @param difficultLevel - indicate the width of the minimax tree.
 * @ïinvariant - spCmd.cmd == ADD_DISC.
 * @return the next move of the user.
 */
SP_COMMAND playSuggestedMove(SPFiarGame* game,int difficultLevel, bool win);
/**
 * print that the program exit, and destroy the game.
 * @param game - The current game state
 */
void printQuit();
#endif
