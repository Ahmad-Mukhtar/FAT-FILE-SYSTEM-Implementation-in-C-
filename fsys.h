#ifndef FSYS_H
#define FSYS_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <time.h>
#include<list>
#include <math.h>
#include<map>


using namespace std;




struct FCB {
	int size; 
    string pathoffile;
	string parentpath;
	string name;
	int fatsize; //fat table of file tells us how many datablocks are consumed by this file

};

struct DirectoryTree {
	string name;
	string type; 
	string datecreation; 
	string parentfolder;
	string path;
	vector<DirectoryTree> innerfolders;
};

class FileSys {
    string sysName;
    string rootpath;
	string currentpath;
	string currentworkingdirectory;
	ofstream ofile;
	ifstream ifile;
	int usedBlocks;
	int freeblocks;
    int blocksize;
	int TotalDatablocks;
	int totaldrivesize;
	vector <DirectoryTree> rootfolders;
	DirectoryTree dt;
	DirectoryTree *currentnode;
	vector<FCB> fcb;

	public:
		FileSys();
		int openFileSys(string name);
		int createFileSys(string name,int blocksize,string rootpath);
		bool checkiffilesysexists(string name);
		bool checkpath(string path);
		bool deletedirectory(string path);
		void readdrive(string name);
		void writedrive();
		string getpath();
		string getname();
		string getcurrentpath();
		void lscommand(string path);
		void cdcommand(string foldername);
		void importcommand(string source,string dest);
		void deletedirectoryvector(string n);
		void adddirectoryinfo(string n);
		int getfilesize(string path);


		
		~FileSys();
};

#endif
