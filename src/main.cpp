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

#include "../common/utils.hpp"

#include "_webdash-client/common/utils.hpp"

namespace fs = std::filesystem;
using namespace std;

using json = nlohmann::json;

const string _WEBDASH_PROJECT_NAME_ = "report-build-state";

void GetBuildState(int argc, char **argv) {
    auto preconfig = TryGetConfig(argc, argv);

    if (!preconfig.has_value()) {
        cout << "No config found" << endl;
        return;
    }

    auto wdConfig = preconfig.value().first;

    string cmd = preconfig.value().second;

    if (cmd.length() == 0) {
        cout << "No command given. Defaulting to `report-build-state`" << endl;   
        cmd = "report-build-state"; 
    }

    string exec_output = "";
    webdash::RunConfig config;
    config.redirect_output_to_str = true;
    auto ret = wdConfig.Run(cmd, config);

    if (ret.size() == 0) {
        cout << "No output. Command name likely not defined in webdash.config.json or wrongly spelled." << endl;
        WebDashCore::Get().Log(WebDash::LogType::ERR, "Failed to get output for command " + cmd);
        return;
    }

    // Break output according to '\n'
    vector<string> output = breakOnNewLine(ret[0].output.c_str());

    // Parse output and check for error line.
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

        WebDashCore::Get().Notify("Error in executing " + wdConfig.GetPath() + ":" + cmd);
    } else {
        cout << "No errors" << endl;
    }
}

int main(int argc, char **argv) {
    cout << "Report Build State Tool" << endl;

    GetBuildState(argc, argv);
    
    return 0;
}