/*
Author: Nicolas ESPINOSA MOOSER
Date: 5/11/20
*/

#include <iostream>
#include <time.h>
using namespace std;

// Variables:
int numOfPlayers = 2;
bool endGame = false;   // Set true to end game
int turn = 0;           // Turn counter, range 0 to 3
int consecTurns = 0;    // Counts num of consecutive 
int numOfPieces = 1;    // Number of pieces
string input;

// Implemented:
class player;
bool validName(string name, int turn);
void printBoard();  // Unfinished?
bool isYes(string input);

// To Implement:
bool isBlocked(int x, int diceRoll);        // Returns true for blocked

class player{
    public:
    int piecePos[4];
    int startPos, endPos;
    int turn;
    string name;
    char letter;

    player(){};
    
    player(int playerTurn){
        turn = playerTurn;
        do{
            cout << "Player " << turn+1 << " choose a colour (Yellow, Blue, Red, Green): ";
            cin >> input;
            if(!validName(input, turn)) cout << "Invalid input, please try again!\n";
        }while(!validName(input, turn));
        name = input;
        letter = name[0];
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
        for(int i = 0; i < numOfPieces; i++) piecePos[i] = -1;
    }

    void leaveHome(){
        for(int i = 0; i < numOfPieces; i++){
            if(piecePos[i] == -1 && !isBlocked(startPos,0)){
                piecePos[i] = startPos;
                break;
            }
        }
    }

    int numOfHomePieces(){
        int homePieces = 0;
        for(int i = 0; i < numOfPieces; i++){
            if(piecePos[i] == -1) homePieces++;
        }
        return homePieces;
    }

    int numOfPiecesAtX(int x){
        int numOfPiecesAtX = 0;
        for(int i = 0; i < numOfPieces; i++) if(piecePos[i] == x) numOfPiecesAtX++;
        return numOfPiecesAtX;
    }
};

player Players[4];

bool validName(string name, int turn){
    if(name != "Yellow" && name != "Blue" && name != "Red" && name != "Green") return 0;
    else{
        for(int i = 0; i < numOfPlayers; i++){
            if(turn == i) continue;
            else if(Players[i].name == name) return 0;
        }
    }
    return 1;
}

void printBoard(){
    for(int i = 0; i < 136; i++) cout << "=";
    cout << "\n";
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 10; j++){
            if(i == 6 && j > 7) continue;
            cout << i << "-";
        }
    }  
    cout << "\n";
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 10; j++){
            if(i == 6 && j > 7) continue;
            cout << j << "-";
        }
    }
    cout << "\n";

    // Tablero HOME
    for(int i = 0; i < 4; i++) cout << "oo--------XX------------oo--------";
    cout << "\n";
    bool printed = false;
    for(int i = 0; i < 68; i++){
        for(int j = 0; j < numOfPlayers; j++){
            if(i == Players[j].startPos && Players[j].numOfHomePieces() != 0){
                if(Players[j].numOfHomePieces() == 1) cout << " " << Players[j].letter;
                else if(Players[j].numOfHomePieces() > 1) cout << Players[j].numOfHomePieces() << Players[j].letter;
                printed = true;
            }
        }
        if(!printed) cout << "  ";
        else printed = false;
    }
    cout << "\n";

    // Tablero Abajo
    for(int i = 0; i < 4; i++) cout << "oo--------XX------------oo--------";
    cout << "\n";
    for(int i = 0; i < 68; i++){
        for(int j = 0; j < numOfPlayers; j++){
            for(int x = 0; x < numOfPieces; x++){
                if(i == Players[j].piecePos[x]){
                    if(Players[j].numOfPiecesAtX(Players[j].piecePos[x]) == 1) cout << " " << Players[j].letter;
                    else if(Players[j].numOfPiecesAtX(Players[j].piecePos[x]) > 1) cout << Players[j].numOfPiecesAtX(Players[j].piecePos[x]) << Players[j].letter;
                    printed = true;
                    break;
                }
            }
        }
        if(!printed) cout << "  ";
        else printed = false;
    }
    cout << "\n";
    for(int i = 0; i < 136; i++) cout << "=";
    cout << "\n";
}

bool isBlocked(int x, int diceRoll){        // Returns true if any case between x and x+diceRoll are blocked
    while(diceRoll >= 0){
        for(int i = 0; i < numOfPlayers; i++){
            for(int j = 0; j < numOfPieces; j++){
                if((x + diceRoll) == Players[i].piecePos[j] && Players[i].piecePos[j] == Players[i].piecePos[j+1]) return true;
            }
        }
        diceRoll--;
    }
    return false;
}

