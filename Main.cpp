#include<iostream>
#include <string>
#include<stdlib.h>
#include<string.h>
#include "fsys.h"
using namespace std;





int  main(int argc, const char** argv) 
{
 string virtual_drive_name;
string path;

 cout<<"Enter the Name of new or Existing Virtual hard drive "<<endl;
 cin>>virtual_drive_name;

 


 FileSys *fs=new FileSys();;
 if(fs->checkiffilesysexists(virtual_drive_name))
 {
    system ("clear");
    fs->openFileSys(virtual_drive_name);
    if(!fs->checkpath(fs->getpath()))
    {
        mkdir(fs->getpath().c_str(),0777);
    }
 }
 else
 {
     cout<<"Drive not found Plz Creating new Drive"<<endl;
    int blockszie;
    cout<<"Enter the BlockSize "<<endl;
    cin>>blockszie;
    if(cin.fail())
    {
        cout<<"Invalid value exiting...";
        return 0;
    }
    cout<<"Enter the valid path n the following way "<<endl<<"/home/Ubuntu/Desktop"<<endl;
    cin>>path;
      if(fs->checkpath(path)==false)
    {
        cout<<"Inavlid Path Exiting.."<<endl;
        return 0;
    }
    path.append("/");
    path.append(virtual_drive_name);
     if(fs->checkpath(path)==true)
    {
        fs->deletedirectory(path);
    }
  
     if(!mkdir(path.c_str(),0777))
    {
        cout<<"DRive was unable to create "<<endl;
    }
    
   
    system ("clear");
    fs->createFileSys(virtual_drive_name,blockszie,path);
 }
cout<<"Supported Commands: "<<endl<<"ls\n"<<"rm\n"<<"mkdir\n"<<"cd\n"<<"import\n"<<"quit\n";
cout<<"Note: Only write command like cd the path will be asked after it "<<endl<<endl;
path=fs->getpath();
string cmd;
 while(true) //infinite loop
 {
     cout<<virtual_drive_name<<":~"<<fs->getcurrentpath()<<"$ ";
     cin>>cmd;
     if(cmd=="ls")
     {
         fs->lscommand(fs->getcurrentpath());
     }
     else if(cmd=="mkdir")
     {
         string n;
         cout<<"Enter Directory Name: "<<endl;
         cin>>n;
         string pth=fs->getcurrentpath().append("/"+n);
         mkdir(pth.c_str(),0777);
         fs->adddirectoryinfo(n);
         fs->writedrive();
     }
     else if(cmd=="rm")
     {
         string n;
         cout<<"Enter Directory Name: "<<endl;
         cin>>n;
         string p=fs->getcurrentpath();
         if(fs->checkpath(p.append("/"+n)))
         {
            fs->deletedirectory(p);
            fs->deletedirectoryvector(n);
            fs->writedrive();
         }
         else
         {
             cout<<"Invalid Directory "<<endl;
         }
         

     }
     else if(cmd=="cd")
     {
        string n;
        cout<<"Enter Directory Name: "<<endl;
        cin>>n;
        string p=fs->getcurrentpath();
        if(!fs->checkpath(p.append("/"+n)))
        {
            cout<<"Invalid DirectoryName "<<endl;
        }
        else
        {
            cout<<"calling"<<endl;
            fs->cdcommand(n);
        }
        
     }
     else if(cmd=="import")
     {
         string source,Dest;
         cout<<"Enter the full Path of the File like \"home/Ubuntu/Desktop/myfile.txt\" "<<endl;
         cin>>source;
         fstream f;
         f.open(source.c_str());
         if(f.fail())
         {
             cout<<"Invalid file path "<<endl;
             f.close();
         }
         else
         {
            f.close();
            cout<<"Enter the Destination path like  \"/home/Ubuntu/Desktop/yourvirtualdrivename/foldertoplacefile\" "<<endl;
            cin>>Dest;
            if(!fs->checkpath(Dest))
            {
                cout<<"Invalid Path "<<endl;
            }

            fs->importcommand(source,Dest);
            fs->writedrive();
         }

     }
     else if(cmd=="quit")
     {
         fs->writedrive();
         cout<<"Exiting...."<<endl;
         return 0;
     }
     


 }
 

delete fs;

    return 0;
}

