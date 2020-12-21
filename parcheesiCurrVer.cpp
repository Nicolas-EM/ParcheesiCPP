/*
Author: Nicolas ESPINOSA MOOSER
Date created: 4/11/20
*/

#include <iostream>
#include <time.h>
using namespace std;

// Global Variables:
const int numOfPlayers = 2;     // Number of Players
const int numOfPieces = 1;      // Number of pieces per player

// Functions Implemented:
class player;                                           // Player object
bool validName(string name, int turn);                  // Return true if input name is valid colour   
bool isBlocked(int x, int diceRoll);                    // Returns true if position (x + diceRoll) is blocked
bool isYes(string input);                               // Returns true for all versions of yes/Yes/Y/y
bool winConditionMet(int turn);                         // Returns true if player.hasWon is true
bool isSafe(int x);                                     // Return true if positon X is safe  
void movePiece(int turn, int diceRoll);                 // Moves pieces
void printBoard();                                      // Prints HOME board and PLAYING board

class player{
    public:
    int piecePos[numOfPieces];              // Array storing piece positions. THIS VERSION ONLY USES piecePos[0]
    bool hasWon = false;                    // Set true for player to win game
    int startPos, endPos;                   // Players starting and ending position on PLAYING board                             
    string name;                            // Full player Name
    char letter;                            // First letter of Player Name

    player(){};                             // Default constructor NOT USED
    
    player(int playerTurn){                 // Constructor
        playerTurn;

        // Check for valid player name
        string input;
        do{
            cout << "Player " << playerTurn+1 << " choose a colour (Yellow, Blue, Red, Green): ";
            cin >> input;
            if(!validName(input, playerTurn)) cout << "Invalid input, please try again!\n";
        }while(!validName(input, playerTurn));

        name = input;
        letter = name[0];

        // Set start & end positons
        switch(letter){ 
            case 'Y':
                startPos = 5;
                endPos = 0;
                break;
            case 'B':
                startPos = 22;
                endPos = 17;
                break;
            case 'R':
                startPos = 39;
                endPos = 34;
                break;
            case 'G':
                startPos = 56;
                endPos = 51;
                break;
            default:
                cout << "ERROR IN ASIGNING TURN AND START/END POSITIONS\n";
        }
        piecePos[0] = -1;           // Only using piecePos[0] as there is only 1 piece in this VERSION
    }

    void leaveHome(){               // Moves piece from HOME to PLAYING board
        if(piecePos[0] == -1){
            piecePos[0] = startPos;
            return;
        }
    }

    int numOfPiecesAtX(int x){      // Returns number of pieces at X
        int numOfPiecesAtX = 0;
        for(int i = 0; i < numOfPieces; i++) if(piecePos[i] == x) numOfPiecesAtX++;
        return numOfPiecesAtX;
    }

    int numOfBoardPieces(){         // Returns number of pieces on PLAYING BOARD
        int output = 0;
        for(int i = 0; i < numOfPieces; i++){
            if(piecePos[i] != -1 && piecePos[i] < 68) output++;
        }
        return output;
    }
};

player Players[numOfPlayers];           // Initialize array of Players

// GAME
int main(){
    srand(time(NULL));
    bool endGame = false;           // Set true to end game
    int turn = 0;                   // Turn counter, range 0 to numOfPlayers-1
    int consecTurns = 0;            // Counts num of consecutive turns

    for(int i = 0; i < numOfPlayers; i++) Players[i] = player(i);       // Create players
    printBoard();

    do{
        if(consecTurns >= 3){                               // Check number of consecutive turns
            cout << Players[turn].name << " rolled 3 consecutive 6's";
            if(Players[turn].numOfPiecesAtX(-1) <= numOfPieces){
                cout << " and must return home\n";
                for(int i = 0; i < numOfPieces; i++){
                    if(Players[turn].piecePos[i] != -1){
                        Players[turn].piecePos[i] = -1;
                        printBoard();
                    }
                }
            }
            else cout << "but had no pieces outside of home. Next persons turn!\n";
            turn = (++turn) % numOfPlayers;
            consecTurns = 0;
        }

        cout << Players[turn].name << "'s turn ";
        if(consecTurns > 0) cout << "again!\n";
        else cout << "\n";

        string input;
        do{
            cout << "Do you want to roll the die? (Y/N): ";
            cin >> input;
            if(!isYes(input)) cout << "Ok. Try that again?\n";
        }while(!isYes(input));

        int diceRoll;
        diceRoll = rand() % 6 + 1;
        for(int i = 0; i < 136; i++) cout << "=";
        cout << "\n\t\t\t\t\t\t" << Players[turn].name << " rolled a " << diceRoll << "\n";

        if(diceRoll == 5 && Players[turn].numOfPiecesAtX(-1) != 0){      // Moving piece onto BOARD
            Players[turn].leaveHome();
            if(Players[turn].piecePos[0] == Players[(turn+1)%2].piecePos[0]){
                Players[(turn+1)%2].piecePos[0] = -1;
                printBoard();
                cout << Players[turn].name << " eats " << Players[(turn+1)%2].name << " and gets 20 extra moves!\n";
                movePiece(turn, 20);
            }
        }
        else movePiece(turn, diceRoll);

        // Check for win
        if(winConditionMet(turn)){
            for(int i = 0; i < 136; i++) cout << "=";
            cout << "\n\t\t\t\t\t" << Players[turn].name << " wins!\n";
            cout << "\t\t\t\t      CONGRATULATIONS\n";
            for(int i = 0; i < 136; i++) cout << "=";
            cout << "\n";
            endGame = true;
        }

        //Ending Turn
        if(diceRoll != 6){
            turn = (++turn) % numOfPlayers;
            consecTurns = 0;
        }
        else consecTurns++;
        if(!endGame) printBoard();
    }while(!endGame);
    return 0;
}

