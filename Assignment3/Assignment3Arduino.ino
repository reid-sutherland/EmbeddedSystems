#include <stdlib.h>    // malloc/calloc
#include <stdio.h>
#include <stdint.h>   // include uint8_t typedefs, not needed for arduino
#include <ctype.h>    // isspace(), tolower()
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

#define NOTE_AS2  20
#define NOTE_D3   21
#define NOTE_F3   0
#define NOTE_FS3  1
#define NOTE_G3   2
#define NOTE_GS3  3
#define NOTE_A3   4
#define NOTE_AS3  5
#define NOTE_B3   6
#define NOTE_C4   7
#define NOTE_CS4  8
#define NOTE_D4   9
#define NOTE_DS4  10
#define NOTE_E4   11
#define NOTE_F4   12
#define NOTE_FS4  13
#define NOTE_G4   14
#define NOTE_GS4  15
#define NOTE_A4   16
#define NOTE_AS4  17
#define NOTE_B4   18
#define NOTE_C5   19

#define NOTE_RS 69

// Encoding information
#define TERMINATOR 0xE0   // Code for R0
#define BINARY_A 0b00000000
#define BINARY_B 0b00100000
#define BINARY_C 0b01000000
#define BINARY_D 0b01100000
#define BINARY_E 0b10000000
#define BINARY_F 0b10100000
#define BINARY_G 0b11000000
#define BINARY_R 0b11100000

// Musical Frequencies
#define FREQ_AS2  116.54
#define FREQ_D3   146.83
#define FREQ_F3   174.61
#define FREQ_FS3  185
#define FREQ_G3   196
#define FREQ_GS3  207.65
#define FREQ_A3   220
#define FREQ_AS3  233.08
#define FREQ_B3   246.94
#define FREQ_C4   261.63
#define FREQ_CS4  277.18
#define FREQ_D4   293.66
#define FREQ_DS4  311.13
#define FREQ_E4   329.63
#define FREQ_F4   349.23
#define FREQ_FS4  369.99
#define FREQ_G4   392
#define FREQ_GS4  415.30
#define FREQ_A4   440
#define FREQ_AS4  466.16
#define FREQ_B4   493.88
#define FREQ_C5   523.25

// Half Periods (in Microseconds)
#define HPM_AS2   4290
#define HPM_D3    3405
#define HPM_F3    2864
#define HPM_FS3   2703
#define HPM_G3    2551
#define HPM_GS3   2408
#define HPM_A3    2273
#define HPM_AS3   2145
#define HPM_B3    2025
#define HPM_C4    1911
#define HPM_CS4   1804
#define HPM_D4    1703
#define HPM_DS4   1607
#define HPM_E4    1517
#define HPM_F4    1432
#define HPM_FS4   1351
#define HPM_G4    1276
#define HPM_GS4   1204
#define HPM_A4    1136
#define HPM_AS4   1073
#define HPM_B4    1012
#define HPM_C5    956


// Max buffer length (obviously lol)
#define MAX_BUFFER_LENGTH 128+1

// Global variables
char userInputBuffer[MAX_BUFFER_LENGTH];
char outputBuffer[MAX_BUFFER_LENGTH];
char songTitles[MAX_NUM_SONGS][MAX_TITLE_LENGTH] =
  {"Call Me Maybe", "moms spaghetti", "oDeSzA", "funkytown"};
uint8_t songs[MAX_NUM_SONGS][MAX_SONG_LENGTH] = {{ (NOTE_B<<5)+2, (NOTE_A<<5)+2, (NOTE_G<<5)+2, (NOTE_R<<5)},{NOTE_R<<5},{NOTE_R<<5},{NOTE_R<<5}};
const char mainMenu[] = "Main Menu\nList Songs\nPlay Song\nCreate Song\n";
const char playMenu[] = "Play Song Menu\nPlay By Number\nSearch By Title\n<-- Go Back\n";


// Required Function headers (with checklist)
void StripEOL(char string[], int n);    // FINISHED, not fully tested (can't simulate \n or \r)
uint8_t DisplayMenu(const char menu[]);   // finished, tested
void ListSongs(char songTitle[MAX_NUM_SONGS][MAX_TITLE_LENGTH]);    // finished, tested
void PlaySong(uint8_t song[]);    // finished, tested
uint8_t PackNote(char letterASCII, uint8_t duration);   // finished, tested
uint8_t UnpackNoteLetterASCII(uint8_t packedNote);    // finished, tested
uint8_t UnpackNoteDuration(uint8_t packednote);   // finished, tested
void StoreSong(uint8_t song[], const char songString[]);    // finished, tested
void PlayNote(uint8_t letterASCII, uint8_t quarters);   // finished, tested
void PlayNotef(uint8_t letterASCII, float quarters);   // EXTRA
int MatchScore(const char countQueryString[], const char templates[]);    // finished, tested

