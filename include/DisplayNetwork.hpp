#ifndef DISPLAYNETWORK_HPP
#define DISPLAYNETWORK_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <atomic>
#include <thread>
#include "Neat.hpp"

using std::thread;
using std::unique_ptr;
typedef unique_ptr<sf::RenderWindow> WindowPtr;

struct point
{
    double x;
    double y;
};

class DisplayNetwork
{
private:
    WindowPtr window;
    Neat *neatEngine;
    thread eventThread;
    int TARGET_WIDTH = 800;
    int TARGET_HEIGHT = 800;
    void draw();
    void update();
    void drawLayer(networkLayer &layer, int layerIdx, int offsetX);
    void drawLinks();
    map<int, point> nodePositions;

public:
    DisplayNetwork(Neat &neatEngine);
    DisplayNetwork(Neat &neatEngine, int width, int height);
    void run();
};

#endif