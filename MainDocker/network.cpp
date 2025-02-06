// GLOBAL HEADERS
#include "globalheaders.h"
#include "network.h"




// DATA FOR REPORT
std::map<int, std::string> usppairmap;
std::map<int, std::string> pwdpairmap;
std::map<int, std::string> usemap;
std::map<int, std::string> passmap;
bool multipleips = false;
bool singleips = false;
std::map<int, std::string> ipaddrmap;
bool fincomplete = false;
std::map<int, std::string> commandrunmap;
std::map<int, std::string> fileviewmap;
std::map<int, std::string> filechangemap;
std::map<int, std::string> fileeditmap;
int singlemultimode = 0; // 1 - SINGLE; 2- MULTI
std::map<int, std::string> extraoptmap;
int extraopt1 = false;
int extraopt2 = false;
int extraopt3 = false;
int extraopt4 = false;
int extraopt5 = false;
int extraopt6 = false;
bool extraopt7 = false;
bool extraopt8 = false;
bool extraopt9 = false;
bool extraopt10 = false;
bool extraopt11 = false;
std::string sshguestversion = "";
std::string reportVersion = "";
std::map<int, std::string> commandsblockedmap;
std::map<int, std::string> commandsranmap;
bool testreport = false;


// MAPS FOR REPORT
int usppairmapnum = 0;
int pwdpairmapnum = 0;
int usemapnum = 0;
int passmapnum = 0;
int ipaddrmapnum = 0;
int commandrunmapnum = 0;
int fileviewmapnum = 0;
int filechangemapnum = 0;
int fileeditmapnum = 0;
int extraoptmapnum = 0;
int commandsblockedmapnum = 0;
int commandsranmapnum = 0;
std::string tempforpassmap = "";


// SYMBOLS
char doublequote = '"';


// MAP FOR PACKETS TO SEND WITH DATA
int totalnumberofpackets = 0;
std::map<int, std::string> alldataofreport;
int packetsize = 1000;






////////////////////////////////////////////////////
//// SEND PACKETS TO SERVER AND RECORD RESPONSE ////
////////////////////////////////////////////////////
// 0 - NEW CONNECTION
// 1 - TOKENID
// 2 - SEND REPORT TO SERVER
std::string sendtoserver(int packettype, std::string data2) {
    if (packettype == 0) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{\"CONNECTION\", \"NEW\"}";
    } else if (packettype == 1) {

    } else if (packettype == 2) {

    } else if (packettype == 3) {

    } else if (packettype == 4) {

    } else if (packettype == 5) {

    } else {

    }

    // DATA2 CHECK FOR NULL
    if (data2 == "") {
        return "ERROR";
        return "ERROR";
    }

    // CREATE NETWORK SOCKET AT ADDRESS
    const char* server_ip = "honeypi.baselinux.net";
    const int server_port = 11829;
    
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // CHECK FOR RESOLVED HOSTNAME
    if (getaddrinfo(server_ip, nullptr, &hints, &res) != 0) {
        logcritical("Unable to resolve hostname!", true);
        return "ERROR";
    }

    // CONFIGURE PORT
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;
    freeaddrinfo(res);

    // CREATE SOCKET
    int serverUpstream = socket(AF_INET, SOCK_STREAM, 0);

    if (serverUpstream < 0) {
        std::cerr << "Socket creation failed.\n";
        return "ERROR";
    }

    if (connect(serverUpstream, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        close(serverUpstream);
        return "ERROR";
    }

    // PROCESS READ
    //std::cout << "Connected to the server at " << server_ip << ":" << server_port << "\n";
    send(serverUpstream, data2.c_str(), data2.length(), 0);
    char bufferread[4096];
    read(serverUpstream, bufferread, 4096);
    std::string yup = bufferread;
    close(serverUpstream);
    return yup;
}





//////////////////////////////////////
//// CHECK UPSTREAM SERVER STATUS ////
//////////////////////////////////////
int checkserverstatus() {
    std::string yup = sendtoserver(0, "");
    
    // CASE STATEMENTS
    if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 18\n\n{state: available}") {
        loginfo("SERVER - Received Valid Connection...", true);
        return 0;
    } else if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 20\n\n{state: unavailable}") {
        loginfo("SERVER - Server Temporarily Unavailable, Continuing...", true);
        return 3;
    } else if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 17\n\n{state: rejected}") {
        loginfo("SERVER - Received REJECTION!", true);
        return 4;
    } else {
        logcritical("SERVER - RECEIVED NOT RESPONSE FROM SERVER!", true);
        std::cout << "RECEIVED:" << yup << std::endl;
        return 1;
    }
    return 2;
}




