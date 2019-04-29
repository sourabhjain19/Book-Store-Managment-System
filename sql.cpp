#include<iostream>
#include<sqlite3.h>

using namespace std;

int main()
{
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("data.db", &DB);
    if (exit) {
        std::cerr << "Error open DB "<< std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
    sqlite3_close(DB);
    return 0;
}