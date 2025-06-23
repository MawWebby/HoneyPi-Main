// MAIN SIMPLE UPDATE SCRIPT
// Matthew Whitworth (MawWebby)
#include <unistd.h>
#include <string>
#include <iostream>

std::string buddyversion = "1";

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
    std::cout << "UPDATE - ( 40%) - Downloading Newer SSH" << std::endl;
    std::string downloadnewerssh = "docker pull mawwebby/honeypotpi:guestsshv1 &> /dev/null";
    if (system(downloadnewerssh.c_str()) != 0) {
        std::cout << "ERROR - UNABLE TO UPDATE! COULD NOT DOWNLOAD NEWER VERSION OF SSH DOCKER!" << std::endl;
        return -2;
    }


    sleep(5);


    // RESTART MAIN DOCKER CONTAINER
    std::cout << "UPDATE - ( 70%) - Starting New Main Docker Container" << std::endl;
    std::string dockerkillcommand = "docker rm HoneyPiMain";
    std::string dockernewcommand = "docker run -p 9090:9090 -p 11535:11535 -p 63599:63599 -v /var/run/docker.sock:/var/run/docker.sock --name HoneyPiMain -d -it --rm --network=localportnetwork mawwebby/honeypotpi:latest &> /dev/null";
    if (system(dockernewcommand.c_str()) != 0) {
        return -6;
    }


    sleep(2);


    std::cout << "UPDATE - (100%) - UPDATE COMPLETE!" << std::endl;


    sleep(2);


    return 0;
}