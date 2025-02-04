//////////////////
// Dependencies //
//////////////////
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include <map>
#include <netdb.h>
#include <csignal>                  // DOCKER CATCH SIGNALS
#include <fcntl.h>                  // USED FOR NON-BLOCKING SIGNALS! - NEED TO ADD
#include <atomic>
#include <iostream>
#include <string>
#include <cstring>                  // For memset
#include <unistd.h>                 // For close()
#include <random>                   // "RANDOM" STRING GENERATOR



// ADDITIONAL FILE LIBRARIES TO INCLUDE
#include "adminconsole.h"
#include "standardloops.h"
#include "encryption.h"
#include "network.h"



// SERVER SIGNALS
extern std::atomic<int> stopSIGNAL;
extern std::atomic<int> updateSIGNAL;
extern std::atomic<int> serverStarted;
extern std::atomic<int> serverErrors;
extern std::atomic<int> sshErrors;
extern std::atomic<int> sshActive;
extern std::atomic<int> sshHeartBeatFirstTime;
extern std::atomic<long long int> timesincelastcheckinSSH;
extern std::atomic<int> restartatttemptsSSH;
extern std::atomic<long long int> timesincestartup;
extern bool bypassterminal;
extern std::string tokenID;
extern bool beta;

// UPDATE VARIABLES
extern std::string updatesforHONEYPI;

// SERVER VERSION
extern std::string honeyversion;

// COMMANDS FOR DOCKER
extern char* dockerkillguestssh;