//////////////////////////////////
///// CLEAR REPORT FROM MAPS /////
//////////////////////////////////
int clearmaps(bool actually) {
    if (actually == false) {
        return 0;
        return 0;
    } else {
        // CLEAR ALL MAPS AND VARIABLES
        usppairmap.clear();
        pwdpairmap.clear();
        usemap.clear();
        passmap.clear();
        multipleips = false;
        singleips = false;
        ipaddrmap.clear();
        fincomplete = false;
        commandrunmap.clear();
        fileviewmap.clear();
        filechangemap.clear();
        fileeditmap.clear();
        singlemultimode = 0;
        extraoptmap.clear();
        extraopt1 = 0;
        extraopt2 = 0;
        extraopt3 = 0;
        extraopt4 = 0;
        extraopt5 = 0;
        extraopt6 = 0;
        extraopt7 = false;
        extraopt8 = false;
        extraopt9 = false;
        extraopt10 = false;
        extraopt11 = false;
        sshguestversion = "";
        reportVersion = "";
        commandsblockedmap.clear();
        commandsranmap.clear();
        testreport = false;
        usppairmapnum = 0;
        pwdpairmapnum = 0;
        usemapnum = 0;
        passmapnum = 0;
        ipaddrmapnum = 0;
        commandrunmapnum = 0;
        fileviewmapnum = 0;
        filechangemapnum = 0;
        fileeditmapnum = 0;
        extraoptmapnum = 0;
        commandsblockedmapnum = 0;
        commandsranmapnum = 0;
        tempforpassmap = "";
        totalnumberofpackets = 0;
        alldataofreport.clear();
        return 0;
        return 0;
    }
    return -1;
}




