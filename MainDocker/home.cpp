//////////////////
// Dependencies //
//////////////////
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
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
#include <cstring>      // For memset
#include <sys/socket.h> // For socket functions
#include <arpa/inet.h>  // For sockaddr_in and inet_pton
#include <unistd.h>     // For close()


const std::string honeyversion = "0.1.0";
const bool debug = false;
const bool testing = false;



////////////////////////
/// ATOMIC VARIABLES ///
////////////////////////
// STATUS SIGNALS
std::atomic<int> stopSIGNAL(0);
std::atomic<int> updateSIGNAL(0);
std::atomic<int> serverErrors(0);
std::atomic<int> sshErrors(0);
std::atomic<int> sshActive(0);
std::atomic<int> sshHeartBeatFirstTime(0);

// DELAY VARIABLES
std::atomic<long long int> timesincelastcheckinSSH(0);

// RESTART ATTEMPTS
std::atomic<int> restartatttemptsSSH(0);

// TIMING VARIABLES
std::atomic<int> timer0(0);
std::atomic<int> timer1(0);
std::atomic<int> timer2(0);
std::atomic<int> timer3(0);
std::atomic<int> timer4(0);
std::atomic<int> timer5(0);
std::atomic<int> timer6(0);
std::atomic<int> timer7(0);
std::atomic<int> timer8(0);
std::atomic<int> timer9(0);
std::atomic<int> timer10(0);
// 0 - RESTART SSH DOCKER VARIABLE
// 1 - TIMER TO STOP BUFFER OVERFLOW ON SSH (63599) DOCKER AND CAUSE CPU CRASH
// 2 - TIMER TO STOP BUFFER OVERFLOW ON ROUTER API (11535) PORT


/////////////////
/// VARIABLES ///
/////////////////

// CONSTANT VARIABLES
const int heartbeattime = 10;

// SYSTEM VARIABLES
bool checkforupdates = true;
int startupchecks = 0;
int encounterederrors = 0;
bool attacked = false;
bool systemup = false;
int heartbeat = 29;
std::string erroroccurred = "";
int packetsreceivedSSH = 0;
int packetsreceivedAPI = 0;
//bool heartbeatreceivedrsttimeSSH = false;


// REPORT VARIABLES - SSH
//bool SSHDockerActive = false;
bool lockoutSSH = false;
int maxattemptsSSH = 5;
bool generatingreportSSH = false;
std::string pubipSSH = "0.0.0.0";
int portSSH = 0;
std::string usernamesSSH[101] = {};
std::string passwordsSSH[101] = {};
std::string keybindsSSH[1000] = {};
std::string fileschangedSSH[101] = {};
std::string fileactionsSSH[101] = {};
std::string pubkeysSSH[101] = {};
std::string prikeysSSH[101] = {};
std::string commandsrunSSH[1001] = {};

// NETWORK VARIABLES
const int serverport1 = 63599;
const int serverport2 = 9090;
const int serverport3 = 11520;
const int BUFFER_SIZE = 1024;
int serverSocket1 = 0;
int serverSocket2 = 0;
int serverSocket3 = 0;
int server_fd, new_socket;
int port1;
int server_fd2, new_socket2;
int server_fd3, new_scoket3;
bool packetactive = false;

// TIME VARIABLES
long long int startuptime = 0;
long long int currenttime = 0;
long long int timesincestartup = 0;
int currenthour = 0;
int currentminute = 0;
int currentsecond = 0;
int currentdayofyear = 0;
int currentdays = 0;
int currentyear = 0;
int currentmonth = 0;
int secondsperyear = 31536000;
int daysperyear = 365.25;
int secondsperday = 86400;
int secondsperhour = 3600;
int secondsperminute = 60;
int minutesperhour = 60;
int hoursperday = 24;
bool calculatingtime = false;

// FILE LOCATIONS
const char* SSHStreamFile = "/home/sshfile.txt";
const char* SSHEncryptedFile = "/home/encryptedssh.txt";


