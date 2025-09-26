#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "miprof.h"

std::vector<std::string> split_by_space(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

std::vector<char*> to_cargs(std::vector<std::string> &tokens) {
    std::vector<char*> args;
    for (auto &s : tokens) args.push_back(const_cast<char*>(s.c_str()));
    args.push_back(nullptr);
    return args;
}

int main() {
    std::string input;

    while (true) {
        std::cout << "minishell> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "exit") break;

        std::vector<std::string> commands;
        std::istringstream iss(input);
        std::string part;
        while (std::getline(iss, part, '|')) {
            if (!part.empty()) commands.push_back(part);
        }

        int num_cmds = commands.size();
        int pipes[2*(num_cmds-1)];

        for (int i = 0; i < num_cmds-1; i++)
            if (pipe(pipes + i*2) < 0) { perror("pipe failed"); return 1; }

        for (int i = 0; i < num_cmds; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                if (i > 0) dup2(pipes[(i-1)*2], STDIN_FILENO);
                if (i < num_cmds-1) dup2(pipes[i*2 + 1], STDOUT_FILENO);

                for (int j = 0; j < 2*(num_cmds-1); j++) close(pipes[j]);

                auto tokens = split_by_space(commands[i]);

                if (!tokens.empty() && tokens[0] == "miprof") {
                    run_miprof(tokens);
                    _exit(0);
                } else {
                    auto args = to_cargs(tokens);
                    execvp(args[0], args.data());
                    perror("execvp failed");
                    _exit(1);
                }
            } else if (pid < 0) { perror("fork failed"); return 1; }
        }

        for (int j = 0; j < 2*(num_cmds-1); j++) close(pipes[j]);

        for (int i = 0; i < num_cmds; i++) {
            int status;
            wait(&status);
        }
    }

    return 0;
}

