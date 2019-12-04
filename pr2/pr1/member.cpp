#include "member.h"
#include "subscription.h"
#include <sstream>
#include <fstream>


Member::Member() {
	this->level[0] = (char)9;
	this->level[1] = '\0';

	this->mileage[0] = '\0';
}
Member::Member(const string new_id) {
	update_id(new_id);
}

Member::Member(const Member & s) {
	update_id(s.id);
	update_password(s.password);
	update_name(s.name);
	update_phoneNum(s.phoneNum);
	update_address(s.address);
	update_mileage(s.mileage);
	update_level((int)s.level[0]);
}

Member & Member::operator = (const Member & s) {
	update_id(s.id);
	update_password(s.password);
	update_name(s.name);
	update_phoneNum(s.phoneNum);
	update_address(s.address);
	update_mileage(s.mileage);
	update_level((int)s.level[0]);

	return *this;
}


bool Member::operator == (const Member & s) {
	if (this->id != s.id) {
		return false;
	}
	if (this->password != s.password) {
		return false;
	}
	if (this->name != s.name) {
		return false;
	}
	if (this->phoneNum != s.phoneNum) {
		return false;
	}
	if (this->address != s.address) {
		return false;
	}
	if (strcmp(this->mileage, s.mileage)) {
		return false;
	}


	return true;
}
bool Member::operator != (const Member & s) {
	if (this->id != s.id) {
		return true;
	}
	if (this->password != s.password) {
		return true;
	}
	if (this->name != s.name) {
		return true;
	}
	if (this->phoneNum != s.phoneNum) {
		return true;
	}
	if (this->address != s.address) {
		return true;
	}
	if (strcmp(this->mileage, s.mileage)) {
		return true;
	}


	return false;
}

istream & operator >> (istream & is, Member & s) { // input

	string token;

	getline(is, token, '|');
	s.update_id(token);
	getline(is, token, '|');
	s.update_password(token);
	getline(is, token, '|');
	s.update_name(token);
	getline(is, token, '|');
	s.update_phoneNum(token);
	getline(is, token, '|');
	s.update_address(token);
	getline(is, token, '\n');
	s.update_mileage(token.c_str());
	



	
	return is;
}
ostream & operator << (ostream & os, Member & s) {  // output
	cout << "ID            : " << s.id << endl;
	cout << "PASSWORD      : " << s.password<< endl;
	cout << "NAME          : " << s.name << endl;
	cout << "PHONE NUMBER  : " << s.phoneNum << endl;
	cout << "ADDRESS       : " << s.address << endl;
	cout << "MILEAGE       : " << s.mileage << endl;
	cout << "LEVEL         : " << (int)s.level[0] << endl;
	return os; 
}


void Member::update_id(const string new_id) { this->id = new_id; }
void Member::update_password(const string new_password) { this->password = new_password; }
void Member::update_name(const string new_name) { this->name = new_name; }
void Member::update_phoneNum(const string new_phoneNum) { this->phoneNum = new_phoneNum; };
void Member::update_address(const string new_address) { this->address = new_address; };
void Member::update_mileage(const char* new_mileage) { strcpy(this->mileage, new_mileage); };
void Member::update_level(const int new_level) { this->level[0] = (char)new_level; }

int Member::get_size() {
	return id.size() + password.size() + name.size() + phoneNum.size() + address.size() + strlen(mileage);
}

int Member::get_level() {
	return (int)this->level[0];
}

