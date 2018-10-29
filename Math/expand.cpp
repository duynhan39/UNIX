#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <stdio.h>
#include <algorithm>

using namespace std;

string merge(string line, string pow, bool isPow);
string process(string input);

bool isOp(char c)
{   return c=='+' || c=='-' || c=='*' || c=='^'; }

int main(int argc, char* argv[])
{
  if(argc==1)
    while(1)
      {
	cout<<"> ";
            
	string input;
	cout<<"\e[1m";
	getline(cin, input);
	cout<<"\e[0m";
	if(input == "quit")
	  break;
   
	string out=process(input);
	cout<<out<<endl;
      }
  else
    {
      for(int i=1; i<argc; i++)
        {
	  string input(argv[i]);
	  cout<<input<<" = "<<process(input)<<endl;
        }
    }
    
  return 0;
}

string process(string input)
{
  string inTemp = tmpnam(nullptr);
  string outTemp = tmpnam(nullptr);
    
  ofstream inF(inTemp);
  inF<<"expand("<<input<<");"<<endl;
  inF.close();
    
  string str = "cat "+inTemp+" | maxima -q > "+outTemp;
    
  char command[str.size() + 1];
  strcpy(command, str.c_str());
    
  system(command);
    
  string line;
  ifstream outF(outTemp);
    
  getline(outF, line);
    
  getline(outF, line);
  line = line.substr(5);
    
  string pow;
  bool isPow=false;
  if(line[1]!='(')
    {
      isPow=true;
      pow=line.substr(7);
        
      getline(outF, line);
    }
    
  line=line.substr(6);
    
  string out;
    
  out+=merge(line, pow, isPow);
    
  while(getline(outF, line))
    {
      isPow=false;
      pow="";
      if(line[0]=='(')
	break;
      for(int i=0; i<line.size(); i++)
	if(line[i]!=' ')
	  {
	    if(!isOp(line[i]))
	      {
		isPow=true;
		pow = line;
		getline(outF, line);
	      }
                
	    break;
	  }
      out+=merge(line, pow, isPow);
    }
    
  outF.close();
    
  char inC[inTemp.size() + 1];
  strcpy(inC, inTemp.c_str());
  char outC[outTemp.size() + 1];
  strcpy(outC, outTemp.c_str());
    
  remove(inC);
  remove(outC);
    
  return out;
}

string mark(string line)
{
  bool isMul=false;
  for(int i=0; i<line.size(); i++){
    if(isMul && line[i]!=' ' && line[i-1]==' ' && !isOp(line[i]))
      line[i-1]='*';
        
    if(isOp(line[i]))
      isMul=false;
    else if(line[i]!=' ')
      isMul=true;
  }
  return line;
}

string merge(string line, string pow, bool isPow)
{
  string out;
  for(int i=0; i<max(line.size(), pow.size()); i++)
    {
      if(i<pow.size() && isPow && pow[i]!=' ')
        {
	  if(pow[i-1]==' ')
            {
	      if(pow[i-2]==' ')
		{
		  out+="^";
		}
	      out+=' ';
            }
	  out+=pow[i];
        }
      if(i<line.size() && line[i]!=' ')
        {
	  if(line[i-1]==' ')
	    out+=' ';
	  out+=line[i];
        }
    }
  string tem;
  tem=mark(out);
  out="";
  for(int i=0; i<tem.size(); i++)
    if(tem[i]!=' ')
      out+=tem[i];
    
  return out;
}

