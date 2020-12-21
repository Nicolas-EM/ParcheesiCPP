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
        colour playerColour;

    public:
        player(){
            for(auto position:piecePos) position = -1;
        }

        void setColour(int n){
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
        
        void leaveHome(){                       // Move piece out of HOME
            for(auto position:piecePos){
                if(position == -1){
                    
                }
            }
        }
        void moveBoardPiece(){                       // Move piece on BOARD

        }
        int getPiecesAtX(int x){
            int piecesAtX = 0;
            for(int i = 0; i < numOfPieces; i++){
                if(piecePos[i] == x) piecesAtX++;
            }
            return piecesAtX;
        }
        int firstAtX(int position){
            for(int i = 0; i < numOfPieces; i++) if(piecePos[i] == position) return i;
            return -1;
        }
        int secondAtX(int position){
            for(int i = firstAtX(position) + 1; i < numOfPieces; i++) if(piecePos[i] == position) return i;
            return -1;
        }
};

void init(playerArray &players, board topBoard, board   bottomBoard); // Initialize boards to GREY colour
void printBoard(const board &topBoard, const board &bottomBoard);
void setColour(colour colour);                  // Set cout colour to colour
bool isSafe(int position);
int getPiecesAtX(int position);

int main(){
    // Var declaration
    playerArray players;
    board topBoard, bottomBoard;

    // Board initialization
    init(players, topBoard, bottomBoard);

    // Game
    // while(true){
        printBoard(topBoard, bottomBoard);
        for(auto player:players){

        }
    // }

    return 0;
}

void init(playerArray &players, board topBoard, board bottomBoard){
    srand(time(NULL));
    int player1_Colour = rand() % numOfPlayers;
    for(auto player:players){
        for(int i = 0; i < 4; i++) player.setColour((player1_Colour + i) % 4);
    }
    for(int i = 0; i < numOfTiles; i++) topBoard[i] = bottomBoard[i] = None;
}

