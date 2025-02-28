#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int speed;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;
bool gameStarted = false;

int zigzagSteps = 5; // Number of steps in one direction before changing
int stepsTaken = 0;  // Counter for steps taken in the current direction
bool movingRight = true;

void SetConsoleSize(int width, int height) {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Set the screen buffer size
    COORD coord = { (short)width, (short)height };
    SetConsoleScreenBufferSize(hConsole, coord);
    
    // Set the window size
    SMALL_RECT rect = { 0, 0, (short)(width - 1), (short)(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
}
void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 1; // Small initial body
    speed = 150; // Start slow
}

void Draw()
{
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == y && j == x)
                cout << "o";
            else if (i == fruitY && j == fruitX)
                cout << "*";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "-";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << endl;
    if (!gameStarted)
        cout << "Press ENTER to start the game!" << endl;
}
void Input()
{
    if (_kbhit()) {
        char currentDirection = dir; // Store the current direction
        switch (_getch()) {
        case 'a':
            if (currentDirection != RIGHT) // Prevent reversing
                dir = LEFT;
            gameStarted = true;
            break;
        case 'd':
            if (currentDirection != LEFT) // Prevent reversing
                dir = RIGHT;
            gameStarted = true;
            break;
        case 'w':
            if (currentDirection != DOWN) // Prevent reversing
                dir = UP;
            gameStarted = true;
            break;
        case 's':
            if (currentDirection != UP) // Prevent reversing
                dir = DOWN;
            gameStarted = true;
            break;
        case 'x':
            gameOver = true;
            break;
        case 13: // ENTER key
            gameStarted = true;
            break;
        }
    }
}
void Logic()
{
    if (!gameStarted)
        return;

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }
    
    /* if (stepsTaken < zigzagSteps) {
        if (movingRight) {
            x++; // Move right
        } else {
            x--; // Move left
        }
        stepsTaken++;
    } else {
        // Change direction
        movingRight = !movingRight; // Toggle direction
        stepsTaken = 0; // Reset step counter
        y++; // Move down
    }
    /* if (movingRight) {
        if (x < width - 1) {
            x++; // Move right
        } else {
            movingRight = false; // Change direction to left
            y++; // Move down
        }
    } else {
        if (x > 0) {
            x--; // Move left
        } else {
            movingRight = true; // Change direction to right
            y++; // Move down
        }
    }
*/

    // End game if snake hits the border
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        speed = max(50, speed - 5); // Increase speed gradually, with a minimum limit
    }
}

int main()
{
	SetConsoleSize(width + 2, height + 2);
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(speed);
    }
    cout << "Game Over! Press any key to exit..." << endl;
    _getch();
    return 0;
}

