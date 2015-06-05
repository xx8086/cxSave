#pragma once

#include <iostream>
#include <fstream>
//#include <vector>
#include <map>
//#include <algorithm>

//typedef std::map<std::string,std::string>::value_type valType;
typedef struct rc4_state
{      
   unsigned char state[256];       
   unsigned char x;        
   unsigned char y;
} rc4_state;

void rc4_init(const unsigned char *key_data_ptr, int key_data_len, rc4_state *key);
void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_state *key);

void Encrypt(char* szIn,char* szOut,char* szKey);
void Decrypt(char* ciphertext,char* reulttext,char* szKey,int ndatas);

bool WtFile(char* strFile,char* strOut);
bool RdFile(char* strFile,std::map<std::string,std::string>& mymap);
