#include<string>
#include<iostream>

using namespace std;

class Database {
    public:
        Database() {
            cout << "Give db name: ";
            cin >> this->db_name;
        }

        Database(string db_name) { this->db_name = db_name; }

        string get_name() { return this->db_name; }

    private:
        string db_name;
};