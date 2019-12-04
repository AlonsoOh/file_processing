#pragma once

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

#define ADMIN_MODE		1
#define USER_MODE		2

string datFileName[4] = {
	"NULL",
	"FileOfMember.dat",
	"FileOfNewsAgency.dat",
	"FileOfSubscription.dat"
};

using namespace std;

void showScreen();



bool User_Deleted = false;





void NewsAgencySubscriptionSystem();

void showSystemScreen(int mode);
void showSystemLogin();
void showLoginScreen(int);

void recordSearch(int mode, string user_id);
void recordInsert(int mode, string user_id);
void recordDelete(int mode, string user_id);
void recordModify(int mode, string user_id);

int selectRecfile(char *bt_key, bool bt_flag);
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
	int login_mode;
	int login_level;

	if (!dat_made) {
		MemberTest(false);
		NewsAgencyTest(false);
		SubscriptionTest(false);
		dat_made = true;

		makeMemberIdx();
		makeAgencyIdx();
		makeSubscriptionIdx();
	}



	while (true) {
		showSystemLogin();
		cin >> input;
		login_mode = atoi(input.c_str());

		system("cls");

		if (login_mode == 3) return;
		else if (login_mode == ADMIN_MODE || login_mode == USER_MODE) {
			showLoginScreen(login_mode);
			break;
		}
	}

	string input_id, input_pw;


	cout << "ID : ";
	cin >> input_id;
	cout << "PW : ";
	cin >> input_pw;

	login_level = memLogin(input_id, input_pw, 0);

	if (login_level == -1) {
		cout << "-------------------------------------------" << endl;
		cout << "|*******        Wrong id/pw!        *******|" << endl;
		cout << "-------------------------------------------" << endl;
		return;
	}
	if (login_mode == ADMIN_MODE && login_level != 1) {
		cout << "-------------------------------------------" << endl;
		cout << "|*******   This id is not admin!    *******|" << endl;
		cout << "-------------------------------------------" << endl;
		return;
	}

	if (login_mode == USER_MODE && login_level == 1) {
		cout << "-------------------------------------------" << endl;
		cout << "|*******     Admin id submitted     *******|" << endl;
		cout << "|*******   Progress in user mode.   *******|" << endl;
		cout << "-------------------------------------------" << endl;
	}

	

	while (true) {
		showSystemScreen(login_mode);
		cin >> input;
		menu_num = atoi(input.c_str());
		cout << endl;

		system("cls");

		switch (menu_num) {
		case 1: recordSearch(login_mode, input_id); break;
		case 2: recordInsert(login_mode, input_id); break;
		case 3: recordDelete(login_mode, input_id); break;
		case 4: recordModify(login_mode, input_id); break;
		case 5: return;
		default: break;
		}

		if (User_Deleted) {
			User_Deleted = false;
			return;
		}
	}
	return;
}

void showSystemLogin() {
	cout << "-------------------------------------------" << endl;
	cout << "|******* 1. Admin mode             *******|" << endl;
	cout << "|******* 2. User mode              *******|" << endl;
	cout << "|******* 3. Exit                   *******|" << endl;
	cout << "-------------------------------------------" << endl;
	cout << "input : ";
}

void showSystemScreen(int mode) {
	if (mode == ADMIN_MODE) {
		cout << "-------------------------------------------" << endl;
		cout << "|******* 1. record search          *******|" << endl;
		cout << "|******* 2. record insert          *******|" << endl;
		cout << "|******* 3. record delete          *******|" << endl;
		cout << "|******* 4. record modify          *******|" << endl;
		cout << "|******* 5. Exit                   *******|" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "input : ";
	}
	else if (mode == USER_MODE) {
		cout << "-------------------------------------------" << endl;
		cout << "|******* 1. search                 *******|" << endl;
		cout << "|******* 2. insert/ regist         *******|" << endl;
		cout << "|******* 3. delete/ withdraw       *******|" << endl;
		cout << "|******* 4. modify/ update         *******|" << endl;
		cout << "|******* 5. Exit                   *******|" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "input : ";
	}
}

void showLoginScreen(int mode) {
	if (mode == ADMIN_MODE) {
		cout << "-------------------------------------------" << endl;
		cout << "|*******         ADMIN MODE         *******|" << endl;
		cout << "-------------------------------------------" << endl;
	}
	else if (mode == USER_MODE) {
		cout << "-------------------------------------------" << endl;
		cout << "|*******         USER MODE          *******|" << endl;
		cout << "-------------------------------------------" << endl << endl;
	}
}

