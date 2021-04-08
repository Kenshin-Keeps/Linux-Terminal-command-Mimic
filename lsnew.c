#include<iostream>
#include<bits/stdc++.h>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<errno.h>
#include<string.h>
#include<time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <sysexits.h>
using namespace std;

void printPriority(struct stat statinfo)
{
    mode_t mode = statinfo.st_mode;
    //Judging the file type
    cout << (S_ISDIR(mode) ? 'd' : '-');

    //Determine USR permissions
    cout << (mode & S_IRUSR ? 'r' : '-');
    cout << (mode & S_IWUSR ? 'w' : '-');
    cout << (mode & S_IXUSR ? 'x' : '-');

    //Determine GRP permissions
    cout << (mode & S_IRGRP ? 'r' : '-');
    cout << (mode & S_IWGRP ? 'w' : '-');
    cout << (mode & S_IXGRP ? 'x' : '-');

    //Judgment of OTH authority
    cout << (mode & S_IROTH ? 'r' : '-');
    cout << (mode & S_IWOTH ? 'w' : '-');
    cout << (mode & S_IXOTH ? 'x' : '-');

    cout << " ";
}

void printOwner(struct stat statinfo)
{
    struct passwd *passwdinfo = getpwuid(statinfo.st_uid);
    cout << passwdinfo->pw_name << " ";
    passwdinfo = getpwuid(statinfo.st_gid);
    cout << passwdinfo->pw_name << " ";
}

void printSize(struct stat statinfo)
{
    cout << setw(6) << statinfo.st_size << " ";
}

//Print Time
void printTime(struct stat statinfo)
{
    time_t rawtime = statinfo.st_mtime;
    struct tm *timeinfo = localtime(&rawtime);
    cout << timeinfo->tm_mon + 1 << "Month " << timeinfo->tm_mday << "Day ";
    if (timeinfo->tm_hour < 9)
        cout << "0" << timeinfo->tm_hour << ":";
    else
        cout << timeinfo->tm_hour << ":";
    
    if (timeinfo->tm_min < 9)
        cout << "0" << timeinfo->tm_min << " ";
    else
        cout << timeinfo->tm_min << " ";
}

void printName(const char* name)
{
    cout << name << " ";
}


void newls(const char *dir, int op_a, int op_l, int op_t){
	struct dirent *d, *dr;
	DIR *dirhead = opendir(dir);
	vector<pair<time_t, string>>vec;
	if(!dirhead){
		if(errno=ENOENT){
			perror("Dictionary doesn't exist");
		}
		else{
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	while((d=readdir(dirhead))!=NULL){
		if(op_a) cout<<d->d_name<<"\t"; 
		else if(op_l && d->d_name[0] !='.'){
			struct stat statinfo;
        		stat(d->d_name, &statinfo);
        		printPriority(statinfo);
        		printOwner(statinfo);
        		printSize(statinfo);
        		printTime(statinfo);
        		printName(d->d_name);
        		cout<<endl; 
		}
		else if(d->d_name[0] !='.'){
			struct stat statinfo;
        		stat(d->d_name, &statinfo);
        		time_t rawtime = statinfo.st_mtime;
        		vec.push_back(make_pair(rawtime, d->d_name));
        		
		}
	}
	if(op_t){
		sort(vec.begin(), vec.end());
		reverse(vec.begin(), vec.end());
		for(int i =0; i<vec.size();i++){
			cout<<vec[i].second<<"\t";
		}
	}
	else{
		for(int i =0; i<vec.size();i++){
			cout<<vec[i].second<<"\t";
		}
	}
}


int main( int argc, const char *argv[])
{
	if(argc==1) newls(".",0,0,0);
	else if(argc==2){
		if(argv[1][0] == '-'){
			int op_a=0, op_l =0, op_t=0;
			char *op = (char*)(argv[1]+1);
			while(*op){
				if(*op=='a') op_a = 1;
				else if(*op=='l') op_l = 1;
				else if(*op=='t') op_t = 1;
				else{
					perror("Option not available");
					exit(EXIT_FAILURE);
				}
				op++;
			}
			newls(".", op_a, op_l, op_t);
		}
	}
	return 0;
	
}
