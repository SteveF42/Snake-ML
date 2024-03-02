#include<iostream>
#include"Engine.hpp"

int main()
{
    Engine engine;
    while(engine.IsRunning())
    {
        engine.Update();
        engine.Render();
    }
    return 0;
}