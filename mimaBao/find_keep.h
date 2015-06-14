#include "secret.h"


#include "htmlayout.h"
#pragma comment(lib,"HTMLayout.lib")
using namespace htmlayout;

#define FILENAME		("secret.lh")

class mainOption
{
public:
	mainOption();
	virtual ~mainOption();
private:
	void Keep(HELEMENT he);
	void Find(HELEMENT he);
	void FindAll(HELEMENT he);
	void Delete(HELEMENT he);
	int GetSecret(std::string str_text, std::string str_password, std::string& str_countsecret);
public:
	void OnButtonClick(HELEMENT button);
};
