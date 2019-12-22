/**
 * Tutorial on pipes: http://www.rozmichelle.com/pipes-forks-dups/
 **/

#include <cstdio>
#include <unistd.h> 
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sys/wait.h>

#include <webdash-config.hpp>
#include <webdash-types.hpp>
#include <webdash-core.hpp>

namespace fs = std::filesystem;
using namespace std;

using json = nlohmann::json;

const string _WEBDASH_PROJECT_NAME_ = "report-build-state";

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

vector<string> doSegment(const char *sentence)
{
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

void GetBuildState(const string cmd, const string configpath) {
    WebDashConfig wdConfig(configpath);

    string exec_output = "";
    webdash::RunConfig config;
    config.redirect_output_to_str = true;
    auto ret = wdConfig.Run(cmd, config);

    if (ret.size() == 0) {
        WebDashCore::Get().Log(WebDash::LogType::ERR, "Failed to get output for command " + cmd);
        return;
    }

    vector<string> output = doSegment(ret[0].output.c_str());

    // To intentionally cause error:
    string errline = "";
    for (size_t i = 0; i < output.size(); ++i) {
        string line = output[i];
        trim(line);

        std::locale loc;
        string lowline;
        for (std::string::size_type i=0; i < line.length(); ++i)
             lowline += std::tolower(line[i],loc);

        if (lowline.find("err") != std::string::npos) {
            errline = line;
        }
    }

    if (errline != "") {
        cout << "Error line exists:" << endl;
        cout << errline << endl;
        cout << endl;

        WebDashCore::Get().Notify("Error in executing " + configpath + ":" + cmd);
    } else {
        cout << "No errors" << endl;
    }
}

int main(int argc, char **argv) {
    cout << "Report Build State Tool" << endl;
    if (argc < 2) {
        cout << "Please provide the desired action." << endl;
        return 0;
    }

    const fs::path configPath = fs::current_path() / "webdash.config.json";
    GetBuildState(argv[1], configPath);
    
    return 0;
}