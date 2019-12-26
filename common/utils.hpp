#include <cstdio>
#include <unistd.h> 
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sys/wait.h>

// trim from start
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

inline vector<string> breakOnNewLine(const char *sentence) {
    std::stringstream ss(sentence);
    std::string to;

    vector<string> ret;
    if (sentence != NULL)
    {
        while(std::getline(ss,to,'\n'))
            ret.push_back(to);
    }

    return ret;
}