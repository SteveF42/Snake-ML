#include"engine.hpp"

Engine::Engine(int rows)
{
    this->rows = rows;
    this->targetResolution = 1200 / rows;
    this->width = targetResolution * rows;
    this->snake = new Snake(rows,rows);
    isRunning = true;
    window.create(sf::VideoMode(width, width), "Snake Game");
}

void Engine::Update()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            isRunning = false;
        }
    }
    DrawGrid();
    DrawSnake();
}

void Engine::DrawGrid(){
    int gap = width / rows;
    sf::Color yellow(125,132,11,120);

    for (int i = 0; i < rows; i++)
    {
        sf::Vertex lineY[] = {
            sf::Vertex(sf::Vector2f(i * gap, 0),yellow),
            sf::Vertex(sf::Vector2f(i * gap, width),yellow)
        };
        for (int j = 0; j < rows; j++)
        {
            sf::Vertex lineX[] = {
                sf::Vertex(sf::Vector2f(0, j * gap),yellow),
                sf::Vertex(sf::Vector2f(width, j * gap),yellow)
            };
            window.draw(lineX, 2, sf::Lines);
        }
        window.draw(lineY, 2, sf::Lines);
    }

}

void Engine::DrawSnake()
{
    int gap = width / rows;
    sf::Color green(0,255,0);
    for (int i = 0; i < snake->GetLength(); i++)
    {
        sf::RectangleShape bodyPart(sf::Vector2f(gap, gap));
        bodyPart.setPosition(snake->GetTailX(i) * gap, snake->GetTailY(i) * gap);
        bodyPart.setFillColor(green);
        window.draw(bodyPart);
    }
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