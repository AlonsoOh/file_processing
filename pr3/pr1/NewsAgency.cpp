#include "subscription.h"
#include "NewsAgency.h"
#include <sstream>
#include <fstream>


NewsAgency::NewsAgency() {

}
NewsAgency::NewsAgency(const char * new_id) {
	update_newsAgencyId(new_id);
}

NewsAgency::NewsAgency(const NewsAgency & s) { // 복사 생성자
	update_newsAgencyId(s.newsAgencyId);
	update_name(s.name);
	update_address(s.address);
}

NewsAgency & NewsAgency::operator = (const NewsAgency & s) {
	update_newsAgencyId(s.newsAgencyId);
	update_name(s.name);
	update_address(s.address);

	return *this;
}


bool NewsAgency::operator == (const NewsAgency & s) {
	if (strcmp(this->newsAgencyId , s.newsAgencyId)) {
		return false;
	}
	if (this->name != s.name) {
		return false;
	}
	if (this->address != s.address) {
		return false;
	}


	return true;
}
bool NewsAgency::operator != (const NewsAgency & s) {
	if (strcmp(this->newsAgencyId, s.newsAgencyId)) {
		return true;
	}
	if (this->name != s.name) {
		return true;
	}
	if (this->address != s.address) {
		return true;
	}


	return false;
}

istream & operator >> (istream & is, NewsAgency & s) { // input

	string token;

	getline(is, token, '|');
	s.update_newsAgencyId(token.c_str());
	getline(is, token, '|');
	s.update_name(token);
	getline(is, token, '\n');
	s.update_address(token);



	return is;
}
ostream & operator << (ostream & os, NewsAgency & s) {  // output
	cout << "NEWSAGENCY ID : " << s.newsAgencyId << endl;
	cout << "NAME          : " << s.name << endl;
	cout << "ADDRESS       : " << s.address << endl ;
	return os;
}


void NewsAgency::update_newsAgencyId(const char * new_id) { strcpy(this->newsAgencyId, new_id); }
void NewsAgency::update_name(const string new_name) { this->name = new_name; }
void NewsAgency::update_address(const string new_address) { this->address = new_address; };

