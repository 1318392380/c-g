#include<bits/stdc++.h>
using namespace std;


//state
bool Isdigit(char);
bool Ischar(char);

class Token{
	string name;//Token-name
	string Val1;//Token-string-attribute
	int Val2;//Token-int-attribute
	double Val3;//Token-double/float/attribute
	int tag;
public:
/*	bool operator <(const Token & t){
		return name<t.name;
	}*/
	Token(){
		name="";tag=0;
	}
	Token(string s,string v){
		name=s,Val1=v;tag=1;
	}
	Token(string s,int v){
		name=s,Val2=v;tag=2;
	}
	Token(string s,double v){
		name=s,Val3=v;tag=3;
	}
	string Tostring(){return Val1;}
	string Toname(){return name;}
	void print(){
		if(tag==0)return ;
		cout<<"< "<<name<<" , ";
		if(tag==1)cout<<Val1;
		if(tag==2)cout<<Val2;
		if(tag==3)cout<<Val3;
		cout<<" >"<<endl;
	}
};
map<string ,Token > words;
void reserve(Token t){
	words[t.Tostring()]=t;
}
bool Isdigit(char s){
	return s>='0'&&s<='9';
}
bool Ischar(char s){
	return (s>='a'&&s<='z')||(s>='A'&&s<='Z')||s=='_';
}
void preprocess(char s[],int len){
	char temp[5000]={0};
	int ind=0;
	for(int i=0;i<len;++i){
		if(i<len-1&&s[i]=='/'&&s[i+1]=='/'){
			while(i<len&&s[i]!='\n')++i;
			continue;
		}
		if(i<len-1&&s[i]=='/'&&s[i+1]=='*'){
			while(i<len-1&&(s[i]!='*'||s[i+1]!='/'))++i;
			++i;
			continue;
		}
		temp[ind++]=s[i];
	}
	strcpy(s,temp);
}
/*
keyword : scanf printf include getchar include return break continue for while if struct int double true false
identity
int
double
operator :  & | ^ = >> << && || ! . + - * / % == <= >= != ~
character : ( ) , # [ ] { } ;
*/
string s_keyword[]={"scanf","printf","include","getchar","return","break","continue","for","while","if",
					"struct","int","double","bool","string"};
string s_operator[]={"&","|","^","=","<<",">>","&&","||","!",".","+","-","*","/","%","==","<=",">=","!=","~",">","<"};
string s_character[]={"(",")",",","#","[","]","{","}",";",":"};
void init(){
	for(auto t:s_keyword){
		Token tmp("keyword",t);
		reserve(tmp);
	}
	for(auto t:s_operator){
		Token tmp("operator",t);
		reserve(tmp);
	}
	for(auto t:s_character){
		Token tmp("character",t);
		reserve(tmp);
	}
}
Token scan(char s[],int &idx,int len){
	char ch=s[idx],peek=s[idx+1];
	while((ch==' '||ch=='\n'||ch=='	')&&idx<len){
		ch=s[++idx],peek=s[idx+1];
	}
	switch(ch){
		case '&':if(peek=='&'){++idx;return words["&&"];}else return words["&"];
		case '|':if(peek=='|'){++idx;return words["||"];}else return words["|"];
		case '<':if(peek=='<'){++idx;return words["<<"];}else if(peek=='='){++idx;return words["<="];}else return words["<"];
		case '>':if(peek=='>'){++idx;return words[">>"];}else if(peek=='='){++idx;return words[">="];}else return words[">"];
		case '!':if(peek=='='){++idx;return words["!="];}else return words["!"];
		case '=':if(peek=='='){++idx;return words["=="];}else return words["="];
	}
	for(auto t:s_operator){
		string now;now=ch;
		if(t==now)return words[t];
	}
	for(auto t:s_character){
		string now;now=ch;
		if(t==now)return words[t];
	}
	if(Isdigit(ch)){
		int suf=ch-'0';double aft=0;
		while(Isdigit(s[idx+1])){
			ch=s[++idx];
			suf=suf*10+ch-'0';
		}
		if(s[idx+1]=='.'){
			++idx;
			while(Isdigit(s[idx+1])){
				ch=s[++idx];
				aft=aft*10+ch-'0';
			}
			while(aft>0)aft/=10;
			aft+=suf;
			Token tmp("double",aft);
			return tmp;
		}
		Token tmp("int",suf);
		return tmp;
	}
	if(Ischar(ch)){
		string now;now=ch;
		while(Ischar(s[idx+1])||Isdigit(s[idx+1])){
			ch=s[++idx];
			now+=ch;
		}
		if(words.find(now)!=words.end())return words[now];
		Token tmp("identity",now);
		return tmp;
	}
	if(ch=='\''){
		string now;now='\'';
		if(s[++idx]!='\'')now+=s[idx],++idx;
		now+='\'';
		Token tmp("char",now);
		return tmp;
	}
	if(ch=='\"'){
		string now;now='\"';
		while((ch=s[++idx])!='\"'){
			now+=ch;
		}
		now+=ch;
		Token tmp("string",now);
		return tmp;
	}
	Token tmp;
	return tmp;
}
	
int main(){
	freopen("prog.txt","r",stdin);
	//freopen("lexer-out.txt","w",stdout);
	char s[5000];
	int len=0;
	while((s[len++]=getchar())!=EOF){
		//cout<<s[len-1]<<endl;
	}
	--len;
	//s[len++]=getchar();cout<<s[len]<<endl;
	//cout<<s<<endl;
	preprocess(s,len);
	//cout<<s<<endl;
	init();
	/*for(auto t:words){
		t.second.print();
	}*/
	for(int idx=0;idx<len;++idx){
		Token t=scan(s,idx,len);
		//cout<<idx<<":";
		t.print();
	}
	/*int idx=0;
	Token t=scan(s,idx,len);
	t.print();*/
	
	return 0;
}
/*
123*/
