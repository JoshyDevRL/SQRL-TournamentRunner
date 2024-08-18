#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

fs::path GetParentDirectory(const std::string& filePath);
fs::path GetFileName(const fs::path& filePath);

std::string ReadFile(const std::string& filename);
void WriteFile(const std::string& filename, const std::string& content);

bool ContainsString(const std::string& filename, const std::string& searchString);
bool ContainsClassWithInheritance(const std::string& content, std::string& className);

void InsertCodeAfterClass(std::string& content, const std::string& className, const std::string& codeToInsert);
void ReplaceString(std::string& content, const std::string& oldStr, const std::string& newStr);

void ProcessPythonFile(const fs::path& filepath, const std::string& oldStr, const std::string& newStr);
void TraverseAndProcess(const fs::path& dirPath);

void ModifyPythonBot(const std::string& filename);

fs::path CopyDirectory(const fs::path& source, const fs::path& destination);