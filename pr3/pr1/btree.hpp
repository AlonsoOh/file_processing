//btree.tc
#include "btnode.h"
#include "indbuff.h"
#include "btree.h"
#include <iostream>

const int MaxHeight = 5;
template <class keyType>
BTree<keyType>::BTree(int order, int keySize, int unique)
: Buffer (1+2*order,sizeof(int)+order*keySize+order*sizeof(int)),
	BTreeFile (Buffer), Root (order)
{
	Height = 1;
	Order = order;
	PoolSize = MaxHeight*2;
	Nodes = new BTNode * [PoolSize];
	BTNode::InitBuffer(Buffer,order);
	Nodes[0] = &Root;
}

template <class keyType>
BTree<keyType>::~BTree()
{
	Close();
	delete Nodes;
}

template <class keyType>
int BTree<keyType>::Open (char * name, int mode)
{
	int result;
	result = BTreeFile.Open(name, mode);
	if (!result) return result;
	// load root
	BTreeFile.Read(Root);
	Height = 1; // find height from BTreeFile!
	return 1;
}

template <class keyType>
int BTree<keyType>::Create (char * name, int mode)
{
	int result;
	result = BTreeFile.Create(name, mode);
	if (!result) return result;
	// append root node
	result = BTreeFile.Write(Root);
	Root.RecAddr=result;
	return result != -1;	
}

template <class keyType>
int BTree<keyType>::Close ()
{
	int result;
	result = BTreeFile.Rewind();
	if (!result) return result;
	result = BTreeFile.Write(Root);
	if (result==-1) return 0;
	return BTreeFile.Close();
}


template <class keyType>
int BTree<keyType>::Insert (const keyType key, const int recAddr)
{
	int result; int level = Height-1; 
	int newLargest=0; keyType prevKey, largestKey; 
	BTNode * thisNode, * newNode, * parentNode;
	thisNode = FindLeaf (key);

	// test for special case of new largest key in tree
	if (key > thisNode->LargestKey())
		{newLargest = 1; prevKey=thisNode->LargestKey();}

	result = thisNode -> Insert (key, recAddr);

	// handle special case of new largest key in tree
	if (newLargest)
		for (int i = 0; i<Height-1; i++) 
		{
			Nodes[i]->UpdateKey(prevKey,key);
			if (i>0) Store (Nodes[i]);
		}

	while (result==-1) // if overflow and not root
	{
		//remember the largest key
		largestKey=thisNode->LargestKey();
		// split the node
		newNode = NewNode();
		thisNode->Split(newNode);
		Store(thisNode); Store(newNode);
		level--; // go up to parent level
		if (level < 0) break;
		// insert newNode into parent of thisNode
		parentNode = Nodes[level];
		result = parentNode->UpdateKey
			(largestKey,thisNode->LargestKey());
		result = parentNode->Insert
			(newNode->LargestKey(),newNode->RecAddr);
		thisNode=parentNode;
	}
	Store(thisNode);
	if (level >= 0) return 1;// insert complete
	// else we just split the root
	int newAddr = BTreeFile.Append(Root); // put previous root into file
	// insert 2 keys in new root node
	Root.Keys[0]=thisNode->LargestKey();
	Root.RecAddrs[0]=newAddr;
	Root.Keys[1]=newNode->LargestKey();
	Root.RecAddrs[1]=newNode->RecAddr;
	Root.NumKeys=2; 
	Height++;
	return 1;	
}

