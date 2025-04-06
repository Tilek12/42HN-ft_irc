#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class CommandHandler {
public:
    bool handleNotice(std::istringstream &iss, std::vector<std::string> &arguments);
};

bool CommandHandler::handleNotice(std::istringstream &iss, std::vector<std::string> &arguments) {
    std::string lagcheck, msg, lagData;

    while (iss >> lagcheck) {
        if (lagcheck.size() >= 9 && lagcheck.compare(0, 9, ":LAGCHECK") == 0) {
            // the case if there is no whitespace between LAGCHECK and the rest of the msg
            if (lagcheck.size() > 9) {
                arguments.push_back(msg);
                arguments.push_back(":LAGCHECK");
                arguments.push_back(lagcheck.substr(9));
                return true;
            }

            arguments.push_back(msg);
            arguments.push_back(":LAGCHECK");

            std::getline(iss, lagData);
            if (!lagData.empty() && lagData[0] == ' ')
                lagData.erase(0, 1);
            if (!lagData.empty())
                arguments.push_back(lagData);
            return true;
        }
        if (!msg.empty()) msg += " ";
        msg += lagcheck;
    }
    return false;
}

int main() {
    CommandHandler handler;

    std::vector<std::string> testCases[] = {
        { "Welcome to the IRC Network !@127.0.0.1 :LAGCHECK 1743961970610447-yeah-:)" },
        { "Welcome to IRC :LAGCHECK123456789" },
        { "Just some text with no lagcheck" },
        { ":LAGCHECK diretly startcs here" },
        { "Stuff before :LAGCHECK" }
    };

    for (const auto &input : testCases) {
        std::vector<std::string> args;
        std::istringstream iss(input[0]);
        std::cout << "\nInput: " << input[0] << std::endl;

        if (handler.handleNotice(iss, args)) {
            std::cout << "Parsed arguments:\n";
            for (size_t i = 0; i < args.size(); ++i) {
                std::cout << "  Arg[" << i << "]: [" << args[i] << "]\n";
            }
        } else {
            std::cout << "No LAGCHECK found or invalid format.\n";
        }
    }

    return 0;
}