/*
// PORT BLOCKING VARIABLES
std::map<int, int> blockportdictionary = {
    {0, 9000},
    {1, 9001},
    {2, 9002},
    {3, 9003},
    {4, 9004},
    {5, 9005},
    {6, 9006},
    {7, 9007},
    {8, 9008},
    {9, 9009},
    {10, 9010},
};
std::map<int, int> sendportdictionary = {
    {0, 9010},
    {1, 9011},
    {2, 9012},
    {3, 9013},
    {4, 9014},
    {5, 9015},
    {6, 9016},
    {7, 9017},
    {8, 9018},
    {9, 9019},
    {10, 9020},
};
std::map<int, int> serverSocketdictionary = {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0},
    {4, 0},
    {5, 0},
    {6, 0},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
};
std::map<int, int> server_fddictionary = {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0},
    {4, 0},
    {5, 0},
    {6, 0},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
};
std::map<int, int> new_socketdictionary = {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0},
    {4, 0},
    {5, 0},
    {6, 0},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
};
std::map<int, bool> serverportsactive = {
    {0, false},
    {1, false},
    {2, false},
    {3, false},
    {4, false},
    {5, false},
    {6, false},
    {7, false},
    {8, false},
    {9, false},
    {10, false},
};
*/








////////////////////////////////
//// DOCKER COMMANDS TO RUN ////
////////////////////////////////
const char* dockerstatuscommand = "docker ps > nul:";
const char* dockerstartguestssh = "docker run -itd --rm --name=SSHVMV1 -p 222:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
const char* dockerstartguestsshNOREMOVE = "docker run -itd --name=SSHVMV1 -p 222:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
//const char* dockerstartguestssh = "docker run -itd --rm --name=SSHVMV1 -p 22:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
//const char* dockerstartguestsshNOREMOVE = "docker run -itd --name=SSHVMV1 -p 22:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
const char* dockerkillguestssh = "docker container kill SSHVMV1 > nul:";
const char* dockerremoveguestssh = "docker container rm SSHVMV1 > nul:";








int timedetector() {
    if (calculatingtime == true) {
        std::cout << "[WARNING] - Call to Time Calculation Called While Already Processing!" << std::endl;
        return 1;

    }  else {
        // TIME
        currenttime = time(NULL);

        // CURRENT SECONDS
        timesincestartup = currenttime - startuptime;
        currentsecond = currenttime % secondsperminute;

        // CURRENT MINUTES
        currentminute = currenttime - currentsecond;
        currentminute = currentminute % 3600;
        currentminute = currentminute / 60;

        // CURRENT HOURS
        currenthour = currenttime - ((currentminute * 60) + currentsecond);
        currenthour = currenthour % hoursperday;
        
        // CURRENT DAYS
        currentdays = currenttime - ((currenthour * 3600) + (currentminute * 60) + currentsecond);
        currentdays = currentdays / 86400;

        // CURRENT YEARS
        currentyear = 1970 + (currentdays / 365.25);

        // DEBUG PRINT VALUES TO CONSOLE
        if (debug == true) {
            std::cout << currentsecond << std::endl;
            std::cout << currentminute << std::endl;
            std::cout << currenthour << std::endl;
            std::cout << currentdays << std::endl;
            std::cout << currentyear << std::endl;
        }

        return 0;
    }

    return 1;
}








////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    std::cout << data2 << std::endl;
}

void sendtologopen(std::string data2) {
    std::cout << data2;
}

void logdebug(std::string data2, bool complete) {
    data2 = "[DEBUG] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void loginfo(std::string data2, bool complete) {
    data2 = "[INFO] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logwarning(std::string data2, bool complete) {
    data2 = "[WARNING] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logcritical(std::string data2, bool complete) {
    data2 = "[CRITICAL] - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logerror(std::string headerdata2, std::string errormessage) {
    std::string data2 = "[ERROR] - " + headerdata2 + " - " + errormessage;
    sendtolog(data2);
}




///////////////////////////////
//// HANDLE DOCKER SIGNALS ////
///////////////////////////////
void handleSignal(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        std::cout << "Received termination signal, shutting down gracefully..." << std::endl;
        stopSIGNAL.store(1);     
    }
}




