#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include "classes/db_file.h"
#include "classes/database.h"

using namespace std;

void example_funciton() {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                               cpr::Authentication{"user", "pass"},
                               cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    std::cout<<"HERE\n";
    std::cout << "Status code: " << r.status_code << '\n';
    std::cout << "Header:\n";
    for (const std::pair<std::string, std::string>& kv : r.header) {
        std::cout << '\t' << kv.first << ':' << kv.second << '\n';
    }
    std::cout << "Text: " << r.text << '\n';
}

int main(int argc, char** argv) {
    Database new_db("dbname");
    cout <<"GET NAME "<< new_db.get_name()<<endl;
    fs::path tmp{"XD"};
    cout<<"tmp "<<tmp<<endl;
    return 0;
}