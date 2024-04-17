#include<iostream>
#include"Engine.hpp"
#include<thread>
#include"DisplayNetwork.hpp"

int main()
{
    Engine engine;
    engine.initialize(2500);
    std::thread t1([&engine](){
        DisplayNetwork display(engine);
        display.run();
    });
    engine.train(250);
    engine.test();
    return 0;
}