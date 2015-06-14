#include "stdafx.h"
#include "find_keep.h"
#include ".\behaviors\behavior_tabs.cpp"


mainOption::mainOption()
{}

mainOption::~mainOption()
{}

void mainOption::Keep(HELEMENT he)
{
	dom::element ele(he);
	dom::element root = ele.root();
	std::wstring wstr_password = L"";
	std::wstring wstr_countnumber = L"";
	std::wstring wstr_countpassword = L"";
	dom::element my_ele = root.find_first(".mypassword");
	json::value obj  = my_ele.get_value();
	wstr_password = obj.to_string();//小工具密码
	my_ele = root.find_first(".mycoundnumber_keep");
	obj  = my_ele.get_value();
	wstr_countnumber = obj.to_string();//账号
	my_ele = root.find_first(".mycoundpassword");
	obj  = my_ele.get_value();
	wstr_countpassword = obj.to_string();//账号密码

	my_ele = root.find_first(".cantkeep");
	my_ele.set_style_attribute("display",L"block");
	my_ele = root.find_first(".cantkeep1");

	if(wstr_password.empty() || wstr_countnumber.empty() || wstr_countpassword.empty())
	{
		std::wstring text(L"输入项有为空，无法保存，请重新输入！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
		return ;
	}

	std::string str_password(aux::w2a(wstr_password.c_str()));
	std::string str_text(aux::w2a(wstr_countnumber.c_str()));
	std::string str_countsecret(aux::w2a(wstr_countpassword.c_str()));
/////////////////////////////////////////////////////////////////////////
	int n = GetSecret(str_text,str_password,str_countsecret);
	std::wstring text(L"账号");
	text.append(wstr_countnumber);
	if(n > 3)
	{
		
		text.append(L"保存成功！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());

		my_ele = root.find_first(".mycoundnumber_keep");
		my_ele.clear();
		my_ele = root.find_first(".mycoundpassword");
		my_ele.clear();
	}
	else if( -1 == n )
	{
		text.append(L"已经存在，不予保存，请重先将其删除后再保存！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
	}
}

void mainOption::Find(HELEMENT he)
{
	dom::element ele(he);
	dom::element root = ele.root();
	std::wstring wstr_password = L"";
	std::wstring wstr_countnumber = L"";
	dom::element my_ele = root.find_first(".mpassword2");
	json::value obj  = my_ele.get_value();
	wstr_password = obj.to_string();
	my_ele = root.find_first(".mycoundnumber_find");
	obj  = my_ele.get_value();
	wstr_countnumber = obj.to_string();

	my_ele = root.find_first(".cantfind");
	my_ele.set_style_attribute("display",L"block");
	my_ele = root.find_first(".cantfind1");
	my_ele.set_style_attribute("display",L"none");
	my_ele = root.find_first(".cantfind2");
	//my_ele.set_style_attribute("display",L"block");
	if(wstr_password.empty() || wstr_countnumber.empty())
	{
		std::wstring text(L"输入项有为空，无法查找，请重新输入！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
		return ;
	}

	std::string str_text(aux::w2a(wstr_countnumber.c_str()));
	std::string str_password(aux::w2a(wstr_password.c_str()));
	std::string keep_countsecret="";
	int n = GetSecret(str_text,str_password,keep_countsecret);
	std::wstring text(L"账号");
	text.append(wstr_countnumber);
	if(0 == n)
	{
		text.append(L"不存在，请重新输入要查询的账号！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
	}
	else if( -1 == n)
	{
		char* p = NULL;
		char* q = NULL;
		char* secret_password = NULL;

		int len = str_password.length()+1;
		secret_password = new char [len];
		secret_password[len-1] = '\0';
		strcpy_s(secret_password,len,str_password.c_str());

		len = keep_countsecret.length()+1;
		p = new char [len];
		p[len-1] = '\0';
		strcpy_s(p,len,keep_countsecret.c_str());

		q = new char [len];
		memset(q,0,len);
		Decrypt(p,q,secret_password,(len-1)/2);

		//my_ele.set_value(q);
		text.append(L"的密码为：");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());

		my_ele = root.find_first(".cantfind1");
		my_ele.set_style_attribute("display",L"block");
		text = aux::a2w(q);
		aux::w2utf str2(text);
		my_ele.set_html((const unsigned char*)(const char*)str2, str2.length());

		delete [] p;
		delete [] q;
		delete [] secret_password;
	}
}

void mainOption::FindAll(HELEMENT he)
{
	dom::element ele(he);
	dom::element root = ele.root();
	std::wstring wstr_password = L"";
	dom::element my_ele = root.find_first(".mpassword3");
	json::value obj  = my_ele.get_value();
	wstr_password = obj.to_string();
	
	my_ele = root.find_first(".cantfindall");
	my_ele.set_style_attribute("display",L"block");
	my_ele = root.find_first(".cantfindall1");
	my_ele.set_style_attribute("display",L"none");
	my_ele = root.find_first(".cantfindall2");
	my_ele.set_style_attribute("display",L"block");
	if(wstr_password.empty())
	{
		std::wstring text(L"小工具密码项输入有为空，无法查找，请重新输入！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
		return ;
	}
////////////////////////////////////////////////////////////////////
	std::string str_password(aux::w2a(wstr_password.c_str()));
	std::map<std::string,std::string> mymap;
	RdFile(FILENAME,mymap);
	if(mymap.empty())
	{	
		std::wstring text(L"密码本为空！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
	}
	else
	{
		my_ele.set_style_attribute("display",L"none");
		my_ele = root.find_first(".cantfindall1");
		my_ele.set_style_attribute("display",L"block");

		char* p = NULL;
		char* q = NULL;
		char* secret_password = NULL;
		std::string return_countnumber = "";
		std::string return_countsecret = "";
		std::wstring wreturn_txt = L"<table width=\"100%\" border=\"1\"><tr><th>账号</th><th>密码</th></tr>";
		int len = str_password.length()+1;
		secret_password = new char [len];
		secret_password[len-1] = '\0';
		strcpy_s(secret_password,len,str_password.c_str());

		std::map<std::string,std::string>::iterator result = mymap.begin();
		while(result != mymap.end())
		{
			return_countsecret = result->second;
			return_countnumber = result->first;		
			result++;
			if(return_countnumber.size()< 2 || return_countsecret.size() < 2)
				continue;
			len = return_countnumber.length()+1;
			p = new char [len];
			p[len-1] = '\0';
			strcpy_s(p,len,return_countnumber.c_str());

			q = new char [len];
			memset(q,0,len);
			Decrypt(p,q,secret_password,(len-1)/2);
			wreturn_txt.append(L"<tr><td>");
			wreturn_txt.append(aux::a2w(q));
			wreturn_txt.append(L"</td><td>");
			delete [] p;
			delete [] q;

			len = return_countsecret.length()+1;
			p = new char [len];
			p[len-1] = '\0';
			strcpy_s(p,len,return_countsecret.c_str());

			q = new char [len];
			memset(q,0,len);
			Decrypt(p,q,secret_password,(len-1)/2);
			wreturn_txt.append(aux::a2w(q));
			wreturn_txt.append(L"</td></tr>");
			delete [] p;
			delete [] q;
		}

		wreturn_txt.append(L"</table>");
		aux::w2utf str2(wreturn_txt);
		my_ele.set_html((const unsigned char*)(const char*)str2, str2.length());
		delete [] secret_password;
	}
}

void mainOption::Delete(HELEMENT he)
{
	dom::element ele(he);
	dom::element root = ele.root();
	std::wstring wstr_password = L"";
	std::wstring wstr_countnumber = L"";

	dom::element my_ele = root.find_first(".mpassword5");
	json::value obj  = my_ele.get_value();
	wstr_password = obj.to_string();
	my_ele = root.find_first(".mycoundnumber_delete");
	obj  = my_ele.get_value();
	wstr_countnumber = obj.to_string();

	my_ele = root.find_first(".cantdelete");
	my_ele.set_style_attribute("display",L"block");
	my_ele = root.find_first(".cantdelete1");
	if(wstr_password.empty() || wstr_countnumber.empty())
	{
		std::wstring text(L"输入项有为空，无法删除，请重新输入！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
		return ;
	}

	std::string str_password(aux::w2a(wstr_password.c_str()));
	std::string str_text(aux::w2a(wstr_countnumber.c_str()));
	wstr_countnumber = L"账号" + wstr_countnumber + L"和密码";

	char* secret_password = NULL;
	int len = str_password.length()+1;
	secret_password = new char [len];
	secret_password[len-1] = '\0';
	strcpy_s(secret_password,len,str_password.c_str());

	int n = str_text.length()+1;
	char* p = new char [n];
	strcpy_s(p,n,str_text.c_str());
	*(p+n-1) = '\0';

	n = 2*(str_text.length()+1);
	char* q = new char[n];
	memset(q,0,sizeof(char)*n);
	Encrypt(p,q,secret_password);
	std::string keep_countnumber(q);

	delete [] p;
	delete [] q;

	std::map<std::string,std::string> mymap;
	RdFile(FILENAME,mymap);
	if(mymap.empty())
	{	
		std::wstring text(L"密码本为空！");
		aux::w2utf str(text);
		my_ele.set_html((const unsigned char*)(const char*)str, str.length());
	}
	else
	{
		std::map<std::string,std::string>::iterator result = mymap.find(keep_countnumber);
		if(result == mymap.end())
		{
			std::wstring text(L"该账号不存在！");
			aux::w2utf str(text);
			my_ele.set_html((const unsigned char*)(const char*)str, str.length());
			return ;
		}
		std::string keep_countsecret(result->second);
		len = keep_countsecret.length()+1;
		p = new char [len];
		p[len-1] = '\0';
		strcpy_s(p,len,keep_countsecret.c_str());

		q = new char [len];
		memset(q,0,len);
		Decrypt(p,q,secret_password,(len-1)/2);
		wstr_countnumber.append(aux::a2w(q));
		wstr_countnumber.append(L"的这条纪录被删除");
		delete [] p;
		delete [] q;

		mymap.erase(keep_countnumber);

		std::ofstream out;
		out.open( FILENAME, std::ios::out| std::ios::trunc);
		out.close();

		result = mymap.begin();
		while(result != mymap.end())
		{
			keep_countnumber = result->first;
			str_text = result->second;
			n = keep_countnumber.length()+str_text.length()+3;// n>1，keep
			p = new char [n];
			p[n-1] = '\0';
			sprintf(p,"%s %s\n",keep_countnumber.c_str(),str_text.c_str());
			WtFile(FILENAME,p);
			delete [] p;
			result++;
		}
		aux::w2utf str2(wstr_countnumber);
		my_ele.set_html((const unsigned char*)(const char*)str2, str2.length());

	}


	delete [] secret_password;
}

void mainOption::OnButtonClick(HELEMENT button)
{
	dom::element ele(button);
	std::wstring wstrName = ele.get_attribute("name");
	if(wstrName.compare(L"keep") == 0)
	{
		Keep(button);
	}
	else if(wstrName.compare(L"find") == 0)
	{
		Find(button);
	}
	else if(wstrName.compare(L"findall") == 0)
	{
		FindAll(button);
	}
	else if(wstrName.compare(L"delete") == 0)
	{
		Delete(button);
	}
}

int mainOption::GetSecret(std::string str_text, std::string str_password, std::string& str_countsecret)
{
	char* p = NULL;
	char* q = NULL;
	int n = str_password.length()+1;
	char* secret_password = new char [n];
	*(secret_password+n-1) = '\0';//小工具密码
	strcpy_s(secret_password,n,str_password.c_str());
///////////////////////////////////////////////////
	
	n = str_text.length()+1;
	p = new char [n];
	strcpy_s(p,n,str_text.c_str());
	*(p+n-1) = '\0';

	n = 2*(n+1);
	q = new char[n];
	memset(q,0,sizeof(char)*n);
	Encrypt(p,q,secret_password);
	std::string keep_countnumber(q);//加密的帐号名

	delete [] p;
	delete [] q;
	n = -1;
	////////////////////////////////////////////////
	std::map<std::string,std::string> mymap;
	RdFile(FILENAME,mymap);
	std::map<std::string,std::string>::iterator result = mymap.find(keep_countnumber);
	if(result == mymap.end())
	{
		n = 0;
	}
	else
	{
		str_countsecret = result->second;
	}
	////////////////////////////////////////////
	if(0 == n && !str_countsecret.empty())
	{
		n = str_countsecret.length()+1;
		p = new char [n];
		strcpy_s(p,n,str_countsecret.c_str());
		*(p+n-1) = '\0';

		n = 2*(str_countsecret.length()+1);
		q = new char [n];
		memset(q,0,sizeof(char)*n);
		Encrypt(p,q,secret_password);
		std::string keep_countsecret(q);
		delete [] p;
		delete [] q;

		n = keep_countnumber.length()+keep_countsecret.length()+3;// n>1，keep
		p = new char [n];
		p[n-1] = '\0';
		sprintf(p,"%s %s\n",keep_countnumber.c_str(),keep_countsecret.c_str());
		WtFile(FILENAME,p);
		delete [] p;
	}
	//////////////////////////////////////////////
	delete [] secret_password;
	return n;

}
