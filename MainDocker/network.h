// HEADER FOR NETWORK.CPP

#ifndef NETWORK_H
#define NETWORK_H

// HONEYPI SERVER STATUS
int checkserverstatus();

// UPDATE TOKEN FROM SERVER
int updateToken();

// CLEAR MAPS OF PACKETS
int clearmaps(bool);

// RECEIVE REPORT FROM SSH
int reportreceiveSSH(std::string);

// SEND TO MAIN SERVER
std::string sendtoserver(int, int, std::string, std::string);


#endif