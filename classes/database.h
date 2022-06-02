#include<string>
#include<iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class Database {
    public:
        Database() {
            cout << "Give name for db folder: ";
            string name_given;
            cin >> name_given;
            this->db_name = "../" + name_given;
        }

        Database(string db_name) { 
            this->db_name = "../" + db_name;
        }

        fs::path get_name() { return db_name; }

        bool init();

        //bool add_file();

        //bool remove_file();

        //bool find_file();

    private:
        fs::path db_name;
};