#include<bits/stdc++.h>

#define pb push_back

using namespace std;
int tot=0;
string c_type[]={"int","double","void","float","bool","string"};
string c_res[]={"else","define","break","continue","scanf","printf","getchar","while","if","for","extern","return"};
string c_op2[]={"||","&&","==","!=","<=",">=",">>","<<"};
string c_op1[]={"#","(",")","[","]","{","}",";","=","&","|","^","<",">","+","-","*","/","%","!",","};

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
	int idx,fa;
	void print(){
		cout<<idx<<":"<<"< "<<name<<" , "<<val<<" >"<<":";
		for(int i=0;i<son.size();++i)cout<<son[i]<<" ";cout<<"||"<<fa<<endl;
	}
}T[100005];

map<string , int> fun;//memorize all the function name
 
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
int preprocess(char s[],int len){//delete all useless character 
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
void lexer(char s[],int &idx,int len){//analyze the lexer
	char ch=s[idx],peek=s[idx+1];
	string now;now=ch;now+=peek;
	for(auto t:c_op2){
		if(t==now){
			add(t,t);
			idx++;
			return ;
		}
	}
	now=ch;
	for(auto t:c_op1){
		if(t==now){
			add(t,t);
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
		//cout<<ch<<"*"<<s[idx]<<endl;
		while(idx+1<len&&(Isdigi(s[idx+1])||s[idx+1]=='.')){
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
		//cout<<ch<<endl;
		while(idx+1<len&&s[idx+1]!='\"'){
			now+=s[++idx];
		}
		now+=s[++idx];
		add("string",now);
		//cout<<now<<"*"<<endl;
		return ;
	}
}
int idx,length,to;
void link(int x,int y){
	T[x].son.pb(y);
	T[y].fa=x;
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
//analyze the synax and create the AST
int program(){
	int now=add("program","");
	for(;idx<=length;){
		//cout<<idx<<endl;
		//if(match("type"))
		link(now,func());
	}
	return now;
}

int func(){
	int now=add("func","");
	link(now,idx++);
	fun[T[idx].val]=now;
	//cout<<T[idx].name<<" "<<now<<endl;
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
	/*
	if(match("scanf")){
		link(now,idx++);//scanf
		link(now,idx++);//(
		idx++;idx++;if(match("&"))idx++;
		if(to=expr())link(now,to);//expr
		link(now,idx++);//)
		link(now,idx++);
		return chk(now);
	}*/
	if(match("scanf")){
		link(now,idx++);//scanf
		link(now,idx++);//(
		idx++;//""
		while(T[idx].name!=")"){
			idx++;
			if(match("&"))idx++;
			if(to=expr())link(now,to);
		}
		link(now,idx++);//)
		link(now,idx++);//;
		return chk(now);
	}
	/*if(match("printf")){
		link(now,idx++);
		link(now,idx++);
		idx++;idx++;
		//cout<<"********";T[idx].print();
		if(to=expr())link(now,to);
		link(now,idx++);
		link(now,idx++);
		return chk(now);
	}*/
	if(match("printf")){
		link(now,idx++);//printf
		link(now,idx++);//(
		link(now,idx++);//""
		while(T[idx].name!=")"){
			idx++;//,
			if(to=expr())link(now,to);
		}
		link(now,idx++);//)
		link(now,idx++);//;
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
	if(to=access2())link(now,to);
	return chk(now);
}
int access2(){
	int now=add("access2","");
	if(match("[")){
		link(now,idx++);
		if(to=expr())link(now,to);
		link(now,idx++);
		if(to=access2())link(now,to);
		//cout<<"***"<<endl;
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
	
/*--------------------*/

void Out_put(int x){
	T[x].print();
	for(int i=0;i<T[x].son.size();++i){
		Out_put(T[x].son[i]);
	}
}	

/*-------output-------*/
string node[5000005];
int Head[500005],Tail[500005];
int tt=0;//node_index

void put_dot(string name,string val,string shap){
	cout<<"	"<<name<<"[label = \""<<val<<"\",shape = "<<shap<<"];"<<endl;
}
void put_link(int name1,int name2,string typ){
	if(name1==0||name2==0)return ;
	cout<<"	"<<node[name1]<<" -> "<<node[name2]<<"[label = \""<<typ<<"\"]"<<endl;
}
int Add(string name,string val,string shap){
	put_dot(name,val,shap);
	node[++tt]=name;
	return tt;
}
string change(int x){
	string now;
	while(x){
		now=now+(char)(x%10+'0');
		x/=10;
	}
	return now;
}
string dfs_expr(int x){
	string now="";
	if(x!=0&&T[x].val!=";")now+=T[x].val;
	for(auto y:T[x].son){
		now+=dfs_expr(y);
	}
	//cout<<x<<endl;
	return now;
}
string _del(string s){//delete the %d %lld %.lf %s %c and add 
	string _now="";
	for(int i=0;i<s.size();++i){
		if(i+1<s.size()&&s[i]=='%'&&s[i+1]=='d'){
			_now=_now+"(*)";
			i++;continue;
		}
		if(i+1<s.size()&&s[i]=='%'&&s[i+1]=='s'){
			_now=_now+"(*)";
			i++;continue;
		}
		if(i+3<s.size()&&s[i]=='%'&&s[i+1]=='l'&&s[i+2]=='l'&&s[i+3]=='d'){
			_now=_now+"(*)";
			i+=3;continue;
		}
		if(i+3<s.size()&&s[i]=='%'&&s[i+1]=='.'&&s[i+2]=='l'&&s[i+3]=='f'){
			_now=_now+"(*)";
			i+=3;continue;
		}
		if(i+1<s.size()&&s[i]=='%'&&s[i+1]=='c'){
			_now=_now+"(*)";
			i++;continue;
		}
		if(s[i]=='\"')_now+="\\";
		if(s[i]=='\'')_now+="\\";
		if(i+1<s.size()&&s[i]=='\\'&&s[i+1]=='n'){
			_now+="\\";
		} 
		_now+=s[i];
	}
	return _now;
}
int dfs(int x){
//	cout<<T[x].name<<endl;
	if(T[x].name=="stmt"){
		int y=T[x].son[0];
		if(T[y].name=="while"){
			Head[x]=Add("pd"+change(x),"≈–∂œ:"+dfs_expr(T[x].son[2]),"diamond");
			Tail[x]=Add("end"+change(x),"while Ω· ¯","oval"); 
			//cout<<"**"<<endl;
			to=dfs(T[x].son[4]);
			//cout<<Head[x]<<":"<<Head[to]<<endl;
			put_link(Head[x],Head[to],"T");
			put_link(Tail[to],Head[x],"");
			put_link(Head[x],Tail[x],"F");
			//cout<<"**"<<endl;
			return x;
		}
		if(T[y].name=="for"){
			Head[x]=Add("csh"+change(x),"≥ı ºªØ:"+dfs_expr(T[x].son[2]),"box");
			int temp=Add("pd"+change(x),"≈–∂œ:"+dfs_expr(T[x].son[4]),"diamond");
			put_link(Head[x],temp,"");
			to=dfs(T[x].son[8]);
			put_link(temp,Head[to],"T");
			int tmmp=Add("gen"+change(x),"∏˙”Ôæ‰:"+dfs_expr(T[x].son[6]),"box");
			put_link(Tail[to],tmmp,"");
			put_link(tmmp,temp,"");
			Tail[x]=Add("end"+change(x),"for Ω· ¯","oval");
			put_link(temp,Tail[x],"F");
			return x;
		}
		if(T[y].name=="if"){
			Head[x]=Add("pd"+change(x),"≈–∂œ:"+dfs_expr(T[x].son[2]),"diamond");
			Tail[x]=Add("end"+change(x),"if Ω· ¯","oval");
			to=dfs(T[x].son[4]);
			put_link(Head[x],Head[to],"T");
			put_link(Tail[to],Tail[x],"");
			if(T[x].son.size()>6){
				to=dfs(T[x].son[6]);
				put_link(Head[x],Head[to],"F");
				put_link(Tail[to],Tail[x],"");
			}else{
				put_link(Head[x],Tail[x],"F");
			}
			return x;
		}
		
		if(T[y].name=="scanf"){
			string now=" ‰»Î:";
			for(int i=2;i<T[x].son.size()-2;++i){
				now=now+" "+dfs_expr(T[x].son[i]);
			}
			Head[x]=Add("in"+change(x),now,"parallelogram");
			Tail[x]=Head[x];
			return x;
		}
		if(T[y].name=="printf"){
			string now=" ‰≥ˆ:"+_del(T[T[x].son[2]].val);
			for(int i=3;i<T[x].son.size()-2;++i){
				//cout<<T[x].son.size()-1<<endl;
				now=now+" "+dfs_expr(T[x].son[i]);
			}
			Head[x]=Add("out"+change(x),now,"parallelogram");
			Tail[x]=Head[x];
			return x;
		}
		if(T[y].name=="block"){
			return dfs(y);
		}
		if(T[y].name=="break"){
			Head[x]=Add("break"+change(x),"break","box");
			Tail[x]=Head[x];
			return x;
		}
		if(T[y].name=="continue"){
			Head[x]=Add("continue"+change(x),"continue","box");
			Tail[x]=Head[x];
			return x;
		}	
		if(T[y].name=="return"){
			Head[x]=Add("return"+change(x),"return","box");
			Tail[x]=Head[x];
			return x;
		}
		if(T[y].name=="decl")return 0;
		//if(T[y].name=="expr"){
		Head[x]=Add("expr"+change(x),dfs_expr(y),"box");
		Tail[x]=Head[x];
		return x;
		//}
	}
	if(T[x].name=="func"){
		return dfs(T[x].son[4]);
	}
	if(T[x].name=="block"){
		return dfs(T[x].son[1]);
	}
	if(T[x].name=="stmts"){
		for(auto t:T[x].son){
			to=dfs(t);
			if(to==0)continue;
			if(Head[x]==0){
				Head[x]=Head[to];
				Tail[x]=Tail[to];
				continue;
			}
			put_link(Tail[x],Head[to],"");
			Tail[x]=Tail[to];
		}
		return x;
	}
	return 0;
}
void out(){
	cout<<"digraph {"<<endl;
	Head[0]=Add("main","ø™ º","oval"); 
	Tail[0]=Add("end","Ω· ¯","oval");
	to=dfs(fun["main"]);
//	cout<<fun["main"]<<endl;
	put_link(Head[0],Head[to],"");
	put_link(Tail[to],Tail[0],"");
	
	for(int i=1;i<=tot;++i){
		//cout<<T[i].name<<endl;
		if(T[i].name=="break"){
			int x=T[i].fa;
			while(x!=0&&(T[x].son.size()==0||(T[T[x].son[0]].name!="for"&&T[T[x].son[0]].name!="while"))){
				//cout<<x<<endl;
				x=T[x].fa;
			}
			put_link(Head[T[i].fa],Tail[x],"break");
			//cout<<Head[T[i].fa]<<" "<<Tail[x]<<endl;
			//cout<<"*******"<<" "<<x<<endl;
		}
		if(T[i].name=="continue"){
			int x=T[i].fa;
			while(x!=0&&(T[x].son.size()==0||(T[T[x].son[0]].name!="for"&&T[T[x].son[0]].name!="while"))){
				//cout<<x<<endl;
				x=T[x].fa;
			}
			if(T[T[x].son[0]].name=="for"){
				put_link(Head[T[i].fa],Head[x]+1,"continue");
			}else{
				put_link(Head[T[i].fa],Head[x],"continue");
			}
		}
		if(T[i].name=="return"){
			put_link(Head[T[i].fa],Tail[0],"return");
		}
	}
	
	cout<<"}"<<endl;
}


int main(){
	freopen("prog.txt","r",stdin);
	freopen("out.txt","w",stdout);
	char s[5000];
	int len=0;
	while((s[len++]=getchar())!=EOF);
	--len;
	len=preprocess(s,len);
	for(int idx=0;idx<len;++idx){
		lexer(s,idx,len);
	}
	
//	for(int i=1;i<=tot;++i)T[i].print();
//	cout<<tot<<endl;
	length=tot;
	::idx=1;
	int root=program();
//  int root=stmts();
/*	for(int i=length+1;i<=tot;++i){
		T[i].print();
	}*/
	//Out_put(length+1);//print AST
	
	out();//print graphviz.dot
	
	return 0;
}