bool Member::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	string s_mileage(mileage, LENMIL);
	string s_level(level, LENLEV);

	numBytes = Buffer.Pack(id.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(password.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(name.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(phoneNum.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(address.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(s_mileage.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(s_level.c_str());
	if (numBytes == -1) return false;

	return true;
}

bool Member::Unpack(IOBuffer & Buffer) {
	int numBytes;
	char buf[STDMAXBUF];
	

	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	id = buf;

	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	password = buf;
	
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	name = buf;
	
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	phoneNum = buf;
	
	numBytes = Buffer.Unpack(buf);
	if (numBytes == -1) return false;
	address = buf;
	
	numBytes = Buffer.Unpack(mileage, LENMIL);
	if (numBytes == -1) return false;
	
	numBytes = Buffer.Unpack(level, LENLEV);
	if (numBytes == -1) return false;

	return true;
}

void showMember() {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		exit(-1);
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int i = 0; i < 10; i++) {
		Member s;
		ifs >> s;
		cout << s << endl;
	}

	ifs.close();

	return;
}

void MemberTest(bool print_rec) {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		exit(-1);
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);

	char filename[20] = "FileOfMember.dat";

	MemberFile.Create(filename, ios::out | ios::trunc);

	/*
	*	input admin and test user
	*/

	Member admin, testuser;
	testuser.update_id("TestUser");
	testuser.update_password("T1234");
	MemberFile.Write(testuser);

	admin.update_id("admin");
	admin.update_password("adminpass");
	admin.update_level(1);
	MemberFile.Write(admin);


	for (int i = 0; i < n; i++) {
		Member s;
		ifs >> s;
		int recaddr;
		if ((recaddr = MemberFile.Write(s)) == -1) {
			if (print_rec) cout << "Write Error!" << endl;
		}

	}
	MemberFile.Close();

	if (print_rec) {
		MemberFile.Open(filename, ios::in);
		for (int i = 0; i < 10; i++) {
			Member s;
			MemberFile.Read(s);
			cout << s << endl;
		}
		MemberFile.Close();
	}
}

int memSearch(char *filename, string input_id, int what_id, int flag) {

	int search_res;
	bool find_flag = false;

	if (true) {		// search for 
		string indfile = filename;
		indfile = indfile.substr(0, indfile.length() - 4);
		char indname[20], key[50];
		strcpy(indname, indfile.c_str());
		strcpy(key, input_id.c_str());


		DelimFieldBuffer buf('|', STDMAXBUF);
		TextIndexedFile<Member> StdIdxedFile(buf, 35, MAXMEMBER);
		StdIdxedFile.Open(indname);

		Member tmpm;

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
		RecordFile <Member> MemberFile(buffer);
		Member s;
		MemberFile.Open(filename, ios::in);

		while (true) {
			search_res = MemberFile.Read(s);

			if (search_res == -1) break;

			if (!strcmp(s.get_id(what_id).c_str(), input_id.c_str())) {
				if (flag == 0) {
					cout << endl << s << endl;
				}
				find_flag = true;
				break;
			}
		}
		MemberFile.Close();
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

// compare id and pw is really exist. if true, return level.
// else, return -1.
int memLogin(string input_id, string input_pw, int flag) {

	int search_res;
	DelimFieldBuffer buffer('|', STDMAXBUF);

	int find_level = -1;

	RecordFile <Member> MemberFile(buffer);
	Member s;
	MemberFile.Open("FileOfMember.dat", ios::in);

	while (true) {
		search_res = MemberFile.Read(s);

		if (search_res == -1) break;

		if (!strcmp(s.get_id(0).c_str(), input_id.c_str())) {
			if (!strcmp(s.get_password(0).c_str(), input_pw.c_str())) {
				find_level = s.get_level();
			}
			else {
				find_level = -1;
			}
			break;
		}
	}

	if (search_res == -1) {
		find_level = -1;
	}
	MemberFile.Close();

	return find_level;
}


int insertMember(string id) {
	Member newMember(id);
	string buf;

	cout << "Input password : ";
	cin >> buf;
	newMember.update_password(buf);

	cout << "Input name : ";
	cin >> buf;
	newMember.update_name(buf);

	cout << "Input phone number : ";
	cin >> buf;
	newMember.update_phoneNum(buf);

	cout << "Input address : ";
	cin >> buf;
	newMember.update_address(buf);

	while (true) {
		cout << "Input mileage : ";
		cin >> buf;

		if (buf.size() != LENMIL - 1) {
			cout << "Check input mileage" << endl;
			continue;
		}
		newMember.update_mileage(buf.c_str());
		break;
	}
	while (true) {
		cout << "Input level : ";
		cin >> buf;

		if (buf.size() != LENLEV - 1) {
			cout << "Check input level" << endl;
			continue;
		}
		newMember.update_level((int)buf.c_str()[0]);
		break;
	}

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	MemberFile.Open("FileOfMember.dat", ios::out | ios::app);
	if (MemberFile.Write(newMember) == -1) {
		cout << "Write Error!" << endl;
	}
	MemberFile.Close();


	// append to index file

	DelimFieldBuffer buffer1('|', STDMAXBUF);
	TextIndexedFile<Member> mmdIdxedFile(buffer1, 35, MAXMEMBER);
	mmdIdxedFile.Open("fileOfMember");

	mmdIdxedFile.Append(newMember);
	mmdIdxedFile.Close();

	return 0;
}
int deleteMember(string id, int flag) {
	Member m;
	Member *memlist = new Member[MAXMEMBER];

	int idx = 0;
	bool delete_flag = false;

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	MemberFile.Open("FileOfMember.dat", ios::in);

	while (MemberFile.Read(m) != -1){
		if (strcmp(m.get_id(0).c_str(), id.c_str())){
			memlist[idx++] = m;
		}
		else{
			delete_flag = true;
		}
	}
	MemberFile.Close();

	if (delete_flag == false) return -1;

	MemberFile.Create("FileOfMember.dat", ios::out | ios::trunc);
	for (int i = 0; i < idx; i++) {
		int recaddr;
		if ((recaddr = MemberFile.Write(memlist[i])) == -1) {
			cout << "Write Error!" << endl;
		}
	} 
	MemberFile.Close();

	delete[]memlist;
	if (flag == 0)	// delete method. otherwise, update
		deleteSubscript(id, 200);

	// make index file agian
	makeMemberIdx();

	return 0;
}
int modifyMember(string id) {
	Member updateMember(id);
	string buf;

	cout << "Update password : ";
	cin >> buf;
	updateMember.update_password(buf);

	cout << "Update name : ";
	cin >> buf;
	updateMember.update_name(buf);

	cout << "Update phone number : ";
	cin >> buf;
	updateMember.update_phoneNum(buf);

	cout << "Update address : ";
	cin >> buf;
	updateMember.update_address(buf);

	while (true) {
		cout << "Update mileage : ";
		cin >> buf;

		if (buf.size() != LENMIL - 1) {
			cout << "Check update mileage" << endl;
			continue;
		}
		updateMember.update_mileage(buf.c_str());
		break;
	}

	while (true) {
		cout << "Update level : ";
		cin >> buf;

		if (buf.size() != LENLEV - 1) {
			cout << "Check update level" << endl;
			continue;
		}
		updateMember.update_level((int)buf.c_str()[0]);
		break;
	}

	deleteMember(id, 1);


	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	MemberFile.Open("FileOfMember.dat", ios::out | ios::app);
	if (MemberFile.Write(updateMember) == -1) {
		cout << "Write Error!" << endl;
	}
	MemberFile.Close();

	// make index file agian
	makeMemberIdx();


	return 0;
}

void makeMemberIdx() {
	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	char filename[20] = "fileOfMember.dat";

	TextIndex MemberIndex(MAXMEMBER);

	MemberFile.Open(filename, ios::in);
	while (1) {	
		Member s;
		int recaddr = MemberFile.Read(s);
		if (recaddr == -1) break;
		MemberIndex.Insert(s.Key(), recaddr);
	}
	MemberFile.Close();

	char idxname[20] = "fileOfMember.ind";

	TextIndexBuffer idxbuf(35, MAXMEMBER);
	BufferFile MemIdxFile(idxbuf);
	MemIdxFile.Create(idxname, ios::out | ios::trunc);
	MemIdxFile.Rewind(); 
	idxbuf.Pack(MemberIndex);
	int result = MemIdxFile.Write();
	MemIdxFile.Close();
}