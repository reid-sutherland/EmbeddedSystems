#include <stdlib.h>		// malloc/calloc
#include <stdio.h>
#include <stdint.h>		// include uint8_t typedefs, not needed for arduino
#include <ctype.h>		// isspace(), tolower()
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
#define TERMINATOR 0xE0		// Code for R0
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
char songTitles[MAX_NUM_SONGS][MAX_TITLE_LENGTH] =
	{"Hello", "Hi Hello hello helLo", "oDeSzA", "there  hi hi hello hello"};
uint8_t songs[MAX_NUM_SONGS][MAX_SONG_LENGTH] = {{ (NOTE_B<<5)+2, (NOTE_A<<5)+2, (NOTE_G<<5)+2, (NOTE_R<<5)},{NOTE_R<<5},{NOTE_R<<5},{NOTE_R<<5}};
int numSongs;


// Required Function headers (with checklist)
void StripEOL(char string[], int n);		// FINISHED, not fully tested (can't simulate \n or \r)
uint8_t DisplayMenu(const char menu[]);		// finished, tested
void ListSongs(char songTitle[MAX_NUM_SONGS][MAX_TITLE_LENGTH]);		// finished, tested
void PlaySong(uint8_t song[]);		// finished, tested
uint8_t PackNote(char letterASCII, uint8_t duration);		// finished, tested
uint8_t UnpackNoteLetterASCII(uint8_t packedNote);		// finished, tested
uint8_t UnpackNoteDuration(uint8_t packednote);		// finished, tested
void StoreSong(uint8_t song[], const char songString[]);		// finished, tested
void PlayNote(uint8_t letterASCII, uint8_t quarters);		// NOT STARTED
int MatchScore(const char countQueryString[], const char templates[]);		// finished, tested
// Additional function headers
int PlayByNumber();		// Helper
int SearchByTitle();		// Helper
void CreateSong();		// Helper
void print_binary(uint8_t n);		// extra
void testMainLoop();		// extra


int main() {

	const char menuMain[] = "Main Menu\nList Songs\nPlay Song\nCreate Song\n";
	const char menuPlay[] = "Play Song Menu\nPlay By Number\nSearch By Title\n<-- Go Back\n";

	// count number of songs (that exist)
	numSongs = 0;
	for (int i = 0; i < MAX_NUM_SONGS; i++) {
		// increment if not null
		if (songs[i][0] != '\0')
			numSongs++;
	}

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

		if (strcmp(userInputBuffer, "test") == 0)
			testMainLoop();

		// TEST functions
		// Display Menu
		// uint8_t choice = DisplayMenu(menuPlay);
		// List Songs
		// ListSongs(songTitles);
		// Pack/Unpack functions
		// uint8_t note = PackNote('E', (uint8_t) 0);
		// char test = UnpackNoteLetterASCII(note);
		// uint8_t test2 = UnpackNoteDuration(note);

		// Match Score (Choose song functionality)
		// int score = 0, topScore = 0, bestMatch = 0;
		// for (int i = 0; i < MAX_NUM_SONGS; i++) {
		// // printf("\n\n%i\n", i);
		// 	score = MatchScore(userInputBuffer, songTitles[i]);
		// 	if (score > topScore) {
		// 		topScore = score;
		// 		bestMatch = i;
		// 	}
		// }
		// // No matches found
		// if (topScore == 0) {
		// 	printf("No matches found\n");
		// }
		// else {
		// 	printf("Best Match:    %i.  %s\n", bestMatch+1, songTitles[bestMatch]);
		// 	printf("PlaySong()\n");
		// }

		// Store Song
		// const char songStringTest[] = "B2A2G3R1B10R0A20G30";
		// uint8_t* songTest = (uint8_t*)calloc(MAX_SONG_LENGTH, sizeof(uint8_t));
		// StoreSong(songTest, songStringTest);
		// while (*songTest != TERMINATOR) {
		// 	print_binary(*songTest);
		// 	songTest++;
		// }
		// print_binary(*songTest);
		// printf("\n");

		// PlaySong
		// const char songStringTest[] = "B2A2G3R1B10R0A20G30";
		// uint8_t* songTest = (uint8_t*)calloc(MAX_SONG_LENGTH, sizeof(uint8_t));
		// StoreSong(songTest, songStringTest);
		// PlaySong(songTest);

		// PlayNote

	}

	return 0;
}


