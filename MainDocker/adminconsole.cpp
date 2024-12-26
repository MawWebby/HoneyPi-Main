#include "globalheaders.h"
#include "adminconsole.h"

int useraccesslevel = 0;

// BACKUP OF DATABASES


// USER LEVELS
// USER 3 - MASTER USER
// USER 2 - EDIT DB/ MAINTAIN SERVER
// USER 1 - ACTIVATE 
// USER 0 - TEST COMMANDS ON SERVER

int currentimunte;
int currenthour;
int currentdays;
int currentyear;


/////////////////////////////////
//// WAIT FOR TERMINAL INPUT ////
/////////////////////////////////
std::string terminalinput() {
    std::string command;
    std::getline(std::cin, command);
    return command;
}



////////////////////////////////////
//// USER ACCESS LEVEL COMMANDS ////
////////////////////////////////////
void level1access() {
    std::cout << std::endl;
    std::cout << "Level 1 Access:" << std::endl;
    std::cout << "ping        | (NO ARGS) | Ping the Internet to Verify Connectivity" << std::endl;
    std::cout << "update      | (NO ARGS) | FUTURE" << std::endl;
    std::cout << "shutdown    | (NO ARGS) | Shutdown the Honeypot" << std::endl;
    std::cout << "exit        | (NO ARGS) | Exit out of console but keep Honeypot running." << std::endl;
    std::cout << "logs        | (NO ARGS) | Read the Logs from Honeypot" << std::endl;
    std::cout << "system      | (NO ARGS) | Enter Bash of the Docker Container" << std::endl;
    std::cout << "generate    | (NO ARGS) | Generate Example of Token Sent to Server" << std::endl;
    std::cout << "pinghoney   | (NO ARGS) | Ping the Honey Server to Verify Connectivity" << std::endl;
}

void level0access() {
    std::cout << std::endl;
    std::cout << "Level 0 Access:" << std::endl;
    std::cout << "commands    | (NO ARGS) | Displays this list of commands" << std::endl;
    std::cout << "status      | (NO ARGS) | Stats of Honeypot" << std::endl;
    std::cout << "login       | (NO ARGS) | Login to Honeypot" << std::endl;
    std::cout << "logout      | (NO ARGS) | Logout of Honeypot" << std::endl;
}




/////////////////////////////////
//// SEND TO TERMINAL SCRIPT ////
/////////////////////////////////
void sendtoterminal(std::string data) {
    std::cout << data << std::endl;
}