void printBoard(const board &topBoard, const board &bottomBoard){
    // Printing numbers
    for (int i = 0; i < numOfTiles; i++) cout << i / 10; 
    cout << "\n";
    for (int i = 0; i < numOfTiles; i++) cout << i % 10; 
    cout << "\n";
    for(int x = 0; x < numOfTiles; x++) cout << '>';
    cout << "\n";

    // Printing topBoard
    for(int i = 0; i < numOfTiles; i++){
        setColour(Grey);
        if(topBoard[i] != None){
            setColour(topBoard[i]);
            // segundaEn(jugadores[calle2[i]], i) + 1;
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
        setColour(Grey);
        if(bottomBoard[i] != None){
            setColour(topBoard[i]);
            // primeraEn(jugadores[calle2[i]], i) + 1;
        }
        else cout << ' ';
    }
    cout << "\n";

    // Printing division line
    for(int i = 0; i < numOfTiles; i++){
        setColour(Grey);
        if(i == 0){
            setColour(Yellow);
            cout << 'V';
        }
        else if(i == 5){
            setColour(Yellow);
            cout << '^';
        }
        else if(i == 17){
            setColour(Blue);
            cout << 'V';
        }
        else if(i == 22){
            setColour(Blue);
            cout << '^';
        }
        else if(i == 34){
            setColour(Red);
            cout << 'V';
        }
        else if(i == 39){
            setColour(Red);
            cout << '^';
        }
        else if(i == 51){
            setColour(Green);
            cout << 'V';
        }
        else if(i == 56){
            setColour(Green);
            cout << '^';
        }
        else cout << '>';
    }

    // Printing end board
    for(int i = Yellow; i != Grey; i++){
        // setColour(i);
    }
}

void setColour(colour colour){
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

bool isSafe(int position){
    if(position == 0 || position == 5 || position == 12 || position == 17 || position == 22 || position == 29 || position == 34 || position == 39 || position == 46 || position == 51 || position == 56 || position == 63) return true;
    else return false;
}

// void mostrar(const tJugadores jugadores, const tCasillas calle1, const tCasillas calle2) {
//    int casilla, ficha;
//    tColor jug;

//    cout << "\x1b[2J\x1b[H"; // Se situa en la esquina superior izquierda
//    setColor(Gris);
//    cout << endl;

//    // Filas con la numeraci�n de las casillas...
//    for (int i = 0; i < NUM_CASILLAS; i++)
//       cout << i / 10;
//    cout << endl;
//    for (int i = 0; i < NUM_CASILLAS; i++)
//       cout << i % 10;
//    cout << endl;

//    // Borde superior...
//    for (int i = 0; i < NUM_CASILLAS; i++)
//       cout << '>';
//    cout << endl;

//    // Primera fila de posiciones de fichas...
//    for (int i = 0; i < NUM_CASILLAS; i++) {
//       setColor(calle2[i]);
//       if (calle2[i] != Ninguno)
//          cout << segundaEn(jugadores[calle2[i]], i) + 1;
//       else
//          cout << ' ';
//       setColor(Gris);
//    }
//    cout << endl;

//    // "Mediana"   
//    for (int i = 0; i < NUM_CASILLAS; i++)
//       if (esSeguro(i))
//          cout << 'o';
//       else
//          cout << '-';
//    cout << endl;


//    // Segunda fila de posiciones de fichas...
//    for (int i = 0; i < NUM_CASILLAS; i++) {
//       setColor(calle1[i]);
//       if (calle1[i] != Ninguno)
//          cout << primeraEn(jugadores[calle1[i]], i) + 1;
//       else
//          cout << ' ';
//       setColor(Gris);
//    }
//    cout << endl;

//    jug = Amarillo;
//    // Borde inferior...
//    for (int i = 0; i < NUM_CASILLAS; i++)
//       if (i == zanataJugador(jug)) {
//          setColor(jug);
//          cout << "V";
//          setColor(Gris);
//       }
//       else if (i == salidaJugador(jug)) {
//          setColor(jug);
//          cout << "^";
//          setColor(Gris);
//          jug = tColor(int(jug) + 1);
//       }
//       else
//          cout << '>';
//    cout << endl;

//    // Metas y casas...
//    for (int i = 0; i < NUM_FICHAS; i++) {
//       casilla = 0;
//       jug = Amarillo;
//       setColor(jug);
//       while (casilla < NUM_CASILLAS) {
//          if (casilla == zanataJugador(jug)) {
//             ficha = primeraEn(jugadores[jug], 101 + i);
//             if (ficha != -1) {
//                cout << ficha + 1;
//                if (cuantasEn(jugadores[jug], 101 + i) > 1) {
//                   ficha = segundaEn(jugadores[jug], 101 + i);
//                   if (ficha != -1) {
//                      cout << ficha + 1;
//                   }
//                   else
//                      cout << "V";
//                }
//                else
//                   cout << "V";
//             }
//             else
//                cout << "VV";
//             casilla++;
//          }
//          else if (casilla == salidaJugador(jug)) {
//             if (jugadores[jug][i] == -1) // En casa
//                cout << i + 1;
//             else
//                cout << "^";
//             jug = tColor(int(jug) + 1);
//             setColor(jug);
//          }
//          else
//             cout << ' ';
//          casilla++;
//       }
//       cout << endl;
//    }

//    // Resto de metas...
//    for (int i = 105; i <= 107; i++) {
//       casilla = 0;
//       jug = Amarillo;
//       setColor(jug);
//       while (casilla < NUM_CASILLAS) {
//          if (casilla == zanataJugador(jug)) {
//             ficha = primeraEn(jugadores[jug], i);
//             if (ficha != -1) {
//                cout << ficha + 1;
//                if (cuantasEn(jugadores[jug], i) > 1) {
//                   ficha = segundaEn(jugadores[jug], i);
//                   if (ficha != -1) {
//                      cout << ficha + 1;
//                   }
//                   else
//                      cout << "V";
//                }
//                else
//                   cout << "V";
//             }
//             else
//                cout << "VV";
//             casilla++;
//             jug = tColor(int(jug) + 1);
//             setColor(jug);
//          }
//          else
//             cout << ' ';
//          casilla++;
//       }
//       cout << endl;
//    }

//    casilla = 0;
//    jug = Amarillo;
//    setColor(jug);
//    while (casilla < NUM_CASILLAS) {
//       cout << ((jugadores[jug][0] == 108) ? '1' : '.');
//       cout << ((jugadores[jug][1] == 108) ? '2' : '.');
//       jug = tColor(int(jug) + 1);
//       setColor(jug);
//       cout << "               ";
//       casilla += 17;
//    }
//    cout << endl;
//    casilla = 0;
//    jug = Amarillo;
//    setColor(jug);
//    while (casilla < NUM_CASILLAS) {
//       cout << ((jugadores[jug][2] == 108) ? '3' : '.');
//       cout << ((jugadores[jug][3] == 108) ? '4' : '.');
//       jug = tColor(int(jug) + 1);
//       setColor(jug);
//       cout << "               ";
//       casilla += 17;
//    }
//    cout << endl << endl;
//    setColor(Gris);
// }

// void pausa() {
//    cout << "Pulsa Intro para continuar...";
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
// }

// void iniciaColores() {
// #ifdef _WIN32
//    for (DWORD stream : {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE}) {
//       DWORD mode;
//       HANDLE handle = GetStdHandle(stream);

//       if (GetConsoleMode(handle, &mode)) {
//          mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
//          SetConsoleMode(handle, mode);
//       }
//    }
// #endif
// }
