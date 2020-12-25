#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <grp.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;
#define CTRL_KEY(k) ((k) & 0x1f)
#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))
#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))
#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108
#define KEY_BACKSPACE 0x0008
int cursorglobal,pindex=0;
string command;
vector<string> dirname;
vector<string> pathname;
char root1[200]="/home/sai/IIITHYDERABADMTECH";
char root[2000]="/home/sai/IIITHYDERABADMTECH";
struct termios orig_termios,t,ot;
//struct winsize windowstruct;
//int from=0;
//int to=0;
void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
}
vector<string> getcontentnames(char dn[2000])
{
  vector<string> contentnames;
  struct dirent *d;
  string ch;
  char temp;
  struct stat st,attr;
  DIR *dr=opendir(dn);
  while((d=readdir(dr))!=NULL)
  {
    contentnames.push_back(d->d_name);
  }
  closedir(dr);
  return contentnames;
}
bool isFile(string source)
{
  char dn[2000];
  strcpy(dn,source.c_str());
  char temp;
  struct stat st;
  stat(dn,&st);
  temp=((S_ISDIR(st.st_mode)) ? 'd' : '-');
  if(temp=='-')
  {
    return true;
  }
  return false;
}
void printcontents(char dn[2000])
{
  dirname.clear();
  cursorglobal=0;
  editorRefreshScreen();
  cout<<"\x1b[1H";
  struct dirent *d;
  string ch;
  char temp;
  struct stat st,attr;
  DIR *dr=opendir(dn);
  if(dr==NULL)
  {
    cout<<"directory cannot be openened"<<endl;
    return;
  }
  else
  {
    while((d=readdir(dr))!=NULL)
    {
             
             string filepath=root;
             filepath+="/";
             filepath+=d->d_name;

             ch.clear();
             dirname.push_back(d->d_name);
             stat(filepath.c_str(),&st);
             temp=((S_ISDIR(st.st_mode)) ? 'd' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IRUSR) ? 'r' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IWUSR) ? 'w' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IXUSR) ? 'x' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IRGRP) ? 'r' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IWGRP) ? 'w' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IXGRP) ? 'x' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IROTH) ? 'r' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IWOTH) ? 'w' : '-');
             ch.push_back(temp);
             temp=((st.st_mode & S_IXOTH) ? 'x' : '-');
             ch.push_back(temp);
             cout<<setw(25)<<left<<ch;
             cout<<setw(25)<<left<<d->d_name;
             
             cout<<setw(15)<<left<<setfill(' ')<<st.st_size;
             char *path=realpath(root,NULL);
             stat(path, &attr);
             cout<<setw(25)<<left<<setfill(' ')<<ctime(&attr.st_mtime);
             

             
        }
  }
  closedir(dr);
}
int getch()
{
    int c = 0;

    tcgetattr(0, &ot);
    memcpy(&t, &ot, sizeof(t));
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &t);
    c = getchar();
    tcsetattr(0, TCSANOW, &ot);
    return c;
}
int kbhit()
{
    int c = 0;

    tcgetattr(0, &ot);
    memcpy(&t, &ot, sizeof(t));
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &t);
    c = getchar();
    tcsetattr(0, TCSANOW, &ot);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}
