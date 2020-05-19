//Lab 5
//Author: Nicholas Jordan
//02/21/2019
//Challenge 3

#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

void ReverseString(char *, int);

int main() {
    string data;
    char backwards[100];
    int string_length;
    int count = 0;
    //open file in read mode
    ifstream file("/usr/share/dict/cracklib-small");
    if (!file.is_open()) {
        cout << "Failed to open file\n";
    } else {
        cout << "Lab 5 Challenge #3\n\nFile successfully opened.\n\n";
        //keep reading until the end of the file is reached
        while (getline(file, data)) {
            string_length = data.length();
            //we only want words that are at least 4 characters long
            if (string_length >= 4) {
                strcpy(backwards, data.c_str());
                //reverse the string read from the file
                ReverseString(backwards, string_length);
                //check if the reversed string is the same as the original
                if (backwards == data) {
                    count++;
                    cout << data << "\t<---->\t" << backwards << endl;
                }
            }
        }
        cout << "\nWord count: " << count << endl;
        file.close();
    }
    return 0;
}

void ReverseString(char * backwards, int string_length) {
    for (int i = 0; i < string_length/2; i++) {
        swap(backwards[i], backwards[string_length - i - 1]);
    }
    return;
}