///////////////////////////////////
///// CREATE REPORT FROM MAPS /////
///////////////////////////////////
std::string createreport() {
    if (fincomplete != true) {
        return "ERROR";
        return "ERROR";
    }

    std::string currentDate = refreshDate();
    std::string currentTime = refreshTime();

    // COMPILE THE REPORT LINE BY LINE
    std::string stringcreate = "// REPORT";
    stringcreate = stringcreate + "\n\n";
    stringcreate = stringcreate + "// MISC." + "\n";
    stringcreate = stringcreate + "tokenID = " + doublequote;
    stringcreate = stringcreate + tokenID + doublequote + "\n";
    stringcreate = stringcreate + "reportV = 1" + "\n";
    stringcreate = stringcreate + "testflight = " + inttostring(beta) + "\n";
    stringcreate = stringcreate + "testreport = " + inttostring(testreport) + "\n";
    stringcreate = stringcreate + "versionreporting = " + honeyversion + "\n";
    stringcreate = stringcreate + "guestreporting = " + sshguestversion + "\n";
    stringcreate = stringcreate + "date = " + doublequote + currentDate + doublequote + "\n";
    stringcreate = stringcreate + "time = " + doublequote + currentTime + doublequote + "\n";
    stringcreate = stringcreate + "method = SSH" + "\n";
    stringcreate = stringcreate + "partofDDOS = " + inttostring(multipleips) + "\n";
    stringcreate = stringcreate + "complete = " + inttostring(fincomplete) + "\n";
    stringcreate = stringcreate + "option1 = " + inttostring(extraopt1) + "\n";
    stringcreate = stringcreate + "option2 = " + inttostring(extraopt2) + "\n";
    stringcreate = stringcreate + "option3 = " + inttostring(extraopt3) + "\n";
    stringcreate = stringcreate + "option4 = " + inttostring(extraopt4) + "\n";
    stringcreate = stringcreate + "option5 = " + inttostring(extraopt5) + "\n";
    stringcreate = stringcreate + "option6 = " + inttostring(extraopt6) + "\n";
    stringcreate = stringcreate + "option7 = " + inttostring(extraopt7) + "\n";
    stringcreate = stringcreate + "option8 = " + inttostring(extraopt8) + "\n";
    stringcreate = stringcreate + "option9 = " + inttostring(extraopt9) + "\n";
    stringcreate = stringcreate + "option10 = " + inttostring(extraopt10) + "\n";
    stringcreate = stringcreate + "option11 = " + inttostring(extraopt11) + "\n";
    stringcreate = stringcreate + "\n\n";
    stringcreate = stringcreate + "//////////////////////////" + "\n";
    stringcreate = stringcreate + "// ATTEMPT COMBINATIONS //" + "\n";
    stringcreate = stringcreate + "usercombo = {" + "\n";
    if (usppairmapnum != 0 && pwdpairmapnum != 0) {
        int ordertwo = 0;
        while (ordertwo < usppairmapnum || ordertwo < pwdpairmapnum) {
            stringcreate = stringcreate + "(" + usppairmap[ordertwo] + ";[$]: " + pwdpairmap[ordertwo] + ")" + "\n";
            ordertwo = ordertwo + 1;
        }
    } else if (usppairmapnum != 0 && pwdpairmapnum == 0) {
        usemap[usemapnum] = usppairmap[usppairmapnum];
        usemapnum = usemapnum + 1;
    } else if (usppairmapnum == 0 && pwdpairmapnum != 0) {
        passmap[passmapnum] = pwdpairmap[pwdpairmapnum];
        passmapnum = passmapnum + 1;
    }
    stringcreate = stringcreate + "}" + "\n\n";
    stringcreate = stringcreate + "//////////////////////" + "\n";
    stringcreate = stringcreate + "// TERMINAL PROCESS //" + "\n";
    stringcreate = stringcreate + "commandprocess = {" + "\n";
    if (commandrunmapnum != 0) {
        int commandoption = 0;
        while (commandoption < commandrunmapnum) {
            stringcreate = stringcreate + doublequote + commandrunmap[commandoption] + doublequote + "\n";
            commandoption = commandoption + 1;
        }
    }
    stringcreate = stringcreate + "}" + "\n\n";
    stringcreate = stringcreate + "////////////////////" + "\n"; 
    stringcreate = stringcreate + "// FILES AFFECTED //" + "\n";
    stringcreate = stringcreate + "files = {" + "\n";
    if (fileviewmapnum != 0) {
        int filevieworder = 0;
        while (filevieworder < fileviewmapnum) {
            stringcreate = stringcreate + doublequote + fileviewmap[filevieworder] + doublequote + "\n";
            filevieworder = filevieworder + 1;   
        }
    }
    stringcreate = stringcreate + "}" + "\n\n";
    stringcreate = stringcreate + "//////////////////////////" + "\n";
    stringcreate = stringcreate + "// FILECHANGES AFFECTED //" + "\n";
    stringcreate = stringcreate + "filechanges = {" + "\n";
    if (fileeditmapnum != 0) {
        int fileeditorder = 0;
        while (fileeditorder < fileeditmapnum) {
            stringcreate = stringcreate + doublequote + fileeditmap[fileeditorder] + doublequote + "\n";
            fileeditorder = fileeditorder + 1;
        }
    }
    stringcreate = stringcreate + "}" + "\n\n";
    stringcreate = stringcreate + "////////////////////" + "\n";
    stringcreate = stringcreate + "// ALL IPS PINGED //" + "\n";
    stringcreate = stringcreate + "ipaddr = {" + "\n";
    if (ipaddrmapnum != 0) {
        int ipaddroption = 0;
        while (ipaddroption < ipaddrmapnum) {
            stringcreate = stringcreate + doublequote + ipaddrmap[ipaddroption] + doublequote + "\n";
            ipaddroption = ipaddroption + 1;
        }
    }
    stringcreate = stringcreate + "}" + "\n\n";
    stringcreate = stringcreate + "/////////////" + "\n";
    stringcreate = stringcreate + "// option1 //" + "\n";
    stringcreate = stringcreate + "extraopt = {" + "\n";
    if (extraoptmapnum != 0) {
        int runningoption = 0;
        while (runningoption < extraoptmapnum) {
            stringcreate = stringcreate + doublequote + extraoptmap[runningoption] + doublequote + "\n";
            runningoption = runningoption + 1;
        }
    }
    stringcreate = stringcreate + "}" + "\n\n";
    stringcreate = stringcreate + "END";

    return stringcreate;
}




