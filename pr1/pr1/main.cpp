#include "member.h"
#include "NewsAgency.h"
#include "subscription.h"
#include <fstream>
#include <vector>
#include "recfile.h"

#define MEMFILE			1
#define AGENCYFILE		2
#define SUBSCRIPTFILE	3

#define MEMID			200
#define AGENCYID		201
#define SUBSCRIPTID		202

#define IDFOUND			135
#define IDNOTFOUND		136

string datFileName[4] = {
	"NULL",
	"FileOfMember.dat",
	"FileOfNewsAgency.dat",
	"FileOfSubscription.dat"
};

using namespace std;

void showScreen();









void NewsAgencySubscriptionSystem();

void showSystemScreen();
void recordSearch();
void recordInsert();
void recordDelete();
void recordModify();

int selectRecfile();
int findByWhatId();





int main() {
	string input;
	int menu_num;

	while (true) {
		showScreen();
		cin >> input;
		menu_num = atoi(input.c_str());
		cout << endl;

		system("cls");

		switch (menu_num) {
		case 1: showMember(); break;
		case 2: showNewsAgency(); break;
		case 3: showSubscription(); break;
		case 4: MemberTest(true); break;
		case 5: NewsAgencyTest(true); break;
		case 6: SubscriptionTest(true); break;
		case 7: NewsAgencySubscriptionSystem(); break;
		case 8: return 0;
		default : break;
		}

	}
	return 0;
}

void showScreen() {
	cout << "-------------------------------------------------" << endl;
	cout << "|******* 1. showMember                   *******|" << endl;
	cout << "|******* 2. showNewsAgency               *******|" << endl;
	cout << "|******* 3. showSubscription             *******|" << endl;
	cout << "|******* 4. MemberTest                   *******|" << endl;
	cout << "|******* 5. NewsAgencyTest               *******|" << endl;
	cout << "|******* 6. SubscriptionTest             *******|" << endl;
	cout << "|******* 7. NewsAgencySubscriptionSystem *******|" << endl;
	cout << "|******* 8. Exit                         *******|" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "input : ";
}


void NewsAgencySubscriptionSystem() {
	string input;
	int menu_num;
	static bool dat_made = false;

	if (!dat_made) {
		MemberTest(false);
		NewsAgencyTest(false);
		SubscriptionTest(false);
		dat_made = true;
	}


	while (true) {
		showSystemScreen();
		cin >> input;
		menu_num = atoi(input.c_str());
		cout << endl;

		system("cls");

		switch (menu_num) {
		case 1: recordSearch(); break;
		case 2: recordInsert(); break;
		case 3: recordDelete(); break;
		case 4: recordModify(); break;
		case 5: return;
		default: break;
		}
	}
	return;
}

void showSystemScreen() {
	cout << "-------------------------------------------" << endl;
	cout << "|******* 1. record search          *******|" << endl;
	cout << "|******* 2. record insert          *******|" << endl;
	cout << "|******* 3. record delete          *******|" << endl;
	cout << "|******* 4. record modify          *******|" << endl;
	cout << "|******* 5. Exit                   *******|" << endl;
	cout << "-------------------------------------------" << endl;
	cout << "input : ";
}

void recordSearch() {
	int where_file;
	int what_id = -1;
	where_file = selectRecfile();

	if (where_file == 0) return;
	else if (where_file == SUBSCRIPTFILE) {
		what_id = findByWhatId();
		if (what_id == 0) return;
	}
	
	string input_id;

	char filename[30] = " ";

	cout << "Find Id :   ";
	cin >> input_id;
	strcpy(filename, (datFileName[where_file]).c_str());




	if (where_file == MEMFILE) {
		memSearch(filename, input_id, what_id, 0);
	}
	else if (where_file == AGENCYFILE) {
		agencySearch(filename, input_id, what_id, 0);
	}
	else if (where_file == SUBSCRIPTFILE) {
		subscriptionSearch(filename, input_id, what_id, 0);
	}

}