// **************** EXTRA FUNCTIONS ****************
//  1. Helper functions similar to required
//  2. Random functions used for various testing
// *************************************************

// This function handles the UI and error checking for
// picking a song to play by number.
// returns 1 if a valid song was picked
// returns 0 if invalid song choice
int PlayByNumber() {
	ListSongs(songTitles);
	// prompt user for choice, return to MainMenu if invalid
	printf("Which song would you like to play? (1-4):  ");
	fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
	int choice = atoi(userInputBuffer);

	// valid choice
	if (choice > 0 && choice <= MAX_NUM_SONGS) {
		choice--;
		printf("Selected:    %i.  %s\n", choice+1, songTitles[choice]);
		PlaySong(songs[choice]);
		return 1;
	}
	// invalid choice
	else {
		printf("\n*** ERROR: Invalid Choice ***\n");
		printf("returning to Play Song Menu\n");
		return 0;
	}
}

int SearchByTitle() {
	printf("\nEnter your search query:  ");
	fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
	// get rid of the endline character
	StripEOL(userInputBuffer, MAX_BUFFER_LENGTH);

	int score = 0, topScore = 0, bestMatch = 0;
	for (int i = 0; i < MAX_NUM_SONGS; i++) {
		score = MatchScore(userInputBuffer, songTitles[i]);
		if (score > topScore) {
			topScore = score;
			bestMatch = i;
		}
	}
	// No matches found
	if (topScore == 0) {
		printf("No matches found\n");
	}
	else {
		printf("Best Match:    %i.  %s\n", bestMatch+1, songTitles[bestMatch]);
		PlaySong(songs[bestMatch]);
	}
	return (bestMatch+1);
}

// Handles the UI and error detection for creating a new song
// Capitalization of letters is done in StoreSong
// Returns 1 and stores the song if no invalid inputs
// Returns 0 and does not store if invalid inputs are present
void CreateSong() {
	printf("\n=====Create Song=====\n");

	// Prompt for index
	printf("Which song would you like to overwrite? (1-4):  ");
	fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
	int choice = atoi(userInputBuffer);		// ignores everything after the number
	if (choice < 1 || choice > 4) {
		printf("\n*** ERROR: Invalid Choice ***\n");
		printf("returning to MainMenu...\n\n");
		return;
	}
	int songIndex = choice-1;		// actual song index is offset by 1

	// Prompt for title
	printf("Enter the Title of your Song:  ");
	fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
	// get rid of the endline character
	StripEOL(userInputBuffer, MAX_TITLE_LENGTH);
	// store buffer in title
	char title[MAX_TITLE_LENGTH];
	strcpy(title, userInputBuffer);

	// Prompt for song string
	printf("FORMAT: [A-G or R (rest) followed by quarter seconds (0-31)]\n");
	printf("Enter your Song:  ");
	fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
	// get rid of the endline character
	StripEOL(userInputBuffer, MAX_SONG_LENGTH);
	// check for valid inputs
	int valid = 1;
	char* pch = userInputBuffer;
	while (*pch != '\0') {
		// check for valid letter (ABCDEFGR abcdefgr)
		char c = *pch;
		if (  (c >= 65 && c <= 71) ||   // A-G
				   c == 82 ||               // R
				  (c >= 97 && c <= 103) ||  // a-g
				   c == 114 ) {             // r
			valid = 1;
		}
		else {
			valid = 0;
		}
		pch++;

		// check for valid duration (0-31)
		int d = atoi(pch);
		// atoi returns 0 if letters found, so make sure its a digit
		if (!isdigit(*pch))
			valid = 0;
		if (d >= 0 && d < 32)	{		// valid duration
			if (d < 10)
				pch++;	// increment once for single digit
			else
				pch += 2;	// increment twice for double digits
		}
		else {
			valid = 0;
		}
		// stop checking if invalid inputs found, return to menu
		if (!valid) {
			printf("\n*** ERROR: Invalid Song String ***\n");
			printf("returning to MainMenu...\n\n");
			return;
		}
	}

	// Store the valid song string, store the song title
	if (valid) {	// this should be true
		char songString[MAX_SONG_LENGTH];
		strcpy(songString, userInputBuffer);
		strcpy(songTitles[songIndex], title);

		StoreSong(songs[songIndex], songString);
		printf("\nSong stored successfully!\n");
	}
	return;
}

