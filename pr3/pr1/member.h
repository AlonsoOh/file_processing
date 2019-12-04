#include <iostream>
#include <string>
#include "delim.h"
#include "recfile.h"
#include "textind.h"
#include "indfile.h"
#include "buffile.h"

#define LENMIL 11
#define LENLEV 2

#define STDMAXBUF 256

#define IDFOUND			135
#define IDNOTFOUND		136

#define MAXMEMBER 1200

using namespace std;

class Member {
private:
	string id;
	string password;
	string name;
	string phoneNum;
	string address;
	char mileage[LENMIL];
	char level[LENLEV];
	string key;
public:
	Member();
	Member(const string new_id);
	Member(const Member& s);
	Member & operator = (const Member & s);
	bool operator == (const Member & s);
	bool operator != (const Member & s);

	friend istream & operator >> (istream & is, Member & s);
	friend ostream & operator << (ostream & is, Member & s);

	void update_id(const string new_id);
	void update_password(const string new_password);
	void update_name(const string new_name);
	void update_phoneNum(const string new_phoneNum);
	void update_address(const string new_address);
	void update_mileage(const char* new_mileage);
	void update_level(const int new_level);

	int get_size();
	int get_level();

	bool Pack(IOBuffer & Buffer) const;
	bool Unpack(IOBuffer & Buffer);

	string get_id(int aux) {
		return id;
	}
	string get_password(int aux) {
		return password;
	}

	char *Key() {
		key.assign(id,0, 35);
		return (char*)(key.c_str());
	}
};

istream & operator >> (istream & is, Member & s);
ostream & operator << (ostream & is, Member & s);


void showMember();
void MemberTest(bool print_rec);
int memSearch(char *filename, string input_id, int what_id, int flag);
int memLogin(string input_id, string input_pw, int flag);

int insertMember(string id);
int deleteMember(string id, int flag);
int modifyMember(string id);

void makeMemberIdx();