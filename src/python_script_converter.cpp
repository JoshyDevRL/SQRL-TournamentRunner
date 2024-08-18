#include "../include/python_script_converter.h"

fs::path GetParentDirectory(const std::string& filePath) {
    fs::path path(filePath);
    fs::path parentPath = path.parent_path();

    if (parentPath.empty()) {
        std::cerr << "No parent directory found for path: " << filePath << std::endl;
        return "";
    }

    return parentPath;
}

fs::path GetFileName(const fs::path& filePath) {
    return filePath.filename();
}

std::string ReadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void WriteFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    file << content;
}

bool ContainsString(const std::string& filename, const std::string& searchString) {
    std::string content = ReadFile(filename);
    if (content.empty()) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return false;
    }
    return content.find(searchString) != std::string::npos;
}

bool ContainsClassWithInheritance(const std::string& content, std::string& className) {
    size_t pos = 0;
    while ((pos = content.find("class ", pos)) != std::string::npos) {
        size_t endPos = content.find(':', pos);
        if (endPos != std::string::npos) {
            std::string classDef = content.substr(pos, endPos - pos);
            std::string baseClass = classDef.substr(classDef.find('(') + 1, classDef.find(')') - classDef.find('(') - 1);
            if (!baseClass.empty()) {
                className = content.substr(pos + 6, endPos - pos - 6);
                return true;
            }
        }
        pos = endPos + 1;
    }
    return false;
}

void InsertCodeAfterClass(std::string& content, const std::string& className, const std::string& codeToInsert) {
    size_t classPos = content.find("class " + className);
    if (classPos != std::string::npos) {
        size_t classEnd = content.find('\n', classPos);
        size_t indentionLevel = content.find_first_not_of(" \t", classEnd + 1) - classEnd - 1;
        std::string indentation(indentionLevel, ' ');
        size_t insertPos = content.find('\n', classEnd) + 1;
        content.insert(insertPos, "\n" + indentation + codeToInsert);
    }
}

void ReplaceString(std::string& content, const std::string& oldStr, const std::string& newStr) {
    std::string::size_type pos = 0;
    while ((pos = content.find(oldStr, pos)) != std::string::npos) {
        content.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

void ProcessPythonFile(const fs::path& filepath, const std::string& oldStr, const std::string& newStr) {
    std::string content = ReadFile(filepath.string());
    if (content.empty()) return;

    ReplaceString(content, oldStr, newStr);
    WriteFile(filepath.string(), content);
}

void TraverseAndProcess(const fs::path& dirPath) {
    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".py") {
            ProcessPythonFile(entry.path(), "rlbot.", "sqrl_python_interface.");
            std::cout << "Processed file: " << entry.path() << std::endl;
        }
    }
}

void ModifyPythonBot(const std::string& filename) {
    std::string mainCode = R"(if __name__ == '__main__':
    import sys
    if len(sys.argv) < 4:
        print("Error: Missing parameters")
        sys.exit(1)
    index = int(sys.argv[1])
    team = int(sys.argv[2])
    name = str(sys.argv[3])
    bot = Bot(index, team, name)
    bot.initialize_agent()
    while True:
        packet = bot.get_packet()
        controller = bot.get_output(packet)
        bot.send_controller(controller)
)";

    std::string initCode = R"(def __init__(self, index, team, name):
        super().__init__()
        pipe_name = name + "_" + str(index)
        self.set_vars(index, team, name, pipe_name)
)";

    std::string content = ReadFile(filename);
    if (content.empty()) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return;
    }

    std::string mainString = "if __name__ == '__main__':";
    if (!ContainsString(filename, mainString)) {
        std::cout << "Adding the '__main__' block" << std::endl;
        content += "\n" + mainCode;
    }

    std::string initString = "def __init__(self, index, team, name):";
    if (!ContainsString(filename, initString)) {
        std::cout << "Adding the __init__ block" << std::endl;
        std::string className;
        if (ContainsClassWithInheritance(content, className)) {
            std::cout << "Valid class found: " << className << std::endl;
            InsertCodeAfterClass(content, className, initCode);
            WriteFile(filename, content);
        }
        else {
            std::cout << "No class valid class found" << std::endl;
        }
    }

    WriteFile(filename, content);

    TraverseAndProcess(GetParentDirectory(filename));

    std::cout << "Code has been modified successfully!" << std::endl;
}

fs::path CopyDirectory(const fs::path& source, const fs::path& destination) {
    try {
        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }
        for (const auto& entry : fs::recursive_directory_iterator(source)) {
            const auto& path = entry.path();
            auto relativePath = fs::relative(path, source);
            auto destPath = destination / relativePath;

            if (fs::is_directory(path)) {
                if (!fs::exists(destPath)) {
                    fs::create_directories(destPath);
                }
            }
            else if (fs::is_regular_file(path)) {
                fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
            }
        }
        std::cout << "Directory copied successfully from " << source << " to " << destination << std::endl;
        return destination;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return fs::path();
    }
}