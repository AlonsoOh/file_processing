#include "subscription.h"
#include "member.h"
#include "NewsAgency.h"
#include <sstream>


Subscription::Subscription() {

}
Subscription::Subscription(const char * new_id) {
	update_subscriptionId(new_id);
}

Subscription::Subscription(const Subscription & s) { // 복사 생성자
	update_subscriptionId(s.subscriptionId);
	update_newsAgencyId(s.newsAgencyId);
	update_memberId(s.memberId);
	update_mileage(s.mileage);
}

Subscription & Subscription::operator = (const Subscription & s) {
	update_subscriptionId(s.subscriptionId);
	update_newsAgencyId(s.newsAgencyId);
	update_memberId(s.memberId);
	update_mileage(s.mileage);

	return *this;
}


bool Subscription::operator == (const Subscription & s) {
	if (strcmp(this->subscriptionId, s.subscriptionId)) {
		return false;
	}
	if (strcmp(this->newsAgencyId, s.newsAgencyId)) {
		return false;
	}
	if (this->memberId != s.memberId) {
		return false;
	}
	if (strcmp(this->mileage, s.mileage)) {
		return false;
	}


	return true;
}
bool Subscription::operator != (const Subscription & s) {
	if (strcmp(this->subscriptionId, s.subscriptionId)) {
		return true;
	}
	if (strcmp(this->newsAgencyId, s.newsAgencyId)) {
		return true;
	}
	if (this->memberId != s.memberId) {
		return true;
	}
	if (strcmp(this->mileage, s.mileage)) {
		return true;
	}

	return false;
}

int Subscription::get_size() {
	return strlen(subscriptionId) + strlen(newsAgencyId) + memberId.size() + strlen(mileage);
}

istream & operator >> (istream & is, Subscription & s) { // input

	string token;

	getline(is, token, '|');
	s.update_subscriptionId(token.c_str());
	getline(is, token, '|');
	s.update_newsAgencyId(token.c_str());
	getline(is, token, '|');
	s.update_memberId(token);
	getline(is, token, '\n');
	s.update_mileage(token.c_str());



	return is;
}
ostream & operator << (ostream & os, Subscription & s) {  // output
	cout << "SUBSCRIPTION ID : " << s.subscriptionId << endl;
	cout << "NEWSAGENCY ID   : " << s.newsAgencyId << endl;
	cout << "MEMBER ID       : " << s.memberId << endl;
	cout << "MILEAGE         : " << s.mileage << endl;
	return os;
}


void Subscription::update_subscriptionId(const char * new_id) { strcpy(this->subscriptionId, new_id); }
void Subscription::update_newsAgencyId(const char * new_id) { strcpy(this->newsAgencyId, new_id); }
void Subscription::update_memberId(const string new_id) { this->memberId = new_id; }
void Subscription::update_mileage(const char * new_mileage) { strcpy(this->mileage, new_mileage); }

