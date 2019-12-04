
#ifndef _SUBSCR_
#define _SUBSCR_


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
#define MAXKEY 257


class Subscription {
private:
	char subscriptionId[LENSUBID];
	char newsAgencyId[LENNEWSID];
	string memberId;
	char mileage[LENMIL];


	char bt_key;

public:

	static char avail_key_list[MAXKEY];
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


	/*PROJ3*/
	// update and get key for btree index file
	void update_key(const char new_key) {
		bt_key = new_key;
	}
	char get_key() {
		return bt_key;
	}

	static void init_bt_key() {
		for (int i = 0; i < MAXKEY; i++)
			avail_key_list[i] = 0;
	}

	static char get_avail_key() {
		for (int i = 1; i < MAXKEY; i++) {
			if (avail_key_list[i] == 0) {
				avail_key_list[i] = 1;
				return i;
			}
		}
		return 0;
	}

	static void free_avail_key(char free_key) {
		avail_key_list[(int)free_key] = 0;
	}

	int get_size();
};



using namespace std;




istream & operator >> (istream & is, Subscription & s);
ostream & operator << (ostream & is, Subscription & s);

void showSubscription();
void SubscriptionTest(bool print_rec);
int subscriptionSearch(char *filename, string input_id, int what_id, int flag, string memid, int mode);

int insertSubscript(string id, string memid, int mode);
int deleteSubscript(string id, int flag);
int modifySubscript(string id, string memid, int mode);


void makeSubscriptionIdx();

int bt_searchSubscriptionIdx(char key, int flag);
int bt_insertSubscriptionIdx(char key, int writeaddr);
int bt_deleteSubscriptionIdx(char key, int flag);
int bt_modifySubscriptionIdx(char key, int flag);

#endif