// Additional function headers
void getSerial(int buffer_length);   // I/O
int PlayByNumber();   // Helper
int SearchByTitle();    // Helper
void CreateSong();    // Helper
void print_binary(uint8_t n);   // extra
void test();    // extra
void uarkFightSong();


// Main loop variables
int mmChoice;   // Main Menu choice
int psmChoice;    // Play Song Menu choice
int pbnValid;   // Play By Number choice is valid
int sbtChoice;    // Search By Title choice (Best Match)
typedef enum States {sInit, sMainMenu, sListSongs, sPlaySongMenu, 
                  sCreateSong, sPlayByNumber, sSearchByTitle, sPlaySong} State;
State state = sInit;

// Pin variables
const int piezoPin = A0;
char my_buffer[50] = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  
  pinMode(piezoPin, OUTPUT);
  
  state = sInit;
  mmChoice = 0;
  psmChoice = 0;
  pbnValid = 0;
  sbtChoice = 0;

  uarkFightSong();
}

void loop() {
  // put your main code here, to run repeatedly:
  
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



// **************** EXTRA FUNCTIONS ****************
//  1. Helper functions similar to required
//  2. Random functions used for various testing
// *************************************************

void getSerial(int buffer_length) {
  int i = 0;
  int keep_reading = 1;
  while(keep_reading){
    while(Serial.available() == 0);
    userInputBuffer[i] = Serial.read();
    if(userInputBuffer[i] == '\n'){
      keep_reading = 0;
    }
    if(i == buffer_length-1){
      keep_reading = 0;
    }
    i+=1;
  }
  StripEOL(userInputBuffer,buffer_length);
}

// This function handles the UI and error checking for
// picking a song to play by number.
// returns 1 if a valid song was picked
// returns 0 if invalid song choice
int PlayByNumber() {
  ListSongs(songTitles);
  // prompt user for choice, return to MainMenu if invalid
  sprintf(outputBuffer, "Which song would you like to play? (1-4):  ");
  Serial.print(outputBuffer);
  getSerial(MAX_BUFFER_LENGTH);
  //StripEOL(userInputBuffer, MAX_BUFFER_LENGTH); 
  //fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
  int choice = atoi(userInputBuffer);

  // valid choice
  if (choice > 0 && choice <= MAX_NUM_SONGS) {
    choice--;
    sprintf(outputBuffer, "Selected:    %i.  %s\n", choice+1, songTitles[choice]);
    Serial.print(outputBuffer);
    PlaySong(songs[choice]);
    return 1;
  }
  // invalid choice
  else {
    sprintf(outputBuffer, "\n*** ERROR: Invalid Choice ***\n");
    Serial.print(outputBuffer);
    sprintf(outputBuffer, "returning to Play Song Menu\n");
    Serial.print(outputBuffer);
    return 0;
  }
}

int SearchByTitle() {
  sprintf(outputBuffer, "\nEnter your search query:  ");
  Serial.print(outputBuffer);
  getSerial(MAX_BUFFER_LENGTH-1);
  //fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);

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
    sprintf(outputBuffer, "No matches found\n");
    Serial.print(outputBuffer);
  }
  else {
    sprintf(outputBuffer, "Best Match:    %i.  %s\n", bestMatch+1, songTitles[bestMatch]);
    Serial.print(outputBuffer);
    PlaySong(songs[bestMatch]);
  }
  return (bestMatch+1);
}

