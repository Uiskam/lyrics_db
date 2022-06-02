#include "database.h"
#include <string>
#include <iostream>
#include <fstream> 
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void Database::init() {
    if(fs::exists(this->db_name)) {
        if(!fs::is_directory(this->db_name)) {
            fs:remove(this->db_name);
            fs::create_directory(this->db_name);
        }
    } else {
        fs::create_directory(this->db_name);
    }
}

void Database::add_file(Db_file file_to_add) {
    string new_file_name = this->db_name.u8string() + "/" + file_to_add.get_file_name();
    ofstream new_file (new_file_name, ofstream::out);
    new_file << file_to_add.get_file_content();
    new_file.close();
}

void Database::remove_file(Db_file file_to_remove) {
    fs::remove(this->get_file_name(file_to_remove));
}

bool Database::find_file(Db_file file_to_find) {
    return fs::exists(this->get_file_name(file_to_find)) && fs::path(this->get_file_name(file_to_find)).extension() == ".txt";
}