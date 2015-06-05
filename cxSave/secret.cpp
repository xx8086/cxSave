#include "stdafx.h"
#include "secret.h"
#include  <string>

#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t

void rc4_init(const unsigned char *key_data_ptr, int key_data_len, rc4_state *key)
{
	unsigned char t;
	unsigned char index1;
	unsigned char index2;
	unsigned char* state;
	unsigned long counter;

	state = &key->state[0];
	for(counter = 0; counter < 256; counter++)
		state[counter] = (unsigned char)counter;
	
	key->x = 0;
	key->y = 0;
	index1 = 0;
	index2 = 0;
	
	for(counter = 0; counter < 256; counter++)
	{
		index2 = (key_data_ptr[index1] + state[counter] + index2) & 0xff;
		swap_byte(&state[counter], &state[index2]);
		index1 = (index1 + 1) % key_data_len;
		/*
		index2 = (index2 + state[counter] + key_data_ptr[(counter+2)%key_data_len])%256;
		swap_byte(&state[counter], &state[index2]);
		*/
	}
}

void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_state *key)
{
	unsigned char t;
	unsigned char x;
	unsigned char y;
	unsigned char* state;
	unsigned char xorIndex;
	int counter;

	x = key->x;
	y = key->y;
	state = &key->state[0];
	for(counter = 0; counter < buffer_len; counter++)
	{
		x = (x + 1) & 0xff;
		y = (state[x] + y) & 0xff;
		swap_byte(&state[x], &state[y]);
		xorIndex = (state[x] + state[y]) & 0xff;
		buffer_ptr[counter] ^= state[xorIndex];
		/*
		x=(x+1)%256;
		y=(y+state[x])%256;
		swap_byte(&state[x], &state[y]);
		xorIndex = (state[x] + state[y])%256;
		buffer_ptr[counter] ^= state[xorIndex];
		*/
	}
	key->x = x;
	key->y = y;
}

static char NUM2HEX(unsigned char x) 
{
	return (x>9?x+'a'-10:x+'9'-9);
}

static unsigned char HEX2NUM(char c) 
{
	return (c>'9'?c+10-'a':c+9-'9');
}


void BinToHex(unsigned char *data, char *str, int databytes)
{
	int i = 0;
	for(i=0; i<databytes; i++)
	{
		str[i + i] = NUM2HEX(data[i]>>4);
		str[i + i + 1] = NUM2HEX(data[i] & 0xf);
	}
	str[i + i] = 0;
}

void HexToBin(char *str, unsigned char * data, int databytes)
{
	unsigned char c;
	int i;
	for(i=0; i<databytes; i++)
	{
		c = HEX2NUM(str[i + i + 1]);
		c |= (HEX2NUM(str[i + i])<<4);
		data[i] = c; 
	}
}

void Decrypt(char* ciphertext,char* reulttext,char* szKey,int ndatas)
{
	char* szTmp = ciphertext;
	char* szTmp2 = reulttext;
	HexToBin(szTmp, (unsigned char*)szTmp2, ndatas);

	rc4_state state;
	rc4_init((unsigned char*)szKey, strlen(szKey), &state);
	rc4((unsigned char*)szTmp2, strlen(szTmp2), &state);
}

void Encrypt(char* szIn,char* szOut,char* szKey)
{
	rc4_state state;
	rc4_init((unsigned char*)szKey, strlen(szKey), &state);
	rc4((unsigned char*)szIn, strlen(szIn), &state);

	BinToHex((unsigned char*)szIn, szOut, strlen(szIn));
}


bool WtFile(char* strFile,char* strOut)
{
	std::ofstream out;
	out.open(strFile, std::ios::out | std::ios::binary|std::ios::app);
	if (out.is_open() == false)
    {
		return false;
    }
	long pos=out.tellp();
	out.seekp(pos);
	out.write(strOut,strlen(strOut));
	out.close();
	return true;
}

bool RdFile(char* strFile,std::map<std::string,std::string>& mymap)
{
	std::ifstream in;
	in.open(strFile, std::ios::in|std::ios::binary|std::ios::ate);
	if(in.is_open() == false)
    {
		return false;
    }
	
	//long size;
	//size = in.tellg();
	in.seekg(0, std::ios::beg);

	char acount_number[129];
	char acount_secter[129];
	while(!in.eof())
	{
		memset(acount_number,0,129);
		memset(acount_secter,0,129);
		in>>acount_number>>acount_secter;

		std::pair<std::map<std::string,std::string>::iterator,bool> ret =
			mymap.insert(std::make_pair(acount_number,acount_secter));
		if(!ret.second)
		{
			std::string str(ret.first->second);//“—¥Ê∏√’À∫≈
			
		}
	}
	in.close();
	return true;
}
