
#include "../JSON/json.hpp"
#include "../include/match.h"
#include "../include/python_bot.h"
#include "../include/python_script_converter.h"

#include "RocketSim.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

using json = nlohmann::json;

namespace fs = std::filesystem;

bool FileExists(const std::string& filePath) {
    return fs::exists(filePath);
}

void startPythonInterface(std::string cmd) {
    system(cmd.c_str());
}

int main(int argc, char* argv[]) {
    int ticks = 36000;
    bool render = false;
    bool copyFiles = false;
    std::string blueName = "example";
    std::string orangeName = "example";
    fs::path filePath = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--render") {
            render = true;
        }
        if (arg == "--copy") {
            copyFiles = true;
        }
        if (arg == "--file" && i + 1 < argc) {
            arg = argv[i + 1];
            try {
                filePath = arg;
            }
            catch (...) {}
        }
        if (arg == "--ticks" && i + 1 < argc) {
            arg = argv[i + 1];
            try {
                ticks = std::stoi(arg);
            } catch (...) { }
        }
    }
    Init("./meshes");

    if (filePath == "" || !FileExists(filePath.string())) {
        std::cerr << "[SQRL] File path invalid" << std::endl;
        exit(1);
    }
    if (copyFiles) {
        filePath = CopyDirectory(GetParentDirectory(filePath.string()), fs::current_path() / GetParentDirectory(filePath.string()).filename().string()) / GetFileName(filePath);
    }

    PythonBot* blueBot = new PythonBot(blueName);
    PythonBot* orangeBot = new PythonBot(orangeName);
    std::cout << "Checking python file at: " << filePath.string() << std::endl;
    ModifyPythonBot(filePath.string());

    std::string blueCmd = "py " + filePath.string() + " 0 0 " + blueName;
    std::string orangeCmd = "py " + filePath.string() + " 1 1 " + orangeName;

    std::thread(startPythonInterface, blueCmd).detach();
    std::thread(startPythonInterface, orangeCmd).detach();

    Match match = Match(blueBot, orangeBot, ticks, render);

    auto startTime = std::chrono::steady_clock::now();
    match.Run();
    blueBot->pythonInterface->Write("EXIT");
    orangeBot->pythonInterface->Write("EXIT");
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "[SQRL] Time Elapsed: " << deltaTime << "ms" << std::endl;
    std::cout << "[SQRL] Score: " << match.blueScore << " | " << match.orangeScore << std::endl;
 
    return 0;
}   