// Handles the UI and error detection for creating a new song
// Capitalization of letters is done in StoreSong
void CreateSong() {
  sprintf(outputBuffer, "\n\n=====Create Song=====\n");
  Serial.print(outputBuffer);

  // Prompt for index
  sprintf(outputBuffer, "Which song would you like to overwrite? (1-4):  ");
  Serial.print(outputBuffer);
  getSerial(MAX_BUFFER_LENGTH-1);
//  fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
  int choice = atoi(userInputBuffer);   // ignores everything after the number
  if (choice < 1 || choice > 4) {
    sprintf(outputBuffer, "\n*** ERROR: Invalid Choice ***\n");
    Serial.print(outputBuffer);
    sprintf(outputBuffer, "returning to MainMenu...");
    Serial.print(outputBuffer);
    return;
  }
  int songIndex = choice-1;   // actual song index is offset by 1

  // Prompt for title
  sprintf(outputBuffer, "\nEnter the Title of your Song:  ");
  Serial.print(outputBuffer);
  getSerial(MAX_TITLE_LENGTH);
  //fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
  // store buffer in title
  char title[MAX_TITLE_LENGTH];
  strcpy(title, userInputBuffer);

  // Prompt for song string
  sprintf(outputBuffer, "\nFORMAT: [A-G or R (rest) followed by quarter seconds (0-31)]\n");
  Serial.print(outputBuffer);
  sprintf(outputBuffer, "Enter your Song:  ");
  Serial.print(outputBuffer);
  getSerial(MAX_SONG_LENGTH);
//  fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
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
    if (d >= 0 && d < 32) {   // valid duration
      if (d < 10)
        pch++;  // increment once for single digit
      else
        pch += 2; // increment twice for double digits
    }
    else {
      valid = 0;
    }
    // stop checking if invalid inputs found, return to menu
    if (!valid) {
      sprintf(outputBuffer, "\n*** ERROR: Invalid Song String ***\n");
      Serial.print(outputBuffer);
      sprintf(outputBuffer, "returning to MainMenu...");
      Serial.print(outputBuffer);
      return;
    }
  }

  // Store the valid song string, store the song title
  if (valid) {  // this should be true
    char songString[MAX_SONG_LENGTH];
    strcpy(songString, userInputBuffer);
    strcpy(songTitles[songIndex], title);

    StoreSong(songs[songIndex], songString);
    sprintf(outputBuffer, "\nSong stored successfully!\n");
    Serial.print(outputBuffer);
  }
  return;
}

// print a binary string from uint8_t
// DISCLAIMER: I found this on stack overflow
void print_binary(uint8_t n) {
  int numbits = sizeof(uint8_t) * 8;
  while(--numbits >= 0) {
    sprintf(outputBuffer, "%c", (n & ((uint8_t)1<<numbits)) ? '1' : '0');
    Serial.print(outputBuffer);
  }
  sprintf(outputBuffer, "\n");
  Serial.print(outputBuffer);
}


