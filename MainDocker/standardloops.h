#include <string>

#ifndef STANDARDLOOPS_H
#define STANDARDLOOPS_H


// SSH LOG OUTPUT FUNCTIONS
void sendtoSSHlog(std::string);

// SSH LOG READ COMMANDS
void readfromsshlogger();

// LOG OUTPUT COMMANDS
void sendtolog(std::string);
void sendtologopen(std::string);
void logdebug(std::string, bool);
void loginfo(std::string, bool);
void logwarning(std::string, bool);
void logcritical(std::string, bool);
void logerror(std::string, std::string);

// LOG READ COMMANDS
void readfromlogger();

// TIME DETECTOR
std::string timedetector();
std::string refreshDate();
std::string refreshTime();

// CONVERSIONS
long long int stringtoint(std::string);
std::string inttostring(long long int);

// GENERATORS
std::string generateToken();

// PING NETWORK
int pingnetwork();

// READ CONFIG FILE
std::map<int, std::string> readconfig();

// UPDATE ALL DOCKER CONTAINERS
int updatedockers();


#endif