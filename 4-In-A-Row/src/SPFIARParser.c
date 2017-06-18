/*
 * SPFIARParser.c
 *
 *  Created on: 2 αιεπι 2017
 *      Author: Dolev
 */
#include "SPFIARParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
bool spParserIsInt(const char* str){
	int len = strlen(str);
	for(int i = 0; i < len; ++i){
		if(i==0 && str[i] == '-') continue;
		if(str[i] > 57 || str[i] < 48) return false;
	}
	return true;
}
//check if we can get more than 2 param
SPCommand spParserPraseLine(const char* str){
	SPCommand command;
	command.validArg = false;
	command.arg = 0;
	command.cmd = SP_INVALID_LINE;
	bool needInt=false;
	char* line =(char*)malloc((strlen(str)+1)*sizeof(char));
	if(line == NULL){
		PrintMallocError();
		exit(42);
	}
	strcpy(line,str);
	char* delim = " \t\r\n\v\f";
	char* word = strtok(line,delim);
	if(word == NULL) {
		free(line);
		return command;
	}
	if (strcmp(word,"suggest_move") == 0){
		command.cmd = SP_SUGGEST_MOVE;
	}
	else if(strcmp(word,"undo_move") == 0)
		command.cmd = SP_UNDO_MOVE;
	else if(strcmp(word,"quit") == 0)
		command.cmd = SP_QUIT;
	else if(strcmp(word,"restart_game") == 0)
		command.cmd = SP_RESTART;
	else if(strcmp(word,"add_disc") == 0){
		command.cmd = SP_ADD_DISC;
		needInt = true;
	}
	else{
		command.cmd = SP_INVALID_LINE;
	}
	char* word2 = strtok(NULL,delim);
	if(needInt == false && word2 == NULL){
		free(line);
		return command;
	} ////null character
	if(needInt == true && word2 != NULL){
		command.validArg = spParserIsInt(word2);
		command.arg = atoi(word2);
		char* word3 = strtok(NULL,delim);
		if(word3 != NULL || command.validArg == false) {
			command.validArg = false;
			command.cmd = SP_INVALID_LINE;
		}
		free(line);
		return command;
	}
	command.cmd = SP_INVALID_LINE;
	free(line);
	return command;
}
void PrintMallocError(){
	printf("Error: malloc has failed");
}

