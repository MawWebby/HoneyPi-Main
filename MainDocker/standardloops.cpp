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




///////////////////////////////////////
// READ CONFIG FILE AND PRINT OUTPUT //
///////////////////////////////////////
// ERROR RETURNS
// [0] = "ERROR"
// [1] = error of some type

// NORMAL RETURNS 
// [0] = Valid Config (True / ERROR)
// [1] = Config File Location (STRING)
// [2] = Auto-Update (1 / 0)
// [3] = Check-Updates (Time in Hours)
// [4] = Channel-Updates (Stable / Beta / Dev)
// [5] = SSH-Guest (1 / 0)
// [6] = Docker Sock Location (STRING)
// [7] = Debug Mode (1 / 0)
// [8] = API KEY (STRING)
// [9] = Main Log Location (STRING / "INTERNAL")
// [10] = SSH Log (STRING / "INTERNAL")
std::map<int, std::string> readconfig() {
    std::map<int, std::string> errormap;
    std::map<int, std::string> correctmap;
    errormap[0] = "ERROR";

    std::ifstream configfile;
    std::string configlocation = "/home/config.txt";
    configfile.open(configlocation.c_str());
    if (configfile.is_open() != true) {
        errormap[1] = "Could Not Open Config File!";
        return errormap;
    }

    std::string newline = "";
    bool completion21 = false;
    bool validheader = false;
    bool valid0 = false;
    bool valid1 = false;
    bool valid2 = false;
    bool valid3 = false;
    bool valid4 = false;
    bool valid5 = false;
    bool valid6 = false;
    bool valid7 = false;
    bool valid8 = false;
    bool valid9 = false;
    bool valid10 = false;

    // MAIN READING LOOP
    while (completion21 == false) {
        if (configfile.eof() == true) {
            completion21 = true;
        }
        getline(configfile, newline);
        if (newline.length() >= 17) {
            // CHECK FOR COMMENT
            if (newline.substr(0,2) != "//" && newline.substr(0,1) != "#") {
                // DETERMINE IF VALID CASE CONDITION
                if (newline.length() > 16) {

                    // HEADER
                    if (newline.substr(0,16) == "HoneyPi Config v") {
                        if (newline.substr(16,2) != "1:") {
                            errormap[1] = "Invalid Version of Config File!";
                            return errormap;
                        } else {
                            validheader = true;
                            correctmap[0] = "True";
                        }
                    }

                    // CONFIG FILE LOCATION
                    else if (newline.substr(0, 17) == " - Config File Lo") {
                        if (newline.length() > 23) {
                            correctmap[1] = newline.substr(26, newline.length() - 26);
                            valid0 = true;
                        } else {
                            errormap[1] = "Config File Location too short";
                        }
                    }

                    // AUTO UPDATE
                    else if (newline.substr(0,17) == " - Auto-Update = ") {
                        if (newline.substr(17,2) == "Ye") {
                            correctmap[2] = "1";
                            valid1 = true;
                        } else if (newline.substr(17,2) == "No") {
                            correctmap[2] = "0";
                            valid1 = true;
                        } else {
                            errormap[1] = "Invalid Case Received (Auto-Update)";
                        }
                    }

                    // Check for Updates
                    else if (newline.substr(0,17) == " - Check-Updates ") {
                        if (newline.substr(19) == "1 week") {
                            correctmap[3] = "164";
                            valid2 = true;
                        } else if (newline.substr(19) == "1 day") {
                            correctmap[3] = "24";
                            valid2 = true;
                        } else if (newline.substr(19) == "1 hour") {
                            correctmap[3] = "1";
                            valid2 = true;
                        } else {
                            errormap[1] = "Invalid Time Received (Check Update)";
                        }
                    }

                    // Update Channel
                    else if (newline.substr(0,17) == " - Channel-Update") {
                        if (newline.substr(21) == "Stable") {
                            correctmap[4] = "Stable";
                            valid3 = true;
                        } else if (newline.substr(21) == "Beta") {
                            correctmap[4] = "Beta";
                            valid3 = true;
                        } else if (newline.substr(21) == "Dev") {
                            correctmap[4] = "Dev";
                            valid3 = true;
                        } else {
                            errormap[1] = "Invalid Update Channel";
                        }
                    }

                    // Start SSH Guest
                    else if (newline.substr(0,17) == " - SSH-Guest = Ye") {
                        correctmap[5] = "1";
                        valid4 = true;
                    } 
                    else if (newline.substr(0,17) == " - SSH-Guest = No") {
                        correctmap[5] = "0";
                        valid4 = true;
                    }

                    // Docker Sock Location
                    else if (newline.substr(0,17) == " - Docker Sock Lo") {
                        if (newline.length() > 36) {
                            correctmap[6] = newline.substr(26);
                            valid5 = true;
                        } else {
                            errormap[1] = "Invalid Docker Sock Location Received";
                        }
                    }

                    // Debug Mode Enabled
                    else if (newline.substr(0,17) == " - DebugMode = Ye") {
                        correctmap[7] = "1";
                        valid6 = true;
                    }
                    else if (newline.substr(0,17) == " - DebugMode = No") {
                        correctmap[7] = "0";
                        valid6 = true;
                    }

                    // API Key
                    else if (newline.substr(0,13) == " - API KEY = ") {
                        correctmap[8] = newline.substr(13, newline.length() - 13);
                        valid7 = true;
                    }

                    // Log Location
                    else if (newline.substr(0,17) == " - Log Location =") {
                        if (newline.substr(18,3) == "INT") {
                            correctmap[9] = "0";
                            valid8 = true;
                        } else {
                            correctmap[9] = newline.substr(18, newline.length() - 18);
                            valid8 = true;
                        }
                    }
                    
                    // SSH Debug Log
                    else if (newline.substr(0,17) == " - SSH Debug Log ") {
                        if (newline.substr(19,3) == "INT") {
                            correctmap[10] = "0";
                            valid9 = true;
                        } else {
                            correctmap[10] = newline.substr(19, newline.length() - 19);
                            valid9 = true;
                        }
                    }
                }
            }
        }

    }

    configfile.close();
    if (validheader == true && valid0 == true && valid1 == true && valid2 == true && valid3 == true && valid4 == true && valid5 == true && valid6 == true && valid7 == true && valid8 == true && valid9 == true) {
        return correctmap;
    } else {
        logwarning("End of File Received in Trying to Review Config File!", true);
        if (errormap[1] == "") {
            errormap[1] = "EOF Received in Config File!";
        }
        
        return errormap;
    }
    errormap[1] = "Skipped If Statement";
    return errormap;
}



// FIX THIS
// STANDARD RETURN
// 1 = UPDATE IS BEING APPLIED SUCCESSFULLY
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
    std::map<int, std::string> configcontents = readconfig();
    if (configcontents[0] == "ERROR") {
        logwarning("Found Error in Config File, COULD NOT CONTINUE!", true);
        return -4;
    }
    std::string dockersocklocation = configcontents[6];
    std::string conflocation = configcontents[1];

    std::string startupdatebuddy = "docker run -d -v " + dockersocklocation + ":/var/run/docker.sock -v " + conflocation + ":/home/config.txt --name UpdateBuddy mawwebby/honeypotpi:updatebuddyV1";
    std::cout << startupdatebuddy << std::endl;
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
    updateSIGNAL.store(1);
    return 1;
}
