#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <typeinfo>
#include <filesystem>
#include "classes/database.h"

using namespace std;

string get_text(string band_name, string song_title,  Database & database, int mode);
string parse_page(string & source_page, int mode);
string client_request(string url);
void parse_input(string & input,  bool first_capital);

int main(int argc, char** argv) {    
    Database lyrics_db("lyrics_db");
    while (1)
    {
        string band_name, song_title;
        string mode_in;
        cout<<"band name: "; 
        getline(cin, band_name,'\n');
        cout<<"song title: ";
        getline(cin, song_title,'\n');
        cout<<"band "<<band_name<< " song: "<<song_title<<endl;
        cout<<"version of the lyrcis (0 for native lyrics, 1 for translated ones, 2 for clearing db): ";
        getline(cin, mode_in,'\n');
        int mode = stoi(mode_in);
        if(mode == 2) { 
            lyrics_db.clear_db();
            continue;
        }
        if(mode != 0 && mode != 1) {
            cout<<"wrong mode!\n";
            continue;
        }
        cout << get_text(band_name,song_title, lyrics_db, mode) << "\n\n";
    }
    
    return 0;
}


string get_text(string band_name, string song_title, Database & database, int mode) {    
    int band_mode[] = {0, 1, 0, 1}, song_mode[] = {0, 0, 1, 1};
    for(int i = 0; i < 4; i++) {
        parse_input(band_name, band_mode[i]);
        parse_input(song_title, song_mode[i]);
        string url = "https://www.tekstowo.pl/piosenka," + band_name + "," + song_title + ".html";
        string resposne = client_request(url);
        if(resposne != "-1" && resposne != "-2") {
            string result = parse_page(resposne, mode);
            if(result == "-3")
                return "song: " + song_title + " by: " + band_name + " is not translated yet!";
            cout<<result<<endl<<endl;
            parse_input(band_name, 0);
            parse_input(song_title, 0);
            string file_name = band_name + "_" + song_title;
            if(mode == 0) {
                file_name += "_oryginal.txt";
            } else {
                file_name += "_translated.txt";
            }
            Db_file new_file(file_name, result);
            database.add_file(new_file);
            return result;
        }
        
    }
    return "song: " + song_title + " by: " + band_name + " is unavailable on tekstowo.pl at the moment";
}

void parse_input(string & input, bool first_capital) {
    for(int i = 0; i < input.size(); i++) {
        if(input[i] == ' ') input[i] = '_';
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

string parse_page(string & source_page, int mode) {
    string find_translation = "translation", find_begin = "<div class=\"inner-text\">", find_end = "</div>", skip = "<br />", check_existance = "Niestety nikt nie dodał jeszcze tłumaczenia tego utworu.";    if(mode == 0) {
        find_translation = "song-text";
    }
    if(source_page.find(check_existance) != string::npos) {
        return "-3";
    }
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