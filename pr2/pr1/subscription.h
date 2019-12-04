#pragma once

#include <iostream>
#include <string>
#include "delim.h"
#include "recfile.h"

#define LENSUBID 17
#define LENNEWSID 13
#define LENMIL 11

#define STDMAXBUF 256

#define IDFOUND			135
#define IDNOTFOUND		136

#define MAXSUBSCRIPT 11000

using namespace std;

class Subscription {
private:
	char subscriptionId[LENSUBID];
	char newsAgencyId[LENNEWSID];
	string memberId;
	char mileage[LENMIL];
public:
	Subscription();
	Subscription(const char *new_id);
	Subscription(const Subscription& s);
	Subscription & operator = (const Subscription & s);
	bool operator == (const Subscription & s);
	bool operator != (const Subscription & s);

	friend istream & operator >> (istream & is, Subscription & s);
	friend ostream & operator << (ostream & is, Subscription & s);

	void update_subscriptionId(const char * new_id);
	void update_newsAgencyId(const char * new_id);
	void update_memberId(const string new_id);
	void update_mileage(const char * new_mileage);

	bool Pack(IOBuffer & Buffer) const;
	bool Unpack(IOBuffer & Buffer);

	string get_id(int aux) {
		string tmp;
		if (aux == 200) {
			return memberId;
		}
		else if (aux == 201) {
			tmp = newsAgencyId;
			return tmp;
		}
		else {
			tmp = subscriptionId;
			return tmp;
		}
	}

	int get_size();
};

istream & operator >> (istream & is, Subscription & s);
ostream & operator << (ostream & is, Subscription & s);

void showSubscription();
void SubscriptionTest(bool print_rec);
int subscriptionSearch(char *filename, string input_id, int what_id, int flag, string memid, int mode);

int insertSubscript(string id, string memid, int mode);
int deleteSubscript(string id, int flag);
int modifySubscript(string id, string memid, int mode);