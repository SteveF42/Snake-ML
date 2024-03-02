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

class Snake
{
private:
    eDirection dir;
    int length;
    int *tailX, *tailY;
    bool gameOver;
    int fruitX, fruitY;
    int score;
    int width, height;

public:
    Snake(int w, int h);
    int GetLength();
    int GetTailX(int i);
    int GetTailY(int i);
    void Setup();
    void Input();
    void Logic();
    bool GetGameOver();
    int GetScore();
};

#endif