#include "engine.hpp"

const int SQUARE_OFFSET = 2;
Engine::Engine(int rows)
{
    this->rows = rows;
    this->width = 1200;
    this->height = width;
    this->gap = width / rows;
    this->snake = new Snake(rows, rows);
    this->offset = (SQUARE_OFFSET * 2) * gap;
    isRunning = true;
    window.create(sf::VideoMode(width + offset, height + offset), "Snake Game");
    window.setFramerateLimit(20);
}

void Engine::Update()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            isRunning = false;
        }
        if (snake->GetGameOver() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            snake->Setup();
        }
    }

    if (!snake->GetGameOver())
    {
        snake->Input();
        snake->Move();
    }

    DrawFruit();
    DrawGrid();
    DrawSnake();
}

void Engine::DrawGrid()
{
    sf::Color yellow(125, 132, 11, 120);
    int xDisplacement = offset / 2;
    int yDisplacement = offset / 2;

    for (int i = 0; i <= rows; i++)
    {
        int x1 = (i * gap) + yDisplacement;
        sf::Vertex lineY[] = {
            sf::Vertex(sf::Vector2f(x1, yDisplacement), yellow),
            sf::Vertex(sf::Vector2f(x1, height + yDisplacement), yellow)};
        window.draw(lineY, 2, sf::Lines);

        for (int j = 0; j <= rows; j++)
        {
            int x2 = (j * gap) + xDisplacement;
            sf::Vertex lineX[] = {
                sf::Vertex(sf::Vector2f(xDisplacement, x2), yellow),
                sf::Vertex(sf::Vector2f(width + xDisplacement, x2), yellow)};
            window.draw(lineX, 2, sf::Lines);
        }
    }
}

void Engine::DrawSnake()
{
    int gap = width / rows;
    sf::Color green(0, 255, 0);
    for (int i = 0; i < snake->GetLength(); i++)
    {
        sf::RectangleShape bodyPart(sf::Vector2f(gap, gap));
        snakePosition tail = snake->GetTail(i);
        bodyPart.setPosition((tail.x + SQUARE_OFFSET) * gap, (tail.y + SQUARE_OFFSET) * gap);
        bodyPart.setFillColor(green);
        window.draw(bodyPart);
    }
}

void Engine::DrawFruit()
{
    int gap = width / rows;
    sf::Color red(255, 0, 0);
    sf::RectangleShape fruit(sf::Vector2f(gap, gap));

    fruit.setPosition((snake->GetFruitX() + SQUARE_OFFSET) * gap, (snake->GetFruitY() + SQUARE_OFFSET) * gap);
    fruit.setFillColor(red);
    window.draw(fruit);
}

void Engine::Render()
{
    window.display();
    window.clear();
}

bool Engine::IsRunning()
{
    return isRunning;
}