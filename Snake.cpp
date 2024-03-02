#include "Snake.hpp"
#include "random"

Snake::Snake(int w, int h)
{
    width = w;
    height = h;
    Setup();
}

void Snake::Setup()
{
    gameOver = false;
    dir = eDirection::STOP;
    tailX = new int[width];
    tailY = new int[height];
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    length = 1;
    tailX[0] = width / 2;
    tailY[0] = height / 2;
}

int Snake::GetTailX(int i)
{
    return tailX[i];
}

int Snake::GetTailY(int i)
{
    return tailY[i];
}

int Snake::GetLength()
{
    return length;
}