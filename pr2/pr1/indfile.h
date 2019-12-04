#ifndef INDFILE
#define INDFILE

#include "recfile.h"
#include "tindbuff.h"
#include <sstream>
#include <string>
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

template <class RecType>
class TextIndexedFile
{public:
	int Read (RecType & record); // read next record
	int Read (char * key, RecType & record); // read by key
	int Append (RecType & record);
	int Update (char * oldKey, RecType & record);
	int Create (char * name, int mode=ios::in|ios::out);
	int Open (char * name, int mode=ios::in|ios::out);
	int Close ();
	TextIndexedFile (IOBuffer & buffer, 
		int keySize, int maxKeys = 100); 
	~TextIndexedFile (); // close and delete
	void Init();
	int Delete(RecType &record);

protected:
	TextIndex Index;
	BufferFile IndexFile;
	TextIndexBuffer IndexBuffer;
	RecordFile<RecType> DataFile;
	char * FileName; // base file name for file
	int SetFileName(char * fileName,
		char *& dataFileName, char *& indexFileName);
};

// template method bodies
template <class RecType>
int TextIndexedFile<RecType>::Read (RecType & record)
{	return DataFile . Read (record, -1); }

template <class RecType>
int TextIndexedFile<RecType>::Read (char * key, RecType & record)
{
	int ref = Index.Search(key);
	if (ref < 0) return -1;
	int result = DataFile . Read (record, ref);
	return result;
}

template <class RecType>
int TextIndexedFile<RecType>::Append (RecType & record)
{
	char * key = record.Key();
	int ref = Index.Search(key);
	if (ref != -1) // key already in file
		return -1;
	ref = DataFile . Append(record);
	int result = Index . Insert (record.Key (), ref);
	return ref;
}

template <class RecType>
int TextIndexedFile<RecType>::Update 
	(char * oldKey, RecType & record)
// Update is left as an exercise. 
//	It requires BufferFile::Update, and BufferFile::Delete
{	return -1;}


template <class RecType>
int TextIndexedFile<RecType>::SetFileName(char * fileName,
	char *& dataFileName, char *& indexFileName)
// generate names for the data file and the index file
{
	if (FileName != 0) // object is already attached to a file
		return 0;
	// set FileName member
	FileName = strdup(fileName);
	// generate real file names
	ostringstream dataName, indexName;
	dataName << FileName <<".dat"<<ends;
	indexName<< FileName<<".ind"<<ends;
	dataFileName = strdup(dataName . str().c_str ());
	indexFileName = strdup(indexName . str().c_str ());
	return 1;
}

template <class RecType>
int TextIndexedFile<RecType>::Create (char * fileName, int mode)
// use fileName.dat and fileName.ind
{
	int result;
	char * dataFileName, * indexFileName;
	result = SetFileName (fileName, dataFileName, indexFileName);
//	cout <<"file names "<<dataFileName<<" "<<indexFileName<<endl;
	if (result == -1) return 0;
	result = DataFile.Create (dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Create (indexFileName, ios::out|ios::in);
	if (!result)
	{
		DataFile . Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	return 1;
}
template <class RecType>
int TextIndexedFile<RecType>::Open (char * fileName, int mode)
// open data and index file and read index file
{
	int result;
	char * dataFileName, * indexFileName;
	result = SetFileName (fileName, dataFileName, indexFileName);
	if (!result) return 0;
	// open files
	result = DataFile.Open (dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Open (indexFileName, mode);
	if (!result)
	{
		DataFile . Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	// read index into memory
	result = IndexFile . Read ();
	if (result != -1)
	{
		result = IndexBuffer . Unpack (Index);
		if (result != -1) return 1;
	}
	// read or unpack failed!
	DataFile.Close();
	IndexFile.Close();
	FileName = 0;
	return 0;
}

template <class RecType>
int TextIndexedFile<RecType>::Close ()
{	int result;
	if (!FileName) return 0; // already closed!
	DataFile . Close();
	IndexFile . Rewind();
	IndexBuffer.Pack (Index);
	result = IndexFile . Write ();
//	cout <<"result of index write: "<<result<<endl;
	IndexFile . Close ();
	FileName = 0;
	return 1;
}

template <class RecType>
TextIndexedFile<RecType>::TextIndexedFile (IOBuffer & buffer,
		int keySize, int maxKeys)
	:DataFile(buffer), Index (maxKeys), 
	IndexFile(IndexBuffer), IndexBuffer(keySize, maxKeys)
{
	FileName = 0;
}

template <class RecType>
TextIndexedFile<RecType>::~TextIndexedFile ()
{	Close(); }





template <class RecType>
void TextIndexedFile<RecType>::Init() {

	DataFile.Rewind();
//	make index from datafile

	while (1) {
		T s;
		int recaddr = DataFile.Read(s);
		if (recaddr == -1) break;

		Index.Insert(s.Key(), recaddr);
	}
	DataFile.Rewind();


	return;
}

template <class RecType>
int TextIndexedFile<RecType>::Delete(RecType &record) {
	
	int ref = Index.Search(record.Key());
	int recAddr;

	if (ref == -1) return -1;			// don't have this record


	Index.Remove(record.Key());

	char * dataFileName, *indexFileName;
	result = SetFileName(fileName, dataFileName, indexFileName);
	if (!result) return 0;

	IndexFile.Close();
	IndexFile.Open(indexFileName, ios::out | ios::trunc);

	IndexFile.Rewind();
	IndexBuffer.Pack(Index);
	int result = IndexFile.Write(-1);
	IndexFile.Close();


	return 0;
}


#endif