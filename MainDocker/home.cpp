#include "globalheaders.h"


std::string honeyversion = "0.2.0";
bool debug = false;
bool beta = true;
bool testing = false;
bool bypassterminal = false;

// ID PARAMETERS
std::string tokenID = "";
std::string apiKEY = "CvuapyqkefZHKhjHLkusI6ZLKfHbYc40xILa1eBEUkOIxYGqHy8sLXX7GVJegk9e";
std::string passstr = "";
std::string loginstr = "";


////////////////////////
/// ATOMIC VARIABLES ///
////////////////////////
// STATUS SIGNALS
std::atomic<int> stopSIGNAL(0);
std::atomic<int> updateSIGNAL(0);
std::atomic<int> serverErrors(0);
std::atomic<int> sshErrors(0);
std::atomic<int> sshActive(0);
std::atomic<int> serverStarted(0);
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
int heartbeattime = 10;

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
std::atomic<long long int> timesincestartup(0);

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
std::string dockerstatuscommand = "docker ps > nul:";
std::string dockerstartguestssh = "docker run -itd --rm --name=SSHVMV1 -p 222:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
std::string dockerstartguestsshNOREMOVE = "docker run -itd --name=SSHVMV1 -p 222:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
//std::string dockerstartguestssh = "docker run -itd --rm --name=SSHVMV1 -p 22:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
//std::string dockerstartguestsshNOREMOVE = "docker run -itd --name=SSHVMV1 -p 22:22 --network=localportnetwork honeypotpi:guestsshv1 > nul:";
std::string dockerkillguestssh = "docker container kill SSHVMV1 > nul:";
std::string dockerremoveguestssh = "docker container rm SSHVMV1 > nul:";




///////////////////////////////
//// HANDLE DOCKER SIGNALS ////
///////////////////////////////
void handleSignal(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        std::cout << "[INFO] - Received shutdown signal..." << std::endl;
        stopSIGNAL.store(1);     
    }
}


int sendverificationtoserver() {

    return 0;
}

bool checkforupdatesfromserver() {
    
    return false;
}

std::string tokenfromserver() {


    return "OK";
}