bool Subscription::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	string s_subid(subscriptionId, LENSUBID);
	string s_newsid(newsAgencyId, LENNEWSID);
	string s_mileage(mileage, LENMIL);

	numBytes = Buffer.Pack(s_subid.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(s_newsid.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(memberId.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(s_mileage.c_str());
	if (numBytes == -1) return false;
	
	return true;
}

bool Subscription::Unpack(IOBuffer & Buffer) {
	int numBytes;
	char buf[STDMAXBUF];
	
	numBytes = Buffer.Unpack(subscriptionId, LENSUBID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(newsAgencyId, LENNEWSID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	memberId = buf;
	numBytes = Buffer.Unpack(mileage, LENMIL);
	if (numBytes == -1) return false;
	
	return true;
}

void showSubscription() {
	ifstream ifs("listOfSubscription.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		exit(-1);
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int i = 0; i < 10; i++) {
		Subscription s;
		ifs >> s;
		cout << s << endl;
	}

	ifs.close();

	return;
}

void SubscriptionTest(bool print_rec) {
	ifstream ifs("listOfSubscription.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		exit(-1);
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Subscription> SubscriptionFile(buffer);


	char filename[30] = "FileOfSubscription.dat";

	SubscriptionFile.Create(filename, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Subscription s;
		ifs >> s;
		int recaddr;
		if ((recaddr = SubscriptionFile.Write(s)) == -1) {
			if (print_rec) cout << "Write Error!" << endl;
		}

	}
	SubscriptionFile.Close();

	if (print_rec) {
		SubscriptionFile.Open(filename, ios::in);
		for (int i = 0; i < 10; i++) {
			Subscription s;
			SubscriptionFile.Read(s);
			cout << s << endl;
		}
		SubscriptionFile.Close();
	}
}


int subscriptionSearch(char *filename, string input_id, int what_id, int flag) {
	int search_res;
	bool find_flag = false;
	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Subscription> SubscriptFile(buffer);
	Subscription s;
	SubscriptFile.Open(filename, ios::in);


	while (true) {
		search_res = SubscriptFile.Read(s);

		if (search_res == -1) break;

		if (!strcmp(s.get_id(what_id).c_str(), input_id.c_str())) {
			if (flag == 0) {
				cout << endl << s << endl;
			}
			find_flag = true;
		}
	}

	if (find_flag == false) {
		if (flag == 0) {
			cout << "There is no ID : " << input_id << "!!\n\n";
		}
		find_flag = false;
	}
	SubscriptFile.Close();

	if (find_flag) return IDFOUND;
	else return IDNOTFOUND;
}

int insertSubscript(string id) {
	Subscription newsubscript(id.c_str());
	string buf;

	if (id.size() != 16) {
		cout << "Check sizeof Id" << endl;
		return -1;
	}

	char filename[30];
	strcpy(filename,"FileOfNewsAgency.dat");

	while (true) {
		cout << "Input NewsAgency Id : ";
		cin >> buf;

		if (agencySearch(filename, buf, -1, 1) == IDNOTFOUND) {
			cout << "Check NewsAgency Id" << endl;
			continue;
		}

		newsubscript.update_newsAgencyId(buf.c_str());
		break;
	}

	strcpy(filename, "FileOfMember.dat");
	while (true) {
		cout << "Input Member Id: ";
		cin >> buf;

		if (memSearch(filename, buf, -1, 1) == IDNOTFOUND) {
			cout << "Check Member Id" << endl;
			continue;
		}

		newsubscript.update_memberId(buf);
		break;
	}


	while (true) {
		cout << "Input mileage : ";
		cin >> buf;

		if (buf.size() != LENMIL - 1) {
			cout << "Check input mileage" << endl;
			continue;
		}
		newsubscript.update_mileage(buf.c_str());
		break;
	}


	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Subscription> subscriptionFile(buffer);
	subscriptionFile.Open("FileOfSubscription.dat", ios::out | ios::app);
	if (subscriptionFile.Write(newsubscript) == -1) {
		cout << "Write Error!" << endl;
	}
	subscriptionFile.Close();


	return 0;
}
int deleteSubscript(string id, int flag) {
	Subscription m;
	Subscription *subscriptlist = new Subscription[MAXSUBSCRIPT];

	int idx = 0;
	bool delete_flag = false;


	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Subscription> subscriptFile(buffer);
	subscriptFile.Open("FileOfSubscription.dat", ios::in);



	while (subscriptFile.Read(m) != -1) {

		if (strcmp(m.get_id(flag).c_str(), id.c_str())) {
			subscriptlist[idx++] = m;
		} 
		else {
			delete_flag = true;
		}
	}

	subscriptFile.Close();


	if (delete_flag == false) return -1;

	subscriptFile.Create("FileOfSubscription.dat", ios::out | ios::trunc);
	for (int i = 0; i < idx; i++) {
		int recaddr;
		if ((recaddr = subscriptFile.Write(subscriptlist[i])) == -1) {
			cout << "Write Error!" << endl;
		}
	}
	subscriptFile.Close();
	delete[]subscriptlist;
	return 0;
}
int modifySubscript(string id) {
	Subscription newsubscript(id.c_str());
	string buf;

	if (id.size() != 16) {
		cout << "Check sizeof Id" << endl;
		return -1;
	}

	char filename[30];
	strcpy(filename, "FileOfNewsAgency.dat");

	while (true) {
		cout << "Input NewsAgency Id : ";
		cin >> buf;

		if (agencySearch(filename, buf, -1, 1) == IDNOTFOUND) {
			cout << "Check NewsAgency Id" << endl;
			continue;
		}

		newsubscript.update_newsAgencyId(buf.c_str());
		break;
	}

	strcpy(filename, "FileOfMember.dat");
	while (true) {
		cout << "Input Member Id: ";
		cin >> buf;

		if (memSearch(filename, buf, -1, 1) == IDNOTFOUND) {
			cout << "Check Member Id" << endl;
			continue;
		}

		newsubscript.update_memberId(buf);
		break;
	}


	while (true) {
		cout << "Input mileage : ";
		cin >> buf;

		if (buf.size() != LENMIL - 1) {
			cout << "Check input mileage" << endl;
			continue;
		}
		newsubscript.update_mileage(buf.c_str());
		break;
	}

	deleteSubscript(id, 1);

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Subscription> subscriptionFile(buffer);
	subscriptionFile.Open("FileOfSubscription.dat", ios::out | ios::app);
	if (subscriptionFile.Write(newsubscript) == -1) {
		cout << "Write Error!" << endl;
	}
	subscriptionFile.Close();


	return 0;
}