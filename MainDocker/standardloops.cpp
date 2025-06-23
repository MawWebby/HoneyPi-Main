#include "globalheaders.h"
#include "standardloops.h"


std::string charactermap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
int serverStatus;
int updating;
int stopping;

// LOGFILE
const char* logfile = "/home/honeypi/log/log.txt";



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

// REFRESH THE DATE
std::string refreshDate() {
    std::string timeclock = timedetector();
    std::string dateformatted = "";
    if (timeclock != "") {
        std::string monthlet = timeclock.substr(4,3);
        if (monthlet == "Jan") {
            dateformatted = "01/";
        } else if (monthlet == "Feb") {
            dateformatted = "02/";
        } else if (monthlet == "Mar") {
            dateformatted = "03/";
        } else if (monthlet == "Apr") {
            dateformatted = "04/";
        } else if (monthlet == "May") {
            dateformatted = "05/";
        } else if (monthlet == "Jun") {
            dateformatted = "06/";
        } else if (monthlet == "Jul") {
            dateformatted = "07/";
        } else if (monthlet == "Aug") {
            dateformatted = "08/";
        } else if (monthlet == "Sep") {
            dateformatted = "09/";
        } else if (monthlet == "Oct") {
            dateformatted = "10/";
        } else if (monthlet == "Nov") {
            dateformatted = "11/";
        } else if (monthlet == "Dec") {
            dateformatted = "12/";
        } else {
            dateformatted = "00/";
        }
    }

    // DAY
    std::string seconddate = timeclock.substr(8,1);
    std::string firstdate = timeclock.substr(9,1);
    if (seconddate == " ") {
        dateformatted = dateformatted + "0";
    } else {
        dateformatted = dateformatted + seconddate;
    }
    dateformatted = dateformatted + firstdate;

    // YEAR
    std::string yearfull = timeclock.substr(12,4);
    dateformatted = dateformatted + "/" + yearfull;

    return dateformatted;
}

// REFRESH THE TIME
std::string refreshTime() {
    std::string timeclock = timedetector();
    std::string firstchar = timeclock.substr(18,1);
    std::string clockfull = "";
    if (firstchar == "") {
        clockfull = "0" + timeclock.substr(19,7);
    } else {
        clockfull = timeclock.substr(18,8);
    }
    return clockfull;
}




////////////////////////////
// Send to Logger Scripts //
////////////////////////////
void sendtolog(std::string data2) {
    serverStatus = serverStarted.load();
    updating = updateSIGNAL.load();
    stopping = stopSIGNAL.load();

    if (serverStatus == false || updating == true || stopping == true || bypassterminal == true) {
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
    updating = updateSIGNAL.load();
    stopping = stopSIGNAL.load(); 

    if (serverStatus == false || updating == true || stopping == true || bypassterminal == true) {
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
    std::string timedet = timedetector();
    data2 = "[DEBUG] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void loginfo(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[INFO ] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logwarning(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[WARN*] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logcritical(std::string data2, bool complete) {
    std::string timedet = timedetector();
    data2 = "[ERRNO] - " + timedet + " - " + data2;
    if (complete == false) {
        sendtologopen(data2);
    } else {
        sendtolog(data2);
    }
}

void logerror(std::string headerdata2, std::string errormessage) {
    std::string timedet = timedetector();
    std::string data2 = "[ERROR] - " + timedet + " - " + headerdata2 + " - " + errormessage;
    sendtolog(data2);
}




/////////////////////////////////
//// READ FROM LOGGER SRIPTS ////
/////////////////////////////////
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




//////////////////////////////////
//// STRING TO INT CONVERSION //// 
//////////////////////////////////
long long int stringtoint(std::string values) {
    long long int valuesdetermine = 0;
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
    if (values.length() > 19) {
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
                logwarning("AN INVALID CHARACTER WAS RECEIVED (STD->INT); THE CHARACTER WAS: " + substringvalue, true);
                return 0;
            }   
        } else {
            completedwhile = true;
        }
        timing9760 = timing9760 + 1;
    }

    return valuesdetermine;
}



//////////////////////////////////
//// INT TO STRING CONVERSION //// 
//////////////////////////////////
std::string inttostring(long long int value) {
    std::string returnvalue = std::to_string(value);
    return returnvalue;
}



/////////////////////////////
// CONVERT STRING TO FLOAT // 
/////////////////////////////
float stringtofloat(std::string input) {
    float returnvalue = 0;

    try {
        returnvalue = std::stof(input); // This will throw an exception
    } catch (const std::invalid_argument& e) {
        logwarning("Invalid Argument: " + input, true);
        return -1;
    } catch (const std::out_of_range& e) {
        logwarning("Out of Range: " + input, true);
        return -1;
    }

    return returnvalue;
}


/////////////////////////////
// CONVERT FLOAT TO STRING //
/////////////////////////////
std::string floattostring(float input) {
    std::string returnvalue = "";
    returnvalue = std::to_string(input);
    return returnvalue;
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






// STANDARD RETURN
// NONE = UPDATE IS BEING APPLIED SUCCESSFULLY
// 0 = NO UPDATE AVAILABLE

// ERROR RETURNS
// -1 = COULD NOT RUN DOCKER DOWNLOAD COMMAND (UPDATE BUDDY)
// -2 = 

int updatedockers() {

    // Download Latest UpdateBuddy
    std::cout << "STARTING UPDATE!" << std::endl;
    std::string downloadupdatebuddy = "docker pull mawwebby/honeypotpi:updatebuddyV1";
    if (system(downloadupdatebuddy.c_str()) != 0) {
        return -1;
    }

    // RUN UpdateBuddy
    std::string startupdatebuddy = "docker run -d -v /var/run/docker.sock:/var/run/docker.sock --name UpdateBuddy mawwebby/honeypotpi:updatebuddyV1";
    if (system(startupdatebuddy.c_str()) != 0) {
        std::string removeupdatebuddy = "docker container rm UpdateBuddy";
        if (system(removeupdatebuddy.c_str()) != 0) {
            return -2;
        } else {
            if (system(startupdatebuddy.c_str()) != 0) {
                return -3;
            }
        }
    }

    return 0;
}
