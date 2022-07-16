#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <bitset>

using namespace std;
map<string,pair<string,int> > OPTAB;
map<string,pair<string,int> >::iterator ito;
map<string,int> RESTAB;
map<string,int>::iterator itr;
map<string,int>::iterator itr2;
map<string,string> SYMTAB;
map<string,string>::iterator its;
map<char,string> REGTAB;
map<char,string>::iterator itrg;

void extract(string a,string word[],int& count);
void createOPTAB();
void createRESTAB();
void createREGTAB();
int power(int b,int e);
int hextodec(string str);
string dectohex(int n);
string binary(int n);
int bintodec(string s);
string binary(int n,int f);

int main()
{
    createOPTAB();
    createRESTAB();
    createREGTAB();
    /*for(ito=OPTAB.begin();ito!=OPTAB.end();ito++)
    {
       cout<<(*ito).first<<"\t"<<(*ito).second.first<<"\t"<<(*ito).second.second<<endl;
    }*/
    ifstream in;
    in.open("prog.txt");
    ofstream out;
    out.open("SYMTAB.txt");
    ofstream temp;
    temp.open("temp.txt");
    out<<"Label\t\tAddress\n";
    out<<"------------------------\n";
    string s,word[5],pchex="0",st,sd;
    int c=0,f=0,format,pcdec=0,startp=0,lengthp;
    //cout<<std::hex<<pcdec<<endl;
    //cout<<std::hex<<pcdec;
    //pass1:
   if(in.is_open())
   {
    while(getline(in,s))
    {
        extract(s,word,c);
        if(f==0 && word[1]=="START" )
        {
            f=1;
            st=word[0];
            sd=word[2];
            pchex=word[2];
            pcdec=hextodec(pchex);
            startp=pcdec;
            SYMTAB[word[0]]=pchex;
            out<<word[1]<<"\t\t"<<pchex<<endl;

        }
        else
        {
            if(word[0][0]=='+')
               format=4,temp<<pcdec<<" "<<format<<" "<<s<<endl,pcdec+=4;
            else
            {
                 ito=OPTAB.find(word[0]);
                 itr=RESTAB.find(word[0]);
                 itr2=RESTAB.find(word[1]);
                 if(ito==OPTAB.end() && itr==RESTAB.end() )  //not found - label  c=3
                 {
                     SYMTAB[word[0]]=dectohex(pcdec);
                     out<<word[0]<<"\t\t"<<dectohex(pcdec)<<endl;
                     if(word[1][0]=='+')
                         format=4,temp<<pcdec<<" "<<format<<" "<<s<<endl,pcdec+=4;
                     else
                     {
                        ito=OPTAB.find(word[1]);
                        if(ito!=OPTAB.end())
                           format=ito->second.second,temp<<pcdec<<" "<<format<<" "<<s<<endl,pcdec+=format;
                        else
                        {//byte word resw resb
                           if(word[1]=="WORD")
                           {
                               temp<<pcdec<<" "<<format<<" "<<s<<endl;
                               pcdec+=3;
                           }
                           else if(word[1]=="BYTE")
                           {
                               int l=word[2].length();
                               l-=3;
                               temp<<pcdec<<" "<<0<<" "<<s<<endl;
                               if(word[2][0]=='C')
                               {
                                   pcdec+=l;
                               }
                               else if(word[2][0]=='X')
                               {
                                   l=ceil(l/2.0);
                                   pcdec+=l;
                               }
                           }
                           else if(word[1]=="RESW")
                           {
                               temp<<pcdec<<" "<<0<<" "<<s<<endl;
                               int num=atoi(word[2].c_str());
                               pcdec+=3*num;
                           }
                           else if(word[1]=="RESB")
                           {
                               temp<<pcdec<<" "<<0<<" "<<s<<endl;
                               int num=atoi(word[2].c_str());
                               pcdec+=num;
                           }
                        }

                     }

                 }
                 else if(itr==RESTAB.end()) // c=2
                 {
                      format=OPTAB[word[0]].second;
                      temp<<pcdec<<" "<<format<<" "<<s<<endl;
                      pcdec+=format;

                 }
                 else
                 {      //base & end
                      if(word[0]=="BASE")
                      { continue;}
                      else if(word[0]=="END")
                      {//length
                         lengthp=pcdec-startp;
                          cout<<dectohex(lengthp)<<endl;
                         continue;
                      }
                 }
            }
        }
    }
    }
    else
      cout<<"error\n";

    /* for(its=SYMTAB.begin();its!=SYMTAB.end();its++)
    {
       cout<<"sym  "<<(*its).first<<"\t"<<(*its).second<<endl;
    }*/
     //in.close();
     out.close();
     in.close();

     //pass2:
    ifstream in2;
    in2.open("temp.txt");
    ofstream out2;
    out2.open("OBJ.txt");
    //H
    int x=(int)st.length();
    out2<<"H^"<<st;
    for(int i=0;i<6-x;i++)
      out2<<" ";
    out2<<"^";
    x=(int)sd.length();
    for(int i=0;i<6-x;i++)
       out2<<"0";
    out2<<sd<<"^";
    x=(int)dectohex(lengthp).length();
    for(int i=0;i<6-x;i++)
       out2<<"0";
    out2<<dectohex(lengthp)<<endl;
    //T
    string tr[100],stemp;
    int sizet[100],i=0;
    int range[50][2];
    while(getline(in2,s))
    {
          extract(s,word,c);
          sizet[i]=atoi(word[1].c_str());
          tr[i]="";
          if(sizet[i]==1)
          {
            ito=OPTAB.find(word[2]);
            if(ito!=OPTAB.end())
               tr[i]=OPTAB[word[2]].first;
            else
            {
               ito=OPTAB.find(word[3]);
               if(ito!=OPTAB.end())
                  tr[i]=OPTAB[word[3]].first;
            }
            cout<<s<<"\t\t"<<tr[i]<<endl;
          }
          else if(sizet[i]==2)
          {
              ito=OPTAB.find(word[2]);
              if(ito!=OPTAB.end())
              {
                 tr[i]=OPTAB[word[2]].first;
                 string reg=word[3];
                 char ctemp=reg[0];
                 tr[i]+=REGTAB[ctemp];
                 if((int)reg.length()>2)
                 {
                     ctemp=reg[2];
                     tr[i]+=REGTAB[ctemp];
                 }
                 else
                    tr[i]+="0";
              }
              else
              {
                   tr[i]=OPTAB[word[3]].first;
                   string reg=word[4];
                   char ctemp=reg[0];
                   tr[i]+=REGTAB[ctemp];
                   if((int)reg.length()>2)
                   {
                      ctemp=reg[2];
                      tr[i]+=REGTAB[ctemp];
                   }
                   else
                      tr[i]+="0";
              }
              cout<<s<<"\t\t"<<tr[i]<<endl;
          }
          else if(sizet[i]==3)
          {
             std::string opc;
             string label,y,n,in,x,b,p,e,disp;
             int op;
             int pc=atoi(word[0].c_str())+3;
             ito=OPTAB.find(word[2]);
             if(ito!=OPTAB.end())
             {
                 y=OPTAB[word[2]].first;
                 label=word[3];
             }
             else
             {
                 y=OPTAB[word[3]].first;
                 label=word[4];
             }
             op=hextodec(y);
             opc=binary(op);
             tr[i]=opc;
             //n="1",im=""1",x="0","e="0",b="0",p="0";

             if(label[0]=='@')
             {
                 //im="0";x="1";
                 cout<<s<<endl;
             }
             else if(label[0]=='#')
             {
                  label=label.substr( 1 );

                  its=SYMTAB.find(label);
                  if(its!=SYMTAB.end()) //label
                  {
                    //cout<<label<<" "<<endl;
                   /* string nixbpe="010010",hexa3;
                    opc+=nixbpe;
                    hexa3="";
                    for(int h=0;h<3-(int)opc.length();h++)
                           hexa3+="0";
                    hexa3+=opc;
                    string dd=SYMTAB[label];


                    //int dis=(hextodec(atoi(dd.c_str()))-pc);*/

                    cout<<s<<endl;

                  }
                  else  //value
                  {
                      string nixbpe,bin,hexa,hexa2;
                      nixbpe="010000";
                      //n="0",im="1",x="0",b="0",p="0",e="0";

                      string dh=dectohex(atoi(label.c_str())) ;
                       //cout<<label<<" dh "<<dh<<endl;
                       bin=opc;
                       bin+=nixbpe;
                       bin= dectohex(bintodec(bin));
                       for(int h=0;h<3-(int)bin.length();h++)
                           hexa2+="0";
                       hexa2+=bin;
                       hexa="";

                       for(int h=0;h<3-(int)dh.length();h++)
                        hexa2+="0";
                        hexa2+=dh;
                        tr[i]=hexa2;

                        cout<<s<<"\t\t"<<tr[i]<<endl;


                  }
             }
             else
             {

                   string::size_type x = label.find( ',' );
                   if (x == string::npos) //not found  label
                   {
                   }
                   else  //label,x
                   {
                   }
                   cout<<s<<endl;
             }



          }
          else if(sizet[i]==4)
          {
             string op,n,i,x,b,p,e,addr;
             cout<<s<<endl;

          }
          else if(sizet[i]==0)  //chang the size
          {
              if(word[3]=="BYTE")
              {
                   stemp=word[4];
                   tr[i]="";
                   if(stemp[0]=='X')
                   {     //for(int j=2;j<(int)stemp.length()-1;j++)
                            tr[i]=stemp.substr(2,(int)stemp.length()-3);
                         sizet[i]=ceil(((int)stemp.length()-3)/2.0);

                   }
                   else if(stemp[0]=='C')
                   {     for(int j=2;j<(int)stemp.length()-1;j++)
                        {
                             int ascii=stemp[j];
                             tr[i]+=dectohex(ascii);
                        }
                       sizet[i]=(int)stemp.length()-3;
                   }
                   cout<<s<<"\t\t"<<tr[i]<<endl;
              }
              else if( word[3]=="WORD")
              {
                  tr[i]=dectohex(atoi(word[4].c_str()));
                  sizet[i]=3;
                  cout<<s<<"\t\t"<<tr[i]<<endl;
              }
              else if( word[3]=="RESW" ||  word[3]=="RESB")
              {//no object code
              cout<<s<<endl;
                i--;
              }

          }
          i++;
    }

    out2<<"E^";
    x=(int)sd.length();
    for(int i=0;i<6-x;i++)
       out2<<"0";
    out2<<sd<<endl;




    return 0;
}
void extract(string a,string word[],int& count)
{
    int i;
    for(i=0;i<5;++i) word[i]="";
    count=0;
    i=0;
    while(a[i]==' '&&i<(int)a.length()) {++i;continue;}
    if(i==(int)a.length()||a[i]=='.')   return;
    for(;i<(int)a.length();)
    {
        while(a[i]==' '&&i<(int)a.length()) {++i;continue;}
        if(i==(int)a.length()) break;
        for(;!(a[i]==' ')&&i<(int)a.length();++i) word[count]+=a[i];
        ++count;
    }
}


