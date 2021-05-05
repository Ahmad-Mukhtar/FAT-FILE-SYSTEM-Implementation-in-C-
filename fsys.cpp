#include<iostream>
#include<stdlib.h>
#include<fstream>
#include <experimental/filesystem>
#include <jsoncpp/json/json.h>
#include<jsoncpp/json/value.h>
#include "fsys.h"
#include <ctime>
#include<queue>
#include <vector>
#include<sstream> 
#include <stdio.h>
#include<dirent.h>

using namespace std;


FileSys::FileSys()
{
    sysName="";
    rootpath="";
	int usedBlocks=0;
	int freeblocks=0;
    int blocksize=0;
	currentnode=new DirectoryTree;


}
int FileSys::createFileSys(string name,int bsize,string path)
{
	sysName=name;
	blocksize=bsize;
	rootpath=path;
	sysName.append(".json");
	freeblocks=1000;
	TotalDatablocks=1000;
	currentpath=rootpath;
	currentworkingdirectory=name;
	usedBlocks=0;
	totaldrivesize=TotalDatablocks*blocksize;
	cout<<"File system created "<<endl;
	return 0;

}
string FileSys::getname()
{
	return sysName;
}
string FileSys::getcurrentpath()
{
	return currentpath;
}
string FileSys::getpath()
{
	return rootpath;
}
bool FileSys::checkpath(string path)
{
	 DIR *d;
    if((d=opendir(path.c_str()))!=NULL)
    {
		
        closedir(d);
		return true;
    }
    else
    {
		return false;
    }

}
bool FileSys::deletedirectory(string path)
{
	if(!experimental::filesystem::remove_all(path.c_str()))
	{	
		
		return false;
	}
	else
	{
	
		return true;
	}
	
}
void FileSys::deletedirectoryvector(string n)
{
	string s;
		if(currentpath==rootpath)
		{
			s=currentpath;
			s.append("/"+n);
			for(int i=0;i<rootfolders.size();i++)
			{
				if(rootfolders[i].name==n)
				{
					
					for(int i=0;i<fcb.size();i++)
						{
							if(fcb[i].parentpath==s)
								{

									usedBlocks-=fcb[i].fatsize;
									freeblocks+=fcb[i].fatsize;
									totaldrivesize+=fcb[i].fatsize *blocksize;
									fcb.erase(fcb.begin()+i);
								}
			
						}
					rootfolders.erase(rootfolders.begin()+i);
				}
			}
		}
		else
		{
			for(int i=0;i<currentnode->innerfolders.size();i++)
			{
				if(currentnode->innerfolders[i].name==n)
				{
					
					for(int j=0;j<fcb.size();j++)
						{
							if(fcb[j].parentpath==currentnode->innerfolders[i].path)
								{
									fcb.erase(fcb.begin()+j);
								}
			
						}
					currentnode->innerfolders.erase(currentnode->innerfolders.begin()+i);
				}
			}
		}
		
		
}
bool FileSys::checkiffilesysexists(string name)
{
	name.append(".json");
	cout<<name<<endl;
	ifstream f(name.c_str());
    return f.good();
}
int FileSys::openFileSys(string name)
{
	Json::Value obj;
	
	name.append(".json");
	ifile.open(name.c_str(),ios::binary);
	

	ifile>>obj;
	sysName=obj["DriveInfo"]["fileinfo"]["sysName"].asString();
	blocksize=obj["DriveInfo"]["fileinfo"]["blocksize"].asInt();
	rootpath=obj["DriveInfo"]["fileinfo"]["rootpath"].asString();
	freeblocks=	obj["DriveInfo"]["fileinfo"]["freeblocks"].asInt();
	TotalDatablocks=obj["DriveInfo"]["fileinfo"]["TotalDatablocks"].asInt();
	usedBlocks=obj["DriveInfo"]["fileinfo"]["UsedBlocks"].asInt();
	currentpath=rootpath;
	currentworkingdirectory=name;
	totaldrivesize=TotalDatablocks*blocksize;
	ifile.close();
	FCB f;
	Json::Value ff;
	for(Json::ValueConstIterator itt=obj["FCB"].begin();itt!=obj["FCB"].end();itt++)
	{
		ff=*itt;
		f.name=ff["name"].asString();
		f.pathoffile=ff["pathoffile"].asString();
		f.parentpath=ff["parentpath"].asString();
		f.size=ff["size"].asInt();
		f.fatsize=ff["fatsize"].asInt();
		fcb.push_back(f);
	}
	queue <DirectoryTree> temp;
	queue <DirectoryTree*> temp1;
	DirectoryTree tmp;
	Json::Value tem;
	for(Json::ValueConstIterator itr=obj["DirectoryTree"].begin();itr!=obj["DirectoryTree"].end();itr++)
    {
		tem=*itr;
		tmp.name=tem["name"].asString();
		tmp.parentfolder=tem["Parentfolder"].asString();
		tmp.path=tem["Path"].asString();
		tmp.datecreation=tem["DateCreation"].asString();
		tmp.type=tem["type"].asString();

		if(tmp.parentfolder==rootpath)
			{
				rootfolders.push_back(tmp);
			}
		else
		{
			temp.push(tmp);
		}
        
    }
	DirectoryTree *ins=new DirectoryTree;
	for(int i=0;i<rootfolders.size();i++)
	{
		ins=&rootfolders[i];

		while(!temp.empty())
		{
			int tsize=temp.size();
			for(int j=0;j<tsize;j++)
			{
				if(!temp.empty()){
					tmp=temp.front();
					temp.pop();
				}
				if(ins->path==tmp.parentfolder)
				{
	
					ins->innerfolders.push_back(tmp);
					
				}
				else
				{
					temp.push(tmp);
				}
				
				
			}
			
			for(int k=0;k<ins->innerfolders.size();k++)
			{
				temp1.push(&ins->innerfolders[k]);
			}
			if(!temp1.empty())
			{
				ins=temp1.front();
				temp1.pop();
			}
		}
	}
	return 0;
}
void FileSys::lscommand(string path)
{
	bool flag=false;
		for(int i=0;i<rootfolders.size();i++)
		{
			if(rootfolders[i].parentfolder==currentpath)
			{
				cout<<rootfolders[i].name<<"    ";
				flag=true;
			}
			
		}

		for(int i=0;i<fcb.size();i++)
		{
			if(fcb[i].parentpath==currentpath)
			{
				cout<<fcb[i].name<<"   ";
			}
		}
		
		if(flag==false)
		{
			for (int i = 0; i < currentnode->innerfolders.size(); i++)
			{
	
				if(currentnode->innerfolders[i].parentfolder==currentpath)
				{
				cout<<currentnode->innerfolders[i].name<<" ";
				}

			}
			cout<<endl;
		}
		else
		{
			cout<<endl;
		}
		
	
	}