//////////////////////////////////////
//// CHECK UPSTREAM SERVER STATUS ////
//////////////////////////////////////
int checkserverstatus() {
    const char* server_ip = "honeypi.baselinux.net"; // Server IP address
    // ADD CHECK FOR DNS!!!
    const int server_port = 11829;           // Server port number
    const std::string message = "HAPI/1.1\nContent-Type:text/json\n\n{\"CONNECTION\", \"NEW\"}";
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(server_ip, nullptr, &hints, &res) != 0) {
        logcritical("Unable to resolve hostname!", true);
    }


    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;

    freeaddrinfo(res);

    int serverUpstream = socket(AF_INET, SOCK_STREAM, 0);

    if (serverUpstream < 0) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }
/*
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported.\n";
        close(serverUpstream);
        return 1;
    }
*/

    if (connect(serverUpstream, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        close(serverUpstream);
        return 1;
    }

    std::cout << "Connected to the server at " << server_ip << ":" << server_port << "\n";
    send(serverUpstream, message.c_str(), message.length(), 0);
    std::cout << "Message sent: " << message << "\n";
    char bufferread[4096];
    read(serverUpstream, bufferread, 4096);
    std::string yup = bufferread;
    close(serverUpstream);

    if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 18\n\n{state: available}") {
        loginfo("SERVER - Received Valid Connection...", true);
        return 0;
    } else if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 20\n\n{state: unavailable}") {
        loginfo("SERVER - Server Temporarily Unavailable, Continuing...", true);
        return 3;

    } else {
        logcritical("SERVER - RECEIVED NOT RESPONSE FROM SERVER!", true);
        std::cout << "RECEIVED:" << yup << std::endl;
        return 1;
    }
    return 2;
}

int sendverificationtoserver() {

}

bool checkforupdatesfromserver() {
    
}

std::string tokenfromserver() {

}




//////////////////////////////////////////
// HANDLE NETWORKED CONNECTIONS (63599) //
//////////////////////////////////////////
void handleConnections63599(int server_fd) {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    while (true) {
        int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        if (new_socket > 0) {
            char buffer63599array[4096] = {0};

            // Attempt to read data from the client
            ssize_t readable = read(new_socket, buffer63599array, 4096);

            if (readable == 0) {
                // Client disconnected gracefully
                loginfo("Client disconnected", true);
            } else if (readable > 0) {
                // Successfully read data
                loginfo(buffer63599array, true);
                std::string buffer63599 = buffer63599array;

                // HEARTBEAT SSH RECEIVED COMMAND
                if (buffer63599 == "heartbeatSSH") {
                    timesincelastcheckinSSH.store(time(NULL));
                    loginfo("HEARTBEAT RECEIVED!", true);
                }

            } 

            close(new_socket);  // Close connection after handling it
        }
    }
}





