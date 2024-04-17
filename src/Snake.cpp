#include <SFML/Graphics.hpp>
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
    tail = SnakePtr(new snakePosition[width * height]);
    SetFruit();
    score = 0;
    length = 1;
    tail[0] = snakePosition(width / 2, height / 2);
}

snakePosition Snake::GetTail(int i)
{
    return tail[i];
}

void Snake::SetFruit()
{
    fruitX = rand() % width;
    fruitY = rand() % height;
}

void Snake::Input(eDirection dir)
{
    this->dir = dir;
}
void Snake::Move()
{
    for (int i = length; i > 0; i--)
    {
        tail[i] = tail[i - 1];
    }
    switch (dir)
    {
    case LEFT:
        tail[0].x--;
        break;
    case RIGHT:
        tail[0].x++;
        break;
    case UP:
        tail[0].y--;
        break;
    case DOWN:
        tail[0].y++;
        break;
    default:
        break;
    }
    if (tail[0].x == fruitX && tail[0].y == fruitY)
    {
        length++;
        score++;
        SetFruit();
    }
    if (tail[0].x >= width || tail[0].x < 0 || tail[0].y >= height || tail[0].y < 0)
    {
        gameOver = true;
    }

    for (int i = 1; i < length; i++)
    {
        if (tail[0].x == tail[i].x && tail[0].y == tail[i].y)
        {
            gameOver = true;
        }
    }
}

bool Snake::GetGameOver()
{
    return gameOver;
}

int Snake::GetLength()
{
    return length;
}

int Snake::GetScore()
{
    return score;
}

double Snake::distanceFromFruit()
{
    return sqrt(pow(tail[0].x - fruitX, 2) + pow(tail[0].y - fruitY, 2));
}

double Snake::foodUp()
{
    if (tail[0].y > fruitY and tail[0].x == fruitX)
    {
        return 1;
    }
    return 0;
}

double Snake::foodDown()
{
    if (tail[0].y < fruitY and tail[0].x == fruitX)
    {
        return 1;
    }
    return 0;
}

double Snake::foodLeft()
{
    if (tail[0].x > fruitX and tail[0].y == fruitY)
    {
        return 1;
    }
    return 0;
}

double Snake::foodRight()
{
    if (tail[0].x < fruitX and tail[0].y == fruitY)
    {
        return 1;
    }
    return 0;
}

double Snake::wallUp()
{
    return (tail[0].y) / static_cast<double>(height);
}

double Snake::wallDown()
{
    return (height - tail[0].y) / static_cast<double>(height);
}

double Snake::wallLeft()
{
    return (tail[0].x) / static_cast<double>(width);
}

double Snake::wallRight()
{
    return (width - tail[0].x) / static_cast<double>(width);
}

double Snake::bodyUp()
{
    for (int i = 1; i < length; i++)
    {
        if (tail[0].y > tail[i].y and tail[0].x == tail[i].x)
        {
            return 1;
        }
    }
    return 0;
}

double Snake::bodyDown()
{
    for (int i = 1; i < length; i++)
    {
        if (tail[0].y < tail[i].y and tail[0].x == tail[i].x)
        {
            return 1;
        }
    }
    return 0;
}

double Snake::bodyLeft()
{
    for (int i = 1; i < length; i++)
    {
        if (tail[0].x > tail[i].x and tail[0].y == tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}

double Snake::bodyRight()
{
    for (int i = 1; i < length; i++)
    {
        if (tail[0].x < tail[i].x and tail[0].y == tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}