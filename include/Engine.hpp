#include "Neat.hpp"
#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine : public Neat
{
private:
    sf::RenderWindow window;
    int targetResolution;
    int width, height;
    int offset;
    bool isRunning;
    int rows;
    int gap;
    Snake *snake;
    int MAX_STEPS = 100;

protected:
    double playGame(Genome *genome) override;
    void testNetwork(Genome *genome) override;

public:
    Engine(int rows = 30);
    void Init();
    void Update();
    void DrawGrid();
    void Render();
    bool IsRunning();
    void DrawSnake();
    void DrawFruit();
};

#endif