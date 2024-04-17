#include<iostream>
#include"Engine.hpp"
#include<thread>
#include"DisplayNetwork.hpp"

int main()
{
    Engine engine;
    std::thread t1([&engine](){
        DisplayNetwork display(engine);
        display.run();
    });
    engine.initialize(5000);
    engine.train(500);
    engine.test();
    return 0;
}