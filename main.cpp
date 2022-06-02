#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <typeinfo>
#include <filesystem>
#include "classes/database.h"

using namespace std;

string get_text(string band_name, string song_title);
string parse_page(string & source_page);

int main(int argc, char** argv) {    
    Database new_db("lyrics_db");
    Db_file new_file("tmp.txt", get_text("asd","Asd"));
    new_db.add_file(new_file);
    return 0;
}

string get_text(string band_name, string song_title) {
    cpr::Response r = cpr::Get(cpr::Url{"https://www.tekstowo.pl/piosenka,rammstein,zick_zack.html"},
                               cpr::Authentication{"user", "pass"},
                               cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    
    return parse_page(r.text);
}

string parse_page(string & source_page) {
    string find_translation = "translation", find_begin = "<div class=\"inner-text\">", find_end = "</div>", skip = "<br />";
    string res = "";
    size_t start_id =  source_page.find(find_translation);
    start_id =  source_page.find(find_begin, start_id);
    start_id += find_begin.size();
    size_t end_id = source_page.find(find_end, start_id);
    int i_max = 0, j_max = 0;
    for(size_t i = start_id; i < end_id; i++) {
        res += source_page[i];
    }
    for(size_t i = 0; i < res.size();) {
        i = res.find(skip,i);
        if(i == string::npos) {
            break;
        }
        res.erase(i, skip.size());
    }
    return res;
}