
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

std::string GetBotName(fs::path path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".cfg") {
            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line)) {
                size_t pos = line.find("name = ");
                if (pos != std::string::npos) {
                    return line.substr(pos + 7);
                }
            }
        }
    }
    return "";
}

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
    fs::path blueFilePath = "";
    fs::path orangeFilePath = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--render") {
            render = true;
        }
        if (arg == "--copy") {
            copyFiles = true;
        }
        if (arg == "--blue" && i + 1 < argc) {
            arg = argv[i + 1];
            try {
                blueFilePath = arg;
            }
            catch (...) {}
        }
        if (arg == "--orange" && i + 1 < argc) {
            arg = argv[i + 1];
            try {
                orangeFilePath = arg;
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

    if (blueFilePath == "" || orangeFilePath == "" || !FileExists(blueFilePath.string()) || !FileExists(orangeFilePath.string())) {
        std::cerr << "[SQRL] File path invalid" << std::endl;
        exit(1);
    }
    if (copyFiles) {
        blueFilePath = CopyDirectory(GetParentDirectory(blueFilePath.string()), fs::current_path() / GetParentDirectory(blueFilePath.string()).filename().string()) / GetFileName(blueFilePath);
        orangeFilePath = CopyDirectory(GetParentDirectory(orangeFilePath.string()), fs::current_path() / GetParentDirectory(orangeFilePath.string()).filename().string()) / GetFileName(orangeFilePath);
    }

    std::cout << "Checking python file at: " << blueFilePath.string() << std::endl;
    ModifyPythonBot(blueFilePath.string());
    std::cout << "Checking python file at: " << orangeFilePath.string() << std::endl;
    ModifyPythonBot(orangeFilePath.string());

    std::vector<std::string> botNames;
    botNames.push_back(GetBotName(fs::current_path() / GetParentDirectory(blueFilePath.string()).filename()));
    botNames.push_back(GetBotName(fs::current_path() / GetParentDirectory(orangeFilePath.string()).filename()));

    int matchId = 0;

    std::replace(botNames[0].begin(), botNames[0].end(), ' ', '$');
    std::replace(botNames[1].begin(), botNames[1].end(), ' ', '$');

    std::string blueCmd = "py " + blueFilePath.string() + std::format(" 0 0 {} {}", botNames[0], matchId);
    std::string orangeCmd = "py " + orangeFilePath.string() + std::format(" 1 1 {} {}", botNames[1], matchId);

    std::thread(startPythonInterface, blueCmd).detach();
    std::thread(startPythonInterface, orangeCmd).detach();

    Match match = Match(0, botNames, ticks, render);
    auto startTime = std::chrono::steady_clock::now();
    std::cout << "Starting match..." << std::endl;
    match.Run();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Time Elapsed: " << deltaTime << "ms" << std::endl;
    std::cout << "Score: " << match.blueScore << " | " << match.orangeScore << std::endl;
    //std::cout << "Blue shots: " << match.pythonInterface->blueBot->tracker->matchShots << std::endl;
 
    return 0;
}   