void recordInsert() {
	int where_file;
	int what_id = -1;
	where_file = selectRecfile();

	if (where_file == 0) return;
	int res = 0;

	string input_id;
	char filename[30] = " ";

	cout << "Find Id :   ";
	cin >> input_id;
	strcpy(filename, (datFileName[where_file]).c_str());

	int idfound = 0;

	if (where_file == MEMFILE) idfound = memSearch(filename, input_id, what_id, 1);
	else if (where_file == AGENCYFILE) idfound = agencySearch(filename, input_id, what_id, 1);
	else if (where_file == SUBSCRIPTFILE) idfound = subscriptionSearch(filename, input_id, what_id, 1);

	if (idfound == IDFOUND) {
		cout << "Already exist id" << endl << endl;
		return;
	}

	if (where_file == MEMFILE) res = insertMember(input_id);
	else if (where_file == AGENCYFILE) res = insertAgency(input_id);
	else if (where_file == SUBSCRIPTFILE) res = insertSubscript(input_id);


	if (res == 0) cout << "Insert done.\n\n";
}

void recordDelete() {
	int where_file;
	int what_id = -1;
	where_file = selectRecfile();

	if (where_file == 0) return;

	string input_id;
	char filename[30] = " ";

	cout << "Find Id :   ";
	cin >> input_id;
	strcpy(filename, (datFileName[where_file]).c_str());

	int idfound = 0;

	if (where_file == MEMFILE) idfound = memSearch(filename, input_id, what_id, 1);
	else if (where_file == AGENCYFILE) idfound = agencySearch(filename, input_id, what_id, 1);
	else if (where_file == SUBSCRIPTFILE) idfound = subscriptionSearch(filename, input_id, what_id, 1);

	if (idfound == IDNOTFOUND) {
		cout << "Cannot found id" << endl << endl;
		return;
	}

	if (where_file == MEMFILE) deleteMember(input_id, 0);
	else if (where_file == AGENCYFILE) deleteAgency(input_id, 0);
	else if (where_file == SUBSCRIPTFILE) deleteSubscript(input_id, 0);
	
	cout << "Delete done!\n\n";
}

void recordModify() {
	int where_file;
	int what_id = -1;
	where_file = selectRecfile();

	if (where_file == 0) return;
	int res = 0;

	string input_id;
	char filename[30] = " ";

	cout << "Find Id :   ";
	cin >> input_id;
	strcpy(filename, (datFileName[where_file]).c_str());

	int idfound = 0;

	if (where_file == MEMFILE) idfound = memSearch(filename, input_id, what_id, 1);
	else if (where_file == AGENCYFILE) idfound = agencySearch(filename, input_id, what_id, 1);
	else if (where_file == SUBSCRIPTFILE) idfound = subscriptionSearch(filename, input_id, what_id, 1);

	if (idfound == IDNOTFOUND) {
		cout << "Cannot found id" << endl << endl;
		return;
	}

	if (where_file == MEMFILE) res = modifyMember(input_id);
	else if (where_file == AGENCYFILE) res = modifyAgency(input_id);
	else if (where_file == SUBSCRIPTFILE) res = modifySubscript(input_id);

	if (res == 0) cout << "Update done!\n\n";

}

int selectRecfile() {
	string input;
	int menu_num;

	while (true) {
		cout << "-------------------------------------------" << endl;
		cout << "---------       Which file?       ---------" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "|******* 1. MemberFile              *******|" << endl;
		cout << "|******* 2. NewsAgencyFile          *******|" << endl;
		cout << "|******* 3. SubscriptionFile        *******|" << endl;
		cout << "|******* 4. go back                 *******|" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "input : ";
		cin >> input;
		menu_num = atoi(input.c_str());
		cout << endl;

		system("cls");

		switch (menu_num) {
		case 1: return MEMFILE;
		case 2: return AGENCYFILE;
		case 3: return SUBSCRIPTFILE;
		case 4: return 0;
		default: break;
		}
	}
	return 0;
}

int findByWhatId() {
	string input;
	int menu_num;

	while (true) {
		cout << "-------------------------------------------" << endl;
		cout << "---------        Which id?        ---------" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "|******* 1. MemberID                *******|" << endl;
		cout << "|******* 2. NewsAgencyID            *******|" << endl;
		cout << "|******* 3. SubscriptionID          *******|" << endl;
		cout << "|******* 4. go back                 *******|" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "input : ";
		cin >> input;
		menu_num = atoi(input.c_str());
		cout << endl;

		system("cls");

		switch (menu_num) {
		case 1: return MEMID;
		case 2: return AGENCYID;
		case 3: return SUBSCRIPTID;
		case 4: return 0;
		default: break;
		}
	}
	return 0;
}