// print a binary string from uint8_t
// DISCLAIMER: I found this on stack overflow
void print_binary(uint8_t n) {
	int numbits = sizeof(uint8_t) * 8;
	while(--numbits >= 0) {
		printf("%c", (n & ((uint8_t)1<<numbits)) ? '1' : '0');
	}
	printf("\n");
}

void testMainLoop() {
	const char mainMenu[] = "Main Menu\nList Songs\nPlay Song\nCreate Song\n";
	const char playMenu[] = "Play Song Menu\nPlay By Number\nSearch By Title\n<-- Go Back\n";
	int mmChoice = 0;		// Main Menu choice
	int psmChoice = 0;		// Play Song Menu choice
	int pbnValid = 0;		// Play By Number choice is valid
	int sbtChoice = 0;		// Search By Title choice (Best Match)
	static enum States {sInit, sMainMenu, sListSongs, sPlaySongMenu, sCreateSong, sPlayByNumber, sSearchByTitle, sPlaySong} state;

	state = sInit;
	// Main menu
	while (1) {
		// State Transitions
		switch (state) {
			case sInit:
				state = sMainMenu;
				break;
			case sMainMenu:
				if (mmChoice == 1)
					state = sListSongs;
				else if (mmChoice == 2)
					state = sPlaySongMenu;
				else if (mmChoice == 3)
					state = sCreateSong;
				else
					state = sMainMenu;
				// reset choice
				mmChoice = 0;
				break;
			case sListSongs:
				if (1)
					state = sMainMenu;
				break;
			case sPlaySongMenu:
				if (psmChoice == 1)
					state = sPlayByNumber;
				else if (psmChoice == 2)
					state = sSearchByTitle;
				else if (psmChoice == 3)
					state = sMainMenu;
				else
					state = sPlaySongMenu;
				// reset choice
				psmChoice = 0;
				break;
			case sCreateSong:
				if (1)
					state = sMainMenu;
				break;
			case sPlayByNumber:
				if (pbnValid)
					state = sPlaySong;
				else if (!pbnValid)
					state = sPlaySongMenu;
				break;
			case sSearchByTitle:
				if (sbtChoice > 0)
					state = sPlaySong;
				else if (sbtChoice == 0)
					state = sPlaySongMenu;
				break;
			case sPlaySong:
				if (1)
					state = sMainMenu;
				break;
			default:
				state = sInit;
				break;
		}

		// State Actions
		switch (state) {
			case sInit:
				break;
			case sMainMenu:
				mmChoice = DisplayMenu(mainMenu);
				break;
			case sListSongs:
				ListSongs(songTitles);
				break;
			case sPlaySongMenu:
				psmChoice = DisplayMenu(playMenu);
				break;
			case sCreateSong:
				CreateSong();
				break;
			case sPlayByNumber:
				pbnValid = PlayByNumber();
				break;
			case sSearchByTitle:
				sbtChoice = SearchByTitle();
				break;
			case sPlaySong:
				break;
			default:
				break;
		}
	}
}


// ****************** REQUIRED FUNCTIONS LISTED HERE ******************
//! Since these are provided functions with descriptions
//  in the assignment document, I'm going to stop pretending
//  to describe them in my own words as if I came up with them :)
// ********************************************************************

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

	// Print menu
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

	// UI flow
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
			printf("*** ERROR: Invalid Choice ***\n\n");
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
	printf("===================\n");
}

