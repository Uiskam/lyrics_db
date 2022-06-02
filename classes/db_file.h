#include<string>
using namespace std;

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