void recordSearch(int mode, string user_id) {
	int where_file;
	int what_id = -1;
	char bt_key = '\0';
	where_file = selectRecfile(&bt_key, true);

	int bt_flag = 0;


	if (where_file == 0) return;
	else if (where_file == SUBSCRIPTFILE && mode == ADMIN_MODE) {

		if (true) {
			bt_flag = 1;
		}
		else {
			what_id = findByWhatId();
			if (what_id == 0) return;
		}
	}

	

	string input_id;

	char filename[30] = " ";
	strcpy(filename, (datFileName[where_file]).c_str());

	/*PROJ3*/
	//search by bt tree
	if (mode == ADMIN_MODE && bt_flag == 1 && where_file == SUBSCRIPTFILE) {
		bt_searchSubscriptionIdx(bt_key, 0);
	}
	else if (mode == ADMIN_MODE) {
		cout << "Find Id :   ";
		cin >> input_id;
	}
	else if (mode == USER_MODE) {
		if (where_file == MEMFILE) {
			cout << "-------------------------------------------" << endl;
			cout << "|You can't search member file in user mode.|" << endl;
			cout << "-------------------------------------------" << endl << endl;
			return;
		}
		else if (where_file == AGENCYFILE) {
			cout << "Find Id :   ";
			cin >> input_id;
		}
		else if (where_file == SUBSCRIPTFILE) {
			input_id = user_id;
			what_id = MEMID;
		}
	}




	if (where_file == MEMFILE) {
		memSearch(filename, input_id, what_id, 0);
	}
	else if (where_file == AGENCYFILE) {
		agencySearch(filename, input_id, what_id, 0);
	}
	else if (where_file == SUBSCRIPTFILE && bt_flag == 0) {
		subscriptionSearch(filename, input_id, what_id, 0, user_id, 1);
	}

}



void recordInsert(int mode, string user_id) {
	int where_file;
	int what_id = -1;
	char bt_key = '\0';
	where_file = selectRecfile(&bt_key, false);

	if (where_file == 0) return;
	int res = 0;

	string input_id;
	char filename[30] = " ";
	strcpy(filename, (datFileName[where_file]).c_str());


	if (mode == ADMIN_MODE) {
		cout << "Find Id :   ";
		cin >> input_id;
	}
	else if (mode == USER_MODE) {
		if (where_file == MEMFILE) {
			cout << "-------------------------------------------" << endl;
			cout << "|   You can't insert member in user mode.   |" << endl;
			cout << "-------------------------------------------" << endl << endl;
			return;
		}
		else if (where_file == AGENCYFILE) {
			cout << "-------------------------------------------" << endl;
			cout << "| You can't insert newsagency in user mode.|" << endl;
			cout << "-------------------------------------------" << endl << endl;
			return;
		}
		else if (where_file == SUBSCRIPTFILE) {
			cout << "Find Id: ";
			cin >> input_id;
		}
	}

	int idfound = 0;

	if (where_file == MEMFILE) idfound = memSearch(filename, input_id, what_id, 1);
	else if (where_file == AGENCYFILE) idfound = agencySearch(filename, input_id, what_id, 1);
	else if (where_file == SUBSCRIPTFILE) idfound = subscriptionSearch(filename, input_id, what_id, 1, user_id, 1);		// Insert new subscription. so, find all view. not only user_id's subscript id.

	if (idfound == IDFOUND) {
		cout << "Already exist id" << endl << endl;
		return;
	}

	if (where_file == MEMFILE) res = insertMember(input_id);
	else if (where_file == AGENCYFILE) res = insertAgency(input_id);
	else if (where_file == SUBSCRIPTFILE) res = insertSubscript(input_id, user_id, mode);


	if (res == 0) cout << "Insert done.\n\n";
}

