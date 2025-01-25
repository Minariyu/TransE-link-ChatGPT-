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
        // cout<<S1<<" "<<S2<<endl;
        int id=entity2id[S1];
        id2name[id]=S2;
    }
    fclose(fin);
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
    	if (flag)
    	{
        	fb_h.push_back(x);
        	fb_r.push_back(z);
        	fb_l.push_back(y);
        }
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
		double lsum=0 ,lsum_filter= 0;
		double rsum = 0,rsum_filter=0;
		double lp_n=0,lp_n_filter;
		double rp_n=0,rp_n_filter;
		map<int,double> lsum_r,lsum_filter_r;
		map<int,double> rsum_r,rsum_filter_r;
		map<int,double> lp_n_r,lp_n_filter_r;
		map<int,double> rp_n_r,rp_n_filter_r;
		map<int,int> rel_num;
        ofstream fout;
        // fout.open("add.txt");
        ve.clear(); vec.clear();
        int cnt=0;
        for (int testid = 0; testid<fb_l.size(); testid+=1)
		{
			int h = fb_h[testid];
			int l = fb_l[testid];
			int rel = fb_r[testid];
			rel_num[rel]+=1;
			vector<pair<int,double> > a;
			for (int i=0; i<entity_num; i++)
			{
				double sum = calc_sum(i,l,rel);
				a.push_back(make_pair(i,sum));
			}
			sort(a.begin(),a.end(),cmp);
			double ttt=0;
			int filter = 0;
            bool dd=0;
            vector<info> v1,v2;
			for (int i=a.size()-1; i>=0; i--)
			{
				if (!dd&&ok[make_pair(a[i].first,rel)].count(l)>0)
					ttt++;
			    if (!dd&&ok[make_pair(a[i].first,rel)].count(l)==0)
			    	filter+=1;
				if (a[i].first ==h)
				{
                    assert(dd==0);
                    if(id2name.count(h)!=0&&id2name.count(l)!=0){
                        vec.push_back((info){
                            id2name[h],
                            id2name[l],
                            id2relation[rel],
                            (int)a.size()-i
                        });
                    }
                    ve.push_back((info){id2entity[h],id2entity[l],id2relation[rel],(int)a.size()-i});
					lsum+=a.size()-i;
					lsum_filter+=filter+1;
					lsum_r[rel]+=a.size()-i;
					lsum_filter_r[rel]+=filter+1;
					if (a.size()-i<=10)
					{
						lp_n+=1;
						lp_n_r[rel]+=1;
					}
					if (filter<10)
					{
						lp_n_filter+=1;
						lp_n_filter_r[rel]+=1;
					}
                    break;
                    dd=1;
                    if(cnt<1000&&a.size()-i>=1000){
                        cnt++;
                        v1.push_back((info){
                            id2entity[h],id2entity[l],id2relation[rel],1
                        });
                    }else break;
				}else if(dd){
                    v1.push_back((info){
                        id2entity[a[i].first],id2entity[l],id2relation[rel],0
                    });
                    if(v1.size()==11) break;
                }
			}
			a.clear();
			for (int i=0; i<entity_num; i++)
			{
				double sum = calc_sum(h,i,rel);
				a.push_back(make_pair(i,sum));
			}
			sort(a.begin(),a.end(),cmp);
			ttt=0;
			filter=0;
            bool d=0;
			for (int i=a.size()-1; i>=0; i--)
			{
				if (!d&&ok[make_pair(h,rel)].count(a[i].first)>0)
					ttt++;
			    if (!d&&ok[make_pair(h,rel)].count(a[i].first)==0)
			    	filter+=1;
				if (a[i].first==l)
				{
                    assert(d==0);
                    if(id2name.count(h)!=0&&id2name.count(l)!=0){
                        vec.push_back((info){
                            id2name[h],
                            id2name[l],
                            id2relation[rel],
                            (int)a.size()-i
                        });
                    }
                    ve.push_back((info){id2entity[h],id2entity[l],id2relation[rel],(int)a.size()-i});
					rsum+=a.size()-i;
					rsum_filter+=filter+1;
					rsum_r[rel]+=a.size()-i;
					rsum_filter_r[rel]+=filter+1;
					if (a.size()-i<=10)
					{
						rp_n+=1;
						rp_n_r[rel]+=1;
					}
					if (filter<10)
					{
						rp_n_filter+=1;
						rp_n_filter_r[rel]+=1;
					}
                    break;
                    d=1;
                    if(cnt<1000&&a.size()-i>=1000){
                        cnt++;
                        v2.push_back((info){
                            id2entity[h],id2entity[l],id2relation[rel],1
                        });
                    }else break;
				}else if(d){
                    v2.push_back((info){
                        id2entity[h],id2entity[a[i].first],id2relation[rel],0
                    });
                    if(v2.size()==11) break;
                }
			}
            for(int i=0;i<v1.size();i++){
                info ssw=v1[i];
                fout<<ssw.h<<" "<<ssw.r<<" "<<ssw.l<<" "<<ssw.x<<'\n';
            }
            for(int i=0;i<v2.size();i++){
                info ssw=v2[i];
                fout<<ssw.h<<" "<<ssw.r<<" "<<ssw.l<<" "<<ssw.x<<'\n';
            }
        }
        fout.close();
		cout<<"left:"<<lsum/fb_l.size()<<'\t'<<lp_n/fb_l.size()<<"\t"<<lsum_filter/fb_l.size()<<'\t'<<lp_n_filter/fb_l.size()<<endl;
		cout<<"right:"<<rsum/fb_r.size()<<'\t'<<rp_n/fb_r.size()<<'\t'<<rsum_filter/fb_r.size()<<'\t'<<rp_n_filter/fb_r.size()<<endl;
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
    FILE* f_kb1 = fopen("./FB15k/train.txt","r");
	while (fscanf(f_kb1,"%s",buf)==1)
    {
        string s1=buf;
        fscanf(f_kb1,"%s",buf);
        string s2=buf;
        fscanf(f_kb1,"%s",buf);
        string s3=buf;
        if (entity2id.count(s1)==0)
        {
            cout<<"miss entity:"<<s1<<endl;
        }
        if (entity2id.count(s2)==0)
        {
            cout<<"miss entity:"<<s2<<endl;
        }
        if (relation2id.count(s3)==0)
        {
            relation2id[s3] = relation_num;
            relation_num++;
        }

        entity2num[relation2id[s3]][entity2id[s1]]+=1;
        entity2num[relation2id[s3]][entity2id[s2]]+=1;
        e2num[entity2id[s1]]+=1;
        e2num[entity2id[s2]]+=1;
        test.add(entity2id[s1],entity2id[s2],relation2id[s3],false);
    }
    fclose(f_kb1);
    FILE* f_kb2 = fopen("./FB15k/valid.txt","r");
	while (fscanf(f_kb2,"%s",buf)==1)
    {
        string s1=buf;
        fscanf(f_kb2,"%s",buf);
        string s2=buf;
        fscanf(f_kb2,"%s",buf);
        string s3=buf;
        if (entity2id.count(s1)==0)
        {
            cout<<"miss entity:"<<s1<<endl;
        }
        if (entity2id.count(s2)==0)
        {
            cout<<"miss entity:"<<s2<<endl;
        }
        if (relation2id.count(s3)==0)
        {
            relation2id[s3] = relation_num;
            relation_num++;
        }
        test.add(entity2id[s1],entity2id[s2],relation2id[s3],false);
    }
    fclose(f_kb2);
    FILE* f_kb3 = fopen("./add.txt","r");
    assert(f_kb3);
	while (fscanf(f_kb3,"%s",buf)==1)
    {
        string s1=buf;
        fscanf(f_kb3,"%s",buf);
        string s3=buf;
        fscanf(f_kb3,"%s",buf);
        string s2=buf;
        int st;
        fscanf(f_kb3,"%d",&st);
        if(st==0) continue;
        if (entity2id.count(s1)==0)
        {
            cout<<"miss entity:"<<s1<<endl;
        }
        if (entity2id.count(s2)==0)
        {
            cout<<"miss entity:"<<s2<<endl;
        }
        if (relation2id.count(s3)==0)
        {
            relation2id[s3] = relation_num;
            relation_num++;
        }
        test.add(entity2id[s1],entity2id[s2],relation2id[s3],false);
        add[make_pair(entity2id[s1],relation2id[s3])][entity2id[s2]]=1;
    }
    fclose(f_kb3);
    FILE* f_kb = fopen("./FB15k/test.txt","r");
	while (fscanf(f_kb,"%s",buf)==1)
    {
        string s1=buf;
        fscanf(f_kb,"%s",buf);
        string s2=buf;
        fscanf(f_kb,"%s",buf);
        string s3=buf;
        if (entity2id.count(s1)==0)
        {
            cout<<"miss entity:"<<s1<<endl;
        }
        if (entity2id.count(s2)==0)
        {
            cout<<"miss entity:"<<s2<<endl;
        }
        if (relation2id.count(s3)==0)
        {
        	cout<<"miss relation:"<<s3<<endl;
            relation2id[s3] = relation_num;
            relation_num++;
        }
        if(add[make_pair(entity2id[s1],relation2id[s3])].count(entity2id[s2])>0){
            continue;
        }
        test.add(entity2id[s1],entity2id[s2],relation2id[s3],true);
    }
    fclose(f_kb);
}


int main(int argc,char**argv)
{
    if (argc<2)
        return 0;
    else
    {
        version = argv[1];
        prepare();
        test.run();
    }
}

