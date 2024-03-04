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
    tail = new snakePosition[width * height];
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    length = 1;
    tail[0] = snakePosition(width / 2, height / 2);
}

snakePosition Snake::GetTail(int i)
{
    return tail[i];
}

int Snake::GetLength()
{
    return length;
}