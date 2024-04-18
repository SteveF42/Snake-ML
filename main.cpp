#include<iostream>
#include"Engine.hpp"
#include<thread>
#include"DisplayNetwork.hpp"

int main()
{
    Engine engine;
    engine.initialize(2000);
    std::thread t1([&engine](){
        DisplayNetwork display(engine);
        display.run();
    });
    engine.train(500);
    engine.test();
    return 0;
}