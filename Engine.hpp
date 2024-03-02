#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine
{
private:
    sf::RenderWindow window;
    int targetResolution;
    int width;
    bool isRunning;
    int rows;
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