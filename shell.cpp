#include <iostream>
#include <vector>
#include <sstream>
#include <zconf.h>
#include <cstring>
#include <algorithm>
#include <sys/wait.h>

using namespace std;

// splits a string up based on a delimiter.
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

// converts a string array into a classic c-style char**.
char** convert(vector<string> words)
{
    char ** arr = new char*[words.size()];

    for(size_t i = 0; i < words.size(); i++){
        arr[i] = new char[words[i].size() + 1];
        strcpy(arr[i], words[i].c_str());
    }
    return arr;
}

int main()
{
    while(true)
    {
        string line;
        cout << "shell=> ";
        getline(cin, line); // get the line of text from the user.

        // split the line up by the spaces.
        vector<string> words = split(line, ' ');

        pid_t pid = fork(); // create a new child process
        if (pid < 0) // if the fork failed.
        {
            cout << "Fork failed." << endl;
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // if we are in the child process
        {
            // convert string into c-style char*
            const char *abs_path_to_exec = words[0].c_str();

            // convert vector of strings into c-style char**
            char ** arr = convert(words);

            // execute whatever command
            execv(abs_path_to_exec, arr);

            exit(EXIT_SUCCESS);
        }
        else { // if parent
            int status = 0;
            wait(&status); // wait until child process is finished.
        }
    }
}
