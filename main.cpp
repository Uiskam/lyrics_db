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
string clear_frontend_spaces(const string & input);
string pick_song_from_db();
void parse_input(string & input,  bool first_capital);

int main(int argc, char** argv) {   
    system("cls");
    Database lyrics_db("lyrics_db");
    while (1)
    {
        string band_name, song_title;
        string mode_in;
        cout<<"Lyrics dowload:\n"
        "0 for native lyrics\n"
        "1 for translated ones\n\n"
        
        "Database functons:\n"
        "2 to display all items in databse\n" //2
        "3 to remove lyrics from database\n"//3
        "10 to ERASE ALL DATA\n\n"
        
        "Display lyrics or search for songs:\n"
        "4 to display selected lyrics in notepad\n" //4
        "5 to pick song from databse and search for it in YouTube\n"  //5
        "6 to search for any song in YouTube\n\n" //6
        
        "9 to quit the program\n"
        "chosen (enter to confirm): ";
        getline(cin, mode_in);
        int mode = stoi(mode_in);
        if(mode == 9) {
            system("cls");
            cout<<"Thank you for using lyrics_finder :)\n";
            return 0;
        }
        if(mode != 4 && mode != 5 && mode != 2 && mode != 3 && mode != 10) {
            cout<<"band name: "; 
            getline(cin, band_name);
            cout<<"song title: ";
            getline(cin, song_title);
            cout<<"band "<<band_name<< " song: "<<song_title<<endl;
            band_name = clear_frontend_spaces(band_name);
            song_title = clear_frontend_spaces(song_title);
        }
        system("cls");
        switch (mode)
        {
        case 0:
            cout << get_text(band_name,song_title, lyrics_db, mode) << endl;
            break;
        case 1:
            cout << get_text(band_name,song_title, lyrics_db, mode) << endl;
            break;
        case 2: 
            cout << "databse begin\n";
            lyrics_db.print_db();
            cout << "end of database\n";
            break;
        case 3:
            {
                string requested_song = lyrics_db.pick_song_from_db();
                if(requested_song != "-1") {
                    Db_file file_to_remove(requested_song,"");
                    lyrics_db.remove_file(file_to_remove);
                }
            }
            break;
        case 4:
            {
                string requested_song = lyrics_db.pick_song_from_db();
                if(requested_song != "-1") {
                    Db_file file_to_display(requested_song,"");
                    string str_system_call = "notepad.exe " + lyrics_db.get_file_name(file_to_display);
                    system( str_system_call.c_str() );
                }
            }
            break;
        case 5:
            {
                string requested_song = lyrics_db.pick_song_from_db();
                if(requested_song != "-1") {
                    for(size_t i  = 0; i < requested_song.size(); i++) {
                        if(requested_song[i] == '_')
                            requested_song[i] = '+';
                    }
                    string str_system_call;
                    cout<<"req "<<requested_song<<endl;
                    if(requested_song.find("original.txt") != string::npos) {
                        str_system_call = "msedge.exe https://www.youtube.com/results?search_query=" + band_name + "+" + requested_song.substr(0, requested_song.size() - 13);
                    } else {
                        str_system_call = "msedge.exe https://www.youtube.com/results?search_query=" + band_name + "+" + requested_song.substr(0, requested_song.size() - 15);
                    }
                    cout << "CALL "<< str_system_call << endl;
                    system(str_system_call.c_str());
                }

            }
            break;
        case 6:
            {
                parse_input(band_name, 0);
                parse_input(song_title, 0);
                for(int i =  0; i < band_name.size(); i++) {
                    if(band_name[i] == '_')
                        band_name[i] = '+';
                }
                for(int i =  0; i < song_title.size(); i++) {
                    if(song_title[i] == '_')
                        song_title[i] = '+';
                }
                string str_system_call = "msedge.exe https://www.youtube.com/results?search_query=" + band_name + "+" + song_title;
                cout << str_system_call.c_str() << endl;
                system(str_system_call.c_str());
            }
            break; 
        case 10:
            lyrics_db.clear_db();
            break;
        default:
            cout << "Wrong mode!\n";
            break;
        }
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
            //cout<<result<<endl<<endl;
            parse_input(band_name, 0);
            parse_input(song_title, 0);
            string file_name = band_name + "_" + song_title;
            if(mode == 0) {
                file_name += "_original.txt";
            } else {
                file_name += "_translated.txt";
            }
            Db_file new_file(file_name, result);
            database.add_file(new_file);
            return "succes";
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
    string find_translation = "translation", find_begin = "<div class=\"inner-text\">", find_end = "</div>", skip = "<br />", check_existance = "Niestety nikt nie dodał jeszcze tłumaczenia tego utworu.";    
    if(mode == 0) {
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

string clear_frontend_spaces(const string & input) {
    string prased_input = "";
    size_t input_begin = 0, input_end = input.size() - 1;
    for(int i = 0; i < input.size() && input[i] == ' '; i++) {
        input_begin++;
    }
    for(size_t i = input.size() - 1; i >=  0 && input[i] == ' '; i--) {
        input_end--;
    }
    for(size_t i = input_begin; i <= input_end; i++) {
        prased_input += input[i];
    }
    return prased_input;
}