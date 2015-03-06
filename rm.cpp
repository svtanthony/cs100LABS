#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>	
#include <cstring>
#include <dirent.h>		//directory entry
#include <errno.h>
#include <iostream>	
#include <string>
#include <sys/ioctl.h>	//terminal size
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <stdio.h>

using namespace std;

bool recursive = 0;

vector<string> variables(int argc,char **argv);
void deletion(string s);

int main(int argc, char *argv[])
{
	vector<string> files = variables(argc, argv);

	for(int i(0) ; i < files.size() ; ++i)
	{
		string s = files[i];
		deletion(s);
	}


	return 0;
}

void deletion(string s)
{
	DIR *pDir;
	struct dirent *file;
	struct stat fileinfo;
	string s2;

	if(stat(s.c_str(),&fileinfo))//bad path
	{
		s2 = "cannot access ";
		s2 += s;
		perror(s2.c_str());
	}
	else // valid path
	{
		if(S_ISDIR(fileinfo.st_mode))//working with file
		{
			if(recursive)
			{
				if(!(pDir = opendir(s.c_str()))){
					s2 = "cannot access ";
					s2 += s;
					perror(s2.c_str());
				}
				else
				{//have open dir
					vector<string> tmp;
					while(file = readdir(pDir))
						tmp.push_back(file->d_name);
			
					struct { const char* d_name;} filea, *file=&filea;	
					string prevPath = s;
					prevPath.append("/");
					for(int f(0); f < tmp.size(); ++f)
					{
						tmp[f].insert(0,prevPath);
						file->d_name = tmp[f].c_str();
	
						if(stat(file->d_name,&fileinfo))//bad path
						{
							s2 = "cannot access ";
							s2 += s;
							perror(s2.c_str());
						}
						else
						{
							string currentDir = prevPath, prevDir = prevPath;
							currentDir.append(".");
							prevDir.append("..");
							
							if(!strcmp(file->d_name,currentDir.c_str()) || !strcmp(file->d_name,prevDir.c_str()))//************************************************
								continue;
							deletion(tmp[f]);
						}
					}
	
					if((closedir(pDir))){//close error
						s2 = "cannot close ";
						s2 += s;
						perror(s2.c_str());
					}
				}
			

				if(rmdir(s.c_str()))
				{
					string s2 = "cannot delete ";
					s2 += s;
					perror(s2.c_str());
				}
			}
			else
				cout << "this is a directory, pass in appropirate flag to delete\n";
		}
		else
			if(-1 == unlink(s.c_str()))
				perror("can't delete");
	}
	
}

vector<string> variables(int argc,char **argv)//sets flags and determines paths
{	
	vector<string> pathVar;	

	for(int i(1); i < argc ; i++)
	if(argv[i][0] == '-' && argv[i][1] != '\0'){//flag passed in, extract all flags
		for(int j(1); argv[i][j] != '\0'; j++)
			switch(argv[i][j])
			{
			case 'r' :
				recursive = 1;
				break;
			default:
				cout << "invalid option -- '" << argv[i][j] << "'\n"
				<< "The available flags is {r}\n";
				exit(1);
			}
	}
	else //path was given
	{
		pathVar.push_back(argv[i]);
	}
	return pathVar;
}

