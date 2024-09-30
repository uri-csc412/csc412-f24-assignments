#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h> // for file descriptor manipulation functions

/*
 the data structure to hold the parsed command
 including any arguments and an optional output file for redirection
*/ 
struct ParsedCommand {
    std::vector<std::string> args; // stores command arguments
    std::string outputFile; // store the output file name for redirection, if any
};

// parses the user input into a ParsedCommand data structure above
ParsedCommand customParseInput(const std::string& input) {
    ParsedCommand command;
    std::string token;
    bool redirectOutput = false;
    bool inQuotes = false;
    for (size_t i = 0; i < input.length(); ++i) {
        char currentChar = input[i];
        // toggle inQuotes flag if a quote is encountered
        if (currentChar == '\"') {
            inQuotes = !inQuotes;
            continue; // skip adding the quote to the token
        }
        // handle space characters
        if (currentChar == ' ' && !inQuotes) {
            if (!token.empty()) {
                if (redirectOutput) {
                    command.outputFile = token;
                    break; // assuming only one output redirection per command
                } else {
                    command.args.push_back(token);
                    token.clear();
                }
            }
        } else if (currentChar == '>' && !inQuotes && token.empty()) {
            redirectOutput = true;
        } else {
            token += currentChar;
        }
    }
    // add the last token if it's not empty
    if (!token.empty()) {
        if (redirectOutput) {
            command.outputFile = token;
        } else {
            command.args.push_back(token);
        }
    }
    return command;
}

// converts a vector of std::string arguments to a vector of char* pointers required by execvp
std::vector<char*> convertArgs(const std::vector<std::string>& args) {
    std::vector<char*> cargs;
    for (const auto& arg : args) {
        cargs.push_back(const_cast<char*>(arg.c_str())); // safe cast since execvp doesn't modify arguments.
    }
    cargs.push_back(nullptr); // execvp expects a NULL-terminated array of arguments.
    return cargs;
}

/*
    This is the function you will use to fork to create a child process
    The child process will execute a user specified command (see the given ParsedCommand struct)
    The three major steps are listed as TODOs for you below
    Make sure you handle any errors and/or fork failure appropriately 
*/
void do_fork(const ParsedCommand& command) {
    // TODO 1: Use 'fork' to create a child process

    // TODO 2: Handle child process case: research file descriptors, 'dup2', and 'execv'

    // TODO 3: Handle parent process case: research 'waitpid'
}


// our old friend main(), this is entry for our shell program ;)
int main() {
    std::string input; // stores user input

    // our main loop: prompt for input, parse, and execute commands until "exit" is entered
    while (std::cout << "412shell> " && std::getline(std::cin, input) && input != "exit") {
        ParsedCommand command = customParseInput(input); // parse the user input into a command
        do_fork(command);
    }

    return 0;
}