bool NewsAgency::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	string s_id(newsAgencyId, LENNEWSID);

	numBytes = Buffer.Pack(s_id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(name.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(address.c_str());
	if (numBytes == -1) return false;
	
	return true;
}

bool NewsAgency::Unpack(IOBuffer & Buffer) {
	int numBytes;
	char buf[STDMAXBUF];
	
	numBytes = Buffer.Unpack(newsAgencyId, LENNEWSID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	name = buf;
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	address = buf;
	
	return true;
}

int NewsAgency::get_size() {
	return strlen(newsAgencyId) + name.size() + address.size();
}

void showNewsAgency() {
	ifstream ifs("listOfNewsAgency.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		exit(-1);
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int i = 0; i < 10; i++) {
		NewsAgency s;
		ifs >> s;
		cout << s << endl;
	}

	ifs.close();

	return;
}

void NewsAgencyTest(bool print_rec) {
	ifstream ifs("listOfNewsAgency.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		exit(-1);
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <NewsAgency> NewsAgencyFile(buffer);

	char filename[30] = "FileOfNewsAgency.dat";

	NewsAgencyFile.Create(filename, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		NewsAgency s;
		ifs >> s;
		int recaddr;
		if ((recaddr = NewsAgencyFile.Write(s)) == -1) {
			if (print_rec) cout << "Write Error!" << endl;
		}

	}
	NewsAgencyFile.Close();

	if (print_rec) {
		NewsAgencyFile.Open(filename, ios::in);
		for (int i = 0; i < 10; i++) {
			NewsAgency s;
			NewsAgencyFile.Read(s);
			cout << s << endl;
		}
		NewsAgencyFile.Close();
	}
}


int agencySearch(char *filename, string input_id, int what_id, int flag) {

	int search_res;
	bool find_flag = false;



	if (true) {
		string indfile = filename;
		indfile = indfile.substr(0, indfile.length() - 4);
		char indname[20], key[50];
		strcpy(indname, indfile.c_str());
		strcpy(key, input_id.c_str());


		DelimFieldBuffer buf('|', STDMAXBUF);
		TextIndexedFile<NewsAgency> StdIdxedFile(buf, 13, MAXAGENCY);
		StdIdxedFile.Open(indname);

		NewsAgency tmpm;

		search_res = StdIdxedFile.Read(key, tmpm);
		if (search_res != -1) {	// found.
			if (flag == 0) {
				cout << endl << tmpm << endl;
			}
			find_flag = 1;
		}
	}
	else {
		DelimFieldBuffer buffer('|', STDMAXBUF);
		RecordFile <NewsAgency> AgencyFile(buffer);
		NewsAgency s;
		AgencyFile.Open(filename, ios::in);

		while (true) {
			search_res = AgencyFile.Read(s);

			if (search_res == -1) break;

			if (!strcmp(s.get_id(what_id).c_str(), input_id.c_str())) {
				if (flag == 0) {
					cout << endl << s << endl;
				}
				find_flag = true;
				break;
			}
		}
		AgencyFile.Close();
	}


	if (search_res == -1) {
		if (flag == 0) {
			cout << "There is no ID : " << input_id << "!!\n\n";
		}
		find_flag = false;
	}

	if (find_flag) return IDFOUND;
	else return IDNOTFOUND;
}

int insertAgency(string id) {
	NewsAgency newAgency(id.c_str());
	string buf;

	if (id.size() != 12) {
		cout << "Check sizeof Id" << endl;
		return -1;
	}


	cout << "Input name : ";
	cin >> buf;
	newAgency.update_name(buf);

	cout << "Input address : ";
	cin >> buf;
	newAgency.update_address(buf);


	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <NewsAgency> AgencyFile(buffer);
	AgencyFile.Open("FileOfNewsAgency.dat", ios::out | ios::app);
	if (AgencyFile.Write(newAgency) == -1) {
		cout << "Write Error!" << endl;
	}
	AgencyFile.Close();

	// append to index file

	DelimFieldBuffer buffer1('|', STDMAXBUF);
	TextIndexedFile<NewsAgency> AgencyIdxedFile(buffer1, LENNEWSID, MAXAGENCY);
	AgencyIdxedFile.Open("fileOfNewsAgency");

	AgencyIdxedFile.Append(newAgency);
	AgencyIdxedFile.Close();

	return 0;
}
int deleteAgency(string id, int flag) {
	NewsAgency m;
	NewsAgency *agencylist = new NewsAgency[MAXAGENCY];


	int idx = 0;
	bool delete_flag = false;


	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <NewsAgency> AgencyFile(buffer);
	AgencyFile.Open("FileOfNewsAgency.dat", ios::in);



	while (AgencyFile.Read(m) != -1) {
		if (strcmp(m.get_id(0).c_str(), id.c_str())) {
			agencylist[idx++] = m;
		}
		else {
			delete_flag = true;
		}
	}
	AgencyFile.Close();



	if (delete_flag == false) return -1;

	AgencyFile.Create("FileOfNewsAgency.dat", ios::out | ios::trunc);
	for (int i = 0; i < idx; i++) {
		int recaddr;
		if ((recaddr = AgencyFile.Write(agencylist[i])) == -1) {
			cout << "Write Error!" << endl;
		}
	}
	AgencyFile.Close();

	delete[]agencylist;
	if (flag == 0)	// delete method. otherwise, update
		deleteSubscript(id, 201);


	// make index file agian
	makeAgencyIdx();

	return 0;
}
int modifyAgency(string id) {
	NewsAgency updateAgency(id.c_str());
	string buf;

	if (id.size() != 12) {
		cout << "Check sizeof Id" << endl;
		return -1;
	}

	cout << "Input name : ";
	cin >> buf;
	updateAgency.update_name(buf);

	cout << "Input address : ";
	cin >> buf;
	updateAgency.update_address(buf);

	deleteAgency(id, 1);

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <NewsAgency> AgencyFile(buffer);
	AgencyFile.Open("FileOfNewsAgency.dat", ios::out | ios::app);
	if (AgencyFile.Write(updateAgency) == -1) {
		cout << "Write Error!" << endl;
	}
	AgencyFile.Close();

	// make index file agian
	makeAgencyIdx();

	return 0;
}


void makeAgencyIdx() {
	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <NewsAgency> NewsAgencyFile(buffer);
	char filename[25] = "fileOfNewsAgency.dat";

	TextIndex NewsAgencyIndex(MAXAGENCY);

	NewsAgencyFile.Open(filename, ios::in);
	while (1) {		// 학생 데이터를 읽어서 인덱스를 생성
		NewsAgency s;
		int recaddr = NewsAgencyFile.Read(s);
		if (recaddr == -1) break;
		NewsAgencyIndex.Insert(s.Key(), recaddr);

	}
	NewsAgencyFile.Close();

	// 생성한 인덱스를 인덱스 파일로 저장

	char idxname[25] = "fileOfNewsAgency.ind";

	TextIndexBuffer idxbuf(LENNEWSID, MAXAGENCY);
	BufferFile MemIdxFile(idxbuf);
	MemIdxFile.Create(idxname, ios::out | ios::trunc);
	MemIdxFile.Rewind(); //헤더 다음의 첫번째 레코드 위치로 이동
	idxbuf.Pack(NewsAgencyIndex);
	int result = MemIdxFile.Write();
	MemIdxFile.Close();
}