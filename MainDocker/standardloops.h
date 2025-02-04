#include <string>

#ifndef STANDARDLOOPS_H
#define STANDARDLOOPS_H


// LOG OUTPUT COMMANDS
void sendtolog(std::string);
void sendtologopen(std::string);
void logdebug(std::string, bool);
void loginfo(std::string, bool);
void logwarning(std::string, bool);
void logcritical(std::string, bool);
void logerror(std::string, std::string);

// LOG INPUT COMMANDS
void readfromlogger();

// TIME DETECTOR
std::string timedetector();
std::string refreshDate();
std::string refreshTime();

// CONVERSIONS
int stringtoint(std::string);

// GENERATORS
std::string generateToken();

// PING NETWORK
int pingnetwork();

// INT TO STRING
std::string inttostring(int);


#endif