bool isYes(string input){
    if(input == "Yes" || input == "YES" || input == "y" || input == "Y") return true;
    else return false;
}

void movePiece(int turn, int diceRoll){
    for(int i = 0; i < numOfPieces; i++){
        if(Players[turn].piecePos[i] != -1 && !isBlocked(Players[turn].piecePos[i], diceRoll)){
            if(numOfPieces == 1){
                Players[turn].piecePos[i] += diceRoll;
                Players[turn].piecePos[i] %= 68;
            }
            else{
                cout << "You can move a piece from -" << Players[turn].piecePos[i] << "- to -" << (Players[turn].piecePos[i] + diceRoll)%68 << "-\n";
                cout << "Would you like to move it? (Y/N): ";
                cin >> input;
                if(isYes(input)){
                    Players[turn].piecePos[i] += diceRoll;
                    Players[turn].piecePos[i] %= 68;
                }
            }
            for(int i = 0; i < numOfPieces; i++){
                for(int j = 0; j < numOfPlayers; j++){
                    if(j == turn) continue;
                    else{
                        for(int x = 0; x < numOfPieces; x++){
                            if(Players[turn].piecePos[i] == Players[j].piecePos[x] && Players[turn].piecePos[i] != -1){
                                Players[j].piecePos[x] = -1;
                                printBoard();
                                cout << Players[turn].name << " eats " << Players[j].name << " and gets 10 extra moves!\n";
                                movePiece(turn, 10);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool winConditionMet(int turn){
    for(int i = 0; i < numOfPieces; i++){
        if(Players[turn].piecePos[i] < Players[turn].startPos && Players[turn].piecePos[i] >= Players[turn].endPos) continue;
        else return false;
    }
    return true;
}

// Yellow Blue Red Green
int main(){
    do{
        cout << "How many players are playing? (1-4): ";
        cin >> numOfPlayers;
        if(numOfPlayers < 1 || numOfPlayers > 4) cout << "Invalid number of players, try again!\n";
    }while(numOfPlayers < 1 || numOfPlayers > 4);
    for(int i = 0; i < numOfPlayers; i++) Players[i] = player(i);
    do{
        printBoard();
        if(consecTurns >= 3){
            cout << Players[turn].name << " rolled 3 consecutive 6's";
            if(Players[turn].numOfHomePieces() <= 3){
                cout << " and must return home\n";
                for(int i = 0; i < numOfPieces; i++){
                    if(Players[turn].piecePos[i] != -1){
                        Players[turn].piecePos[i] = -1;     // This just returns furthest piece home, posibly let player choose which piece to move?
                        printBoard();
                    }
                }
            }
            else cout << "but had no pieces outside of home. Next persons turn!\n";
            turn = (++turn) % 2;
            consecTurns = 0;
        }
        cout  << Players[turn].name << "'s turn ";
        if(consecTurns > 0) cout << "again!\n";
        else cout << "\n";
        int diceRoll;
        do{
            cout << "Enter dice roll: ";
            cin >> diceRoll;
            if(diceRoll <= 0 || diceRoll > 6) cout << "Invalid dice roll! Valid inputs 1-6.\nTry Again!\n";
        }while(diceRoll <= 0 || diceRoll > 6);

        //Case: Has a piece home
        if(Players[turn].numOfHomePieces() != 0){
            if(diceRoll == 5 && !isBlocked(Players[turn].startPos,0)){
                if(Players[turn].numOfHomePieces() == numOfPieces) Players[turn].leaveHome();
                else{
                    cout << "=================================================\n";
                    cout << "You rolled a 5 and can move a piece onto the BOARD\n";
                    if(numOfPieces == 1) Players[turn].leaveHome();
                    else{
                        cout << "Would you like to move the piece out? (Y/N): ";
                        cin >> input;
                        if(isYes(input)) Players[turn].leaveHome();
                        else movePiece(turn, diceRoll);
                    }
                }
            }
            else movePiece(turn, diceRoll);
        }
        else movePiece(turn, diceRoll);
        
        if(winConditionMet(turn)){
            for(int i = 0; i < 136; i++) cout << "=";
            cout << "\n";
            cout << Players[turn].name << " wins!\n";
            cout << "CONGRATULATIONS\n";
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
    }while(!endGame);
    return 0;
}