int esc(void)
{
    int c;

    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

int get()
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? esc() : c;
}
void createfile(string command)
{
       string ctype,filename,dest,path,temp;
       temp=root1;
       int i;
       for(i=0;command[i]!=' ';i++)
       {
        ctype.push_back(command[i]);
       }
       i++;
       for(;command[i]!=' ';i++)
       {
        filename.push_back(command[i]);
       }
       i++;
       for(;i<command.length();i++)
       {
        dest.push_back(command[i]);
       }
       if(dest.compare(".")==0)
       {
        path=temp+"/"+filename;
       }
       else if(dest[0]=='~'&&dest[1]=='/')
       {
        dest.erase(0,1);
        path=temp+dest+"/"+filename;
       }
       int fp=open(path.c_str(),O_CREAT|O_EXCL,S_IRWXU);
       
}
void createdirectory(string command)
{
       string ctype,dirname,dest,path,temp;
       temp=root1;
       int i;
       for(i=0;command[i]!=' ';i++)
       {
        ctype.push_back(command[i]);
       }
       i++;
       for(;command[i]!=' ';i++)
       {
        dirname.push_back(command[i]);
       }
       i++;
       for(;i<command.length();i++)
       {
        dest.push_back(command[i]);
       }
       if(dest.compare(".")==0)
       {
        path=temp+"/"+dirname;
       }
       else if(dest[0]=='~'&&dest[1]=='/')
       {
        dest.erase(0,1);
        path=temp+dest+"/"+dirname;
       }
       //cout<<path;
       int result=mkdir(path.c_str(),0777);
}
void gotocommand(string command)
{
  string ctype,dirpath;
  int i;
   for(i=0;command[i]!=' ';i++)
   {
    ctype.push_back(command[i]);
   }
   i++;
   for(;i<command.length();i++)
   {
     dirpath.push_back(command[i]);
   }
   pindex++;
   pathname.erase(pathname.begin()+pindex,pathname.end());
   pathname.push_back(dirpath);
   memset(root,0,sizeof(root));
   strcpy(root,dirpath.c_str());
   printcontents(root);
}
void copydirectory(string source,string destination)
{
  //step 1 create top level directory
  char temp[100];
  string srctemp,desttemp;
  strcpy(temp,source.c_str());
  vector<string> contents=getcontentnames(temp);
  for(int i=0;i<contents.size();i++)
  {
    srctemp.clear();
    desttemp.clear();
    if(contents[i]==".")
    {
      continue;
    }
    if(contents[i]=="..")
    {
      continue;
    }
    srctemp=source;
    srctemp+="/";
    srctemp+=contents[i];
    desttemp=destination;
    desttemp+="/";
    desttemp+=contents[i];
    if(isFile(srctemp))
    {
      //copy file
      char buf[BUFSIZ];
      size_t size;
      int source = open(srctemp.c_str(), O_RDONLY, 0);
      int dest = open(desttemp.c_str(), O_WRONLY | O_CREAT , 0644);
      while ((size = read(source, buf, BUFSIZ)) > 0) {
        write(dest, buf, size);
      }

    }
    else
    {
      //copy directory
      int result=mkdir(desttemp.c_str(),0777);
      copydirectory(srctemp,desttemp);

    }
  }
}
  
