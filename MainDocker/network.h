// HEADER FOR NETWORK.CPP

#ifndef NEWTORK_H
#define NETWORK_H

// HONEYPI SERVER STATUS
int checkserverstatus();

// CLEAR MAPS OF PACKETS
int clearmaps(bool);

// RECEIVE REPORT FROM SSH
int reportreceiveSSH(std::string);

// SEND TO MAIN SERVER
std::string sendtoserver(int, int, std::string, std::string);


#endif