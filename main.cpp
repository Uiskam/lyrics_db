#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <typeinfo>
#include <filesystem>
#include "classes/database.h"

using namespace std;

string get_text(string band_name, string song_title,  Database & database);
string parse_page(string & source_page);
string client_request(string url);
void parse_input(string & input,  bool first_capital);

int main(int argc, char** argv) {    
    Database lyrics_db("lyrics_db");
    cout<<"CHUJ LURWA\n";
    while (1)
    {
        string band_name, song_title;
        cout<<"band name: "; 
        cin>>band_name;
        cout<<"song title: ";
        cin>>song_title;
        cout << get_text(band_name,song_title, lyrics_db) << "\n\n";
    }
    
    return 0;
}


string get_text(string band_name, string song_title, Database & database) {    
    int band_mode[] = {0, 1, 0, 1}, song_mode[] = {0, 0, 1, 1};
    for(int i = 0; i < 4; i++) {
        parse_input(band_name, band_mode[i]);
        parse_input(song_title, song_mode[i]);
        string url = "https://www.tekstowo.pl/piosenka," + band_name + "," + song_title + ".html";
        string resposne = client_request(url);
        if(resposne != "-1" && resposne != "-2") {
            string tmp = parse_page(resposne);
            cout<<tmp<<endl<<endl;
            parse_input(band_name, 0);
            parse_input(song_title, 0);
            string file_name = band_name + "_" + song_title + ".txt";
            Db_file new_file(file_name, tmp);
            database.add_file(new_file);
            return tmp;
        }
    }
    return "song: " + song_title + " by: " + band_name + " is unavailable on tekstowo.pl at the moment";
}

void parse_input(string & input, bool first_capital) {
    for(int i = 0; i < input.size(); i++) {
        switch (input[i]) {
        case 'ą':
            input[i] = 'a';
            break;
        case 'ć':
            input[i] = 'c';
            break;
        case 'ę':
            input[i] = 'e';
            break;
        case 'ł':
            input[i] = 'l';
            break;
        case 'ń':
            input[i] = 'n';
            break;
        case 'ó':
            input[i] = 'o';
            break;
        case 'ś':
            input[i] = 's';
            break;
        case 'ź':
            input[i] = 'z';
            break;
        case 'ż':
            input[i] = 'z';
            break;
        case ' ':
            input[i] = '_';
            break;
        default:
            break;
        }
        input[i] = tolower(input[i]);
    }
    if(first_capital)
        input[0] = toupper(input[0]);
}

string client_request(string url) {
    cpr::Response r = cpr::Get(cpr::Url{url},
                               cpr::Authentication{"user", "pass"},
                               cpr::Parameters{{"anon", "true"}, {"key", "value"}});

     if(r.status_code == 404) {
        return "-1";
    } else if (r.status_code != 200) {
        return "-2";
    }                          
    return r.text;
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