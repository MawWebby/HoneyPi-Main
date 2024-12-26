#include <string>

#ifndef ENCRYPTION_H
#define ENCRYPTION_H


// LOGIN CREDENTIALS
int logincredentials(std::string, std::string);


// DETERMINE ENCRYPTION METHOD
int encryptionmethod(std::string, int);



// UCRYPT ENCRYPTION
std::string ucrypt_decrypt(std::string);
std::string ucrypt_Ecrypt(std::string);

#endif