void uarkFightSong() {
  PlayNotef(20, 2);
  PlayNotef(21, 2);
  PlayNotef(0, 2);
  PlayNotef(3, 2);
  PlayNotef(7, 2);
  PlayNotef(10, 2);
  PlayNotef(14, 1);
  PlayNotef(69, 1);
    
  
  PlayNotef(2, 1);
  PlayNotef(3, 1);
  PlayNotef(5, 2);

  PlayNotef(2, 1);
  PlayNotef(3, 1);
  PlayNotef(5, 2);

  PlayNotef(2, 1);
  PlayNotef(3, 1);
  PlayNotef(5, 1);
  PlayNotef(69, 1);
  PlayNotef(5, 2);
  PlayNotef(69, 1);

  PlayNotef(5, 1);
  PlayNotef(7, 1);
  PlayNotef(9, 1);
  PlayNotef(10, 1);
  PlayNotef(69, 1);
  PlayNotef(10, 2);
  PlayNotef(9, 2);
  PlayNotef(7, 2);
  PlayNotef(5, 5);
  PlayNotef(69, 1);


  PlayNotef(7, 1);
  PlayNotef(5, 1);
  PlayNotef(3, 1);
  PlayNotef(69, 5);

  PlayNotef(5, 1);
  PlayNotef(3, 1);
  PlayNotef(2, 1);
  PlayNotef(69, 5);

  PlayNotef(3, 1);
  PlayNotef(2, 1);
  PlayNotef(0, 2);
  PlayNotef(4, 2);
  PlayNotef(7, 2);
  PlayNotef(9, 2);

  PlayNotef(5, 1);
  PlayNotef(7, 2);
  PlayNotef(8, 1);
  PlayNotef(9, 1);
  PlayNotef(69, 1);

  
  PlayNotef(2, 1);
  PlayNotef(3, 1);
  PlayNotef(5, 2);

  PlayNotef(2, 1);
  PlayNotef(3, 1);
  PlayNotef(5, 2);

  PlayNotef(2, 1);
  PlayNotef(3, 1);
  PlayNotef(5, 1);
  PlayNotef(69, 1);
  PlayNotef(5, 2);
  PlayNotef(69, 1);

  PlayNotef(5, 1);
  PlayNotef(7, 1);
  PlayNotef(9, 1);
  PlayNotef(10, 1);
  PlayNotef(69, 1);
  PlayNotef(10, 2);
  PlayNotef(12, 2);
  PlayNotef(13, 2);
  PlayNotef(14, 5);
  PlayNotef(69, 1);


  PlayNotef(14, 1);
  PlayNotef(12, 1);
  PlayNotef(10, 1);
  PlayNotef(7, 1);
  PlayNotef(7, 1.5);
  PlayNotef(9, 0.5);
  PlayNotef(10, 1);
  PlayNotef(7, 1);
  PlayNotef(7, 1.5);
  PlayNotef(69, 0.5);
  
  PlayNotef(5, 1);
  PlayNotef(2, 1);
  PlayNotef(2, 1.5);
  PlayNotef(3, 0.5);
  PlayNotef(5, 1);
  PlayNotef(2, 1);
  PlayNotef(2, 1);
  PlayNotef(5, 1);
  PlayNotef(10, 1);
  PlayNotef(69, 3);
  PlayNotef(12, 1);
  PlayNotef(69, 3);
  PlayNotef(10, 5);
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
  sprintf(outputBuffer, "\n\n=====");
  Serial.print(outputBuffer);
  token = strtok(menuCopy, "\n");
  sprintf(outputBuffer, "%s", token);
  Serial.print(outputBuffer);
  sprintf(outputBuffer, "=====\n");
  Serial.print(outputBuffer);
  int numEquals = strlen(token) + 10;
  // print each list element
  int optCount = 0;
  token = strtok(NULL, "\n");
  while (token != NULL) {
    optCount++;
    sprintf(outputBuffer, "%d:  %s\n", optCount, token);
    Serial.print(outputBuffer);
    // get the next option if it exists
    token = strtok(NULL, "\n");
  }
  // error if no options to print (we should never get here)
  if (optCount == 1) {
    sprintf(outputBuffer, "Error: No options to display\n");
    Serial.print(outputBuffer);
    return -1;
  }
  // print end of menu
//  for (int i = 0; i < numEquals; i++)
//    sprintf(outputBuffer, "=");
//  sprintf(outputBuffer, "\n");
//  Serial.print(outputBuffer);
  Serial.print("===================\n");

  // UI flow
  // prompt user for choice until they enter a valid choice
  int choice = 0;
  while (1) {
    sprintf(outputBuffer, "Please Enter Choice (1-%d):  ", optCount);
    Serial.print(outputBuffer);
//    int i = 0;
//    int keep_reading = 1;
//    while(keep_reading){
//      while(Serial.available() == 0);
//      userInputBuffer[i] = Serial.read();
//      if(userInputBuffer[i] == '\n'){
//        keep_reading = 0;
//      }
//      if(i == MAX_BUFFER_LENGTH-2){
//        keep_reading = 0;
//      }
//      i+=1;
//    }
    getSerial(MAX_BUFFER_LENGTH);
    //StripEOL(userInputBuffer, MAX_BUFFER_LENGTH);
    //fgets(userInputBuffer, MAX_BUFFER_LENGTH-1, stdin);
    choice = atoi(userInputBuffer);
    // end condition
    if (choice > 0 && choice <= optCount)
      break;
    // loop condition
    else {
      sprintf(outputBuffer, "*** ERROR: Invalid Choice ***\n\n");
      Serial.print(outputBuffer);
    }
  }

  return choice;
}

// *** List Songs ***
void ListSongs(char titles[MAX_NUM_SONGS][MAX_TITLE_LENGTH]) {
  sprintf(outputBuffer, "\n\n=====Song List=====\n");
  Serial.print(outputBuffer);
  // no titles to print
  if (*titles[0] == '\0') {
    sprintf(outputBuffer, "Oops, you don't have any song titles!\n");
    Serial.print(outputBuffer);
    sprintf(outputBuffer, "Nothing to print.\n");
    Serial.print(outputBuffer);
  }
  // print each title
  for (int i = 0; i < MAX_NUM_SONGS; i++) {
    // stop on empty title
    if (*titles[i] == '\0') {
      break;
    }
    sprintf(outputBuffer, "%d:  Title:  %s\n", i+1, titles[i]);
    Serial.print(outputBuffer);
  }
  // print equals
  sprintf(outputBuffer, "===================\n");
  Serial.print(outputBuffer);
}

