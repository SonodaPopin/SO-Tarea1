#include "miprof.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

pid_t child_pid_global;

void sigalrm_handler(int) {
    kill(child_pid_global, SIGKILL);
}

void run_miprof(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Uso: miprof [ejec|ejecsave archivo|ejecutar maxtiempo] comando args...\n";
        return;
    }

    std::string mode = args[1];
    std::string logfile;
    int maxtime = 0;
    int cmd_index = 2;

    if (mode == "ejec") {
    } else if (mode == "ejecsave") {
        if (args.size() < 4) { std::cerr << "Falta archivo\n"; return; }
        logfile = args[2];
        cmd_index = 3;
    } else if (mode == "ejecutar") {
        if (args.size() < 4) { std::cerr << "Falta tiempo maximo\n"; return; }
        maxtime = std::stoi(args[2]);
        cmd_index = 3;
    } else {
        std::cerr << "Modo no válido\n";
        return;
    }

    std::vector<std::string> real_cmd(args.begin() + cmd_index, args.end());
    std::vector<char*> cargs;
    for (auto &s : real_cmd) cargs.push_back(const_cast<char*>(s.c_str()));
    cargs.push_back(nullptr);

    struct timeval start, end;
    gettimeofday(&start, nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        execvp(cargs[0], cargs.data());
        perror("execvp failed");
        _exit(1);
    } else if (pid > 0) {
        if (maxtime > 0) {
            child_pid_global = pid;           
            signal(SIGALRM, sigalrm_handler);
            alarm(maxtime);
        }

        int status;
        struct rusage usage;
        wait4(pid, &status, 0, &usage);

        gettimeofday(&end, nullptr);
        double real_time = (end.tv_sec - start.tv_sec) +
                           (end.tv_usec - start.tv_usec)/1e6;
        double user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec/1e6;
        double sys_time  = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec/1e6;
        long max_mem     = usage.ru_maxrss;

        std::ostringstream report;
        report << "Comando: ";
        for (size_t i = cmd_index; i < args.size(); i++) report << args[i] << " ";
        report << "\nReal: " << real_time << "s, "
               << "User: " << user_time << "s, "
               << "Sys: "  << sys_time  << "s\n"
               << "MaxRSS: " << max_mem << " KB\n";

        if (!logfile.empty()) {
            std::ofstream out(logfile, std::ios::app);
            out << report.str() << "\n";
        } else {
            std::cout << report.str();
        }
    }
}

