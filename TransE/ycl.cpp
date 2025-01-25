#include<bits/stdc++.h>
using namespace std;
const int N=1e5+5;
string s[N];
int main(){
	string T;
	ifstream fin;
	ofstream fout;
	fin.open("/Users/minari/Documents/创新实践/TransE/fb15k_mid-wdurl-wpurl_14951.txt");
	fout.open("/Users/minari/Documents/创新实践/TransE/ycl.txt");
	while(1){
		getline(fin,T);
		if(fin.eof()) break;
		int now=0;
		for(int i=0;i<T.length();i++){
			if(T[i]==' '||T[i]=='\t') now++;
			else s[now]=s[now]+T[i];
		}
		if(s[0]!=""&&s[2].length()>1){
			fout<<s[0]<<" "<<s[2]<<'\n';
		}
		s[0]=s[1]=s[2]="";
	}
	fin.close();
	fout.close();
}