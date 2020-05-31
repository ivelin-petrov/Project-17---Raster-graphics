#ifndef COMMANDS_H__
#define COMMANDS_H__

#include <fstream>
#include "session.h"

class Commands{
  public:
    static Session load(const std::string& image);
    static void help();
    static void serialization(const Session& session);
    static Session deserialization(int _ID);

    static void choices(const std::string& line, Session& session);

    static Session open(const std::string& file_name);   // in this project functions 'load' and 'deserialization' are used instead of open
    static void close(const std::string& file_name, const Session& session);
    static void save(Session& session);                                           // saves all images
    static void saveas(Session& session, const std::string& new_file);           // saves only the first image in a new file

    static bool foundID(int _ID);
    static Session _switch(int _ID); // session's ID

    static void exit();
};

#endif