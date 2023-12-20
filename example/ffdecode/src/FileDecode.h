
#pragma once

#include <string>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>

class FileDecode{

public:
    FileDecode(std::string file);
    static void Init();

    void start();
    void StartRtsp();

private:
    std::string filename;
    log4cplus::Logger logger = log4cplus::Logger::getRoot();
};