void processCommand(const std::string& command) {
    bool foundcommand = false;

    // SHOW GENERAL HELP IN COMMANDS
    if (command == "commands") {
        switch (useraccesslevel) {
            case 0:
                level0access();
                break;
            case 1:
                level1access();
                level0access();
                break;
        }
        foundcommand = true;
    }

    // SERVER STATUS COMMAND
    if (command == "status") {
        std::cout << "Server Status" << std::endl;
        std::cout << std::endl;
        std::cout << "Thread Status" << std::endl;
        foundcommand = true;
    }

    // LOGIN COMMAND
    if (command == "login") {
        system("clear");
        std::cout << "Username: ";
        std::string username = terminalinput();
        system("clear");
        std::cout << "Password: ";
        std::string password = terminalinput();
        system("clear");
        sleep(1);
        logwarning("Attempted Login @ " + username + "; Pass: " + password, true);
        int result = logincredentials(username, password);
        if (result != 0) {
            useraccesslevel = result;
            std::cout << "Login was Successful" << std::endl;
            loginfo("Login Successful", true);
        } else {
            useraccesslevel = 0;
            std::cout << "Login DENIED!" << std::endl;
            logcritical("Login DENIED @ " + username + "; Pass: " + password, true);
        }
        sleep(3);
        system("clear");
        foundcommand = true;
    }

    // PING THE NETWORK
    if (command == "ping") {
        if (useraccesslevel >= 1) {
            std::cout << "Pinging Internet..." << std::endl;
            int pinger = pingnetwork();
            if (pinger == 0) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "ERROR" << std::endl;
            }
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // PING HONEY SERVER
    if (command == "pinghoney") {
        std::cout << "Pinging the Server..." << std::endl;
        int checknetworkconnectivitystart = checkserverstatus();
        if (checknetworkconnectivitystart == 0) {
            std::cout << "Successful Response Received" << std::endl;
        } else if (checknetworkconnectivitystart == 1) {
            std::cout << "The Connection Failed due to a Socket Error! (ERROR 1)" << std::endl;
            std::cout << "Additional Information may be found in 'logs'" << std::endl;
        } else if (checknetworkconnectivitystart == 2) {
            std::cout << "No Valid Response Received (ERROR 2)" << std::endl;
        } else if (checknetworkconnectivitystart == 3) {
            std::cout << "Server Temporarily Unavailable..." << std::endl;
        }
    }

    // UPDATE COMMAND
    if (command == "update") {
        // FUTURE UPDATE COMMAND
    }

    // SHUTDOWN COMMAND
    if (command == "shutdown") {
        if (useraccesslevel >= 1) {
            stopSIGNAL.store(1);
            std::cout << "THE HONEYPOT IS SHUTTING DOWN NOW!" << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // EXIT TERMINAL BUT KEEP RUNNING IN BACKGROUND
    if (command == "exit") {
        if (useraccesslevel >= 1) {
            std::cout << "Exiting out of shell...\n";
            return;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // OPEN AND READ LOG FILES
    if (command == "logs") {
        if (useraccesslevel >= 1) {
            readfromlogger();
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // ENTER BASH OF SYSTEM
    if (command == "system") {
        if (useraccesslevel >= 1) {
            std::cout << "TRANSFERRING TO DOCKER SUBSYSTEM!" << std::endl;
            std::cout << "Type 'exit' to return to this console" << std::endl;
            sleep(2);
            system("bash2");
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // GENERATE RANDOM STRINGS FOR API TOKENS AND AMONG OTHER THINGS
    if (command == "generate") {
        if (useraccesslevel >= 1) {
            std::string randomstring = generateToken();
            std::cout << "GENERATED TOKEN: " << randomstring << std::endl;
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }

    // LOGOUT OF CONSOLE
    if (command == "logout") {
        std::cout << "Logging out..." << std::endl;
        sleep(1);
        system("clear");
        sleep(1);
        foundcommand = true;
        useraccesslevel = 0;
    }



    // START ANALYZING FIRST WORD IF NOT FOUND
    std::string firstseveral = "";
    std::string firstfour = "";
    if (command.length() >= 8 && foundcommand == false) {
        firstseveral = command.substr(0,8);
        firstfour = command.substr(0,4);
    } else if (command.length() >= 4) {
        firstfour = command.substr(0,4);
    } else {
        firstseveral = command;
        firstfour = command;
    }

    

    // WHICH ENCRYPTION METHOD
    if (firstfour == "menc") {
        if (useraccesslevel >= 1) {
            std::cout << "Analyzing Encrypted String" << std::endl;
            std::string datatoload = command.substr(5, command.length() - 5);
            int results = encryptionmethod(datatoload, 1);
            if (results == 1) {
                std::cout << "HACKSWEEP ENCRYPTION" << std::endl;
            } else if (results == 2) {
                std::cout << "UCRYPT ENCRYPTION" << std::endl;
            } else {
                std::cout << "AN ERROR OCCURRED ANALYZING!" << std::endl;
            }
        } else {
            std::cout << "Sorry, you do not have permissions to perform this action." << std::endl;
        }
        foundcommand = true;
    }


    // MAKE SURE THE COMMAND IS FOUND
    if (foundcommand == false) {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Try 'commands' instead" << std::endl;
    }
}



// MAIN INTERACTIVE TERMINAL COMMAND
void interactiveTerminal() {
    sleep(10);
    system("clear");
    std::cout << "HoneyPi Terminal" << std::endl;
    std::cout << "HoneyPi Server Version: " << honeyversion << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    while (true) {
        std::string command;
        switch (useraccesslevel) {
            case 0:
                std::cout << ">> ";
                break;
            case 1:
                std::cout << "admin >> ";
                break;
        }
        command = terminalinput();
        sendtolog("[CONSOLE] - Received Command: " + command);

        if (command.empty() != true) {
            processCommand(command);
        }
    }
}