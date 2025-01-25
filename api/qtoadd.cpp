#include<iostream>
#include<cstring>
#include<cstdio>
#include<map>
#include<vector>
#include<string>
#include<ctime>
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<assert.h>
using namespace std;

bool debug=false;
bool L1_flag=1;

string version;
string trainortest = "test";

map<string,int> relation2id,entity2id;
map<int,string> id2entity,id2relation;
map<string,string> mid2name,mid2type;
map<int,map<int,int> > entity2num;
map<int,int> e2num;
map<pair<string,string>,map<string,double> > rel_left,rel_right;

int relation_num,entity_num;
int n= 100;
char buf[100000];

map<int,string> id2name;
map<string,int> name2id;
const int N=1e5+5;
char s1[N],s2[N];
void ycl(){
    FILE *fin=fopen("/Users/minari/Documents/创新实践/api/ycl.txt","r");
    assert(fin);
    while(fscanf(fin,"%s%s",s1,s2)==2){
        string S1=s1,S2="";
        for(int i=strlen(s2)-1;i>=0;i--){
            if(s2[i]=='/') break;
            S2=s2[i]+S2;
        }
        if(entity2id.count(S1)==0)
        {
            cout<<"miss entity:"<<S1<<endl;
        }
        // cout<<S1<<" "<<S2<<endl;
        int id=entity2id[S1];
        id2name[id]=S2;
		name2id[S2]=id;
    }
    fclose(fin);
}

void prepare()
{
    FILE* f1 = fopen("/Users/minari/Documents/创新实践/api/FB15k/entity2id.txt","r");
	FILE* f2 = fopen("/Users/minari/Documents/创新实践/api/FB15k/relation2id.txt","r");
	cout<<f1<<" "<<f2<<endl;
	int x;
	while (fscanf(f1,"%s%d",buf,&x)==2)
	{
		string st=buf;
		entity2id[st]=x;
		id2entity[x]=st;
		mid2type[st]="None";
		entity_num++;
	}
	while (fscanf(f2,"%s%d",buf,&x)==2)
	{
		string st=buf;
		relation2id[st]=x;
		id2relation[x]=st;
		relation_num++;
	}
	fclose(f1); fclose(f2);
}


char bufh[100000],bufr[100000],buft[100000];
void qtoadd(){
	FILE *fp=fopen("/Users/minari/Documents/创新实践/api/output.txt","r");
	assert(fp);
	ofstream fout;
	fout.open("/Users/minari/Documents/创新实践/api/add_to_train.txt");
	int st;
	while(fscanf(fp,"%s%s%s%d",bufh,bufr,buft,&st)==4){
		string hs=bufh;
		string rs=bufr;
		string ts=buft;
		fout<<id2entity[name2id[hs]]<<" "<<rs<<" "<<id2entity[name2id[ts]]<<" "<<st<<'\n';
	}
	fclose(fp);
	fout.close();
}

int main(int argc,char**argv)
{
	prepare();
	ycl();
	qtoadd();
}

