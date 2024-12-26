#include "globalheaders.h"
#include "network.h"

//////////////////////////////////////
//// CHECK UPSTREAM SERVER STATUS ////
//////////////////////////////////////
int checkserverstatus() {
    const char* server_ip = "honeypi.baselinux.net"; // Server IP address
    // ADD CHECK FOR DNS!!!
    const int server_port = 11829;           // Server port number
    const std::string message = "HAPI/1.1\nContent-Type:text/json\n\n{\"CONNECTION\", \"NEW\"}";
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(server_ip, nullptr, &hints, &res) != 0) {
        logcritical("Unable to resolve hostname!", true);
    }


    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;

    freeaddrinfo(res);

    int serverUpstream = socket(AF_INET, SOCK_STREAM, 0);

    if (serverUpstream < 0) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }

    if (connect(serverUpstream, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        close(serverUpstream);
        return 1;
    }

    std::cout << "Connected to the server at " << server_ip << ":" << server_port << "\n";
    send(serverUpstream, message.c_str(), message.length(), 0);
    std::cout << "Message sent: " << message << "\n";
    char bufferread[4096];
    read(serverUpstream, bufferread, 4096);
    std::string yup = bufferread;
    close(serverUpstream);

    if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 18\n\n{state: available}") {
        loginfo("SERVER - Received Valid Connection...", true);
        return 0;
    } else if (yup == "HAPI/1.1 403 OK\nContent-Type:text/json\nContent-Length: 20\n\n{state: unavailable}") {
        loginfo("SERVER - Server Temporarily Unavailable, Continuing...", true);
        return 3;

    } else {
        logcritical("SERVER - RECEIVED NOT RESPONSE FROM SERVER!", true);
        std::cout << "RECEIVED:" << yup << std::endl;
        return 1;
    }
    return 2;
}