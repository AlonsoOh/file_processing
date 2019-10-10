#pragma once

#include <iostream>
#include <string>
#include "delim.h"
#include "recfile.h"

#define LENNEWSID 13

#define STDMAXBUF 256

#define IDFOUND			135
#define IDNOTFOUND		136

#define MAXAGENCY 1200

using namespace std;

class NewsAgency {
private:
	char newsAgencyId[LENNEWSID];
	string name;
	string address;
public:
	NewsAgency();
	NewsAgency(const char * new_id);
	NewsAgency(const NewsAgency& s);
	NewsAgency & operator = (const NewsAgency & s);
	bool operator == (const NewsAgency & s);
	bool operator != (const NewsAgency & s);

	friend istream & operator >> (istream & is, NewsAgency & s);
	friend ostream & operator << (ostream & is, NewsAgency & s);

	void update_newsAgencyId(const char * new_id);
	void update_name(const string new_name);
	void update_address(const string new_address);

	bool Pack(IOBuffer & Buffer) const;
	bool Unpack(IOBuffer & Buffer);

	string get_id(int aux) {
		return newsAgencyId;
	}

	int get_size();
};

istream & operator >> (istream & is, NewsAgency & s);
ostream & operator << (ostream & is, NewsAgency & s);


void showNewsAgency();
void NewsAgencyTest(bool print_rec);
int agencySearch(char *filename, string input_id, int what_id, int flag);

int insertAgency(string id);
int deleteAgency(string id, int flag);
int modifyAgency(string id);