template <class keyType>
int BTree<keyType>::Remove (const keyType key, const int recAddr)
{
	BTNode *thisNode, *parentNode;
	BTNode *merge_tmp, *split_tmp;
	thisNode = FindLeaf(key);		//find where node to delete key
	int level = Height - 1;
	bool largekey_delete = false;

	keyType last_largekey = thisNode->LargestKey();
	// if delete key is delete node's largest key 
	if (last_largekey == key) {
		largekey_delete = true;
	}


	// delete key and recaddr
	// res is -1, it is underflow so merge to memory utilization
	int res = thisNode->Remove(key, recAddr);


	if (largekey_delete) {	// delete key is largest key

		for (int i = 0; i < Height - 1; i++) {
			// if parents' node have deleted key, change to new largestkey
			Nodes[i]->UpdateKey(last_largekey, thisNode->LargestKey());
			if (i > 0) Store(Nodes[i]);
		}
	}
	Store(thisNode);	// store changed node


	// in delete node's key is underflow
	while (res == -1 && level - 1 >= 0) {

		int sibling_num = Nodes[level - 1]->NumKeys;	// search parent and find my sibling's number

		keyType deleteLargekey, updateLargekey;
		int i;
		parentNode = Nodes[level - 1];
		for (i = 0; i < sibling_num; i++) {
			// if delete_node and merge_tmp is same, break;
			if ((keyType)parentNode->Keys[i] == last_largekey) {
				break;
			}
		}

		bool merge_success = false;
		bool left_flag = false;
		// if underflow node can merge with left node?
		if (i > 0 && merge_success == false) {
			merge_tmp = Fetch(Nodes[level - 1]->RecAddrs[i - 1]);
			updateLargekey = merge_tmp->LargestKey();
			deleteLargekey = thisNode->LargestKey();
			if (merge_tmp->Merge(thisNode)) {
				merge_success = true;
				thisNode = merge_tmp;
				left_flag = true;
			}
		}
		// if underflow node can merge with right node?
		if (i + 1 < sibling_num && merge_success == false) {
			merge_tmp = Fetch(Nodes[level - 1]->RecAddrs[i + 1]);
			updateLargekey = thisNode->LargestKey();
			deleteLargekey = merge_tmp->LargestKey();
			if (thisNode->Merge(merge_tmp)) {
				merge_success = true;
			}
		}



		// if success merge, change parent's key and addr
		if (merge_success) {

			level--; // go up to parent level
			if (level < 0) break;
			// insert newNode into parent of thisNode

			parentNode = Nodes[level];

			res = parentNode->Remove(deleteLargekey);
			if (left_flag) parentNode->UpdateKey(updateLargekey, deleteLargekey);
			if (!left_flag) parentNode->UpdateKey(updateLargekey, deleteLargekey);

			Store(thisNode);
			Store(parentNode);
			thisNode = parentNode;

			// for parent is underflow too
			continue;
		}

		{
			bool split_success = false;

			// if underflow node can merge with left node?
			if (i > 0 && split_success == false) {
				split_tmp = Fetch(Nodes[level - 1]->RecAddrs[i - 1]);
				deleteLargekey = split_tmp->LargestKey();

				keyType dkey = deleteLargekey, nowlargekey;
				int daddr = split_tmp->Search(dkey);


				// remove key and insert underflow node
				split_tmp->Remove(dkey, daddr);
				nowlargekey = split_tmp->LargestKey();

				thisNode->Insert(dkey, daddr);


				for (int k = 0; k < level; k++) {
					// if parents' node have deleted key, change to new largestkey
					Nodes[k]->UpdateKey(dkey, nowlargekey);
					if (k > 0) Store(Nodes[k]);
				}
				Store(thisNode);
				Store(split_tmp);


			}
			// if underflow node can merge with right node?
			if (i + 1 < sibling_num && split_success == false) {
				split_tmp = Fetch(Nodes[level - 1]->RecAddrs[i + 1]);
				deleteLargekey = thisNode->LargestKey();

				keyType dkey = split_tmp->Keys[0], nowlargekey;
				int daddr = split_tmp->Search(dkey);

				// remove key and insert underflow node
				split_tmp->Remove(dkey, daddr);
				thisNode->Insert(dkey, daddr);

				nowlargekey = thisNode->LargestKey();

				for (int k = 0; k < level; k++) {
					// if parents' node have deleted key, change to new largestkey
					Nodes[k]->UpdateKey(dkey, nowlargekey);
					if (k > 0) Store(Nodes[k]);
				}

				Store(thisNode);
				Store(split_tmp);
			}
		}
		break;
	}

	return 0;
}

template <class keyType>
int BTree<keyType>::Search (const keyType key, const int recAddr)
{
	BTNode * leafNode;
	leafNode = FindLeaf (key); 
	return leafNode -> Search (key, recAddr);
}

template <class keyType>
void BTree<keyType>::Print (ostream & stream) 
{
	stream << "BTree of height "<<Height<<" is "<<endl;
	Root.Print(stream);
	if (Height>1)
		for (int i = 0; i<Root.numKeys(); i++)
		{
			Print(stream, Root.RecAddrs[i], 2);
		}
	stream <<"end of BTree"<<endl;
}

template <class keyType>
void BTree<keyType>::Print 
	(ostream & stream, int nodeAddr, int level) 
{
	BTNode * thisNode = Fetch(nodeAddr);
	stream<<"Node at level "<<level<<" address "<<nodeAddr<<' ';
	thisNode -> Print(stream);
	if (Height>level)
	{
		level++;
		for (int i = 0; i<thisNode->numKeys(); i++)
		{
			Print(stream, thisNode->RecAddrs[i], level);
		}
		stream <<"end of level "<<level<<endl;
	}
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::FindLeaf (const keyType key)
// load a branch into memory down to the leaf with key
{
	int recAddr, level;
	for (level = 1; level < Height; level++)
	{
		recAddr = Nodes[level-1]->Search(key,-1,0);//inexact search
		Nodes[level]=Fetch(recAddr);
	}
	return Nodes[level-1];
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::NewNode ()
{// create a fresh node, insert into tree and set RecAddr member
	BTNode * newNode = new BTNode(Order);
	int recAddr = BTreeFile . Append(*newNode);
	newNode -> RecAddr = recAddr;
	return newNode;
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::Fetch(const int recaddr)
{// load this node from File into a new BTreeNode
	int result;
	BTNode * newNode = new BTNode(Order);
	result = BTreeFile.Read (*newNode, recaddr);
	if (result == -1) return NULL;
	newNode -> RecAddr = result;
	return newNode;
}

template <class keyType>
int BTree<keyType>::Store (BTreeNode<keyType> * thisNode)
{
	return BTreeFile.Write(*thisNode, thisNode->RecAddr);
}
