#include<string>
#include<iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class Db_file {
    public:
        Db_file() { this->file_name = this->file_content = ""; }

        Db_file(string file_name, string file_content) {
            this->file_name = file_name;
            this->file_content = file_content;
        }

        void set_name(string new_name) { this->file_name = new_name; }

        void set_content(string new_content) {{ this->file_content = new_content; }}

        string get_file_name() {return this->file_name;}

        string get_file_content() {return this->file_content;}

    private:
        string file_name;
        string file_content;

};

class Database {
    public:
        Database() {
            cout << "Give name for db folder: ";
            string name_given;
            cin >> name_given;
            this->db_name = "../" + name_given;
            this->init();
        }

        Database(string db_name) { 
            this->db_name = "../" + db_name;
            this->init();
        }

        fs::path get_name() { return db_name; }

        string get_file_name(Db_file file) { 
            return this->db_name.u8string() + "/" + file.get_file_name();
        }

        void init();

        void add_file(Db_file file_to_add);

        void remove_file(Db_file file_to_remove);

        bool find_file(Db_file file_to_find);

        void clear_db();

        void print_db();
        
    private:
        fs::path db_name;
};