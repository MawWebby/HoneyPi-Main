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
std::string doublequote = "\"";


// MAP FOR PACKETS TO SEND WITH DATA
int totalnumberofpackets = 0;
std::map<int, std::string> alldataofreport;
int packetsize = 1000;

// HAPI REQUESTS/RESPONSES
const std::string startreporting = "{" + doublequote + "STATE" + doublequote + ":" + doublequote + "START_REPORT" + doublequote + "}";
const std::string apireject = "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 17\n\n{state: rejected}";
const std::string apisuccess = "HAPI/1.1 200 OK\nContent-Type:text/json\nContent-Length: 17\n\n{state: success}";



// CREATE NETWORK SOCKET AT ADDRESS
//const char* server_ip = "honeypi.baselinux.net";
const char* server_ip = "10.72.91.159";
const int server_port = 11829;



////////////////////////////////////////////////////
//// SEND PACKETS TO SERVER AND RECORD RESPONSE ////
////////////////////////////////////////////////////
// 0  - NEW CONNECTION
// 1  - LOGIN WITH TOKENID
// 2  - LOGIN WITH USER/PASS
// 3  - CHECK FOR VERSION
// 4  - UPDATE SCRIPT FOR NEW VERSION
// 5  - INITIALIZE SIGNAL WITH NEW REPORT
// 6  - Report Portion of Full
// 7  - Last Packet of Report
// 8  - Report to Main Server (30 mins)
// 9  - 
// 10 - 
// 11 - 
// 12 - 
// 13 - 
// 14 - 
// 15 - 
// 16 - 
// 17 - 
// 18 - 
// 19 - 
std::string sendtoserver(int packettype, int optionnumber, std::string optionstring, std::string data2) {
    if (packettype == 0) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "NEW" + doublequote + "}";
    } else if (packettype == 1) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "ESTABLISH" + doublequote + "; " + doublequote + "LOGIN" + doublequote + ", " + doublequote + "API=" + apiKEY + doublequote + "}";
    } else if (packettype == 2) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "ESTABLISH" + doublequote + "; " + doublequote + "LOGIN" + doublequote + ", " + doublequote + "LOGIN=" + loginstr + doublequote + "; " + doublequote + ";;;;PASS=" + passstr + doublequote + "}";
    } else if (packettype == 3) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "CHECK_FOR_UPDATE" + doublequote + "; " + doublequote + "VERSION" + doublequote + ", " + doublequote + "ID=" + honeyversion + doublequote + "}";
    } else if (packettype == 4) {

    } else if (packettype == 5) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "NEW_REPORT" + doublequote + "; " + doublequote + "TOTALPACKETS" + doublequote + ", " + doublequote + inttostring(optionnumber) + doublequote + "}"; 
    } else if (packettype == 6) {
        data2 = "HAPI/1.1\nContent-Type:test/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "REPORT_PART" + doublequote + "; " + doublequote + "REPORTNUM" + doublequote + ", " + doublequote + inttostring(optionnumber) + doublequote + "; " + doublequote + "TOKENID" + doublequote + ", " + doublequote + tokenID + doublequote + "; " + doublequote + "DATA" + doublequote + ", " + doublequote + optionstring + doublequote + "}";
    } else if (packettype == 7) {
        data2 = "HAPI/1.1\nContent-Type:text/json\n\n{" + doublequote + "CONNECTION" + doublequote + ", " + doublequote + "REPORT_PART" + doublequote + "; " + doublequote + "REPORTNUM" + doublequote + ", " + doublequote + inttostring(optionnumber) + doublequote + "; " + doublequote + "REPORTFINISH=TRUE" + doublequote + "; " + doublequote + "TOKENID" + doublequote + ", " + doublequote + tokenID + doublequote + "; " + doublequote + "DATA" + doublequote + ", " + doublequote + optionstring + doublequote + "}";
    } else if (packettype == 8) {

    } else if (packettype == 9) {

    } else if (packettype == 10) {

    } else if (packettype == 11) {

    } else if (packettype == 12) {

    } else if (packettype == 13) {

    } else if (packettype == 14) {

    } else if (packettype == 15) {

    } else if (packettype == 16) {

    } else if (packettype == 17) {

    } else if (packettype == 18) {

    } else if (packettype == 19) {

    } else if (packettype == 20) {

    } else {

    }

    // DATA2 CHECK FOR NULL
    if (data2 == "") {
        return "ERROR1";
        return "ERROR1";
    }

    
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // CHECK FOR RESOLVED HOSTNAME
    if (getaddrinfo(server_ip, nullptr, &hints, &res) != 0) {
        logcritical("Unable to resolve hostname!", true);
        return "ERROR2";
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
        return "ERROR3";
    }

    if (connect(serverUpstream, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        close(serverUpstream);
        return "ERROR4";
    }

    // PROCESS READ
    send(serverUpstream, data2.c_str(), data2.length(), 0);
    char bufferread[4096];
    read(serverUpstream, bufferread, 4096);
    std::string yup = bufferread;
    close(serverUpstream);
    sleep(1);


    // DEBUG (FIX THIS)
    //std::cout << "Connected to the server at " << server_ip << ":" << server_port << "\n";
    //std::cout << "data2: " << data2 << std::endl;
    //std::cout << yup << std::endl;

    

    sleep(1);
    return yup;
}