// *** Play Song ***
void PlaySong(uint8_t song[]) {
  const uint8_t* songPtr = song;  // songs iterator
  uint8_t note, letter, duration; // note features
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
  uint8_t cur = 0x01;
  while (duration) {
    if (duration % 2 == 1)
      code |= cur;
    cur <<= 1;
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
  return (packedNote+65);
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
  const char* cur = songString;
  char letter;
  uint8_t duration;
  // read through the song string
  while (*cur != '\0') {
    // read char
    letter = *cur;
    // capitalize lowercase letters
    if (letter >= 97 && letter <= 122) {
      // TODO: remove this!
      // printf("Capitalizing letter '%c' to letter '%c'\n", letter, letter-32);
      letter -= 32;
    }
    cur++;
    // read uint8
    duration = (uint8_t) atoi(cur);
    // increment pointer by how many digits read
    if (duration > 9)
      cur += 2;
    else
      cur++;

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
  int numIterations = 0;
  int halfPeriodMicroseconds;
  int freq;
  
  switch (letterASCII) {
    case NOTE_A+65:
      numIterations = (FREQ_A3/4)*quarters;
      halfPeriodMicroseconds = HPM_A3;
      freq = FREQ_A3;
      break;
    case NOTE_B+65:
      numIterations = (FREQ_B3/4)*quarters;
      halfPeriodMicroseconds = HPM_B3;
      freq = FREQ_B3;
      break;
    case NOTE_C+65:
      numIterations = (FREQ_C4/4)*quarters;
      halfPeriodMicroseconds = HPM_C4;
      freq = FREQ_C4;
      break;
    case NOTE_D+65:
      numIterations = (FREQ_D4/4)*quarters;
      halfPeriodMicroseconds = HPM_D4;
      freq = FREQ_D4;
      break;
    case NOTE_E+65:
      numIterations = (FREQ_E4/4)*quarters;
      halfPeriodMicroseconds = HPM_E4;
      freq = FREQ_E4;
      break;
    case NOTE_F+65:
      numIterations = (FREQ_F4/4)*quarters;
      halfPeriodMicroseconds = HPM_F4;
      freq = FREQ_F4;
      break;
    case NOTE_G+65:
      numIterations = (FREQ_G4/4)*quarters;
      halfPeriodMicroseconds = HPM_G4;
      freq = FREQ_G4;
      break;
    case NOTE_R:
      numIterations = 250*quarters;
      halfPeriodMicroseconds = 500;
      freq = 1000;
      break;
    default: 
      numIterations = (FREQ_A3/4)*quarters;
      halfPeriodMicroseconds = HPM_A3;
  }
  for (int i = 0; i < numIterations; i++) {
    if (letterASCII != NOTE_R) 
      digitalWrite(piezoPin, HIGH);
      delayMicroseconds(halfPeriodMicroseconds);
      digitalWrite(piezoPin, LOW);
      delayMicroseconds(halfPeriodMicroseconds);
  }
}

// *** Alternative PlayNote Function ***
void PlayNotef(uint8_t letterASCII, float quarters) {
  int numIterations = 0;
  int halfPeriodMicroseconds;
  int freq;

  switch (letterASCII) {
    case NOTE_AS2:
      numIterations = (FREQ_AS2/4)*quarters;
      halfPeriodMicroseconds = HPM_AS2;
      freq = FREQ_F3;
      break;
    case NOTE_D3:
      numIterations = (FREQ_D3/4)*quarters;
      halfPeriodMicroseconds = HPM_D3;
      freq = FREQ_F3;
      break;
    case NOTE_F3:
      numIterations = (FREQ_F3/4)*quarters;
      halfPeriodMicroseconds = HPM_F3;
      freq = FREQ_F3;
      break;
     case NOTE_FS3:
      numIterations = (FREQ_FS3/4)*quarters;
      halfPeriodMicroseconds = HPM_FS3;
      freq = FREQ_F3;
      break;
     case NOTE_G3:
      numIterations = (FREQ_G3/4)*quarters;
      halfPeriodMicroseconds = HPM_G3;
      freq = FREQ_F3;
      break;
     case NOTE_GS3:
      numIterations = (FREQ_GS3/4)*quarters;
      halfPeriodMicroseconds = HPM_GS3;
      freq = FREQ_F3;
      break;
     case NOTE_A3:
      numIterations = (FREQ_A3/4)*quarters;
      halfPeriodMicroseconds = HPM_A3;
      freq = FREQ_F3;
      break;
     case NOTE_AS3:
      numIterations = (FREQ_AS3/4)*quarters;
      halfPeriodMicroseconds = HPM_AS3;
      freq = FREQ_F3;
      break;
     case NOTE_B3:
      numIterations = (FREQ_B3/4)*quarters;
      halfPeriodMicroseconds = HPM_B3;
      freq = FREQ_F3;
      break;
     case NOTE_C4:
      numIterations = (FREQ_C4/4)*quarters;
      halfPeriodMicroseconds = HPM_C4;
      freq = FREQ_F3;
      break;
     case NOTE_CS4:
      numIterations = (FREQ_CS4/4)*quarters;
      halfPeriodMicroseconds = HPM_CS4;
      freq = FREQ_F3;
      break;
     case NOTE_D4:
      numIterations = (FREQ_D4/4)*quarters;
      halfPeriodMicroseconds = HPM_D4;
      freq = FREQ_F3;
      break;
     case NOTE_DS4:
      numIterations = (FREQ_DS4/4)*quarters;
      halfPeriodMicroseconds = HPM_DS4;
      freq = FREQ_F3;
      break;
     case NOTE_E4:
      numIterations = (FREQ_E4/4)*quarters;
      halfPeriodMicroseconds = HPM_E4;
      freq = FREQ_F3;
      break;
     case NOTE_F4:
      numIterations = (FREQ_F4/4)*quarters;
      halfPeriodMicroseconds = HPM_F4;
      freq = FREQ_F3;
      break;
     case NOTE_FS4:
      numIterations = (FREQ_FS4/4)*quarters;
      halfPeriodMicroseconds = HPM_FS4;
      freq = FREQ_F3;
      break;
     case NOTE_G4:
      numIterations = (FREQ_G4/4)*quarters;
      halfPeriodMicroseconds = HPM_G4;
      freq = FREQ_F3;
      break;
     case NOTE_GS4:
      numIterations = (FREQ_GS4/4)*quarters;
      halfPeriodMicroseconds = HPM_GS4;
      freq = FREQ_F3;
      break;
     case NOTE_A4:
      numIterations = (FREQ_A4/4)*quarters;
      halfPeriodMicroseconds = HPM_A4;
      freq = FREQ_F3;
      break;
     case NOTE_AS4:
      numIterations = (FREQ_AS4/4)*quarters;
      halfPeriodMicroseconds = HPM_AS4;
      freq = FREQ_F3;
      break;
     case NOTE_B4:
      numIterations = (FREQ_B4/4)*quarters;
      halfPeriodMicroseconds = HPM_B4;
      freq = FREQ_F3;
      break;
     case NOTE_C5:
      numIterations = (FREQ_C5/4)*quarters;
      halfPeriodMicroseconds = HPM_C5;
      freq = FREQ_F3;
      break;
     case NOTE_RS:
      numIterations = 250*quarters;
      halfPeriodMicroseconds = 500;
      freq = 1000;
      break;
  }
  
  for (int i = 0; i < numIterations; i++) {
    if (letterASCII != NOTE_RS) 
      analogWrite(piezoPin, 24);
//      digitalWrite(piezoPin, HIGH);
      delayMicroseconds(halfPeriodMicroseconds);
      analogWrite(piezoPin, 0);
//      digitalWrite(piezoPin, LOW);
      delayMicroseconds(halfPeriodMicroseconds);
  }
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
  int score = 0;    // match score
  char *pch;  // ptr to traverse tokens

  // can only use strtok for one thing at a time, so
  // let's get those query tokens beforehand, then get template tokens
  char queryTokens[MAX_BUFFER_LENGTH][MAX_BUFFER_LENGTH];
  int queryCount = 0;
  queryPtr = strtok(queryBuffer, " ");  // read first query token
  while (queryPtr != NULL) {
    // convert query token to lowercase
    pch = queryPtr;
    while (*pch != '\0') {
      *pch = tolower(*pch);
      pch++;
    }
    // copy token into query array
    strcpy(queryTokens[queryCount], queryPtr);
    queryPtr = strtok(NULL, " "); // read next query token
    queryCount++;
  }

  // for each query token, read each template token and look for matches
  for (int queryIter = 0; queryIter < queryCount; queryIter++) {
    strcpy(templateBuffer, templates);  // recopy templates into buffer
    templatePtr = strtok(templateBuffer, " ");  // read first template token
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
        score++;  // Match!
        break;
      }
      templatePtr = strtok(NULL, " ");  // read next template token
    }
  }
  return score;
}