////////////////////////////////////////////////////////
//// SPLIT REPORT INTO MULTIPLE PACKETS FOR SENDING ////
////////////////////////////////////////////////////////
int splitreportintopack(std::string data1) {
    if (data1 == "") {
        // NULL STRING RECEIVED/IGNORE
        return -1;
        return -1;
    }

    alldataofreport.clear();
    totalnumberofpackets = 0;

    if (data1.length() <= packetsize) {
        alldataofreport[totalnumberofpackets] = data1;
        totalnumberofpackets = totalnumberofpackets + 1;
    } else if (data1.length() > packetsize) {
        int multiplier = 0;
        while (data1.length() > (multiplier + 1) * packetsize) {
            alldataofreport[multiplier] = data1.substr((multiplier * packetsize), packetsize);
            multiplier = multiplier + 1;
            totalnumberofpackets = totalnumberofpackets + 1;
        }
        alldataofreport[multiplier] = data1.substr((multiplier * packetsize), (data1.length() - (multiplier * packetsize)));
        totalnumberofpackets = totalnumberofpackets + 1;
        return 0;
    } else {
        return -2;
        return -2;
    }
    return totalnumberofpackets;
}




///////////////////////////////////
///// CREATE REPORT FROM MAPS /////
///////////////////////////////////
int sendpacketreports(int splittingreport) {
    if (splittingreport != 0) {
        int currentsend = 0;
        while (currentsend <= splittingreport) {
            std::string partstr = alldataofreport[currentsend];

            currentsend = currentsend + 1;
        }
    } else {
        return -1;
        return -1;
    }
    return -2;
}



