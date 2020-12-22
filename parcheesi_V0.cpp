/*  Author: Nicolas Espinosa Mooser
    Date:   22nd Dec. 2020
    Note:   Programmed for UCM Fundamentals of Programming
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>// Para pausa()
#ifdef _WIN32 // Se define automáticamente en Visual Studio
#include <windows.h>
#undef max// Para poder usar max() en pausa()
#endif
using namespace std;

const int numOfPlayers = 4;
const int numOfPieces = 4;
const int numOfTiles = 68;
const bool Debug = false;
const string file = "prueba5.txt";
class player;
typedef enum{Yellow, Blue, Red, Green, Grey, None} colour;
typedef player playerArray[numOfPlayers];
typedef colour board[numOfTiles];

class player{
    private:
        int piecePos[numOfPieces];
        colour playerColour;    // USED IN PLAYERHOME()

    public:
        player(){
            for(int i = 0; i < numOfPieces; i++) piecePos[i] = -1;
        }

        void setPlayerColour(int n){    // USED IN PLAYERHOME() Set player colour
            switch(n){
                case 0:
                    playerColour = Yellow;
                    break;
                case 1:
                    playerColour = Blue;
                    break;
                case 2:
                    playerColour = Red;
                    break;
                case 3:
                    playerColour = Green;
                    break;
                default:
                    cerr << "Failed to assign player colour\n";
                    cerr << "Attempted to assign int " << n << "\n";
            }
        }
        void leaveHome() const{         // Move piece out of HOME

        }
        void moveBoardPiece(){          // Move piece on BOARD

        }
        int getPiecesAtX(int position) const{ // Returns number of pieces in position X
            int piecesAtX = 0;
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] == position) piecesAtX++;
            }
            return piecesAtX;
        }
        // int getPlayerHome() const{
        //     switch(playerColour){
        //         case 0:
        //             return 5;
        //         case 1:
        //             return 22;
        //         case 2:
        //             return 39;
        //         case 3:
        //             return 56;
        //         default:
        //             cerr << "Failed to return player Home\n";
        //     }
        //     return -1;
        // }
        int firstAtX(int position) const{     // Returns index of FIRST piece to arrive at position
            for(int i = 0; i < numOfPieces; i++) if(piecePos[i] == position) return i;
            return -1;
        }
        int secondAtX(int position) const{    // Returns index of SECOND piece to arrive at position
            for(int i = firstAtX(position) + 1; i < numOfPieces; i++) if(piecePos[i] == position) return i;
            return -1;
        }
        int getPiecePos(int index) const{
            return piecePos[index];
        }
        bool hasWon() const{                  // Returns true if player has won (All pieces in position 108)
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] != 108) return false;
            }
            return true;
        }
};

void initColours();                                                                         // Initialize colours for WINDOWS
void init(playerArray &players, board topBoard, board   bottomBoard, colour &playerTurn);   // Initialize boards to GREY colour
void printBoard(const board &topBoard, const board &bottomBoard, const playerArray players);                           // Prints BOARD
void setPrintColour(const colour colour);                                                         // Set cout text colour to colour
bool isSafe(const int position);                                                                  // Returns true if position is safe
bool isBridge(const board topBoard, const board bottomBoard, const int position);                             // Returns true if a player has 2 pieces at position
void pause();                                                                               // Waits for user to press Enter
string colourToString(const colour playerColour);
int playerHome(const colour player);
int piecesAtX(const int positionX, const playerArray players);

int main(){
    // Var declaration
    playerArray players;
    board topBoard, bottomBoard;
    colour playerTurn;
    int diceRoll;
    initColours();      // Windows only

    // Board initialization
    init(players, topBoard, bottomBoard, playerTurn);

    // Game
    // while(true){
        printBoard(topBoard, bottomBoard, players);
        setPrintColour(playerTurn);
        cout << "It is " << colourToString(playerTurn) << "'s turn\n";
        pause();
        diceRoll = rand() % 6 + 1;
        cout << colourToString(playerTurn) << " rolled a " << diceRoll << "\n";
    // }

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
            cout << players[topBoard[i]].secondAtX(i) + 1;
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
                // if(colour(int(player) + 1) < 2) player = colour(int(player) + 1);
                // cout << "Next player = " << colour(int(player) + 1);
                // cout << "\tCurr player = " << int(player);
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

void pause() {
   cout << "Press Enter to continue...";
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// void cargar(playerArray jugadores, tColor& jugadorTurno,
//    tCasillas calle1, tCasillas calle2) {
//    ifstream archivo;
//    int jugador, casilla;
//    archivo.open(Archivo);
//    if (archivo.is_open()) {
//       for (int i = 0; i < NUM_JUGADORES; i++)
//          for (int f = 0; f < NUM_FICHAS; f++) {
//             archivo >> casilla;
//             jugadores[i][f] = casilla;
//             if ((casilla >= 0) && (casilla < NUM_CASILLAS))
//                if (calle1[casilla] == Ninguno)
//                   calle1[casilla] = tColor(i);
//                else
//                   calle2[casilla] = tColor(i);
//          }
//       archivo >> jugador;
//       jugadorTurno = tColor(jugador);
//       archivo.close();
//    }
// }