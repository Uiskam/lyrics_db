#include "database.h"
#include <string>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

bool Database::init() {
    
    fs::create_directories("../XD");
    return 0;
}