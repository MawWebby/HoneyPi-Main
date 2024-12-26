#include "globalheaders.h"
#include "standardloops.h"


std::string charactermap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
int serverStatus;
int updating;
int stopping;

// LOGFILE
const char* logfile = "/home/honeypi/log/log.txt";



////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    serverStatus = serverStarted.load();
    stopping = stopSIGNAL.load();
    updating = updateSIGNAL.load();

    if (serverStatus == 0 || stopping == 1 || updating == 1) {
        std::cout << data2 << std::endl;
    }
    std::ofstream logfilestream;
    logfilestream.open(logfile, std::ofstream::out | std::ofstream::app);
    if (logfilestream.is_open() == true) {
        logfilestream << data2 << std::endl;
    } else {
        std::cout << "[ERRO] - COULD NOT SAVE TO LOGFILE!" << std::endl;
        std::cout << "[ERRO] - MESSAGE: " << data2 << std::endl;
    }
    logfilestream.close();
}

void sendtologopen(std::string data2) {
    serverStatus = serverStarted.load();
    stopping = stopSIGNAL.load();
    updating = updateSIGNAL.load();
    
    if (serverStatus == 0 || stopping == 1 || updating == 1) {
        std::cout << data2;
    }
    std::ofstream logfilestream;
    logfilestream.open(logfile, std::ofstream::out | std::ofstream::app);
    if (logfilestream.is_open() == true) {
        logfilestream << data2;
    } else {
        std::cout << "[ERRO] - COULD NOT SAVE TO LOGFILE!" << std::endl;
        std::cout << "[ERRO] - MESSAGE: " << data2 << std::endl;
    }
    logfilestream.close();
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





//////////////////////////////
// READ FROM Logger Scripts //
//////////////////////////////
void readfromlogger() {
    std::ifstream logfilestream;
    logfilestream.open(logfile);
    if (logfilestream.is_open() == true) {
        char linebits[2048];
        while (logfilestream.eof() == false) {
            logfilestream.getline(linebits, 2048);
            std::cout << linebits << std::endl;
        }
        std::cout << std::endl << "Reached End of Log File" << std::endl;
    } else {
        std::cout << "[ERRO] - COULD NOT OPEN LOGFILE!" << std::endl;
    }
    logfilestream.close();
}




//////////////////////
//// TIMER SCRIPT ////
//////////////////////
std::string timedetector() {
    time_t time10 = time(0); 
    std::string dateandtime = ctime(&time10);
    std::string datestd = dateandtime.substr(0,10);
    std::string clockstd = dateandtime.substr(10, 9);
    std::string yearstd = dateandtime.substr(20,4);
    std::string newformat = datestd + ", " + yearstd + " " + clockstd;
    return newformat;
}




//////////////////////////////////
//// STRING TO INT CONVERSION //// 
//////////////////////////////////
int stringtoint(std::string values) {
    long int valuesdetermine = 0;
    bool completedwhile = false;
    int timing9760 = 0;
    int timing9760max = values.length();
    bool validcase = false;
    bool periodcheck = false;
    
    // CHECK FOR NULL
    if (values.length() == 0) {
        loginfo("RECEIVED NULL STRING TO ANALYZE! (STD->INT)", true);
        return 0;
    }

    // CHECK FOR LONG INT
    if (values.length() > 9) {
        loginfo("RECEIVED A SUPER LONG STRING TO ANALYZE! (STD->INT)", true);
        return 0;
    }

    // MAIN CHECK LOOP
    std::string substringvalue = "";
    while (completedwhile == false && timing9760 <= timing9760max) {
        if (timing9760 + 1 <= timing9760max) {
            validcase = false;
            if (periodcheck == false) {
                valuesdetermine = valuesdetermine * 10;
                periodcheck = false;
            } else {
                periodcheck = false;;
            }
            substringvalue = values.substr(timing9760, 1);
            if (substringvalue == "0") {
                validcase = true;
            } else if (substringvalue == "1") {
                validcase = true;
                valuesdetermine = valuesdetermine + 1;
            } else if (substringvalue == "2") {
                validcase = true;
                valuesdetermine = valuesdetermine + 2;
            } else if (substringvalue == "3") {
                validcase = true;
                valuesdetermine = valuesdetermine + 3;
            } else if (substringvalue == "4") {
                validcase = true;
                valuesdetermine = valuesdetermine + 4;
            } else if (substringvalue == "5") {
                validcase = true;
                valuesdetermine = valuesdetermine + 5;
            } else if (substringvalue == "6") {
                validcase = true;
                valuesdetermine = valuesdetermine + 6;
            } else if (substringvalue == "7") {
                validcase = true;
                valuesdetermine = valuesdetermine + 7;
            } else if (substringvalue == "8") {
                validcase = true;
                valuesdetermine = valuesdetermine + 8;
            } else if (substringvalue == "9") {
                validcase = true;
                valuesdetermine = valuesdetermine + 9;
            }


            // DECIMALS (IGNORE THIS CASE)
            // FIX THIS
            else if (substringvalue == ".") {
                validcase = true;
                periodcheck = true;
            }


            // INVALID STRING
            if (validcase != true) {
                loginfo("AN INVALID CHARACTER WAS RECEIVED (STD->INT); THE CHARACTER WAS: ", false);
                sendtolog(substringvalue);
                completedwhile = true;
                return 0;
            }   
        } else {
            completedwhile == true;
        }
        timing9760 = timing9760 + 1;
    }

    return valuesdetermine;
}


/////////////////////////////////////
//// GENERATE API RANDOM STRINGS ////
/////////////////////////////////////
std::string generateToken() {

    // Define the list of possible characters
    const std::string CHARACTERS = charactermap;

    // Create a random number generator
    std::random_device rd;
    std::mt19937 generator(rd());

    // Create a distribution to uniformly select from all
    // characters
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    // Generate the random string
    std::string random_string = "PI";
    for (int i = 0; i < 62; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}


//////////////////////
//// PING NETWORK ////
//////////////////////
int pingnetwork() {
    return system("ping -c 5 8.8.8.8 > nul:");
}