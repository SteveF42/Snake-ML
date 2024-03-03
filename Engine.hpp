#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine
{
private:
    sf::RenderWindow window;
    int targetResolution;
    int width, height;
    int offset;
    bool isRunning;
    int rows;
    int gap;
    Snake* snake;

public:
    Engine(int rows = 30);
    void Init();
    void Update();
    void DrawGrid();
    void Render();
    bool IsRunning();
    void DrawSnake();
    sf::RenderWindow *GetWindow();
};

#endif