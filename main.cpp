#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <sys/types.h>
#include <sys/stat.h>
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
    
    struct stat info;
    const char* pathname = "lyrics_db";
    if( stat( pathname, &info ) != 0 )
        printf( "cannot access %s\n", pathname );
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
        printf( "%s is a directory\n", pathname );
    else
        printf( "%s is no directory\n", pathname );

    Db_file newFile("dupa", "chuj");
    cout << "file name " << newFile.get_file_name() << endl;
    Database new_db();
    return 0;
}