
#include "../JSON/json.hpp"
#include "../include/match.h"
#include "../include/bot.h"
#include "RocketSim.h"

#include <iostream>
#include <chrono>
#include <thread>

using json = nlohmann::json;


int main(int argc, char* argv[]) {
    int ticks = 36000;
    bool render = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--render") {
            render = true;
        }
        if (arg == "--ticks" && i + 1 < argc) {
            arg = argv[i + 1];
            try {
                ticks = std::stoi(arg);
            } catch (...) { }
        }
    }
    Init("./meshes");


    Bot* blueBot = new Bot();
    Bot* orangeBot = new Bot();

    Match match = Match(blueBot, orangeBot, render);

    auto startTime = std::chrono::steady_clock::now();
    match.Run(ticks);
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "DT: " << deltaTime << std::endl;

    return 0;
}   