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

void Database::clear_db() {
    for (auto& path: fs::directory_iterator(this->db_name)) {
        fs::remove_all(path);
    }
}

void Database::print_db() {
    int index = 1;
    for (const auto & entry : fs::directory_iterator(this->db_name)) {
        cout << "song no " << index++ << ": " << entry.path().filename() << endl;
    }
}

string Database::get_nth_song_name(int n) {
    int index = 1;
    for (const auto & entry : fs::directory_iterator(this->db_name)) {
        if(index++ == n) return entry.path().filename().u8string();
    }
    return "Song with nb " + to_string(n) + " do not exisit in databse!";
}

string Database::pick_song_from_db() {
    this->print_db();
    string str_song_index;
    cout << "Song no: ";
    getline(cin, str_song_index);
    string requested_song  = this->get_nth_song_name(stoi(str_song_index));
    if(requested_song.find("do not exisit in databse") != string::npos) {
        cout << requested_song << endl;
        return "-1";
    } 
    return requested_song;
}