//////////////////////////////////////////
// HANDLE NETWORKED CONNECTIONS (11535) //
//////////////////////////////////////////
// FIX THIS - RESTRUCTURE THIS TO NEW CONTEXT! 
void handle11535Connections(int server_fd2) {
    char buffer[1024] = {0};
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int new_socket2;
    ssize_t valread;
    std::string hello = "HEY!";

    if ((new_socket2 = accept(server_fd2, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    } else {
        loginfo("11535 port initialized", true);
    }

    while(true) {
        read(new_socket2, buffer, 1024);
        sendtologopen(buffer);

        if (buffer != NULL && attacked == false) {

            // HEARTBEAT COMMAND TO NOT SPAM LOG
            if (strcmp(buffer, "heartbeatSSH")) {
                if (heartbeat >= 10) {
                    loginfo("Received heartbeat from SSH Guest VM", true);
                } else {
                    heartbeat = heartbeat + 1;
                }
            } 

        } else {
            if (buffer != NULL && attacked == true) {

            } else {
                if (buffer == NULL) {
                    logcritical("INVALID CONNECTION RECEIVED, ignoring...", true);
                }
            }
        }


        // ANTI-CRASH PACKET FLOW CHECK
        if (timer2.load() == time(NULL)) {
            packetsreceivedAPI = packetsreceivedAPI + 1;
            if (packetsreceivedAPI >= 10) {
                // KILL CONTAINER
                logcritical("PACKET OVERFLOW DETECTED ON ROUTER API!", true);
                close(server_fd2);
            }
        } else {
            timer2.store(time(NULL));
            packetsreceivedAPI = 0;
        }



 //        Send a hello message to the client
         send(new_socket2, hello.c_str(), hello.size(), 0);
         std::cout << "Hello message sent" << std::endl;
    }
}


/////////////////////////////////////////
// HANDLE NETWORKED CONNECTIONS (8080) // 
/////////////////////////////////////////
void handle8080Connections(int server_fd3) {

}




//////////////////////////////
// REPORT GENERATING SCRIPT //
//////////////////////////////
int createreport() {
    logwarning("CREATING REPORT (THIS COULD TAKE SEVERAL MINUTES!)", true);

    sleep(5);

    // OPEN FILE
    std::ifstream inputStream;
    inputStream.open(SSHStreamFile);
    std::ofstream encryptedStream;
    encryptedStream.open(SSHEncryptedFile);
    if (inputStream.is_open() != true && encryptedStream.is_open() != true) {
        logcritical("AN ERROR OCCURRED WITH THE SSH FILE!", true);
        logcritical("COULD NOT CONTINUE", true);
        encounterederrors = encounterederrors + 1;
        return 1;
        return 1;
    } else {
        std::string inputstring;
        std::string encryptedstring;
        std::string key = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::string compressedkey = "zxywv";
        encryptedStream << compressedkey << '\n';
        int testtastic = 0;
        int testtasticmax = 5;
        bool completion31 = false;
        while (completion31 != true) {
            getline(inputStream, inputstring);
            if (inputstring == "") {
                testtastic = testtastic + 1;
                if (testtastic >= testtasticmax) {
                    completion31 = true;
                }
            } else {
                int hat = 0;
                while (inputstring.length() >= hat) {
                    // HAT TWO THIRTY
                }
            }
        }

        // SEND TO SERVER!
    }

    return 1;
}










int createnetworkport63599() {
    int PORT = 63599;
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    std::string hello = "Hello from server";
    int opt = 1;

    // SETUP NETWORK PORTS
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 63599
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // REACHED HERE
    sendtologopen("...");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the network address and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //fcntl(server_fd, F_SETFL, O_NONBLOCK);

    return server_fd;
}



/////////////////////////
// THE MAIN CRASH LOOP //
/////////////////////////





////////////////////////////
// THE MAIN SETUP SCRIPTS //
//////////////////////////// 
int setup() {
    sendtolog("Hello, World from 2515!");
    sendtolog("  _____     _____     ____________      _____      ____  ________________   ____         ____           ______________     ________________  ");
    sendtolog("  |   |     |   |    /            `     |   `      |  |  |               |  `  `        /   /           |             `   |               |  ");
    sendtolog("  |   |     |   |   /              `    |    `     |  |  |  |¯¯¯¯¯¯¯¯¯¯¯¯    `  `      /   /            |   |¯¯¯¯¯¯`   |  |_____    ______|  ");
    sendtolog("  |   |     |   |  /   /¯¯¯¯¯¯¯¯`   `   |     `    |  |  |  |____________     `  `    /   /             |   |______/   |        |   |        ");
    sendtolog("  |    ¯¯¯¯¯    |  |   |         |   |  |      `   |  |  |               |     `  `  /   /              |   __________/         |   |        ");
    sendtolog("  |    _____    |  |   |         |   |  |   |`  `  |  |  |               |      `  `/   /               |   |                   |   |        ");
    sendtolog("  |   |     |   |  |   |         |   |  |   | `  ` |  |  |  |¯¯¯¯¯¯¯¯¯¯¯¯        |     |                |   |                   |   |        ");
    sendtolog("  |   |     |   |  |   |         |   |  |   |  `  `|  |  |  |____________        |     |                |   |                   |   |        ");
    sendtolog("  |   |     |   |  `   `¯¯¯¯¯¯¯¯¯    /  |   |   `     |  |               |       |     |                |   |             |¯¯¯¯¯     ¯¯¯¯¯|  ");
    sendtolog("  |   |     |   |   `               /   |   |    `    |  |               |       |     |                |   |             |               |  ");
    sendtolog("  ¯¯¯¯¯     ¯¯¯¯¯    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯    ¯¯¯¯      `¯¯¯   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯        ¯¯¯¯¯¯                 ¯¯¯¯¯             ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯   ");
    sendtolog("HoneyPot Edition!");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("Program by Matthew Whitworth (MawWebby)");
    sendtolog("Version: " + honeyversion);
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    sendtolog("");
    



    // DELAY FOR SYSTEM TO START FURTHER (FIGURE OUT CURRENT TIME)
    timedetector();
    loginfo("Starting", true);
    sleep(2);




    // SET DOCKER CONTAINER OPTIONS
    loginfo("DOCKER - Setting Container Options...", false);
    signal(SIGTERM, handleSignal);
    signal(SIGINT, handleSignal);
    sendtolog("OK");
    sleep(0.5);




    startupchecks = startupchecks + timedetector();


    // DETERMINE NETWORK CONNECTIVITY
    loginfo("Determining Network Connectivity...", false);
    int learnt = system("ping -c 5 8.8.8.8 > nul:");

    if (learnt == 0) {
        sendtolog("Done");
    } else {
        sendtolog("ERROR");
        logcritical("UNABLE TO DETERMINE NETWORK CONNECTIVITY!", true);
        logcritical("Killing", true);
        startupchecks = startupchecks + 1;
        return 1;
        return 1;
        return 1;
    }



    // CHECK UPSTREAM SERVER STATUS
    int checknetworkconnectivitystart = checkserverstatus();

    if (checknetworkconnectivitystart == 0) {
        // VALID RESPONSE
    } else if (checknetworkconnectivitystart == 1) {
        // INVALID RESPONSE
    } else if (checknetworkconnectivitystart == 2) {
        // ERROR IN COMMAND
    } else if (checknetworkconnectivitystart == 3) {
        // TEMPORARILY UNAVAILABLE
    } else {
        // UNCAUGHT EXCEPTION
    }



    loginfo("Verifying System...", false);

    // FUTURE LOOP OF INTEGRITY

    sendtolog("DONE (no checks needed)");
    





    // CHECK FOR SYSTEM UPDATES
    loginfo("Checking for Updates...", false);
    if (checkforupdates == true) {
        // CHECK FOR SYSTEM UPDATES
        int returnedvalue = system("apt-get update > nul:");
        if (returnedvalue == 0) {
            sendtolog("Done");
        } else {
            sendtolog("ERROR");
            logcritical("UNABLE TO CHECK FOR SYSTEM UPDATES!", true);
            logcritical("This could be potentially dangerous!", true);
            logcritical("KILLING PROCESS!", true);
            startupchecks = startupchecks + 1;
            return 1;
            return 1;
            return 1;
        }

        // REMOVED AUTO UPGRADE FOR POTENTIAL PACKAGE CONFLICTS AND BROKEN INSTALLS


        // CHECK FOR UPDATES FROM SERVER
        // TEST COMMUNICATION WITH SERVER

    } else {
        sendtolog("disabled");
        logwarning("UNABLE TO CHECK FOR UPDATES! (SYSTEM DISABLED)", true);
    }


    // CHECK FOR DOCKER UPDATES



    


    // OPEN HACKING FILES
    loginfo("[INFO] - Opening File...", false);
//    fstream myFile;
 //   myFile.open("hackerlogv1.txt");

//    if (myFile.is_open() == false) {
 //       sendtologclosed("ERROR");
 //       startupchecks = startupchecks + 1;
 //   } else {
 //       sendtologclosed("DONE");
 //   }
    sendtolog("future");





    // CHECK DOCKER STATUS
    loginfo("Checking for Docker Control...", false);
    sleep(1);
    int output = system(dockerstatuscommand);
    if (output == 0) {
        sendtolog("Done");
    } else {
        sendtolog("ERROR");
        startupchecks = startupchecks + 1;
        logcritical("Could not communicate with docker", true);
        return 1;
        return 1;
        return 1;
    }






    // OPEN NETWORK SERVER PORTS (1/3)
    loginfo("Opening Server Ports (1/3)", false);

    int server63599 = createnetworkport63599();

    if (server63599 != 0) {
        sendtolog("Done");
    } else {
        sendtolog("ERROR");
        startupchecks = startupchecks + 1;
        logcritical("ERROR OCCURRED TRYING TO OPEN NETWORK PORTS!", true);
        return 1;
        return 1;
        return 1;
    }
    
    sleep(2);


    int PORT = 11535;

    // OPEN NETWORK SERVER PORTS (2/3)
    loginfo("Opening Server Ports (2/3)...", false);

    int server_fd2, new_socket2;
    ssize_t valread2;
    struct sockaddr_in address2;
    socklen_t addrlen2 = sizeof(address2);
    int opt2 = 1;
    
    sleep(1);

    if((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        // FIX THIS
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 11535
    if (setsockopt(server_fd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt2, sizeof(opt2))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // REACHED HERE
    sendtologopen("...");
    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT);

    // Binding the socket to the network address and port
    if (bind(server_fd2, (struct sockaddr*)&address2, sizeof(address2)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd2, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    sendtolog("Done");
    sleep(2);





    // SERVER PORT LISTEN THREAD (2/3) (11535)
    loginfo("Creating server thread on port 11535 listen...", false);

    sleep(2);
    std::thread acceptingClientsThread2(handle11535Connections, server_fd2);
    acceptingClientsThread2.detach();
    sleep(1);

    sendtolog("Done");






    // START GUEST DOCKER CONTAINER FOR SSH
    loginfo("Starting Guest Docker Container (SSH) - ", false);
    sleep(2);
    int status;

    if (debug == true) {
        status = system(dockerstartguestsshNOREMOVE);
    } else {
        status = system(dockerstartguestssh);
    }

    if (status == 0) {
        sshActive.store(1);
        timesincelastcheckinSSH.store(time(NULL) + 10);
        sendtolog("Done");
    } else {
        status = system(dockerkillguestssh);
        sleep(3);
        status = system(dockerremoveguestssh);
        sleep(1);
        status = system(dockerstartguestssh);

        if (status == 0) {
            sshActive.store(1);
            timesincelastcheckinSSH.store(time(NULL) + 1);
            sendtolog("Done");
        } else {
            sshActive.store(0);
            timesincelastcheckinSSH.store(0);
            logcritical("SSH DOCKER DID NOT START SUCCESSFULLY", true);
            startupchecks = startupchecks + 1;
        }
    }



    // SYSTEM STARTED
    loginfo("Updating API Token...", false);


    // FUTURE NETWORK COMMUNICATION TO UPDATE API TOKENS

    sendtolog("future");


    // SERVER PORT LISTEN THREAD
    loginfo("Creating server thread on port 63599 listen...", false);

    sleep(2);
    std::thread acceptingClientsThread(handleConnections63599, server63599);
    acceptingClientsThread.detach();
    sleep(1);

    sendtolog("Done");

    
    
    return startupchecks;
}






int main() {

    // SETUP LOOP
    int startupc = setup();

    

    // STARTUP CHECKS
    if (startupc != 0) {
        logcritical("STARTUP CHECKS RETURNED EXIT CODE 1", true);
        logcritical("THE SYSTEM COULD NOT CONTINUE!", true);
        logcritical("ALL DOCKER CONTAINERS WILL BE STOPPED", true);

        // ADD FUTURE DOCKER CONTAINER INFORMATION
        close(serverport1);
        close(serverport2);
        sleep(10);
        //int completion = system("docker kill * > nul:");
        sleep(10);

        // EXIT AND STOP PROCESSES
        return(1);
        return(1);
        return(1);
    }

    

/*
    if (testing == true) {
        loginfo("Beta Testing Active...", false);
        sleep(1);
        sendtolog("Nothing to Test");
    } else {
        loginfo("Not beta testing/Removing beta file...", true);
        startupchecks = startupchecks + system("rm test");
    }
*/

    loginfo("Main HoneyPi has started successfully", true);
    bool runnning = true;


    // MAIN RUNNING LOOP
    while(true && startupchecks == 0 && encounterederrors == 0 && runnning == true) {

        if (attacked == false) {
            sleep(2);
        } else {
            sleep(0.25);
            logwarning("Guest VM has been attacked, reporting...", true);
        }

        if (generatingreportSSH == true) {
            encounterederrors = encounterederrors + createreport();
        }

        if (stopSIGNAL.load() == 1) {
            runnning = false;
        }


        // WATCHDOG IN MAIN LOOP
        int differenceintimeSSH = time(NULL) - timesincelastcheckinSSH.load();
        int sshDockerActive = sshActive.load();
        if (sshDockerActive == 1) {
            int heartbeatfirsttimeSSH = sshHeartBeatFirstTime.load();
            if (heartbeatfirsttimeSSH == true) {
                if (differenceintimeSSH >= 30) {
                    logwarning("30 seconds since last SSH Heartbeat received", true);
                }

                if (differenceintimeSSH >= 45) {
                    logcritical("45 seconds since last SSH Heartbeat received, assuming dead", true);
                    close(server_fd);
                    sshActive.store(0);
                    system(dockerkillguestssh);
                    sleep(3);
                    system(dockerremoveguestssh);
                    timer0.store(time(NULL));
                }
            } else {
                if (differenceintimeSSH >= 240) {
                    logwarning("240 seconds since first expected SSH Heartbeat received", true);
                }

                if (differenceintimeSSH >= 300) {
                    logcritical("300 seconds since first expected SSH Heartbeat received, assuming dead", true);
                    close(server_fd);
                    sshActive.store(0);
                    system(dockerkillguestssh);
                    sleep(3);
                    system(dockerremoveguestssh);
                    timer0.store(time(NULL));
                }
            }
            
        } else {
            if (timer0.load() != 0) {
                long long int changeintime = time(NULL) - timer0.load();

                if (changeintime >= 60 && lockoutSSH != true) {
                    int retryattempts = restartatttemptsSSH.load();
                    if (retryattempts >= maxattemptsSSH) {
                        lockoutSSH = true;
                        logcritical("LOCKING OUT SSH FROM STARTING!", true);
                    } else {
                        retryattempts = retryattempts + 1;
                        restartatttemptsSSH.store(retryattempts);

                        logwarning("Attempting to restart SSH VM", true);
                        system(dockerkillguestssh);
                        sleep(3);
                        system(dockerremoveguestssh);
                        sleep(3);
                        system(dockerstartguestssh);
                        sshActive.store(1);
                        timesincelastcheckinSSH.store(time(NULL) + 10);
                        int port63599 = createnetworkport63599();
                        sleep(2);
                        std::thread acceptingClientsThread(handleConnections63599, port63599);
                        acceptingClientsThread.detach();
                    }
                }
            } else {
                logwarning("Attempting to restart in 60 seconds!", true);
                timer0.store(time(NULL));
            }
        }
    }

    // ENCOUNTERED ERRORS
    if (encounterederrors != 0) {
        logcritical("HONEYPI HAS ENCOUNTERED UNRECOVERABLE ERRORS WHILE RUNNING!", true);
        logcritical("HONEYPI WILL NOW ATTEMPT A LOG DUMP!", true);
        std::cout << encounterederrors << std::endl;
        std::cout << erroroccurred << std::endl;


        std::cout << usernamesSSH << std::endl;
        std::cout << passwordsSSH << std::endl;
        std::cout << keybindsSSH << std::endl;
        std::cout << fileschangedSSH << std::endl;
        std::cout << fileactionsSSH << std::endl;
        std::cout << pubipSSH << std::endl;
        std::cout << portSSH << std::endl;
        std::cout << pubkeysSSH << std::endl;
        std::cout << prikeysSSH << std::endl;


        logcritical("ATTEMPTING TO KILL ALL DOCKER CONTAINERS!!!", true);
        close(serverport1);
        close(serverport2);
        sleep(10);
        //int completion = system("docker kill * > nul:");
        sleep(10);
    }

    if (runnning == true) {
        loginfo("CALLED TO STOP SERVER!", true);
        sleep(3);
        return 0;
    }
}