//////////////////////////////////////////
// HANDLE NETWORKED CONNECTIONS (63599) //
//////////////////////////////////////////
void handleConnections63599(int server_fd) {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int stopsignals = stopSIGNAL.load();
    int updatesignals = updateSIGNAL.load();

    while (stopsignals == 0 && updatesignals == 0) {
        int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        stopsignals = stopSIGNAL.load();
        updatesignals = updateSIGNAL.load();

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

                bool found63599 = false;

                // HEARTBEAT SSH RECEIVED COMMAND
                if (buffer63599 == "heartbeatSSH") {
                    timesincelastcheckinSSH.store(time(NULL));
                    found63599 = true;
                }

                // PACKET TO SEND TO REPORT
                if (found63599 == false && buffer63599.length() >= 5) {
                    if (buffer63599.substr(3,1) == ":") {
                        int returntype = reportreceiveSSH(buffer63599);
                        if (returntype < 0) {
                            logcritical("INVALID PACKET RECEIVED!", true);
                            logcritical("RECEIVED: " + buffer63599, true);
                        }
                    }
                }
            } 
            close(new_socket);  // Close connection after handling it
        }
    }
    return;
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

    
    int stopsignals = stopSIGNAL.load();
    int updatesignals = updateSIGNAL.load();
    while(stopsignals == 0 && updatesignals == 0) {
        read(new_socket2, buffer, 1024);
        sendtologopen(buffer);
        stopsignals = stopSIGNAL.load();
        updatesignals = updateSIGNAL.load();


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








//////////////////////////////////////////
// CREATE NETWORKED CONNECTIONS (63599) // 
//////////////////////////////////////////
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
    // START LOG LOOP
    startupchecks = system("rm /home/honeypi/log/log.txt");
    startupchecks = startupchecks + system("touch /home/honeypi/log/log.txt");


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
    




    // SET DOCKER CONTAINER OPTIONS
    loginfo("DOCKER - Setting Container Options...", false);
    signal(SIGTERM, handleSignal);
    signal(SIGINT, handleSignal);
    sendtolog("OK");
    sleep(0.5);



    // DETERMINE NETWORK CONNECTIVITY
    loginfo("Determining Network Connectivity...", false);
    int learnt = pingnetwork();

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
    loginfo("Checking Upstream Server Connectivity...", false);
    int checknetworkconnectivitystart = checkserverstatus();

    // CASES
    if (checknetworkconnectivitystart == 0) {
        // VALID RESPONSE
        // CONTINUE
        sendtolog("OK");
    } else if (checknetworkconnectivitystart == 1) {
        // INVALID RESPONSE
        sendtolog("ERROR (1)");
        startupchecks = startupchecks + 1;
    } else if (checknetworkconnectivitystart == 2) {
        // ERROR IN COMMAND
        sendtolog("ERROR (2)");
        startupchecks = startupchecks + 2;
    } else if (checknetworkconnectivitystart == 3) {
        // TEMPORARILY UNAVAILABLE
        // CONTINUE
        sendtolog("Wait (3)");
    } else if (checknetworkconnectivitystart == 4) {
        // RECEIVED REJECTION
        //FIX THIS ON SERVER SIDE
        //startupchecks = startupchecks + 1;
        sendtolog("REJECTED");
    } else {
        // UNCAUGHT EXCEPTION
        startupchecks = startupchecks + 5;
    }




    // UPDATE API TOKEN
    startupchecks = startupchecks + updateToken();



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
    int output = system(dockerstatuscommand.c_str());
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

    sleep(2);
    std::thread acceptingClientsThread2(handle11535Connections, server_fd2);
    acceptingClientsThread2.detach();
    sleep(1);
    loginfo("Creating server thread on port 11535 listen...Done", true);






    // START GUEST DOCKER CONTAINER FOR SSH
    loginfo("Starting Guest Docker Container (SSH) - ", false);
    sleep(2);

    if (startupchecks == 0) {
        int status;

        if (debug == true) {
            status = system(dockerstartguestsshNOREMOVE.c_str());
        } else {
            status = system(dockerstartguestssh.c_str());
        }

        if (status == 0) {
            sshActive.store(1);
            timesincelastcheckinSSH.store(time(NULL) + 10);
            sendtolog("Done");
        } else {
            status = system(dockerkillguestssh.c_str());
            sleep(3);
            status = system(dockerremoveguestssh.c_str());
            sleep(1);
            status = system(dockerstartguestssh.c_str());

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
    } else {
        logcritical("", true);
        logcritical("STARTUP CHECKS DID NOT RETURN 0!", true);
        logcritical("", true);
        logcritical("NOT STARTING GUEST DOCKERS!", true);
        std::cout << "[ERROR] - RETURNED VALUE OF: " << startupchecks << std::endl;
    }



    // SYSTEM STARTED
    loginfo("Updating API Token...", false);


    // FUTURE NETWORK COMMUNICATION TO UPDATE API TOKENS

    sendtolog("future");


    // SERVER PORT LISTEN THREAD

    sleep(2);
    std::thread acceptingClientsThread(handleConnections63599, server63599);
    acceptingClientsThread.detach();
    sleep(1);
    loginfo("Creating server thread on port 63599 listen...Done", false);

    // STARTUP CHECKS
    if (startupchecks != 0) {
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

    std::thread adminConsole(interactiveTerminal);
    adminConsole.detach();


    // START CONSOLE!
    /*
    if (bypassterminal == false) {
        sleep(1);
        std::thread consoleTerminal(interactiveTerminal);
        consoleTerminal.detach();
        sleep(1);
    }
    */


    return startupchecks;
}






int main() {

    // SETUP LOOP
    int startupc = setup();
    serverStarted.store(1);
    

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


    loginfo("Main HoneyPi has started successfully", true);
    bool runnning = true;


    // MAIN RUNNING LOOP
    while(startupchecks == 0 && encounterederrors == 0 && runnning == true) {

        if (attacked == false) {
            sleep(2);
        } else {
            sleep(0.25);
            logwarning("Guest VM has been attacked, reporting...", true);
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
                    system(dockerkillguestssh.c_str());
                    sleep(3);
                    system(dockerremoveguestssh.c_str());
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
                    system(dockerkillguestssh.c_str());
                    sleep(3);
                    system(dockerremoveguestssh.c_str());
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
                        system(dockerkillguestssh.c_str());
                        sleep(3);
                        system(dockerremoveguestssh.c_str());
                        sleep(3);
                        system(dockerstartguestssh.c_str());
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
    } else {
        loginfo("CALLED TO SHUT DOWN SERVER!", true);
        loginfo("Waiting for other threads to finish...", true);
        sleep(3);
        return 0;
    }
    return 200;
}