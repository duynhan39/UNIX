#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <iomanip>
#include <pwd.h>
#include <grp.h>
#include <map>
#include <algorithm>

using namespace std;

struct FileID {
    string fileName;
    struct dirent *dirP;
};

int lsl (string dir);
string uid_name( uid_t uid );
string gid_name( gid_t gid );
char dataSize(double &size);

bool compare (FileID i, FileID j) { return (i.fileName<j.fileName); }

int lsl (string dir)
{
    DIR *dp;
    struct dirent *dirpO;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    vector<FileID> fileList;

    while ((dirpO = readdir(dp)) != NULL) {
        FileID temp;
        temp.fileName = dirpO->d_name;
        temp.dirP = dirpO;
        fileList.push_back(temp);
    }
    sort(fileList.begin(), fileList.end(), compare);
    
    for(auto each:fileList) {
        struct dirent *dirp = each.dirP;
        
        if(dirp->d_name[0] == '.')
            continue;
        
        struct stat info;
        string const path = dir+"/"+dirp->d_name;
        stat(path.c_str(), &info);
        int mode = info.st_mode;
        string permission = "----------";
        
        if ( S_ISDIR(mode) )  permission[0] = 'd';
        if ( S_ISCHR(mode) )  permission[0] = 'c';    // char devices
        if ( S_ISBLK(mode) )  permission[0] = 'b';    // block device
        
        if ( mode & S_IRUSR ) permission[1] = 'r';
        if ( mode & S_IWUSR ) permission[2] = 'w';
        if ( mode & S_IXUSR ) permission[3] = 'x';
        
        if ( mode & S_IRGRP ) permission[4] = 'r';
        if ( mode & S_IWGRP ) permission[5] = 'w';
        if ( mode & S_IXGRP ) permission[6] = 'x';
        
        if ( mode & S_IROTH ) permission[7] = 'r';
        if ( mode & S_IWOTH ) permission[8] = 'w';
        if ( mode & S_IXOTH ) permission[9] = 'x';
        
        double size = (double)info.st_size;
        char unit = dataSize(size);
        if(size > 10)
            size = (int) size;
        else {
            size = size*10.0;
            size = (int) size;
            size/=10.0;
        }
        cout
        <<permission
        <<" "<< setw(2) << info.st_nlink
        <<" "<< setw(7) << uid_name(info.st_uid)
        <<" "<< setw(6) << gid_name(info.st_gid)
        <<" "<< setw(5) << size << unit
        <<" "<< 4+ctime(info->st_mtime)
        
        <<" "<< string(dirp->d_name)
        <<endl;
        
        
    }
    closedir(dp);
    return 0;
}

string uid_name( uid_t uid ) {
    struct passwd *pwd;
    if ((pwd = getpwuid(uid)) != NULL)
        return pwd->pw_name;
    return "";
}

string gid_name( gid_t gid ) {
    struct group *grp;
    if ((grp = getgrgid(gid)) != NULL)
        return grp->gr_name;
    return "";
}

char dataSize(double &size) {
    string name = "BKMGTPEZY";
    
    int index=0;
    while(size > 1000) {
        size/=1000;
        index++;
    }
    return name[index];
}

int main(int argc, char* argv[])
{
    string dir = string(".");
    
    if(argc == 1)
        dir = ".";
    else
        dir = argv[1];
    if(dir[dir.length()-1]=='/')
        dir = dir.substr(0, dir.length()-1);
    
    lsl(dir);
    
    return 0;
}
