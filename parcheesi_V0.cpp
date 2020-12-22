/*  Author: Nicolas Espinosa Mooser
    Date:   22nd Dec. 2020
    Note:   Programmed for UCM Fundamentals of Programming
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>   // For pause
#ifdef _WIN32 // Auto-defined in Visual Studio
#include <windows.h>
#undef max  // In order to use max() in pause()
#endif
using namespace std;

// Global variables and typedef
const bool Debug = true;
const string gameFile = "prueba5.txt";
const int numOfPlayers = 4;     // Number of players
const int numOfPieces = 4;      // Number of pieces per player
const int numOfTiles = 68;      // Number of tiles on the board, excluding ending zone (tiles 100 to 108)
class player;
typedef enum{Yellow, Blue, Red, Green, Grey, None} colour;
typedef player playerArray[numOfPlayers];
typedef colour board[numOfTiles];

// Functions
void initColours();                                                                             // Initialize colours for WINDOWS
void init(playerArray &players, board topBoard, board   bottomBoard, colour &playerTurn);       // Initialize boards to GREY colour
void printBoard(const board &topBoard, const board &bottomBoard, const playerArray players);    // Prints BOARD
void setPrintColour(const colour colour);                                                       // Set cout text colour to colour
bool isSafe(const int position);                                                                // Returns true if position is safe
bool isBridge(const board topBoard, const board bottomBoard, const int position);               // Returns true if a player has 2 pieces at position
void pause();                                                                                   // Waits for user to press Enter
string colourToString(const colour playerColour);                                               // Returns string descrpition of colour
int playerHome(const colour player);                                                            // Returns position of player's home
int piecesAtX(const int positionX, const playerArray players);                                  // Returns the number of pieces at position X
void loadGame(playerArray &players, board &topBoard, board &bottomBoard, colour &playerTurn);                                         // Load game from txt

class player{
    private:
        int piecePos[numOfPieces], lastMovedPiece, playerHome;
        colour playerColour;

    public:
        player(){
            for(int i = 0; i < numOfPieces; i++) piecePos[i] = -1;
        }

        void setPosition(int index, int position){              // Sets piece at index to position (Used for loading game from txt)
            piecePos[index] = position;
        }
        void setPlayerColour(int n){                            // Set player colour
            switch(n){
                case 0:
                    playerColour = Yellow;
                    playerHome = 5;
                    break;
                case 1:
                    playerColour = Blue;
                    playerHome = 22;
                    break;
                case 2:
                    playerColour = Red;
                    playerHome = 39;
                    break;
                case 3:
                    playerColour = Green;
                    playerHome = 56;
                    break;
                default:
                    cerr << "Failed to assign player colour\n";
                    cerr << "Attempted to assign int " << n << "\n";
            }
        }
        void leaveHome(board &topBoard, board &bottomBoard){    // Move piece out of HOME
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] == -1){
                    if(topBoard[playerHome] == playerColour){
                        if(bottomBoard[playerHome] == playerColour){
                            cout << "Your piece can not exit as it is blocked!\n";
                            return;
                        }
                        else{
                            bottomBoard[playerHome] = playerColour;
                            piecePos[i] = playerHome;
                            return;
                        }
                    }
                    else{
                        topBoard[playerHome] = playerColour;
                        piecePos[i] = playerHome;
                        return;
                    }
                }
            }
        }
        void moveBoardPiece(){                                  // Move piece on BOARD
            for(int piece = 0; piece < numOfTiles; piece++){
                if(piecePos[piece] != 1){

                }
            }
        }
        void returnHome(board &topBoard, board &bottomBoard){   // Returns last moved piece to HOME if piece is NOT IN END ZONE
            if(piecePos[lastMovedPiece] > 100) cout << "You rolled 3 consecutive 6's, you lose your turn!\n";
            else{
                cout << "The piece in position " << piecePos[lastMovedPiece] << " has been returned home!\n";
                if(bottomBoard[lastMovedPiece] == playerColour) bottomBoard[lastMovedPiece] = Grey;
                else{
                    topBoard[lastMovedPiece] = Grey;
                }
                piecePos[lastMovedPiece] = -1;

            }
        }
        int getPiecesAtX(int position) const{                   // Returns number of pieces in position X
            int piecesAtX = 0;
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] == position) piecesAtX++;
            }
            return piecesAtX;
        }
        int firstAtX(int position) const{                       // Returns index of FIRST piece to arrive at position
            for(int i = 0; i < numOfPieces; i++) if(piecePos[i] == position) return i;
            return -1;
        }
        int secondAtX(int position) const{                      // Returns index of SECOND piece to arrive at position
            for(int i = firstAtX(position) + 1; i < numOfPieces; i++) if(piecePos[i] == position) return i;
            return -1;
        }
        int getPiecePos(int index) const{                       // Returns piece position of index
            return piecePos[index];
        }
        bool allAtHome() const{                                 // Returns true if ALL pieces are at HOME
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] != -1) return false;
            }
            return true;
        }
        bool hasWon() const{                                    // Returns true if player has won (All pieces in position 108)
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] != 108) return false;
            }
            return true;
        }
};

int main(){
    // Var declaration
    playerArray players;
    board topBoard, bottomBoard;
    colour playerTurn;
    int diceRoll, consecTurn = 0;
    initColours();      // Windows only

    // Board initialization
    init(players, topBoard, bottomBoard, playerTurn);
    if(Debug) loadGame(players, topBoard, bottomBoard, playerTurn);

    // Game
    while(true){
        printBoard(topBoard, bottomBoard, players);
        setPrintColour(playerTurn);
        cout << "It is " << colourToString(playerTurn) << "'s turn\n";
        pause();
        diceRoll = rand() % 6 + 1;
        cout << colourToString(playerTurn) << " rolled a " << diceRoll << "\n";
        if(diceRoll == 5) players[playerTurn].leaveHome(topBoard, bottomBoard);
        else{
            if(consecTurn == 2 && diceRoll == 6){
                players[playerTurn].returnHome(topBoard, bottomBoard); //cout << "Return home!\n";
                for(int i = 0; i < numOfPieces; i++) cout << players[playerTurn].getPiecePos(i) << "\n";
            }
            players[playerTurn].moveBoardPiece();
        }
        if(diceRoll == 6 && consecTurn != 2) consecTurn++;
        else{
            consecTurn = 0;
            playerTurn = colour((int(playerTurn) + 1) % numOfPlayers);
        }
    }

    return 0;
}

void initColours(){
#ifdef _WIN32
   for (DWORD stream : {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE}) {
      DWORD mode;
      HANDLE handle = GetStdHandle(stream);

      if (GetConsoleMode(handle, &mode)) {
         mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
         SetConsoleMode(handle, mode);
      }
   }
#endif
}

void init(playerArray &players, board topBoard, board bottomBoard, colour &playerTurn){
    srand(time(NULL));
    playerTurn = colour(rand() % numOfPlayers);
    for(int i = 0; i < numOfTiles; i++) topBoard[i] = bottomBoard[i] = None;
    colour player = Yellow;
    for(int i = 0; i < numOfPlayers; i++){
        players[player].setPlayerColour(player);
        player = colour((int(player) + 1) % numOfPlayers);
    }
}

void printBoard(const board &topBoard, const board &bottomBoard, const playerArray players){
    // Printing numbers
    for (int i = 0; i < numOfTiles; i++) cout << i / 10; 
    cout << "\n";
    for (int i = 0; i < numOfTiles; i++) cout << i % 10; 
    cout << "\n";
    for(int x = 0; x < numOfTiles; x++) cout << '>';
    cout << "\n";

    // Printing topBoard
    for(int i = 0; i < numOfTiles; i++){
        setPrintColour(Grey);
        if(topBoard[i] != None){
            setPrintColour(topBoard[i]);
            cout << players[topBoard[i]].firstAtX(i) + 1;
        }
        else cout << ' ';
    }
    cout << "\n";

    // Printing safe spots
    for(int i = 0; i < numOfTiles; i++){
        if(isSafe(i)) cout << "o";
        else cout << "-";
    }
    cout << "\n";

    // Printing bottomBoard
    for(int i = 0; i < numOfTiles; i++){
        setPrintColour(Grey);
        if(bottomBoard[i] != None){
            setPrintColour(bottomBoard[i]);
            cout << players[bottomBoard[i]].secondAtX(i) + 1;
        }
        else cout << ' ';
    }
    cout << "\n";

    // Printing BOTTOM division line
    colour player = Yellow;
    for (int i = 0; i < numOfTiles; i++)
        if(i == playerHome(player)-5){
            setPrintColour(player);
            cout << 'V';
            setPrintColour(Grey);
        }
        else if(i == playerHome(player)){
            setPrintColour(player);
            cout << '^';
            setPrintColour(Grey);
            player = colour(int(player) + 1);
        }
        else cout << '>';
    cout << '\n';

    // Printing end board
    int tile, piece;
    for (int i = 0; i < numOfPieces; i++) {
        tile = 0;
        player = Yellow;
        setPrintColour(player);
        while(tile < numOfTiles) {
            if(tile == playerHome(player)-5) {
                piece = players[player].firstAtX(101+i);
                if (piece != -1){
                    cout << piece + 1;
                    if(players[player].getPiecesAtX(101+i) > 1){
                        tile = players[player].secondAtX(101+i);
                        if(piece != -1) cout << piece + 1;
                        else cout << 'V';
                    }
                    else cout << 'V';
                }
                else cout << "VV";
                tile++;
            }
            else if(tile == playerHome(player)) {
                if(players[player].getPiecePos(i) == -1){
                    cout << i+1;
                }
                else cout << '^';
                player = colour((int(player) + 1) % numOfPlayers);
                setPrintColour(player);
            }
            else cout << ' ';
            tile++;
        }
        cout << "\n";
    }
    tile = 0;
    player = Yellow;
    setPrintColour(player);
    while(tile < numOfTiles){
        cout << ((players[player].getPiecePos(0) == 108) ? '1' : '.');
        cout << ((players[player].getPiecePos(1) == 108) ? '2' : '.');
        player = colour(int(player) + 1);
        setPrintColour(player);
        cout << "               ";
        tile += 17;
    }
    cout << "\n";
    tile = 0;
    player = Yellow;
    setPrintColour(player);
    while(tile < numOfTiles){
        cout << ((players[player].getPiecePos(2) == 108) ? '3' : '.');
        cout << ((players[player].getPiecePos(3) == 108) ? '4' : '.');
        player = colour(int(player) + 1);
        setPrintColour(player);
        cout << "               ";
        tile += 17;
    }
    cout << "\n";
    setPrintColour(Grey);
}

void setPrintColour(const colour colour){
    switch(colour){
        case Red:
            cout << "\x1b[31m";
            break;
        case Green:
            cout << "\x1b[32m";
            break;
        case Yellow:
            cout << "\x1b[33m";
            break;
        case Blue:
            cout << "\x1b[34m";
            break;
        case Grey:
        default:
            cout << "\x1b[0m";
    }
}

bool isSafe(const int position){
    if(position == 0 || position == 5 || position == 12 || position == 17 || position == 22 || position == 29 || position == 34 || position == 39 || position == 46 || position == 51 || position == 56 || position == 63) return true;
    else return false;
}

bool isBridge(const board topBoard, const board bottomBoard, const int position){
    if(topBoard[position] == bottomBoard[position]) return true;
    return false;
}

string colourToString(const colour playerColour){
    switch(playerColour){
        case 0:
            return "Yellow";
        case 1:
            return "Blue";
        case 2:
            return "Red";
        case 3:
            return "Green";
        default:
            cerr << "Invalid colour index\n";
    }
    return "Invalid colour index";
}

int playerHome(const colour player){
    switch(player){
        case 0:
            return 5;
        case 1:
            return 22;
        case 2:
            return 39;
        case 3:
            return 56;
        case 4:
        case 5:
            return -1;
        default:
            cerr << "Invalid colour. It has no home\n";
    }
    return -1;
}

int piecesAtX(const int positionX, const playerArray players){
    int numOfPieces = 0;
    for(int i = 0; i < numOfPlayers; i++){
        numOfPieces += players[i].getPiecesAtX(positionX);
    }
    return numOfPieces;
}

void pause(){
   cout << "Press Enter to continue...";
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void loadGame(playerArray &players, board &topBoard, board &bottomBoard, colour &playerTurn){
    ifstream inputFile;
    int player, tile;
    inputFile.open(gameFile);
    if(inputFile){
        for(int i = 0; i < numOfPlayers; i++){
            for(int piece = 0; piece < numOfPieces; piece++){
                inputFile >> tile;
                players[i].setPosition(piece, tile);
                if((tile >= 0) && (tile < numOfTiles)){
                    if(topBoard[tile] == None) topBoard[tile] = colour(i);
                    else bottomBoard[tile] = colour(i);
                }
            }
        }
        inputFile >> player;
        playerTurn = colour(player);
        inputFile.close();
    }
    else cerr << "Failed to open " << gameFile << "\n";
}