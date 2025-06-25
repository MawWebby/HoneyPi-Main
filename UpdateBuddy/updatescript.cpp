// MAIN SIMPLE UPDATE SCRIPT
// Matthew Whitworth (MawWebby)
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

std::string buddyversion = "1";





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
        std::cout << "End of File Received in Trying to Review Config File!" << std::endl;
        if (errormap[1] == "") {
            errormap[1] = "EOF Received in Config File!";
        }
        
        return errormap;
    }
    errormap[1] = "Skipped If Statement";
    return errormap;
}






// MADE FOR VERSIONS 0.X - 1.X
// NORMAL RETURN VALUE
// 0 => COMPLETED SUCCESSFULLY

// ERROR RETURN VALUES
// -1 => Download Failed (Main)
// -2 => Download Failed (SSH)
// -3 => Restart Failed (Main)

int main() {
    std::cout << "UPDATING MAIN HONEYPOT TO NEW VERSION" << std::endl;
    std::cout << "UpdateBuddyV" << buddyversion << std::endl;
    std::cout << "UPDATE - (  0%) - Waiting 15 seconds before continuing..." << std::endl;
    sleep(15);
    std::string systemcapable = "docker container stop HoneyPiMain && docker container rm HoneyPiMain &> /dev/null";


    // Stop Main Honeypot
    std::cout << "UPDATE - (  0%) - Stopping Main Honeypot" << std::endl;
    std::string stopmain = "docker stop HoneyPiMain &> /dev/null";
    if (system(stopmain.c_str()) != 0) {
        return -4;
    }


    // Stop Main Honeypot
    std::cout << "UPDATE - (  0%) - Stopping Guest SSH" << std::endl;
    std::string stopssh = "docker stop SSHVMV1 &> /dev/null";
    if (system(stopssh.c_str()) != 0) {
        return -5;
    }


    sleep(15);


    // DOWNLOADING NEW MAIN CONTAINER
    std::cout << "UPDATE - ( 10%) - Downloading New Main Docker" << std::endl;
    std::string downloadnewermaindocker = "docker pull mawwebby/honeypotpi:latest &> /dev/null";
    if (system(downloadnewermaindocker.c_str()) != 0) {
        std::cout << "ERROR - UNABLE TO UPDATE! COULD NOT DOWNLOAD NEWER VERSION OF MAIN DOCKER!" << std::endl;
        return -1;
    }



    // Downloading New SSH Container
    std::cout << "UPDATE - ( 30%) - Downloading Newer SSH" << std::endl;
    std::string downloadnewerssh = "docker pull mawwebby/honeypotpi:guestsshv1 &> /dev/null";
    if (system(downloadnewerssh.c_str()) != 0) {
        std::cout << "ERROR - UNABLE TO UPDATE! COULD NOT DOWNLOAD NEWER VERSION OF SSH DOCKER!" << std::endl;
        return -2;
    }


    sleep(10);


    // CHECK FOR CONFIG FILE
    std::cout << "UPDATE - ( 40%) - Checking for Config File" << std::endl;
    std::ifstream basisconfig;
    std::string inputlocation = "/home/config.txt";
    basisconfig.open(inputlocation.c_str());
    if (basisconfig.is_open() != true) {
        std::cout << "UPDATE - ( 50%) - No Config File Found, Creating New..." << std::endl;
        std::string createnewconfig = "cp /usr/src/honeypi/defaultconfig.txt /home/config.txt";
        system(createnewconfig.c_str());
    } else {
        std::cout << "UPDATE - ( 50%) - Config File Found" << std::endl;        
    }
    basisconfig.close();


    // CHECK CONTENTS OF CONFIG FILE
    std::cout << "UPDATE - ( 60%) - Checking Contents in Config File" << std::endl;
    std::map<int, std::string> configcontents = readconfig();
    if (configcontents[0] == "ERROR") {
        std::cout << "FOUND INVALID IN CONFIG FILE!" << std::endl;
        std::cout << "ERROR LOGGED: " << configcontents[1] << std::endl;
        sleep(1);
        std::cout << "Creating New File with Defaults to Replace!" << std::endl;
        std::cout << "Printing Previous to Console:" << std::endl;
        std::string catcommand = "cat /home/config.txt";
        system(catcommand.c_str());
        std::string rmcommand = "rm /home/config.txt";
        std::string createnewconfig = "cp /usr/src/honeypi/defaultconfig.txt /home/config.txt";
        system(rmcommand.c_str());
        system(createnewconfig.c_str());
    }


    // RESTART MAIN DOCKER CONTAINER
    std::cout << "UPDATE - ( 70%) - Starting New Main Docker Container" << std::endl;
    std::string dockerkillcommand = "docker rm HoneyPiMain";
    std::string dockersocklocation = configcontents[6];
    std::string conflocation = configcontents[1];
    std::string dockernewcommand = "docker run -p 9090:9090 -p 11535:11535 -p 63599:63599 -v " + dockersocklocation + ":/var/run/docker.sock -v " + conflocation + ":/home/config.txt --name HoneyPiMain -d -it --rm --network=localportnetwork mawwebby/honeypotpi:latest &> /dev/null";
    if (system(dockernewcommand.c_str()) != 0) {
        return -6;
    }


    sleep(2);


    std::cout << "UPDATE - (100%) - UPDATE COMPLETE!" << std::endl;


    sleep(2);


    return 0;
}