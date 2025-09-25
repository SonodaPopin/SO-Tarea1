#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    std::string input;

    while (true) {
        std::cout << "minishell> ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) continue;

        std::vector<char*> args;
        for (auto &s : tokens) args.push_back(&s[0]);
        args.push_back(nullptr);

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args.data());
            perror("execvp failed");
            return 1;
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            perror("fork failed");
        }
    }

    return 0;
}
