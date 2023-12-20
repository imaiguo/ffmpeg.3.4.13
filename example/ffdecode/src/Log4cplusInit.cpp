

#include <string>
#include <iostream>
#include <filesystem>

#include <log4cplus/Initializer.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/loglog.h>

log4cplus::Initializer initializer;

bool Log4cplusInitFunc(std::string exepath){

#ifdef __MINGW64__
     std::experimental::filesystem::create_directory("log");
     std::experimental::filesystem::create_directory("picffmpeg");
#else
    std::filesystem::create_directory("log");
    std::filesystem::create_directory("picffmpeg");
#endif

    size_t pos = exepath.find_last_of("\\");
    std::string newpath = exepath.substr(0, pos+1);
    log4cplus::tstring tPath = LOG4CPLUS_STRING_TO_TSTRING(newpath + "config\\log4cplus.properties" );
    std::wcout << "current config -> [" << tPath  << "]." << std::endl;

    log4cplus::PropertyConfigurator::doConfigure(tPath);

    return true;
}