/////////////////////////////////////////
// RECEIVE FROM SSH/SEND TO PACKET MAP //
/////////////////////////////////////////
// 0 - US1 - USERNAMES/PASSWORD COMBO
// 33 - US2 - USERNAMES/PASSWORD COMBO.2
// 1 - USE - USERNAME
// 2 - PWD - PASSWORD
// 3 - MUP - MULTIPLEIP
// 4 - SUP - SINGLEIP
// 5 - ADD - IPADDR
// 6 - FIN - COMPLETE
// 7 - CMD - COMMANDRUN
// 8 - FVW - FILEVIEW
// 9 - FCH - FILECHANGES
// 10 - FED - FILEEDITS
// 11 - SMM - SINGLE/MULTI MODE
// 12 - EXA - EXTRAOPT ARRAY
// 13 - EX1 - EXTRAOPT1
// 14 - EX2 - EXTRAOPT2
// 15 - EX3 - EXTRAOPT3
// 16 - EX4 - EXTRAOPT4
// 17 - EX5 - EXTRAOPT5
// 18 - EX6 - EXTRAOPT6
// 19 - EX7 - EXTRAOPT7
// 20 - EX8 - EXTRAOPT8
// 21 - EX9 - EXTRAOPT9
// 22 - E10 - EXTRAOPT10
// 23 - E11 - EXTRAOPT11
// 24 - SOF - GUEST REPORT
// 25 - ROP - reportV
// 26 - CMB - COMMAND BLOCK
// 27 - CMR - COMMAND RUN
// 28 - KIL - TERMINATE SESSION
// 29 - WAT - MAYDAY WATCHDOG KILL
// 30 - MUR - BLOCK ALL CONTAINERS TO KILL (ACTIVE TARGET OF ATTACK)
// 31 - TES - TEST
// 32 - FTE - VERIFY TEST
int reportreceiveSSH(std::string data1) {
    std::string header1 = "";
    if (data1.length() >= 5) {
        header1 = data1.substr(0,3);
    } else {
        return -2;
    }

    if (beta == true) {
        loginfo("HEAD" + header1, true);
    }
    
    // MAIN CONVERSION TYPE
    if (header1 == "US1") {
        std::string users123 = data1.substr(5, data1.length() - 5);
        usppairmap[usppairmapnum] = users123;
        usppairmapnum = usppairmapnum + 1;
    } else if (header1 == "US2") {
        std::string passcombo = data1.substr(5, data1.length() - 5);
        pwdpairmap[pwdpairmapnum] = passcombo;
        pwdpairmapnum = pwdpairmapnum + 1;
    } else if (header1 == "USE") {
        std::string username = data1.substr(5, data1.length() - 5);
        usemap[usemapnum] = username;
        usemapnum = usemapnum + 1;
    } else if (header1 == "PWD") {
        std::string password = data1.substr(5, data1.length() - 5);
        passmap[passmapnum] = password;
        passmapnum = passmapnum + 1;
    } else if (header1 == "MUP") {
        std::string multiIP = data1.substr(5, data1.length() - 5);
        if (multiIP == "false") {
            multipleips = false;
        } else {
            multipleips = true;
        }
    } else if (header1 == "SUP") {
        std::string singleIP = data1.substr(5, data1.length() - 5);
        if (singleIP == "false") {
            singleips = false;
        } else {
            singleips = true;
        }
    } else if (header1 == "ADD") {
        std::string ipaddr = data1.substr(5, data1.length() - 5);
        ipaddrmap[ipaddrmapnum] = ipaddr;
        ipaddrmapnum = ipaddrmapnum + 1;
    } else if (header1 == "FIN") {
        std::string completionstatus = data1.substr(5, data1.length() - 5);
        if (completionstatus == "true") {
            loginfo("Creating Report...", true);
            fincomplete = true;
            std::string compiledreport = createreport();
            std::cout << "REPORT" << std::endl;
            std::string encryptstring = ucrypt_Ecrypt(compiledreport);
            int resultantvec = splitreportintopack(encryptstring);
            std::cout << compiledreport << std::endl;
            loginfo("Finished Compiling Report", true);
            loginfo("Sending Report to Server", true);
            int returnvalue = sendpacketreports(resultantvec);
            if (returnvalue == 0) {
                loginfo("Completed Sending Report to Server!", true);
                loginfo("Removing and Recreating SSH Container", true);
                system(dockerkillguestssh);
                // system start ssh container

            }
            
            
            
            // SEND REPORT TO SERVER







        } else {
            fincomplete = false;
        }
    } else if (header1 == "CMD") {
        std::string commandran = data1.substr(5, data1.length() - 5);
        commandrunmap[commandrunmapnum] = commandran;
        commandrunmapnum = commandrunmapnum + 1;
    } else if (header1 == "FVW") {
        std::string fileviewer = data1.substr(5, data1.length() - 5);
        fileviewmap[fileviewmapnum] = fileviewer;
        fileviewmapnum = fileviewmapnum + 1;
    } else if (header1 == "FCH") {
        std::string filechanger = data1.substr(5, data1.length() - 5);
        filechangemap[filechangemapnum] = filechanger;
        filechangemapnum = filechangemapnum + 1;
    } else if (header1 == "FED") {
        std::string fileediter = data1.substr(5, data1.length() - 5);
        fileeditmap[fileeditmapnum] = fileediter;
        fileeditmapnum = fileeditmapnum + 1;
    } else if (header1 == "SMM") {
        std::string singlemultimodestr = data1.substr(5, data1.length() - 5);
        if (singlemultimodestr == "1") {
            singlemultimode = 1;
        } else if (singlemultimodestr == "2") {
            singlemultimode = 2;
        } else {
            singlemultimode = 0;
        }
    } else if (header1 == "EXA") {
        std::string extraoptarr = data1.substr(5, data1.length() - 5);
        extraoptmap[extraoptmapnum] = extraoptarr;
        extraoptmapnum = extraoptmapnum = 1;
    } else if (header1 == "EX1") {
        std::string exop = data1.substr(5, data1.length() - 5);
        extraopt1 = stringtoint(exop);
    } else if (header1 == "EX2") {
        std::string exop = data1.substr(5, data1.length() - 5);
        extraopt2 = stringtoint(exop);
    } else if (header1 == "EX3") {
        std::string exop = data1.substr(5, data1.length() - 5);
        extraopt3 = stringtoint(exop);
    } else if (header1 == "EX4") {
        std::string exop = data1.substr(5, data1.length() - 5);
        extraopt4 = stringtoint(exop);
    } else if (header1 == "EX5") {
        std::string exop = data1.substr(5, data1.length() - 5);
        extraopt5 = stringtoint(exop);
    } else if (header1 == "EX6") {
        std::string exop = data1.substr(5, data1.length() - 5);
        extraopt6 = stringtoint(exop);
    } else if (header1 == "EX7") {
        std::string exop = data1.substr(5, data1.length() - 5);
        if (exop == "true") {
            extraopt7 = true;
        } else {
            extraopt7 = false;
        }
    } else if (header1 == "EX8") {
        std::string exop = data1.substr(5, data1.length() - 5);
        if (exop == "true") {
            extraopt8 = true;
        } else {
            extraopt8 = false;
        }
    } else if (header1 == "EX9") {
        std::string exop = data1.substr(5, data1.length() - 5);
        if (exop == "true") {
            extraopt9 = true;
        } else {
            extraopt9 = false;
        }
    } else if (header1 == "E10") {
        std::string exop = data1.substr(5, data1.length() - 5);
        if (exop == "true") {
            extraopt10 = true;
        } else {
            extraopt10 = false;
        }
    } else if (header1 == "E11") {
        std::string exop = data1.substr(5, data1.length() - 5);
        if (exop == "true") {
            extraopt11 = true;
        } else {
            extraopt11 = false;
        }
    } else if (header1 == "SOF") {
        std::string guestversion = data1.substr(5, data1.length() - 5);
        sshguestversion = guestversion;
    } else if (header1 == "ROP") {
        std::string reportversion = data1.substr(5, data1.length() - 5);
        reportVersion = reportversion;
    } else if (header1 == "CMB") {
        std::string commandsblocked = data1.substr(5, data1.length() - 5);
        commandsblockedmap[commandsblockedmapnum] = commandsblocked;
        commandsblockedmapnum = commandsblockedmapnum + 1;
    } else if (header1 == "CMR") {
        std::string commandrans = data1.substr(5, data1.length() - 5);
        commandsranmap[commandsranmapnum] = commandrans;
        commandsranmapnum = commandsranmapnum + 1;
    } else if (header1 == "KIL") {
        // STOP SSH DOCKER CONTAINER
        std::string terminatedsessions = data1.substr(5, data1.length() - 5);
        std::string watchdogkillcontainer = data1.substr(5, data1.length() - 5);
        if (watchdogkillcontainer == "false") {
            logwarning("Received Packet to Stop Container but packet said false?", true);
        } else if (watchdogkillcontainer == "test") {
            // CONTINUE (EDGE CASE)
        } else {
            int killguest = system(dockerkillguestssh);
        }
    } else if (header1 == "WAT") {
        // MAYDAY WATCHDOG KILL!!!
        std::string watchdogkillcontainer = data1.substr(5, data1.length() - 5);
        if (watchdogkillcontainer == "false") {
            logwarning("Received Packet to Kill Container but packet said false?", true);
        } else if (watchdogkillcontainer == "test") {
            // CONTINUE (EDGE CASE)
        } else {
            logcritical("RECEIVED COMMAND TO TERMINATE SSH SESSION!!!", true);
            logcritical("REMOVING ALL CONTAINERS AND CLOSING ALL PORTS!!!", true);
            logcritical("THIS IS CRITICAL!!!", true);
            std::cout << "RECEIVED COMMAND TO TERMINATE ALL SSH SESSIONS!!!" << std::endl;
            std::cout << "REMOVING ALL CONTAINERS AND CLOSING ALL PORTS!!!" << std::endl;
            std::cout << "THIS IS CRITICAL!!!" << std::endl;

            int killguest = system(dockerkillguestssh);
            if (killguest != 0) {
                int killswitch = system("docker kill *");
                if (killswitch != 0) {
                    int shutdown = system("sudo shutdown now");
                    if (shutdown != 0) {
                        std::cout << "ERROR! (Setting Signal True)" << std::endl;
                        stopSIGNAL.store(1);
                    } else {
                        std::cout << "Success in Stopping Termination (3)" << std::endl;
                    }
                } else {
                    std::cout << "Success in Stopping Termination (2)" << std::endl;
                }
            } else {
                std::cout << "Sucess in Stopping Termination (1)" << std::endl;
            }
        }
    } else if (header1 == "MUR") {
        std::string killallcontainers = data1.substr(5, data1.length() - 5);
        
    } else if (header1 == "TES") {
        std::string testerport = data1.substr(5, data1.length() - 5);
        if (testerport == "true" || testerport == "test") {
            testreport = true;
        } else {
            testreport = false;
        }
    } else if (header1 == "FTE") {
        std::string finishtestreport = data1.substr(5, data1.length() - 5);
        if (finishtestreport == "true" || finishtestreport == "test") {
            testreport = true;
        } else {
            testreport = false;
        }
    } else {
        logcritical("RECEIVED INVALID PACKET FROM SSH", true);
        logcritical("RECEIVED: " + data1, true);
        return -4;
    }
    return 0;
}







