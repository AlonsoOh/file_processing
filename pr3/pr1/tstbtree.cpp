//tstbtree.cc
#include "btnode.h"
#include "btree.h"
#include <iostream>

using namespace std;

const char * keys="CSDTAMPIBWNGURKEHOLJYQZFXV";

const int BTreeSize = 4;
int main (int argc, char ** argv)
{
	int result, i;
	BTree <char> bt (BTreeSize);
	result = bt.Create ((char *)"testbt.dat",ios::in|ios::out | ios::trunc);
	if (!result){cout<<"Please delete testbt.dat"<<endl;return 0;}
	for (i = 0; i<26; i++)
	{
		cout<<"Inserting "<<keys[i]<<endl;
		result = bt.Insert(keys[i],i);
		bt.Print(cout);
	}
	bt.Search(1,1);
	return 1;
}