// *** Play Song ***
void PlaySong(uint8_t song[]) {
	printf("\nPlaySong\n");
	const uint8_t* songPtr = song;	// songs iterator
	uint8_t note, letter, duration;	// note features
	// loop through song notes
	note = *songPtr;
	while (note != TERMINATOR) {
		letter = UnpackNoteLetterASCII(note);
		duration = UnpackNoteDuration(note);
		// play the note
		PlayNote(letter, duration);
		note = *++songPtr;
	}
}

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
	// clear the first 3 bits
	return (packedNote & 0x1F);
}

// *** Store Song ***
// Capitalizes letters found in songString before packing
// Assumes the songsString is valid (checking done in CreateSong)
// Does not need a terminating R0, but will stop if it finds one
void StoreSong(uint8_t song[], const char songString[]) {
	// pointer to current song being written to
	uint8_t* songIndex = song;
	// pointer to first character in songString
	const char* cursor = songString;
	char letter;
	uint8_t duration;
	// read through the song string
	while (*cursor != '\0') {
		// read char
		letter = *cursor;
		// capitalize lowercase letters
		if (letter >= 97 && letter <= 122) {
			// TODO: remove this!
			// printf("Capitalizing letter '%c' to letter '%c'\n", letter, letter-32);
			letter -= 32;
		}
		cursor++;
		// read uint8
		duration = (uint8_t) atoi(cursor);
		// increment pointer by how many digits read
		if (duration > 9)
			cursor += 2;
		else
			cursor++;

		// terminate here if R0 found
		if (letter == 'R' && duration == 0) {
			break;
		}
		// pack note into byte
		uint8_t noteByte = PackNote(letter, duration);
		// add note to song
		*songIndex = noteByte;
		songIndex++;
	}

	// append terminating character last
	uint8_t noteByte = PackNote('R', 0);
	*songIndex = noteByte;
}

// *** Play Note ***
void PlayNote(uint8_t letterASCII, uint8_t quarters) {
	printf("Play Note for %c%i\n", (char)letterASCII, quarters);
}

// *** Matching Function
int MatchScore(const char countQueryString[], const char templates[]) {
	// score is 0 if either string is null
	if (countQueryString[0] == '\0' || templates[0] == '\0')
		return 0;

	// pointers to act as cursors
	char* queryPtr;
	char* templatePtr;
	// allocate memory and copy string
	char* queryBuffer = (char*) calloc(MAX_BUFFER_LENGTH, sizeof(char));
	strcpy(queryBuffer, countQueryString);
	char* templateBuffer = (char*) calloc(MAX_SONG_LENGTH, sizeof(char));
	strcpy(templateBuffer, templates);

	//! Read each token from both strings
	int score = 0;		// match score
	char *pch;	// ptr to traverse tokens

	// can only use strtok for one thing at a time, so
	// let's get those query tokens beforehand, then get template tokens
	char queryTokens[MAX_BUFFER_LENGTH][MAX_BUFFER_LENGTH];
	int queryCount = 0;
	queryPtr = strtok(queryBuffer, " ");	// read first query token
	while (queryPtr != NULL) {
		// convert query token to lowercase
		pch = queryPtr;
		while (*pch != '\0') {
			*pch = tolower(*pch);
			pch++;
		}
		// copy token into query array
		strcpy(queryTokens[queryCount], queryPtr);
		queryPtr = strtok(NULL, " ");	// read next query token
		queryCount++;
	}

	// for each query token, read each template token and look for matches
	for (int queryIter = 0; queryIter < queryCount; queryIter++) {
		strcpy(templateBuffer, templates);	// recopy templates into buffer
		templatePtr = strtok(templateBuffer, " ");	// read first template token
		while (templatePtr != NULL) {
			// convert template token to lowercase
			pch = templatePtr;
			while (*pch != '\0') {
				*pch = tolower(*pch);
				pch++;
			}

			// increment score on first match
			// move on to next query token
			if (strcmp(queryTokens[queryIter], templatePtr) == 0) {
				score++;	// Match!
				break;
			}
			templatePtr = strtok(NULL, " ");	// read next template token
		}
	}
	return score;
}