int power(int b,int e)
{
    int res=1;
    for(int i=1;i<=e;i++)
      res*=b;
    return res;
}


int hextodec(string str)
{
    int res=0,j=0;
    for(int i=str.length()-1;i>=0;i--)
    {
        if(str[i]>='0' && str[i]<='9')
            res+=(str[i]-'0')*power(16,j);
        else
            res+=(str[i]-'7')*power(16,j);
        j++;
    }
    return res;

}

string dectohex(int n)
{
    std::stringstream stream;
    stream << std::hex << n;
    std::string result( stream.str() );
    for(int i=0;i<(int)result.length();i++)
    {
      if(result[i]>='a' && result[i]<='f')
        result[i]-=32;
    }

    return result;
}


void createOPTAB()
{
    OPTAB["ADD"].first="18";
    OPTAB["ADD"].second=3;

    OPTAB["ADDF"].first="58";
    OPTAB["3"].second=3;

    OPTAB["ADDR"].first="90";
    OPTAB["ADDR"].second=2;

    OPTAB["AND"].first="40";
    OPTAB["AND"].second=3;

    OPTAB["CLEAR"].first="B4";
    OPTAB["CLEAR"].second=2;

    OPTAB["COMP"].first="28";
    OPTAB["COMP"].second=3;

    OPTAB["COMPF"].first="88";
    OPTAB["COMPF"].second=3;

    OPTAB["COMPR"].first="A0";
    OPTAB["COMPR"].second=2;

    OPTAB["DIV"].first="24";
    OPTAB["DIV"].second=3;

    OPTAB["DIVF"].first="64";
    OPTAB["DIVF"].second=3;

    OPTAB["DIVR"].first="9C";
    OPTAB["DIVR"].second=2;

    OPTAB["FIX"].first="C4";
    OPTAB["FIX"].second=1;

    OPTAB["FLOAT"].first="C0";
    OPTAB["FLOAT"].second=1;

    OPTAB["HIO"].first="F4";
    OPTAB["HIO"].second=1;

    OPTAB["J"].first="3C";
    OPTAB["J"].second=3;

    OPTAB["JEQ"].first="30";
    OPTAB["JEQ"].second=3;

    OPTAB["JGT"].first="34";
    OPTAB["JGT"].second=3;

    OPTAB["JLT"].first="38";
    OPTAB["JLT"].second=3;

    OPTAB["JSUB"].first="48";
    OPTAB["JSUB"].second=3;

    OPTAB["LDA"].first="00";
    OPTAB["LDA"].second=3;

    OPTAB["LDB"].first="68";
    OPTAB["LDB"].second=3;

    OPTAB["LDCH"].first="50";
    OPTAB["LDCH"].second=3;

    OPTAB["LDF"].first="70";
    OPTAB["LDF"].second=3;

    OPTAB["LDL"].first="08";
    OPTAB["LDL"].second=3;

    OPTAB["LDS"].first="6C";
    OPTAB["LDS"].second=3;

    OPTAB["LDT"].first="74";
    OPTAB["LDT"].second=3;

    OPTAB["LDX"].first="04";
    OPTAB["LDX"].second=3;

    OPTAB["LPS"].first="D0";
    OPTAB["LPS"].second=3;

    OPTAB["MUL"].first="20";
    OPTAB["MUL"].second=3;

    OPTAB["MULF"].first="60";
    OPTAB["MULF"].second=3;

    OPTAB["MULR"].first="98";
    OPTAB["MULR"].second=2;

    OPTAB["NORM"].first="C8";
    OPTAB["NORM"].second=1;

    OPTAB["OR"].first="44";
    OPTAB["OR"].second=3;

    OPTAB["RD"].first="D8";
    OPTAB["RD"].second=3;

    OPTAB["RMO"].first="AC";
    OPTAB["RMO"].second=2;

    OPTAB["RSUB"].first="4C";
    OPTAB["RSUB"].second=3;

    OPTAB["SHIFTL"].first="A4";
    OPTAB["SHIFTL"].second=2;

    OPTAB["SHIFTR"].first="A8";
    OPTAB["SHIFTR"].second=2;

    OPTAB["SIO"].first="F0";
    OPTAB["SIO"].second=1;

    OPTAB["SSK"].first="EC";
    OPTAB["SSK"].second=3;

    OPTAB["STA"].first="0C";
    OPTAB["STA"].second=3;

    OPTAB["STB"].first="78";
    OPTAB["STB"].second=3;

    OPTAB["STCH"].first="54";
    OPTAB["STCH"].second=3;

    OPTAB["STF"].first="80";
    OPTAB["STF"].second=3;

    OPTAB["STI"].first="D4";
    OPTAB["STI"].second=3;

    OPTAB["STL"].first="14";
    OPTAB["STL"].second=3;

    OPTAB["STS"].first="7C";
    OPTAB["STS"].second=3;

    OPTAB["STSW"].first="E8";
    OPTAB["STSW"].second=3;

    OPTAB["STT"].first="84";
    OPTAB["STT"].second=3;

    OPTAB["STX"].first="10";
    OPTAB["STX"].second=3;

    OPTAB["SUB"].first="1C";
    OPTAB["SUB"].second=3;

    OPTAB["SUBF"].first="5C";
    OPTAB["SUBF"].second=3;

    OPTAB["SUBR"].first="94";
    OPTAB["SUBR"].second=2;

    OPTAB["SVC"].first="B0";
    OPTAB["SVC"].second=2;

    OPTAB["TD"].first="E0";
    OPTAB["TD"].second=3;

    OPTAB["TIO"].first="F8";
    OPTAB["TIO"].second=1;

    OPTAB["TIX"].first="2C";
    OPTAB["TIX"].second=3;

    OPTAB["TIXR"].first="B8";
    OPTAB["TIXR"].second=2;

    OPTAB["WD"].first="DC";
    OPTAB["WD"].second=3;

}