void copycommand(string command)
{
  vector<string>cmd;
  string temp,temp1;
  for(int i=0;i<command.length();i++)
  {
    if(command[i]!=' ')
    {
      temp.push_back(command[i]);
    }
    else
    {
      cmd.push_back(temp);
      temp.clear();
    }
  }
  //finding source and destination
  cmd.push_back(temp);
  char src[100];
  strcpy(src,root);//src
  temp=root1;
  string cntr=cmd[cmd.size()-1];
  cntr.erase(0,1);
  string path;
  path+=temp;
  path+=cntr;
  char destination[100];
  strcpy(destination,path.c_str());//destination
  for(int i=1;i<cmd.size()-1;i++)
  {
        temp.clear();
        temp=src;
        temp+="/";
        temp+=cmd[i];
        temp1.clear();
        temp1=destination;
        temp1+="/";
        temp1+=cmd[i];
     if(isFile(temp))
     {
        
        //copying files
        char buf[BUFSIZ];
        size_t size;
        int source = open(temp.c_str(), O_RDONLY, 0);
        int dest = open(temp1.c_str(), O_WRONLY | O_CREAT , 0644);
        while ((size = read(source, buf, BUFSIZ)) > 0) {
          write(dest, buf, size);
        }
        //cout<<temp<<"   "<<temp1<<"  ";
     }
     else
     {
      //copying directory recursively
      //first we will make a directory at the destination path using src and dest
      //mkdir command
        
        int result=mkdir(temp1.c_str(),0777);
        copydirectory(temp,temp1);


     }
  }
  cout<<"copying done";
}
void deletefile(string command)
{
  //parse the command
       string ctype,path,dest,temp;
       temp=root1;
       int i;
       for(i=0;command[i]!=' ';i++)
       {
        ctype.push_back(command[i]);
       }
       i++;
       for(;i<command.length();i++)
       {
        dest.push_back(command[i]);
       }
       if(dest[0]=='~'&&dest[1]=='/')
       {
        dest.erase(0,1);
        path=temp+dest;
       }
       char relpath[100];
       strcpy(relpath,path.c_str());
       int ans=remove(relpath);
        if(ans==0)
        {
          cout<<"deleted succesfully";
        }
        else
        {
          cout<<"not deleted";
        }




}
bool isEmpty(string source)
{
   char tempr[100];
   strcpy(tempr,source.c_str());
   vector<string>contents=getcontentnames(tempr);
   if(contents.size()==2)
   {
    return true;
   }
   return false;
}
void deleterecursively(string source)
{
  char tempr[100];
  string temp;
  strcpy(tempr,source.c_str());
  vector<string>contents=getcontentnames(tempr);
  //delete recursively
  for(int i=0;i<contents.size();i++)
  {
    temp.clear();
    if(contents[i]==".")
    {
      continue;
    }
    if(contents[i]=="..")
    {
      continue;
    }
    //deleting a file
        temp=source;
        temp+="/";
        temp+=contents[i];
    if(isFile(temp))
    {
        
        char path[100];
        strcpy(path,temp.c_str());
        int ans=remove(path);
    }
    else//deleting a directory
    {
        //if directory is empty we will delete otherwise we get into directory
       char path[100];
       strcpy(path,temp.c_str());
        if(!(isEmpty(temp)))//directory is not empty
        {
           deleterecursively(temp);
        }
        int ans=remove(path);

    }
  }

}
void deletedirectory(string command)
{
  string ctype,path,dest,temp;
   temp=root1;
   int i;
   for(i=0;command[i]!=' ';i++)
   {
    ctype.push_back(command[i]);
   }
   i++;
   for(;i<command.length();i++)
   {
    dest.push_back(command[i]);
   }
   if(dest[0]=='~'&&dest[1]=='/')
   {
    dest.erase(0,1);
    path=temp+dest;
   }
   //after generation of path delete recursively
   deleterecursively(path);
   char pathr[100];
   strcpy(pathr,path.c_str());
   int ans=remove(pathr);
   cout<<"directory deleted succesfully";

}
void movecommand(string command)
{
  vector<string>cmd;
  string temp,temp1,temp2;
  for(int i=0;i<command.length();i++)
  {
    if(command[i]!=' ')
    {
      temp.push_back(command[i]);
    }
    else
    {
      cmd.push_back(temp);
      temp.clear();
    }
  }
  cmd.push_back(temp);
  char src[100];
  strcpy(src,root);//src
  temp=root1;
  string cntr=cmd[cmd.size()-1];
  cntr.erase(0,1);
  string path;
  path+=temp;
  path+=cntr;
  char destination[100];
  strcpy(destination,path.c_str());
  for(int i=1;i<cmd.size()-1;i++)
  {
        temp.clear();
        temp1.clear();
        temp=src;
        temp+="/";
        temp+=cmd[i];
        temp1=destination;
        temp1+="/";
        temp1+=cmd[i];
    if(isFile(temp))
     {
        
        //copying files
        char buf[BUFSIZ];
        size_t size;
        int source = open(temp.c_str(), O_RDONLY, 0);
        int dest = open(temp1.c_str(), O_WRONLY | O_CREAT , 0644);
        while ((size = read(source, buf, BUFSIZ)) > 0) {
          write(dest, buf, size);
        }
        //deleting file
        char srcpath[100];
        strcpy(srcpath,temp.c_str());
        int ans=remove(srcpath);
        
      }
      else//moving a directory
      {
        //temp source and temp1 destination
        //copying directory
        int result=mkdir(temp1.c_str(),0777);
        copydirectory(temp,temp1);
        //deleting directory
        deleterecursively(temp);
        char pathr[100];
        strcpy(pathr,temp.c_str());
        int ans=remove(pathr);

           
      }


  }
  cout<<"moving files and directories is done succesfully";
}

bool searchrecursively(string source,string dest)
{
  char tempr[500];
  string temp;
  strcpy(tempr,source.c_str());
  vector<string> contents=getcontentnames(tempr);
  bool result=false;
  for(int i=0;i<contents.size();i++)
  {
     if(contents[i]=="."||contents[i]=="..")
     {
      continue;
     }
     if(contents[i].compare(dest)==0)
     {
        return true;
     }
     temp.clear();
     temp=source;
     temp+="/";
     temp+=contents[i];
     if(isFile(temp))
     {
       continue;
     }
     result=result||searchrecursively(temp,dest);
     
  }
  return result;
}
void searchcommand(string command)
{
   string ctype,dest,temp;
   temp=root;
   int i;
   for(i=0;command[i]!=' ';i++)
   {
    ctype.push_back(command[i]);
   }
   i++;
   for(;i<command.length();i++)
   {
    dest.push_back(command[i]);
   }
   //cout<<ctype<<" "<<temp<<" "<<destination;
   if(searchrecursively(temp,dest))
   {
    cout<<"True";
   }
   else
   {
    cout<<"False";
   }
   
}
void renamecommand(string command)
{
  string ctype,oldname,newname;
  int i;
   for(i=0;command[i]!=' ';i++)
   {
    ctype.push_back(command[i]);
   }
   i++;
   for(;command[i]!=' ';i++)
   {
    oldname.push_back(command[i]);
   }
   i++;
   for(;i<command.length();i++)
   {
    newname.push_back(command[i]);
   }
   //cout<<ctype<<oldname<<newname;
   string source=root;
   source+="/";
   source+=oldname;
   string dest=root;
   dest+="/";
   dest+=newname;
   //cout<<source<<"  "<<dest;
   if(rename(source.c_str(),dest.c_str())==0)
   {
    cout<<"file renamed succesfully";
   }
   else
   {
    cout<<"file not renamed";
   }


}

