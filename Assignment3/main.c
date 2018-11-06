#include <stdlib.h>		// malloc/calloc
#include <stdio.h>
#include <stdint.h>		// include uint8_t typedefs, not needed for arduino
#include <ctype.h>		// isspace()
#include <string.h>

// Song information
#define MAX_NUM_SONGS 4
#define MAX_SONG_LENGTH 64
#define MAX_TITLE_LENGTH 64
#define NOTE_A 0
#define NOTE_B 1
#define NOTE_C 2
#define NOTE_D 3
#define NOTE_E 4
#define NOTE_F 5
#define NOTE_G 6
#define NOTE_R 7

// Max buffer length (obviously lol)
#define MAX_BUFFER_LENGTH 128+1

// Global variables
char userInputBuffer[MAX_BUFFER_LENGTH];
char songTitles[MAX_NUM_SONGS][MAX_TITLE_LENGTH] = {"Title1", "Title2", "Title3", "Title4"};
char song[MAX_NUM_SONGS][MAX_SONG_LENGTH] = {{(NOTE_B<<5)+2 ,(NOTE_A<<5) +2,(NOTE_G<<5) +2 },{NOTE_R<<5},{NOTE_R<<5},{NOTE_R<<5}};


// Function headers (with checklist)
void StripEOL(char string[], int n);		// FINISHED, not fully tested
uint8_t DisplayMenu(const char menu[]);		// FINISHED, TESTED
void ListSongs(char songTitle[MAX_NUM_SONGS][MAX_TITLE_LENGTH]);		// not started
void PlaySong(uint8_t song[]);		// not started
uint8_t PackNote(char letterASCII, uint8_t duration);		// not started
uint8_t UnpackNoteLetterASCII(uint8_t packedNote);		// not started
uint8_t UnpackNoteDuration(uint8_t packednote);		// not started
void StoreSong(uint8_t song[], const char songString[]);		// not started
void PlayNote(uint8_t letterASCII, uint8_t quarters);		// not started
int MatchScore(const char countQueryString[], const char templates[]);		// not started


int main() {

	const char menuMain[] = "Main Menu\nCreate Song\nPlay Song\nList Songs\n";
	const char menuPlay[] = "Play Menu\nSearch By Title\nNumber\n";

	// LOOP
	while (1) {

		// print >>>
		printf("Test >>> ");
		// read input
		fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);

		// check for whitespace
		char* pch = userInputBuffer;
		int isSpace = 1;
		while (*pch != '\0') {
			if (!isspace((unsigned char) *pch))
				isSpace = 0;
			pch++;
		}
		// if whitespace, do nothing
		if (isSpace == 1 || strcmp(userInputBuffer, "") == 0)
			continue;
		// check for exit
		if (strcmp(userInputBuffer, "exit()\n") == 0 || strcmp(userInputBuffer, "exit\n") == 0)
			break;
		// get rid of the endline character
		StripEOL(userInputBuffer, strlen(userInputBuffer));
		DisplayMenu(menuPlay);
	}

	return 0;
}

// Function that searchs string for a \n or \r
//  when found, replaces token with \0, and stops
void StripEOL(char string[], int n) {
	char *pch = string;
	while (pch < string+n) {
		if (*pch == '\n' || *pch == '\r') {
			*pch = '\0';
			return;
		}
		pch++;
	}
}

// Since these are provided functions with descriptions in the assignment document,
//  I'm going to stop pretending to describe them in my own words as if I came up with them

//DisplayMenu
// it only reads until the first non-number of user input, everything else is discarded
uint8_t DisplayMenu(const char menu[]) {
	// create a new char array for tokenizing the menu string
	char* menuCopy = (char*)calloc(strlen(menu), sizeof(char));
	strcpy(menuCopy, menu);
	char* token;

	// print menu title
	printf("\n=====");
	token = strtok(menuCopy, "\n");
	printf("%s", token);
	printf("=====\n");
	int numEquals = strlen(token) + 10;
	// print each list element
	int optCount = 0;
	token = strtok(NULL, "\n");
	while (token != NULL) {
		optCount++;
		printf("%d:  %s\n", optCount, token);
		// get the next option if it exists
		token = strtok(NULL, "\n");
	}
	// error if no options to print (we should never get here)
	if (optCount == 1) {
		printf("Error: No options to display\n");
		return -1;
	}
	// print end of menu
	for (int i = 0; i < numEquals; i++)
		printf("=");
	printf("\n");
	// prompt user for choice until they enter a valid choice
	int choice = 0;
	while (1) {
		printf("Please Enter Choice (1-%d):  ", optCount);
		fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
		choice = atoi(userInputBuffer);
		// end condition
		if (choice > 0 && choice <= optCount)
			break;
		// loop condition
		else
			printf("*** Invalid Input ***\n\n");
	}

	return choice;
}
