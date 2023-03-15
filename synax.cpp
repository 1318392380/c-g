#include<bits/stdc++.h>

#define pb push_back

using namespace std;
int tot=0;
string c_type[]={"int","double","void","float","bool","string"};
string c_res[]={"else","define","break","continue","scanf","printf","getchar","while","if","for","extern","return"};
string c_op2[]={"||","&&","==","!=","<=",">=",">>","<<"};
string c_op1[]={"#","(",")","[","]","{","}",";","=","&","|","^","<",">","+","-","*","/","%","!"};

/*-----decls------*/
int program();
int func();
int ofunc();
int decls();
int decl();
int block();
int stmts();
int stmt();
int expr();
int cond2();
int cond1();
int bit2();
int rel2();
int rel1();
int bit1();
int op2();
int op1();
int Not();
int Minus();
int access();
int access2();
int factor();
/*---------*/

struct Token{
	string name,val;
	vector<int> son;
	int idx;
	void print(){
		cout<<idx<<":"<<"< "<<name<<" , "<<val<<" >"<<":";
		for(int i=0;i<son.size();++i)cout<<son[i]<<" ";cout<<endl;
	}
}T[100005];
int add(string a,string b){
	T[++tot].name=a,T[tot].val=b,T[tot].idx=tot;
	return tot;
}

bool Isdigi(char s){
	return s>='0'&&s<='9';
}
bool Ischar(char s){
	return (s>='a'&&s<='z')||(s>='A'&&s<='Z')||s=='_';
}
int preprocess(char s[],int len){
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
	memset(s,0,sizeof(char)*5000);
	strcpy(s,temp);
	return ind;
}
void lexer(char s[],int &idx,int len){
	char ch=s[idx],peek=s[idx+1];
	string now;now=ch;now+=peek;
	for(auto t:c_op2){
		if(t==now){
			add(t,"");
			idx++;
			return ;
		}
	}
	now=ch;
	for(auto t:c_op1){
		if(t==now){
			add(t,"");
			return ;
		}
	}
	if(Ischar(ch)){
		now=ch;
		while(idx+1<len&&(Ischar(s[idx+1])||Isdigi(s[idx+1]))){
			now+=s[++idx];
		}
		for(auto t:c_type){
			if(t==now){
				add("type",t);
				return ;
			}
		}
		for(auto t:c_res){
			if(t==now){
				add(t,"");
				return ;
			}
		}
		add("id",now);
		return ;
	}
	if(Isdigi(ch)){
		now=ch;
		while(idx+1<len&&(Isdigi(s[idx+1]||s[idx+1]=='.'))){
			now+=s[++idx];
		}
		add("num",now);
		return ;
	}
	if(ch=='\''){
		now=ch;
		while(idx+1<len&&s[idx+1]!='\''){
			now+=s[++idx];
		}
		now+=s[++idx];
		add("char",now);
		return ;
	}
	if(ch=='\"'){
		now=ch;
		while(idx+1<len&&s[idx+1]!='\"'){
			now+=s[++idx];
		}
		now+=s[++idx];
		add("string",now);
		return ;
	}
}
int idx,length,to;
void link(int x,int y){
	T[x].son.pb(y);
}
bool emp(int x){
	return T[x].son.size()==0;
}
int chk(int x){
	if(emp(x))return 0;
	if(T[x].son.size()==1)return T[x].son[0];
	return x;
}

bool match(string s){
	return T[idx].name==s;
}
/*--------synax--------*/

int program(){
	int now=add("program","");
	for(;idx<=length;){
		cout<<idx<<endl;
		//if(match("type"))
		link(now,func());
	}
	return now;
}