void recordDelete(int mode, string user_id) {
	int where_file;
	int what_id = -1;
	char bt_key = '\0';

	bool bt_flag = true;
	where_file = selectRecfile(&bt_key, true);

	if (where_file == 0) return;

	string input_id;
	char filename[30] = " ";
	strcpy(filename, (datFileName[where_file]).c_str());

	/*PROJ3*/
	//delete by bt tree
	if (where_file == SUBSCRIPTFILE && bt_flag == true && mode==ADMIN_MODE) {
		bt_deleteSubscriptionIdx(bt_key, -1);

		return;
	}
	else if (mode == ADMIN_MODE) {
		cout << "Find Id :   ";
		cin >> input_id;
	}
	else if (mode == USER_MODE) {
		if (where_file == MEMFILE) {

			while (true) {
				string tmpinput;
				cout << "-------------------------------------------" << endl;
				cout << "|  Are you agree with member withdrawal?   |" << endl;
				cout << "|******* 1. Y                       *******|" << endl;
				cout << "|******* 2. N                       *******|" << endl;
				cout << "-------------------------------------------" << endl;
				cout << "input : ";
				cin >> tmpinput;
				int tmpnum = atoi(tmpinput.c_str());

				system("cls");
				if (tmpnum == 1) { input_id = user_id; break; }
				else if (tmpnum == 2) return;
			}

			User_Deleted = true;
		}
		else if (where_file == AGENCYFILE) {
			cout << "-------------------------------------------" << endl;
			cout << "| You can't delete newsagency in user mode.|" << endl;
			cout << "-------------------------------------------" << endl << endl;
			return;
		}
		else if (where_file == SUBSCRIPTFILE) {
			cout << "Find Id :   ";
			cin >> input_id;
		}
	}

	int idfound = 0;

	if (where_file == MEMFILE) idfound = memSearch(filename, input_id, what_id, 1);
	else if (where_file == AGENCYFILE) idfound = agencySearch(filename, input_id, what_id, 1);
	else if (where_file == SUBSCRIPTFILE) idfound = subscriptionSearch(filename, input_id, what_id, 1,user_id, mode);

	if (idfound == IDNOTFOUND) {
		cout << "Cannot found id" << endl << endl;
		return;
	}

	if (where_file == MEMFILE) deleteMember(input_id, 0);
	else if (where_file == AGENCYFILE) deleteAgency(input_id, 0);
	else if (where_file == SUBSCRIPTFILE) deleteSubscript(input_id, 0);
	
	cout << "Delete done!\n\n";
}

void recordModify(int mode, string user_id) {
	int where_file;
	int what_id = -1;
	char bt_key = '\0';
	where_file = selectRecfile(&bt_key, true);

	if (where_file == 0) return;
	int res = 0;
	bool bt_flag = true;

	string input_id;
	char filename[30] = " ";
	strcpy(filename, (datFileName[where_file]).c_str());


	/*PROJ3*/
	//modify by bt tree
	if (where_file == SUBSCRIPTFILE && bt_flag == true && mode == ADMIN_MODE) {

		bt_modifySubscriptionIdx(bt_key, -1);
		return;
	}
	else if (mode == ADMIN_MODE) {
		cout << "Find Id :   ";
		cin >> input_id;
	}
	else if (mode == USER_MODE) {
		if (where_file == MEMFILE) {
			input_id = user_id;
		}
		else if (where_file == AGENCYFILE) {
			cout << "-------------------------------------------" << endl;
			cout << "| You can't modify newsagency in user mode.|" << endl;
			cout << "-------------------------------------------" << endl << endl;
			return;
		}
		else if (where_file == SUBSCRIPTFILE) {
			cout << "Find Id :   ";
			cin >> input_id;
		}
	}

	int idfound = 0;

	if (where_file == MEMFILE) idfound = memSearch(filename, input_id, what_id, 1);
	else if (where_file == AGENCYFILE) idfound = agencySearch(filename, input_id, what_id, 1);
	else if (where_file == SUBSCRIPTFILE) idfound = subscriptionSearch(filename, input_id, what_id, 1, user_id, mode);

	if (idfound == IDNOTFOUND) {
		cout << "Cannot found id" << endl << endl;
		return;
	}

	if (where_file == MEMFILE) res = modifyMember(input_id);
	else if (where_file == AGENCYFILE) res = modifyAgency(input_id);
	else if (where_file == SUBSCRIPTFILE) res = modifySubscript(input_id, user_id, mode);

	if (res == 0) cout << "Update done!\n\n";

}

int selectRecfile(char* bt_key, bool bt_flag) {
	string input;
	int menu_num;

	char tmp_key;
	string tmp_key_in;

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

			/*PROJ3*/
			// if admin want subscription file, input key for b tree.
		case 3: 
			if (bt_flag == true) {
				cout << "-------------------------------------------" << endl;
				cout << "---------    Subscription File    ---------" << endl;
				cout << "---------        Input key        ---------" << endl;
				cout << "-------------------------------------------" << endl;
				cout << "input : ";
				cin >> tmp_key_in;

				int tt = stoi(tmp_key_in);
				*bt_key = (char)tt;

				cout << (*bt_key) << endl;
				system("cls");
			}
			return SUBSCRIPTFILE;
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