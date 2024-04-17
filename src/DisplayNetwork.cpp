#include "DisplayNetwork.hpp"
#include <memory>

using std::make_unique;

DisplayNetwork::DisplayNetwork(Neat &neatEngine)
{
    this->neatEngine = &neatEngine;
    this->window = make_unique<sf::RenderWindow>(sf::VideoMode(TARGET_WIDTH, TARGET_HEIGHT), "Neat");
}

DisplayNetwork::DisplayNetwork(Neat &neatEngine, int width, int height)
{
    this->neatEngine = &neatEngine;
    this->TARGET_HEIGHT = height;
    this->TARGET_WIDTH = width;
    this->window = make_unique<sf::RenderWindow>(sf::VideoMode(TARGET_WIDTH, TARGET_HEIGHT), "Neat");
}

void DisplayNetwork::draw()
{
    this->window->clear();
    this->update();
    this->window->display();
}

void DisplayNetwork::drawLayer(networkLayer &layer, int layerIdx, int layerCount)
{
    // draw node
    sf::CircleShape nodeShape(15.f);          // create a circle shape for the nodes
    nodeShape.setFillColor(sf::Color::Green); // set the fill color of the nodes
    int layerSize = layer.nodes.size();
    int idx = 0;
    double offsetY = TARGET_HEIGHT / (layerSize);
    double offsetX = TARGET_WIDTH / (layerCount);
    double centerY = offsetY / 2;
    double centerX = offsetX / 2;
    for (auto &node : layer.nodes)
    {
        // set the position of the node shape based on the node's coordinates
        nodeShape.setPosition((offsetX * layerIdx) + centerX, (offsetY * idx++) + centerY);
        // draw the node shape to the window
        this->window->draw(nodeShape);
        double x = nodeShape.getPosition().x + nodeShape.getRadius();
        double y = nodeShape.getPosition().y + nodeShape.getRadius();
        nodePositions.insert({node->getID(), {x, y}});
    }
}

void DisplayNetwork::drawLinks()
{
    Genome bestGenome = Genome(this->neatEngine->getBestGenome());
    map<int, LinkPtr> &links = bestGenome.getLinks();
    for (auto &[id, link] : links)
    {
        NodeGene *inNode = link->getFromNode();
        NodeGene *outNode = link->getToNode();
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(nodePositions[inNode->getID()].x, nodePositions[inNode->getID()].y)),
            sf::Vertex(sf::Vector2f(nodePositions[outNode->getID()].x, nodePositions[outNode->getID()].y))};
        if (link->isEnabled() == true)
        {
            line[0].color = sf::Color::Blue;
            line[1].color = sf::Color::Cyan;
        }
        else
        {
            line[0].color = sf::Color::Magenta;
            line[1].color = sf::Color::Red;
            line->color = sf::Color::Red;
        }

        this->window->draw(line, 2, sf::Lines);
    }
}

void DisplayNetwork::update()
{
    nodePositions.clear();
    Genome bestGenome = Genome(this->neatEngine->getBestGenome());
    map<int, networkLayer> layers = bestGenome.getLayers();
    // draw nodes and links
    int layerCount = layers.size();
    int layerIdx = 0;
    for (auto &[id, layer] : layers)
    {
        if (layer.type == OUTPUT)
            continue;
        drawLayer(layer, layerIdx++, layerCount);
    }

    networkLayer outputLayer = layers[Genome::OUTPUT_LAYER];
    drawLayer(outputLayer, layerIdx, layerCount);
    drawLinks();
}

void DisplayNetwork::run()
{

    sf::Event event;
    while (this->window->isOpen())
    {
        // event
        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->window->close();
        }
        this->draw();
    }
}