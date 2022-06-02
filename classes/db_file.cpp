#include<string>
#include "db_file.h"
using namespace std;

Db_file::Db_file() {
    file_name = file_content = "";
}

Db_file::Db_file(string file_name, string file_content) {
    this->file_name = file_name;
    this->file_content = file_content;
}

void Db_file::set_name(string new_name) {
    this->file_name = new_name;
}

void Db_file::set_content(string new_content) {
    this->file_content = new_content;
}