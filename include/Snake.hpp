#ifndef SNAKE_HPP
#define SNAKE_HPP
#include <memory>

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

using std::unique_ptr;
typedef unique_ptr<snakePosition[]> SnakePtr;

class Snake
{
private:
    eDirection dir;
    int length;
    SnakePtr tail;
    bool gameOver;
    int fruitX, fruitY;
    int score;
    int width, height;

public:
    Snake(int w, int h);
    int GetLength();
    snakePosition GetTail(int i);
    void Setup();
    void Input(eDirection dir);
    void Move();
    int GetFruitX() { return fruitX; };
    int GetFruitY() { return fruitY; };
    double foodUp();
    double foodDown();
    double foodLeft();
    double foodRight();
    double wallUp();
    double wallDown();
    double wallLeft();
    double wallRight();
    double bodyUp();
    double bodyDown();
    double bodyLeft();
    double bodyRight();
    double bodyUpDistance();
    double bodyDownDistance();
    double bodyLeftDistance();
    double bodyRightDistance();
    double distanceFromFruit();
    eDirection GetDirection() { return dir; }
    void SetFruit();
    bool GetGameOver();
    int GetScore();
};

#endif