//////////////////////////////////////
//// CHECK UPSTREAM SERVER STATUS ////
//////////////////////////////////////
int checkserverstatus() {
    std::string yup = sendtoserver(0, 0, "", "");
    
    // CASE STATEMENTS
    if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 18\n\n{state: available}") {
//        loginfo("SERVER - Received Valid Connection...", true);
        return 0;
    } else if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 20\n\n{state: unavailable}") {
  //      loginfo("SERVER - Server Temporarily Unavailable, Continuing...", true);
        return 3;
    } else if (yup == apireject) {
   //     loginfo("SERVER - Received REJECTION!", true);
        return 4;
    } else {
   //     logcritical("SERVER - RECEIVED NOT RESPONSE FROM SERVER!", true);
        std::cout << "RECEIVED (INVALID) = " << yup << std::endl;
        return 1;
    }
    return 2;
}




//////////////////////////////////
//// UPDATE TOKEN FROM SERVER ////
//////////////////////////////////
int updateToken() {
    int status = 0;
    loginfo("Updating ID from Server...", false);
    std::string newID = sendtoserver(1, 0, "", "");
    //std::cout << "ERRORED" << newID.length() << newID << std::endl;
    if (newID != "" && newID.length() >= 148) {
        //std::cout << newID.substr(59,7) << "{}" << newID.substr(67,7) << "{}" << newID.substr(76,5) << "{}" << newID.substr(81,2) << "{}" << newID.substr(147,1) << std::endl;
        if (newID.substr(59,7) == "{state:" && newID.substr(67,7) == "success" && newID.substr(76,5) == "TOKEN" && newID.substr(81,2) == ": " && newID.substr(147,1) == "}") {
            tokenID = newID.substr(83,64);
            std::cout << "IT WORKED!: " << tokenID << std::endl;
            sendtolog("Done");
        } else {
            sendtolog("ERROR (INVALID RESPONSE)");
            status = status + 1;
        }
    } else {
        sendtolog("ERROR (NULL RESPONSE)");
        status = status + 1;
    }
    return status;
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
// RESTRUCTURE INTO MAP
std::map<int, std::string> splitreportintopack(std::string data1) {
    std::map <int, std::string> packets;
    packets.clear();
    if (data1 == "") {
        // NULL STRING RECEIVED/IGNORE
        logcritical("Split Function Received Null Data", true);
        return packets;
        return packets;
    }

    totalnumberofpackets = 0;
    if (data1.length() <= packetsize) {
        packets[totalnumberofpackets] = data1;
        totalnumberofpackets = totalnumberofpackets + 1;
    } else if (data1.length() > packetsize) {
        int multiplier = 0;
        while (data1.length() > (multiplier + 1) * packetsize) {
            packets[multiplier] = data1.substr((multiplier * packetsize), packetsize);
            multiplier = multiplier + 1;
            totalnumberofpackets = totalnumberofpackets + 1;
        }
        packets[multiplier] = data1.substr((multiplier * packetsize), (data1.length() - (multiplier * packetsize)));
        totalnumberofpackets = totalnumberofpackets + 1;
        return packets;
    } else {
        logcritical("Split Function Received < 0 Packet Length!!!", true);
        return packets;
        return packets;
    }
    logcritical("Split Function Jumped If Case!", true);
    return packets;
}




///////////////////////////////////
///// CREATE REPORT FROM MAPS /////
///////////////////////////////////
int sendpacketreports(std::map<int, std::string> packetssend) {
    int splittingreport = packetssend.size();
    std::cout << "PACKET SIZE:" << splittingreport << std::endl;
    if (splittingreport > 1) {
        int currentsend = 0;
        while (currentsend <= splittingreport - 1) {
            std::string partstr = packetssend[currentsend];

            std::cout << "PACKET" << currentsend << packetssend[currentsend] << std::endl;



            // SEND TO SERVER GOES HERE

            currentsend = currentsend + 1;
        }

        return currentsend;
    } else if (splittingreport == 1) {
        std::string readyresult = "";
        while (readyresult != startreporting) {
            readyresult = sendtoserver(5,1,"","");
            if (readyresult == startreporting) {
                std::cout << "RECEIVED APPROVAL FROM SERVER, Starting to Send Report!" << std::endl;
                sleep(5);
            } else if (readyresult == apireject) {
                std::cout << "Received Reject, Waiting 300 seconds." << std::endl;
                sleep(300);
            } else {
                std::cout << "WAITING FOR SERVER RESPONSE! (Trying Again 30 Seconds)" << std::endl;
                std::cout << "RECEIVED: " << readyresult << std::endl;
                sleep(30);                
            }
        }

        if (readyresult == startreporting) {
            // SEND TO SERVER GOES HERE
            std::string packettyper = sendtoserver(7, 1, packetssend[0], "");
            if (packettyper == apisuccess) {
                loginfo("Report Sent to Server Successfully!!!", true);
                std::cout << "YAY!" << std::endl;
                return 1;
            } else if (packettyper == apireject) {
                logcritical("Received Rejection from Server!", true);
                return -6;
            } else {
                logcritical("ERROR OCCURRED FROM Server in Sending Report.", true);
                logcritical("RECEIVED " + packettyper, true);
                return -5;
            }
        } else {
            logcritical("SEND Packets to Main Server Failed at Requesting Transaction from Server!", true);
            return -4;
        }
        logcritical("SEND Packets Jumped Accept IF Statement!", true);
        return -3;
    } else {
        logcritical("SEND Packets Did Not Receive Valid Map Size!", true);
        return -1;
    }
    logcritical("SEND Packets Jumped If Statement!", true);
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
            std::map<int, std::string> resultantvec = splitreportintopack(encryptstring);
            std::cout << compiledreport << std::endl;
            loginfo("Finished Compiling Report", true);
            loginfo("Sending Report to Server", true);
            std::cout << encryptstring << std::endl;
            int returnvalue = sendpacketreports(resultantvec);
            if (returnvalue > 0) {
                loginfo("Completed Sending Report to Server!", true);
                loginfo("Removing and Recreating SSH Container", true);
                system(dockerkillguestssh.c_str());
                // system start ssh container

                clearmaps(true);
            } else if (false) {
                // FIX THIS
            } else if (false) {

            } else {

            }
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
            system(dockerkillguestssh.c_str());
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

            int killguest = system(dockerkillguestssh.c_str());
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







