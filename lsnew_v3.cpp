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
    cout << statinfo.st_nlink << " ";
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
    switch(timeinfo->tm_mon){
    	case 0:
    		cout<<"Jan ";
    		break;
    	case 1:
    		cout<<"Feb ";
    		break;
    	case 2:
    		cout<<"Mar ";
    		break;
    	case 3:
    		cout<<"Apr ";
    		break;
    	case 4:
    		cout<<"May ";
    		break;
    	case 5:
    		cout<<"Jun ";
    		break;
    	case 6:
    		cout<<"Jul ";
    		break;
    	case 7:
    		cout<<"Aug ";
    		break;
    	case 8:
    		cout<<"Sep ";
    		break;
    	case 9:
    		cout<<"Oct ";
    		break;
    	case 10:
    		cout<<"Nov ";
    		break;
    	case 11:
    		cout<<"Dec ";
    		break;
    	default:
    		cout<<"Invalid ";
    		break;
    }
    //cout << timeinfo->tm_mon<< "Month "; 
    cout << timeinfo->tm_mday << " ";
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

int getBlockSize(const char* directory) {
   int size = 0;

   DIR *d;
   struct dirent *dir;
   struct stat fileStat;
   d = opendir(directory);
   if (d) {
       while ((dir = readdir(d)) != NULL) {
           if (dir->d_name[0] != '.') { 
               // Create the path to stat
               char info_path[PATH_MAX + 1];
               strcpy(info_path, directory);
               if (directory[strlen(directory) - 1] != '/')
                   strcat(info_path, "/");
               strcat(info_path, dir->d_name);

               stat(info_path, &fileStat);

               size += fileStat.st_blocks;
           }
       }
   }

   return size/2;
}


void newls(const char *dir, int op_a, int op_l, int op_t){
	struct dirent *d, *dr;
	DIR *dirhead = opendir(dir);
	vector<pair<time_t, string>>vec;
	int size = 0;
	if(!dirhead){
		if(errno=ENOENT){
			perror("Dictionary doesn't exist");
		}
		else{
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	if(op_l){
		size = getBlockSize(".");
		cout << "total " << size;
		cout << "\n"; 
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