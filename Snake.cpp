#include "Snake.hpp"
#include "random"
#include <SFML/Graphics.hpp>

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

void Snake::Input()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        dir = eDirection::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        dir = eDirection::RIGHT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        dir = eDirection::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        dir = eDirection::DOWN;
    }
}
void Snake::Move()
{
    for (int i = length; i > 0; i--)
    {
        tail[i] = tail[i - 1];
    }
    switch (dir)
    {
    case eDirection::LEFT:
        tail[0].x--;
        break;
    case eDirection::RIGHT:
        tail[0].x++;
        break;
    case eDirection::UP:
        tail[0].y--;
        break;
    case eDirection::DOWN:
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
    if (tail[0].x >= width)
    {
        tail[0].x = 0;
    }
    else if (tail[0].x < 0)
    {
        tail[0].x = width - 1;
    }
    if (tail[0].y >= height)
    {
        tail[0].y = 0;
    }
    else if (tail[0].y < 0)
    {
        tail[0].y = height - 1;
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