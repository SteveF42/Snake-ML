#ifndef SNAKE_HPP
#define SNAKE_HPP

enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct snakePosition
{
    snakePosition(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    snakePosition()
    {
        x = -1;
        y = -1;
    }
    int x;
    int y;
};

class Snake
{
private:
    eDirection dir;
    int length;
    snakePosition *tail;
    bool gameOver;
    int fruitX, fruitY;
    int score;
    int width, height;

public:
    Snake(int w, int h);
    int GetLength();
    snakePosition GetTail(int i);
    void Setup();
    void Input();
    void Move();
    int GetFruitX() { return fruitX; };
    int GetFruitY() { return fruitY; };
    void SetFruit();
    bool GetGameOver();
    int GetScore();
};

#endif