void createRESTAB()
{
    RESTAB["BASE"]=0;
    RESTAB["BYTE"]=1;
    RESTAB["WORD"]=1;
    RESTAB["RESW"]=1;
    RESTAB["RESB"]=1;
    RESTAB["END"]=0;
}


void createREGTAB()
{
   REGTAB['A']="0";
   REGTAB['X']="1";
   REGTAB['L']="2";
//   REGTAB['PC']="8";
//   REGTAB['SW']="9";
   REGTAB['B']="3";
   REGTAB['S']="4";
   REGTAB['T']="5";
   REGTAB['F']="6";
}


string binary( int n )
  {
  bitset <sizeof( int ) * 8> bits( n );
  string result = bits.to_string <char, char_traits <char>, allocator <char> > ();
 // string::size_type x = result.find( '1' );
  //if (x != string::npos)
    result = result.substr( 24 ,6);

  return result;
  }
  string binary( int n ,int f )
  {
  bitset <sizeof( int ) * 8> bits( n );
  string result = bits.to_string <char, char_traits <char>, allocator <char> > ();
 // string::size_type x = result.find( '1' );
  //if (x != string::npos)
    result = result.substr( 20);

  return result;
  }

int bintodec(string s)
{
    int decimal=0,c=0;
    for(int counter = (int)s.length()-1; counter >=0; counter--)
    {
     if(s.c_str()[counter] == '1')
       {
          decimal += pow(2, c);
       }
       c++;
    }
    //cout<<" d "<<decimal<<endl;
    return decimal;

}


