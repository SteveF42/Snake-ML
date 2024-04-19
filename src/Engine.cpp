#include "engine.hpp"

const int SQUARE_OFFSET = 2;
Engine::Engine(int rows) : Neat(12, 4)
{
    this->rows = rows;
    this->width = 1200;
    this->height = width;
    this->gap = width / rows;
    this->snake = new Snake(rows, rows);
    this->offset = (SQUARE_OFFSET * 2) * gap;
    isRunning = true;
}

void Engine::InitScreen()
{

    window.create(sf::VideoMode(width + offset, height + offset), "Snake Game");
    window.setFramerateLimit(40);
}

void Engine::Init()
{
    snake->Setup();
    snake->Input(RIGHT);
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

double Engine::playGame(Genome *genome)
{
    Init();
    double score = 0;
    double bonus = 0;
    int moveLimit = 0;
    int totalSteps = 0;
    const int MAX_STEPS = 100;
    double penalty = 0;
    double prevDistance = snake->distanceFromFruit();
    while (true)
    {
        // Update();
        // Render();
        std::vector<double> inputs = {
            snake->foodUp(),
            snake->foodDown(),
            snake->foodLeft(),
            snake->foodRight(),
            snake->wallUp(),
            snake->wallDown(),
            snake->wallLeft(),
            snake->wallRight(),
            snake->bodyUpDistance(),
            snake->bodyDownDistance(),
            snake->bodyLeftDistance(),
            snake->bodyRightDistance(),
        };
        std::vector<double> output = genome->activate(inputs);
        int idx = 0;
        double max = output[0];
        for (int i = 0; i < output.size(); i++)
        {
            if (output[i] > max)
            {
                max = output[i];
                idx = i;
            }
        }

        eDirection dir, snakeDir = snake->GetDirection();

        if (idx == 0)
            dir = UP;
        else if (idx == 1)
            dir = DOWN;
        else if (idx == 2)
            dir = LEFT;
        else if (idx == 3)
            dir = RIGHT;

        if ((dir == UP && snakeDir == DOWN) || (dir == DOWN && snakeDir == UP) ||
            (dir == LEFT && snakeDir == RIGHT) || (dir == RIGHT && snakeDir == LEFT))
        {
            penalty += 50;
            dir = snakeDir;
        }

        snake->Input(dir);
        snake->Move();
        if (snake->distanceFromFruit() < prevDistance)
        {
            bonus += 0.1;
        }
        else if (snake->distanceFromFruit() > prevDistance)
        {
            penalty += 0.1;
        }
        prevDistance = snake->distanceFromFruit();
        if (snake->GetScore() > score)
        {
            score = snake->GetScore();
            moveLimit = 0;
        }
        if (snake->GetGameOver() || moveLimit++ > MAX_STEPS)
        {
            break;
        }
        totalSteps++;
    }
    return (score * 100) + (totalSteps / 10) - penalty + bonus;
}

void Engine::testNetwork(Genome *genome)
{
    double fitness = 0;
    int moveLimit = 0;
    int totalSteps = 0;
    Init();
    InitScreen();
    while (isRunning)
    {
        Update();
        Render();
        std::vector<double> inputs = {
            snake->foodUp(),
            snake->foodDown(),
            snake->foodLeft(),
            snake->foodRight(),
            snake->wallUp(),
            snake->wallDown(),
            snake->wallLeft(),
            snake->wallRight(),
            snake->bodyUpDistance(),
            snake->bodyDownDistance(),
            snake->bodyLeftDistance(),
            snake->bodyRightDistance(),
        };
        std::vector<double> output = genome->activate(inputs);
        int idx = 0;
        double max = output[0];
        for (int i = 0; i < output.size(); i++)
        {
            if (output[i] > max)
            {
                max = output[i];
                idx = i;
            }
        }

        eDirection dir = snake->GetDirection();

        if (idx == 0)
            dir = UP;
        else if (idx == 1)
            dir = DOWN;
        else if (idx == 2)
            dir = LEFT;
        else if (idx == 3)
            dir = RIGHT;

        snake->Input(dir);
        snake->Move();
        if (snake->GetScore() > fitness)
        {
            fitness = snake->GetScore();
            moveLimit = 0;
        }
        if (snake->GetGameOver())
        {
            fitness = snake->GetScore();
            Init();
        }
        totalSteps++;

        for (auto &d : output)
        {
            std::cout << d << " ";
        }
        std::cout << '\n';
    }
}