void FileSys::importcommand(string source,string Dest)
{
	experimental::filesystem::path sourcefile=source;
	FCB f;
	f.name=sourcefile.filename();
	f.pathoffile=source;
	f.pathoffile.append("/"+f.name);
	f.parentpath=Dest;
	f.size=getfilesize(source);
	f.fatsize=0;
	if(f.size>totaldrivesize)
	{
		cout<<"Not Enough Space "<<endl;
		return;
	}

	bool check=false;
	for(int i=0;i<f.size/blocksize;i++)			//check how many blocks are consumed by this file
	{
		f.fatsize++;
		check=true;
	}
	if(!check)			//if only 1 block is consumed
	{
		f.size++;
	}
	int totalblocksconsumed=f.fatsize*blocksize;
	totaldrivesize-=totalblocksconsumed;
	usedBlocks+=f.fatsize;
	freeblocks-=f.fatsize;
	fcb.push_back(f);
}
void  FileSys::cdcommand(string foldername)
{
	for(int i=0;i<rootfolders.size();i++)
	{
		if(rootfolders[i].name==foldername)
		{
			currentnode=&rootfolders[i];
			currentpath.append("/"+foldername);
			return;
		}
	
	}

	for (int i = 0; i < currentnode->innerfolders.size(); i++)
	{
		if(currentnode->innerfolders[i].name==foldername)
		{
			currentnode=&currentnode->innerfolders[i];
			currentpath.append("/"+foldername);
			return;
		}
	}

	return;
	
}
void FileSys::adddirectoryinfo(string n)
{
	time_t now = time(0);
   char* d = ctime(&now);
	dt.datecreation=d;
	dt.type="Folder";
	dt.parentfolder=currentpath;
	string p=currentpath;
	dt.path=p.append("/"+n);
	dt.name=n;

	if(currentpath==rootpath)
	{
		rootfolders.push_back(dt);
	}
	else
	{
		currentnode->innerfolders.push_back(dt);
	}
	
	
}
int FileSys::getfilesize(string path)
{
   ifstream in_file(path, ios::binary);
   in_file.seekg(0, ios::end);
   int file_size = in_file.tellg();
   return file_size;
}
void FileSys::writedrive()
{
	Json::Value obj;
	remove(sysName.c_str());
	
	obj["DriveInfo"]["fileinfo"]["sysName"]=sysName;
	obj["DriveInfo"]["fileinfo"]["blocksize"]=blocksize;
	obj["DriveInfo"]["fileinfo"]["freeblocks"]=freeblocks;
	obj["DriveInfo"]["fileinfo"]["rootpath"]=rootpath;
	obj["DriveInfo"]["fileinfo"]["TotalDatablocks"]=TotalDatablocks;
	obj["DriveInfo"]["fileinfo"]["UsedBlocks"]=usedBlocks;
	obj["DriveInfo"]["fileinfo"]["totaldrivesize"]=totaldrivesize;


	int l=0;
	string a,b;
	for(int i=0;i<fcb.size();i++)
	{
		a="file"+to_string(l+1);
		++l;
		obj["FCB"][a]["name"]=fcb[i].name;
		obj["FCB"][a]["parentpath"]=fcb[i].parentpath;
		obj["FCB"][a]["pathoffile"]=fcb[i].pathoffile;
		obj["FCB"][a]["size"]=fcb[i].size;
		obj["FCB"][a]["fatsize"]=fcb[i].fatsize;
	}
	

	

	int k=0;
	for(int i=0;i<rootfolders.size();i++)
	{
		queue<DirectoryTree> q;
		q.push(rootfolders[i]);
		obj["DirectoryTree"][rootfolders[i].name]["name"]=rootfolders[i].name;
		obj["DirectoryTree"][rootfolders[i].name]["type"]=rootfolders[i].type;
		obj["DirectoryTree"][rootfolders[i].name]["DateCreation"]=rootfolders[i].datecreation;
		obj["DirectoryTree"][rootfolders[i].name]["Parentfolder"]=rootfolders[i].parentfolder;
		obj["DirectoryTree"][rootfolders[i].name]["Path"]=rootfolders[i].path;


		while (!q.empty())
		{
			dt=q.front();
			q.pop();
			for(int j=0;j<dt.innerfolders.size();j++)
			{
				b="Folder"+to_string(k+1);
				++k;
				
				obj["DirectoryTree"][b]["name"]=dt.innerfolders[j].name;
				obj["DirectoryTree"][b]["type"]=dt.innerfolders[j].type;
				obj["DirectoryTree"][b]["DateCreation"]=dt.innerfolders[j].datecreation;
				obj["DirectoryTree"][b]["Parentfolder"]=dt.innerfolders[j].parentfolder;
				obj["DirectoryTree"][b]["Path"]=dt.innerfolders[j].path;
				q.push(dt.innerfolders[j]);

				
			}
			
		}
	}
	
	
	
	ofile.open(sysName.c_str(),ios::out);
	ofile<<obj;
	ofile.close();
}
FileSys::~FileSys()
{	
	delete currentnode;
}