int func(){
	int now=add("func","");
	link(now,idx++);
	link(now,idx++);
	link(now,idx++);
	if(match("type")||match("extern"))link(now,decls());
	link(now,idx++);
	link(now,block());
	
	return now;
}
int ofunc(){
	int now=add("func","");
	link(now,idx++);
	link(now,idx++);
	while(!match(")")){
		if(to=expr())link(now,to);
		if(match(","))link(now,idx++);
	}
	link(now,idx++);
	return chk(now);
}
int decls(){
	int now=add("decls","");
	while(match("type")||match("extern")){
		link(now,decl());
		if(!match(","))break;
		link(now,idx++);
	}
	return now;
}	
int decl(){
	int now=add("decl","");
	link(now,idx++);
	link(now,expr());
	return now;
}
int block(){
	if(!match("{"))return 0;
	int now=add("block","");
	link(now,idx++);
	if(to=stmts())link(now,to);
	link(now,idx++);
	return chk(now);
}
int stmts(){
	int now=add("stmts","");
	while(to=stmt()){
		link(now,to);
		//cout<<to<<endl;
	}
	return chk(now);
}
int stmt(){
	int now=add("stmt","");
	if(match("while")){
		link(now,idx++);
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=stmt())link(now,to);
		return chk(now);
	}
	if(match("if")){
		link(now,idx++);
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=stmt())link(now,to);
		if(match("else")){
			link(now,idx++);
			if(to=stmt())link(now,to);
		}
		return chk(now);
	}
	if(match("for")){
		link(now,idx++);
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=stmt())link(now,to);
		return chk(now);
	}
	if(match("{")){
		link(now,block());
		return chk(now);
	}
	if(match("extern")){
		link(now,decl());
		return chk(now);
	}
	if(match("type")){
		link(now,decl());
		link(now,idx++);
		return chk(now);
	}
	if(match("continue")){
		link(now,idx++);
		link(now,idx++);
		return chk(now);
	}
	if(match("break")){
		link(now,idx++);link(now,idx++);
		return chk(now);
	}
	if(match("return")){
		link(now,idx++);
		//cout<<":"<<idx<<endl;
		if(to=expr())link(now,to);
		link(now,idx++);
		return chk(now);
	}
	if(match("scanf")){
		link(now,idx++);//scanf
		link(now,idx++);//(
		idx++;idx++;if(match("&"))idx++;
		if(to=expr())link(now,to);//expr
		link(now,idx++);//)
		link(now,idx++);
		return chk(now);
	}
	if(match("printf")){
		link(now,idx++);
		link(now,idx++);
		idx++;idx++;
		if(to=expr())link(now,expr());
		link(now,idx++);
		link(now,idx++);
		return chk(now);
	}
	if(to=expr()){
		link(now,to);
		link(now,idx++);
	}
	return chk(now);
}
int expr(){
	int now=add("expr","");
	if(to=cond2())link(now,to);
	if(match("=")){
		link(now,idx++);
		if(to=expr())link(now,to);
	}
	return chk(now);
}
int cond2(){
	int now=add("cond2","");
	if(to=cond1())link(now,to);
	if(match("||")){
		link(now,idx++);
		link(now,cond2());
	}
	return chk(now);
}
int cond1(){
	int now=add("cond1","");
	if(to=bit2())link(now,to);
	if(match("&&")){
		link(now,idx++);
		link(now,cond1());
	}
	return chk(now);
}
int bit2(){
	int now=add("bit2","");
	if(to=rel2())link(now,to);
	if(match("&")||match("|")||match("^")){
		link(now,idx++);
		link(now,bit2());
	}
	return chk(now);
}
int rel2(){
	int now=add("rel2","");
	if(to=rel1())link(now,to);
	if(match("==")||match("!=")){
		link(now,idx++);
		link(now,rel2());
	}
	return chk(now);
}
int rel1(){
	int now=add("rel1","");
	if(to=bit1())link(now,to);
	if(match("<")||match("<=")||match(">=")||match(">")){
		link(now,idx++);
		link(now,rel1());
	}
	return chk(now);
}
int bit1(){
	int now=add("bit1","");
	if(to=op2())link(now,to);
	if(match("<<")||match(">>")){
		link(now,idx++);
		link(now,bit1());
	}
	return chk(now);
}
int op2(){
	int now=add("op2","");
	if(to=op1())link(now,to);
	if(match("+")||match("-")){
		link(now,idx++);
		link(now,op2());
	}
	return chk(now);
}
int op1(){
	int now=add("op1","");
	if(to=Not())link(now,to);
	if(match("*")||match("/")||match("%")){
		link(now,idx++);
		link(now,op1());
	}
	return chk(now);
}
int Not(){
	int now=add("not","");
	if(match("!")){
		link(now,idx++);
	}
	if(to=Minus())link(now,to);
	return chk(now);
}
int Minus(){
	int now=add("minus","");
	if(match("-")){
		link(now,idx++);
	}
	if(to=access())link(now,to);
	return chk(now);
}
int access(){
	int now=add("access","");
	if(to=factor())link(now,to);
	if(to=access2())link(now,access2());
	return chk(now);
}
int access2(){
	int now=add("access2","");
	if(match("[")){
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=access2())link(now,to);
	}
	return chk(now);
}
int factor(){
	int now=add("factor","");
	if(match("(")){
			link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		return chk(now);
	}
	if(match("num")){
		link(now,idx++);
		return chk(now);
	}
	if(match("id")){
		if(T[idx+1].name=="(")link(now,ofunc());
		else link(now,idx++);
		return chk(now);
	}
	if(match("getchar")){
		link(now,idx++);
		idx++;idx++;
		return chk(now);
	}
	if(match("string")){
		link(now,idx++);
		return chk(now);
	}
	if(match("char")){
		link(now,idx++);
		return chk(now);
	}
	
	return chk(now);
}
	
		

void dfs(int x){
	T[x].print();
	for(int i=0;i<T[x].son.size();++i){
		dfs(T[x].son[i]);
	}
}	
int main(){
	freopen("prog.txt","r",stdin);
	char s[5000];
	int len=0;
	while((s[len++]=getchar())!=EOF){
	}
	--len;
	len=preprocess(s,len);
	for(int idx=0;idx<len;++idx){
		lexer(s,idx,len);
	}
	
	for(int i=1;i<=tot;++i){
		T[i].print();
	}
	
	length=tot;
	::idx=1;
	int root=program();
//  int root=stmts();
/*	for(int i=length+1;i<=tot;++i){
		T[i].print();
	}*/
	dfs(length+1);
//	cout<<::idx<<endl;
	return 0;
}
