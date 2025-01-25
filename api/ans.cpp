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
map<string,int> name2id;
map<string,string> mid2name,mid2type;
map<int,map<int,int> > entity2num;
map<int,int> e2num;
map<pair<string,string>,map<string,double> > rel_left,rel_right;

int relation_num,entity_num;
int n= 100;

double sigmod(double x)
{
    return 1.0/(1+exp(-x));
}

double vec_len(vector<double> a)
{
	double res=0;
	for (int i=0; i<a.size(); i++)
		res+=a[i]*a[i];
	return sqrt(res);
}

void vec_output(vector<double> a)
{
	for (int i=0; i<a.size(); i++)
	{
		cout<<a[i]<<"\t";
		if (i%10==9)
			cout<<endl;
	}
	cout<<"-------------------------"<<endl;
}

double sqr(double x)
{
    return x*x;
}

char buf[100000],buf1[100000];

int my_cmp(pair<double,int> a,pair<double,int> b)
{
    return a.first>b.first;
}

double cmp(pair<int,double> a, pair<int,double> b)
{
	return a.second<b.second;
}

struct info{
    string h,l,r;
    int x;
    bool operator <(const info &rhs)const{
        return x>rhs.x;
    }
};
vector<info> ve,vec;


map<int,string> id2name;

int H,R,ST;
const int N=1e5+5;
char s1[N],s2[N];
void ycl(){
    FILE *fin=fopen("./ycl.txt","r");
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
        int id=entity2id[S1];
        id2name[id]=S2;
		name2id[S2]=id;
    }
    fclose(fin);
    FILE* f_kb = fopen("./ask.txt","r");
	while(fscanf(f_kb,"%s",buf)==1)
    {
        string s1=buf;
        fscanf(f_kb,"%s",buf);
        string s2=buf;
		fscanf(f_kb,"%d",&ST);
		H=name2id[s1];
		R=relation2id[s2];
    }
    fclose(f_kb);
}
class Test{
    vector<vector<double> > relation_vec,entity_vec;

    vector<int> h,l,r;
    vector<int> fb_h,fb_l,fb_r;
    map<pair<int,int>, map<int,int> > ok;
    double res ;
public:
    void add(int x,int y,int z, bool flag)
    {
        ok[make_pair(x,z)][y]=1;
    }

    int rand_max(int x)
    {
        int res = (rand()*rand())%x;
        if (res<0)
            res+=x;
        return res;
    }
    double len;
    double calc_sum(int e1,int e2,int rel)
    {
        double sum=0;
        if (L1_flag)
        	for (int ii=0; ii<n; ii++)
            sum+=-fabs(entity_vec[e2][ii]-entity_vec[e1][ii]-relation_vec[rel][ii]);
        else
        for (int ii=0; ii<n; ii++)
            sum+=-sqr(entity_vec[e2][ii]-entity_vec[e1][ii]-relation_vec[rel][ii]);
        return sum;
    }
    void run()
    {
        FILE* f1 = fopen(("relation2vec."+version).c_str(),"r");
        FILE* f3 = fopen(("entity2vec."+version).c_str(),"r");
        cout<<relation_num<<' '<<entity_num<<endl;
        int relation_num_fb=relation_num;
        relation_vec.resize(relation_num_fb);
        for (int i=0; i<relation_num_fb;i++)
        {
            relation_vec[i].resize(n);
            for (int ii=0; ii<n; ii++)
                fscanf(f1,"%lf",&relation_vec[i][ii]);
        }
        entity_vec.resize(entity_num);
        for (int i=0; i<entity_num;i++)
        {
            entity_vec[i].resize(n);
            for (int ii=0; ii<n; ii++)
                fscanf(f3,"%lf",&entity_vec[i][ii]);
            if (vec_len(entity_vec[i])-1>1e-3)
            	cout<<"wrong_entity"<<i<<' '<<vec_len(entity_vec[i])<<endl;
        }
        fclose(f1);
        fclose(f3);
		map<int,int> rel_num;
        ofstream fout;
		fout.open("/Users/minari/Documents/创新实践/api/ans.txt");
        int cnt=0;
		int h = H;
		int rel = R;
		int st = ST;
        cout<<h<<" "<<rel<<" "<<st<<endl;
		vector<pair<int,double> > a;
		if(st==0){
			for (int i=0; i<entity_num; i++)
			{
				double sum = calc_sum(i,h,rel);
				a.push_back(make_pair(i,sum));
			}
			sort(a.begin(),a.end(),cmp);
			for(int i=9;i>=0;i--){
				fout<<id2name[a[a.size()-i].first]<<endl;
			}
		}else{
			for (int i=0; i<entity_num; i++)
			{
				double sum = calc_sum(h,i,rel);
				a.push_back(make_pair(i,sum));
			}
			sort(a.begin(),a.end(),cmp);
			for(int i=9;i>=0;i--){
				fout<<id2name[a[a.size()-i].first]<<endl;
			}
		}
        fout.close();
    }

};
Test test;

map<pair<int,int>,map<int,int> > add;

void prepare()
{
    FILE* f1 = fopen("./FB15k/entity2id.txt","r");
	FILE* f2 = fopen("./FB15k/relation2id.txt","r");
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
}


int main()
{
    version = "bern";
    prepare();
    ycl();
    test.run();
}

