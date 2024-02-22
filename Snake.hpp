#ifndef SNAKE_HPP
#define SNAKE_HPP

class Snake
{
private:
    int x, y;
    int length;
    int direction;
    int tailX[100], tailY[100];
    bool gameOver;
    int fruitX, fruitY;
    int score;
    int width, height;
public:
    Snake(int w, int h);
    void Setup();
    void Input();
    void Logic();
    bool GetGameOver();
    int GetScore();
};

#endif