bool validName(string name, int turn){                  
    if(name != "Yellow" && name != "Blue" && name != "Red" && name != "Green") return false;
    else{
        for(int i = 0; i < numOfPlayers; i++){
            if(turn == i) continue;
            else if(Players[i].name == name) return false;      // If a previous player already has that name return false
        }
    }
    return true;
}

bool isBlocked(int turn, int x, int diceRoll){      // If player (turn+1)%2 is at (x + diceRoll) AND isSafe return true;      
    if(Players[(turn+1)%2].piecePos[0] == (x + diceRoll) && isSafe(Players[(turn+1)%2].piecePos[0])) return true;
    return false;
}

bool isYes(string input){                               
    if(input == "Yes" || input == "YES" || input == "y" || input == "Y" || input == "1") return true;
    else return false;
}

bool winConditionMet(int turn){                         
    if(Players[turn].hasWon) return true;       // If player turn has won, return true
    else return false;
}

bool isSafe(int x){                               
    if(x == 0 || x == 5 || x == 12 || x == 17 || x == 22 || x == 29 || x == 34 || x == 39 || x == 46 || x == 51 || x == 56 || x == 63 || x >= 69) return true;
    else return false;
}

void movePiece(int turn, int diceRoll){
    if(Players[turn].piecePos[0] != -1 && !isBlocked(turn, Players[turn].piecePos[0], diceRoll)){         // Move PLAYING board pieces
        // Move only if will not move past end Position
        if(!(((Players[turn].piecePos[0] + diceRoll) % 68) > Players[turn].endPos && ((Players[turn].piecePos[0] + diceRoll) % 68) <= Players[turn].startPos)){
            Players[turn].piecePos[0] += diceRoll;
            Players[turn].piecePos[0] %= 68;
            if(Players[turn].piecePos[0] == Players[turn].endPos){      // Win condition
                Players[turn].hasWon = true;
                return;
            }
            if(Players[turn].piecePos[0] == Players[(turn+1)%2].piecePos[0] && Players[turn].piecePos[0] != -1){    // Check eat piece 
                Players[(turn+1)%2].piecePos[0] = -1;
                printBoard();
                for(int i = 1; i < 21; i++){
                    if((Players[turn].piecePos[0] + i)%68 == Players[turn].endPos){     // Will not move past end position with 20 moves
                        cout << Players[turn].name << " eats " << Players[(turn+1)%2].name << " but can't move 20 extra moves!\n";
                        return;
                    }
                }
                cout << Players[turn].name << " eats " << Players[(turn+1)%2].name << " and gets 20 extra moves!\n";
                movePiece(turn, 20);
                return;
            }
            return;
        }
    }                 
    // If can't move
    for(int i = 0; i < 136; i++) cout << "=";
    cout << "\n\t\t\t\t\t" <<  Players[turn].name << " couldn't move any pieces!\n";
}

void printBoard(){                                      
    for(int i = 0; i < 136; i++) cout << "=";
    cout << "\n";
    // Top numbers (10's values)
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 10; j++){
            if(i == 6 && j > 7) continue;
            cout << i;
        }
    }  
    cout << "\n";
    // Bottom numbers (1's values)
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 10; j++){
            if(i == 6 && j > 7) continue;
            cout << j;
        }
    }
    cout << "\n";

    // HOME board
    for(int i = 0; i < 4; i++) cout << "o----X------o----";
    cout << "\n";
    bool printed = false;
    for(int i = 0; i < 68; i++){
        for(int j = 0; j < numOfPlayers; j++){
            if(i == Players[j].startPos && Players[j].numOfPiecesAtX(-1) != 0){
                cout << Players[j].letter;
                printed = true;
            }
        }
        if(!printed) cout << " ";
        else printed = false;
    }
    cout << "  HOME\n";

    // PLAYING board
    for(int i = 0; i < 4; i++) cout << "o----X------o----";
    cout << "\n";
    for(int i = 0; i < 68; i++){
        for(int player = 0; player < numOfPlayers; player++){
            if(Players[player].piecePos[0] == i){
                cout << Players[player].letter;
                printed = true;
            }
        }
        if(!printed) cout << " ";
        else printed = false;
    }
    cout << "  BOARD\n";

    cout << "\n";
    for(int i = 0; i < 136; i++) cout << "=";
    cout << "\n";
}