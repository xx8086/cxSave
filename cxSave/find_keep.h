#include "secret.h"


#include "htmlayout.h"
#pragma comment(lib,"HTMLayout.lib")
using namespace htmlayout;

#define FILENAME		("secret.lh")


void Keep(HELEMENT he);
void Find(HELEMENT he);
void FindAll(HELEMENT he);
void Delete(HELEMENT he);

void OnButtonClick(HELEMENT button);

int GetSecret(std::string str_text,std::string str_password,std::string& str_countsecret);