void commandmode()
{
  int c;
  while(1)
  {
    c=get();
    if(c == 'q')
    {
      command.clear();
      return;
    }
    else if(c ==  KEY_ENTER)
    {
       string ctype;
       for(int i=0;command[i]!=' ';i++)
       {
        ctype.push_back(command[i]);
       }
       if(ctype.compare("create_file")==0)
       {
         createfile(command);
       }
       if(ctype.compare("create_dir")==0)
       {
        createdirectory(command);
       }
       if(ctype.compare("goto")==0)
       {
        gotocommand(command);
       }
       if(ctype.compare("copy")==0)
       {
        copycommand(command);
       }
       if(ctype.compare("delete_file")==0)
       {
        //deletefile 
        deletefile(command);
       }
       if(ctype.compare("delete_dir")==0)
       {
        //deletedirectory
        deletedirectory(command);
       }
       if(ctype.compare("move")==0)
       {
         movecommand(command);
       }
       if(ctype.compare("search")==0)
       {
          searchcommand(command);
       }
       if(ctype.compare("rename")==0)
       {
        renamecommand(command);
       }
       cursorglobal--;
       command.clear();
       cout<<endl;

    }
    else if(c == '\177')
    {
      command.pop_back();
      printf("\33[2K\r");
      cout<<command;
    }
    else
    {
      command.push_back(c);
      cout<<command[command.length()-1];
    }
  }
}
int main() {
  printcontents(root);
  pathname.push_back("/home/sai/IIITHYDERABADMTECH");
  int c;
  while(1)
  {
    c=get();
    if(c == KEY_UP)
    {
      cursorglobal++;
      cursorupward(1);
      
    }
    if(c == KEY_DOWN)
    {
      cursorglobal--;
      cursordownward(1);
      
    }
    if(c == KEY_ENTER)
    {
       int index=dirname.size()-cursorglobal;
       string temp=root;
       temp+="/";
       temp+=dirname[index];
       if(isFile(temp))
       {
          char tp[100];
          strcpy(tp,temp.c_str());
         pid_t pid;
         pid=fork();
         if(pid==0)//child process
         {
          execlp("gedit","gedit",tp,NULL);

         }
         else
         {
          wait(NULL);
         }
       }
       else
       {
       pindex++;
       pathname.erase(pathname.begin()+pindex,pathname.end());
       pathname.push_back(temp);
       memset(root,0,sizeof(root));
       strcpy(root,temp.c_str());
       printcontents(root);
       }
    }
    if(c == KEY_LEFT)
    {
       
      if(pindex<pathname.size()-1)
      {
        pindex++;
        string temp=pathname[pindex];
        memset(root,0,sizeof(root));
        strcpy(root,temp.c_str());
         printcontents(root);
      }
    }
    if(c == KEY_RIGHT)
    {
       if(pindex>0)
       {
          pindex--;
          string temp=pathname[pindex];
          memset(root,0,sizeof(root));
          strcpy(root,temp.c_str());
          printcontents(root);
       }   
    }
    if(c == '\177')
    {
      if(pindex>0)
      {
          pathname.erase(pathname.begin()+pindex,pathname.end());
          pindex--;
          string temp=pathname[pindex];
          memset(root,0,sizeof(root));
          strcpy(root,temp.c_str());
          printcontents(root);

      }
    }
    if(c == 'H')
    {
          pathname.clear();
          pindex=0;
          pathname.push_back("/home/sai");
          string temp=pathname[0];
          memset(root,0,sizeof(root));
          strcpy(root,temp.c_str());
          printcontents(root);

    }
    if(c==':')
    {
      command.clear();
      commandmode();
    }
  }
  return 0;
}
