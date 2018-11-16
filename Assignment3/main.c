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

// Encoding information
#define BINARY_A 0b00000000
#define BINARY_B 0b00100000
#define BINARY_C 0b01000000
#define BINARY_D 0b01100000
#define BINARY_E 0b10000000
#define BINARY_F 0b10100000
#define BINARY_G 0b11000000
#define BINARY_R 0b11100000

// Max buffer length (obviously lol)
#define MAX_BUFFER_LENGTH 128+1

// Global variables
char userInputBuffer[MAX_BUFFER_LENGTH];
char songTitles[MAX_NUM_SONGS][MAX_TITLE_LENGTH] = {"Title1", "Title2", "Title3", "Title4"};
char songs[MAX_NUM_SONGS][MAX_SONG_LENGTH] = {{(NOTE_B<<5)+2 ,(NOTE_A<<5) +2,(NOTE_G<<5) +2 },{NOTE_R<<5},{NOTE_R<<5},{NOTE_R<<5}};


// Function headers (with checklist)
void StripEOL(char string[], int n);		// FINISHED, not fully tested (can't simulate \n or \r)
uint8_t DisplayMenu(const char menu[]);		// FINISHED, TESTED
void ListSongs(char songTitle[MAX_NUM_SONGS][MAX_TITLE_LENGTH]);		// FINISHED, TESTED
void PlaySong(uint8_t song[]);		// not started
uint8_t PackNote(char letterASCII, uint8_t duration);		// FINISHED, TESTED
uint8_t UnpackNoteLetterASCII(uint8_t packedNote);		// FINISHED, TESTED
uint8_t UnpackNoteDuration(uint8_t packednote);		// FINISHED, TESTED
void StoreSong(uint8_t song[], const char songString[]);		// not started
void PlayNote(uint8_t letterASCII, uint8_t quarters);		// not started
int MatchScore(const char countQueryString[], const char templates[]);		// not started
void print_binary(uint8_t n);		// extra


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

		// TEST functions
		// get rid of the endline character
		StripEOL(userInputBuffer, strlen(userInputBuffer));
		// uint8_t choice = DisplayMenu(menuPlay);
		// ListSongs(songTitles);
		// uint8_t note = PackNote('E', (uint8_t) 0);
		// char test = UnpackNoteLetterASCII(note);
		// uint8_t test2 = UnpackNoteDuration(note);
	}

	return 0;
}

// ********** FUNCTIONS listed here **********
//! Since these are provided functions with descriptions in the assignment document,
//  I'm going to stop pretending to describe them in my own words as if I came up with them :)


// *** Strip EOL ***
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


// *** Display Menu ***
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


// *** List Songs ***
void ListSongs(char titles[MAX_NUM_SONGS][MAX_TITLE_LENGTH]) {
	printf("\n=====Song List=====\n");
	// no titles to print
	if (*titles[0] == '\0') {
		printf("Oops, you don't have any song titles!\n");
		printf("Nothing to print.\n");
	}
	// print each title
	for (int i = 0; i < MAX_NUM_SONGS; i++) {
		// stop on empty title
		if (*titles[i] == '\0') {
			break;
		}
		printf("%d:  Title:  %s\n", i+1, titles[i]);
	}
	// print equals
	printf("===================\n\n");
}


// *** Play Song ***
void PlaySong(uint8_t song[]);


// *** Pack/Unpack Note Functions ***
// PackNote takes input provided from a songString and packs into bytes as described
// above. Note that the letter is NOT stored in ASCII.
// This function assumes that the inputs are valid, and capitalized
uint8_t PackNote(char letterASCII, uint8_t duration) {
	uint8_t note = 0x00;

	// just in case, make sure duration is not more than 31
	if (duration > 31) {
		duration = 31;
	}
	// set the lower 5 bits based on duration
	// TODO: THIS IS TOTALLY UNNECESSARY
	uint8_t code = 0x00;
	uint8_t cursor = 0x01;
	while (duration) {
		if (duration % 2 == 1)
			code |= cursor;
		cursor <<= 1;
		duration /= 2;
	}
	// write the 5 bit code to the note
	note = code;

	// set the upper 3 bits based on letter value
	switch (letterASCII) {
		case 'A':
			note = note | BINARY_A;
			break;
		case 'B':
			note = note | BINARY_B;
			break;
		case 'C':
			note = note | BINARY_C;
			break;
		case 'D':
			note = note | BINARY_D;
			break;
		case 'E':
			note = note | BINARY_E;
			break;
		case 'F':
			note = note | BINARY_F;
			break;
		case 'G':
			note = note | BINARY_G;
			break;
		case 'R':
			note = note | BINARY_R;
			break;
		// default case should not be reached
		default:
			note = note | 0b11111111;
	}

	return note;
}
// UnpackNoteLetterASCII returns ASCII character of a packed note.
// Returns capital letters
uint8_t UnpackNoteLetterASCII(uint8_t packedNote) {
	packedNote >>= 5;

	if (packedNote == 7) {
		// return 'R'
		return (uint8_t) 'R';
	}
	// 'A' starts on 65
	return (packedNote + 65);
}
// UnpackNoteDuration returns the number of quarter seconds a note will be played.
uint8_t UnpackNoteDuration(uint8_t packedNote) {
	// clear the last 3 bits
	return (packedNote & 0x1F);
}


// *** Store Song ***
// Capitalizes letters found in songString before packing
void StoreSong(uint8_t song[], const char songString[]) {
	// pointer to first character in songString
	char* cursor = songString;
	while (*cursor != '\0') {
		
	}
}


// *** Play Note ***
void PlayNote(uint8_t letterASCII, uint8_t quarters);


// *** Matching Function
int MatchScore(const char countQueryString[], const char templates[]);


// print a binary string from uint8_t
// DISCLAIMER: I found this on stack overflow
void print_binary(uint8_t n) {
	int numbits = sizeof(uint8_t) * 8;
	while(--numbits >= 0) {
		printf("%c", (n & ((uint8_t)1<<numbits)) ? '1' : '0');
	}
	printf("\n");
}
