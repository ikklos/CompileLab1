#include<iostream>
#include<stack>
#include<deque>
#include"compilelab1.h"
#include"mylexer.h"
#include"ttype.h"
int tagcount = 0;
int lev[40];
int opnum[40];
MyTab mytab;
std::string getNewTag(){
    char s[20] = "";
    sprintf(s,"%d",tagcount++);
    std::string res = "_T";
    return res + s;
}
std::vector<TokenInfo> tokens = std::vector<TokenInfo>();
int cur = 0;
std::deque<std::string> blockcache;
std::vector<std::string> codeseg;
std::stack<std::deque<std::string>> argStack;
std::stack<std::pair<std::string,std::string>> whiletag;
std::string nowVeri = "";
std::string nowType = "local";
bool start(){
    std::cout << "start" << std::endl;
    if(cur >= tokens.size())return false;
    int id = tokens[cur].id;
    if(id != T_INT && id != T_VOID){
        return false;
    }
    if(!func()){
        return false;
    }
    if(!start1())return false;
    
    return true;
}
bool func(){
    std::cout << "func" << std::endl;
    int id = tokens[cur++].id;
    if(id == T_INT){
        return IF();
    }else if(id == T_VOID){
        return VF();
    }
    return false;
}
bool start1(){//s'
    std::cout << "start1" << std::endl;
    if(cur >= tokens.size()){
        return true;
    }
    if(tokens[cur].id == T_INT || tokens[cur].id == T_VOID){
        return start();
    }
    return false;
}
bool IF(){
    TokenInfo info = tokens[cur++];
    char temp[30];
    int count;
    switch(info.id){
    case T_MAIN:
        mytab.infunc();
        info = tokens[cur++];
        if(info.id != T_LPAREN) return false;
        info = tokens[cur++];
        if(info.id == T_INT){
            info = tokens[cur++];
            if(info.id != T_IDENTIFIER)return false;
            mytab.reg(info.word,"arg");
            info = tokens[cur++];
            if(info.id != T_COMMA)return false;
            info = tokens[cur++];
            if(info.id != T_INT)return false;
            info = tokens[cur++];
            if(info.id != T_IDENTIFIER)return false;
            mytab.reg(info.word,"arg");
            info = tokens[cur++];
            if(info.id != T_RPAREN)return false;
        }else if(info.id != T_RPAREN) return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        count = mytab.outfunc();
        codeseg.push_back("main:");
        codeseg.push_back("push ebp");
        codeseg.push_back("mov ebp,esp");
        sprintf(temp,"sub esp,%d",(count+1)*4);
        codeseg.push_back(temp);
        while(!blockcache.empty()){
            codeseg.push_back(blockcache.front());
            blockcache.pop_front();
        }
        sprintf(temp,"add esp,%d",(count+1)*4);
        codeseg.push_back(temp);
        codeseg.push_back("ret");
        break;
    case T_IDENTIFIER:
        nowVeri = info.word;
        if(!IF1()) return false;
        break;
    }
    return true;
}
bool IF1(){
    TokenInfo info = tokens[cur];
    if(info.id == T_LPAREN){
        cur++;
        mytab.infunc();
        if(!Args())return false;
        info = tokens[cur++];
        if(info.id != T_RPAREN)return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        int count = mytab.outfunc();
        char temp[30];
        codeseg.push_back(nowVeri+":");
        codeseg.push_back("push ebp");
        codeseg.push_back("mov ebp,esp");
        sprintf(temp,"sub esp,%d",(count+1)*4);
        codeseg.push_back(temp);
        while(!blockcache.empty()){
            codeseg.push_back(blockcache.front());
            blockcache.pop_front();
        }
        sprintf(temp,"add esp,%d",(count+1)*4);
        codeseg.push_back(temp);
        codeseg.push_back("ret");
    }else return false;
    return true;
}
bool VF(){
    std::cout << "VF" << std::endl; 
    TokenInfo info = tokens[cur++];
    if(info.id != T_IDENTIFIER){
        return false;
    }
    std::string funcname = info.word;
    info = tokens[cur++];
    if(info.id != T_LPAREN)return false;
    if(!Args())return false;
    info = tokens[cur++];
    if(info.id != T_RPAREN)return false;
    info = tokens[cur++];
    if(info.id != T_BLOCKL)return false;
    if(!B1())return false;
    info = tokens[cur++];
    if(info.id != T_BLOCKR)return false;
    int count;
    char temp[30];
    count = mytab.outfunc();
    codeseg.push_back(funcname+":");
    codeseg.push_back("push ebp");
    codeseg.push_back("mov ebp,esp");
    sprintf(temp,"sub esp,%d",(count+1)*4);
    codeseg.push_back(temp);
    while(!blockcache.empty()){
        codeseg.push_back(blockcache.front());
        blockcache.pop_front();
    }
    sprintf(temp,"add esp,%d",(count+1)*4);
    codeseg.push_back(temp);
    codeseg.push_back("ret");
    return true;
}
bool Dp1(){
    std::cout << "Dp1" << std::endl;
    TokenInfo info = tokens[cur];
    if(info.id == T_END || info.id == T_COMMA){
        if(!Dp2())return false;
    }else if(info.id == T_ASSIGN){
        cur++;
        if(!T())return false;
        blockcache.push_back("pop eax");
        SInfo sinfo = mytab.acc(nowVeri);
        char temp[20];
        sprintf(temp,"mov [ebp-%d],eax",sinfo.p*4);
        blockcache.push_back(temp);
        if(!Dp2())return false;
    }
    return true;
}
bool Dp2(){
    if(tokens[cur].id == T_END){//epsilon
        return true;
    }
    if(tokens[cur++].id == T_COMMA){  //,idD1'
        nowType = "local";
        TokenInfo info = tokens[cur++];
        if(info.id != T_IDENTIFIER)return false;
        if(tokens[cur].id == T_ASSIGN || tokens[cur].id == T_END){
            return Dp1();
        }else return false;
    }
    return false;
}
bool B1(){
    std::cout << "B1" <<std::endl;
    if(tokens[cur].id == T_BLOCKR){ //epsilon
        return true;
    }
    if(tokens[cur].id == T_INT
    ||tokens[cur].id == T_RETURN
    ||tokens[cur].id == T_IDENTIFIER
    ||tokens[cur].id == T_CONTINUE
    ||tokens[cur].id == T_BREAK
    ||tokens[cur].id == T_IF
    ||tokens[cur].id == T_WHILE){ //B
        return B();
    }
    return false;
}
bool B(){
    std::cout << "B" << std::endl;
    if(tokens[cur].id == T_INT
    ||tokens[cur].id == T_RETURN
    ||tokens[cur].id == T_IDENTIFIER
    ||tokens[cur].id == T_CONTINUE
    ||tokens[cur].id == T_BREAK
    ||tokens[cur].id == T_IF
    ||tokens[cur].id == T_WHILE){ //AB'
        if(!A())return false;
        if(!B1())return false;
        return true;
    }
    return false;
}
bool A(){
    std::cout << "A" << std::endl;
    if(tokens[cur].id == T_INT && Dp()){
        return true;
    }
    if(tokens[cur].id == T_RETURN && R()){
        return true;
    }
    if(tokens[cur].id == T_IDENTIFIER && E()){
        return true;
    }
    TokenInfo info = tokens[cur++];
    if(info.id == T_CONTINUE){
        info = tokens[cur++];
        if(info.id != T_END)return false;
        if(!whiletag.empty()){
            blockcache.push_back("jmp "+ whiletag.top().first);
        }
        return true;
    }else if(info.id == T_BREAK){
        info = tokens[cur++];
        if(info.id != T_END)return false;
        if(!whiletag.empty()){
            blockcache.push_back("jmp "+ whiletag.top().second);
        }
        return true;
    }else if(info.id == T_IF){
        TokenInfo temp = info;
        std::string f = getNewTag();
        info = tokens[cur++];
        if(info.id != T_LPAREN)return false;
        if(!T())return false;
        blockcache.push_back("pop eax");
        blockcache.push_back("test eax");
        blockcache.push_back("jne " + f);
        mytab.inblock();
        info = tokens[cur++];
        if(info.id != T_RPAREN)return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B1())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        blockcache.push_back(f+":");
        mytab.outblock();
        return Ep();
    }else if(info.id == T_WHILE){
        TokenInfo temp = info;
        std::string t1 = getNewTag();
        std::string t2 = getNewTag();
        whiletag.push({t1,t2});
        info = tokens[cur++];
        if(info.id != T_LPAREN)return false;
        blockcache.push_back(t1+":");
        if(!T())return false;
        blockcache.push_back("pop eax");
        blockcache.push_back("test eax");
        blockcache.push_back("je " + t2);
        mytab.inblock();
        info = tokens[cur++];
        if(info.id != T_RPAREN)return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B1())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        blockcache.push_back(t2+":");
        whiletag.pop();
        mytab.outblock();
        return true;
    }
    return false;
}
bool Ep(){
    if(tokens[cur].id == T_ELSE){
        cur++;
        TokenInfo info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        mytab.inblock();
        if(!B1())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        mytab.outblock();
        return true;
    }else{
        if(tokens[cur].id == T_INT
        ||tokens[cur].id == T_RETURN
        ||tokens[cur].id == T_IDENTIFIER
        ||tokens[cur].id == T_CONTINUE
        ||tokens[cur].id == T_BREAK
        ||tokens[cur].id == T_IF
        ||tokens[cur].id == T_WHILE
        ||tokens[cur].id == T_BLOCKR){
            return true;
        }
    }
    return false;
}
bool Dp(){
    std::cout << "Dp" << std::endl;
    TokenInfo info = tokens[cur++];
    if(info.id != T_INT)return false;
    info = tokens[cur++];
    if(info.id != T_IDENTIFIER)return false;
    nowVeri = info.word;
    nowType = "local";
    mytab.reg(nowVeri,nowType);
    if(!Dp1())return false;
    info = tokens[cur++];
    if(info.id != T_END)return false;
    return true;
}
bool R(){
    TokenInfo info = tokens[cur++];
    if(info.id != T_RETURN)return false;
    if(!T())return false;
    info = tokens[cur++];
    if(info.id != T_END)return false;
    blockcache.push_back("pop eax");
    return true;
}
bool E(){
    std::cout << "E" <<std::endl;
    TokenInfo info = tokens[cur++];
    if(info.id != T_IDENTIFIER)return false;
    nowVeri = info.word;
    if(!P())return false;
    info = tokens[cur++];
    if(info.id != T_END)return false;
    return true;
}
bool P(){
    std::cout << "P" << std::endl;
    TokenInfo info = tokens[cur++];
    if(info.id == T_LPAREN){
        if(!Tp())return false;
        while(!argStack.empty()){
            while(!argStack.top().empty()){
                blockcache.push_back(argStack.top().front());
                argStack.top().pop_front();
            }
            argStack.pop();
        }
        info = tokens[cur++];
        char temp[20];
        if(info.id != T_RPAREN)return false;
        if(nowVeri == "println_int"){
            blockcache.push_back("push eax");
            blockcache.push_back("push offset format_str");
            blockcache.push_back("call printf");
        }else{
            sprintf(temp,"call %s",nowVeri.c_str());
            blockcache.push_back(temp);
        }
    }else if(info.id == T_ASSIGN){
        if(!T())return false;
        SInfo sinfo = mytab.acc(nowVeri);
        blockcache.push_back("pop eax");
        char temp[20];
        sprintf(temp,"mov [ebp-%d],eax",sinfo.p*4);
        blockcache.push_back(temp);
    }else return false;
    return true;
}
void OPoutput(int op){
    std::cout << "OP:" << op << std::endl;
    int num = opnum[op];
    std::deque<std::string>& p = blockcache;
    if(nowType == "ARG" && !argStack.empty()){
        p = argStack.top();
    }
    if(num == 1){
        p.push_back("pop eax\n");
    }else{
        p.push_back("pop ebx\npop eax\n");
    }
    std::string tag;
    char temp[30];
    switch (op)
    {
    case T_PLUS:
        p.push_back("add eax,ebx\npush eax\n");
        break;
    case T_SUB:
        p.push_back("sub eax,ebx\npush eax\n");
        break;
    case T_MULTIPLY:
        p.push_back("mul ebx\npush eax\n");
        break;
    case T_DIVIDE:
        p.push_back("div ebx\npush eax\n");
        break;
    case T_LT:
        p.push_back("mov ecx,1\n");
        p.push_back("cmp eax,ebx\n");
        tag = getNewTag();
        sprintf(temp,"jl %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("dec ecx\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ecx\n");
        break;
    case T_GT:
        p.push_back("mov ecx,1\n");
        p.push_back("cmp eax,ebx\n");
        tag = getNewTag();
        sprintf(temp,"jg %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("dec ecx\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ecx\n");
        break;
    case T_LE:
        p.push_back("mov ecx,1\n");
        p.push_back("cmp eax,ebx\n");
        tag = getNewTag();
        sprintf(temp,"jle %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("dec ecx\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ecx\n");
        break;
    case T_GE:
        p.push_back("mov ecx,1\n");
        p.push_back("cmp eax,ebx\n");
        tag = getNewTag();
        sprintf(temp,"jge %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("dec ecx\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ecx\n");
        break;
    case T_EQ:
        p.push_back("mov ecx,1\n");
        p.push_back("cmp eax,ebx\n");
        tag = getNewTag();
        sprintf(temp,"je %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("dec ecx\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ecx\n");
        break;
    case T_NE:
        p.push_back("mov ecx,1\n");
        p.push_back("cmp eax,ebx\n");
        tag = getNewTag();
        sprintf(temp,"jne %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("dec ecx\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ecx\n");
        break;
    case T_MOD:
        p.push_back("xor edx,edx\n");
        p.push_back("div ebx\n");
        p.push_back("push edx\n");
        break;
    case T_LAND:
        p.push_back("cmp eax,0\n");
        tag = getNewTag();
        sprintf(temp,"je %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("mov eax,1\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("cmp ebx,0\n");
        tag = getNewTag();
        sprintf(temp,"je %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("mov ebx,1\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("and eax,ebx\n");
        p.push_back("push eax\n");
        break;
    case T_LOR:
        p.push_back("cmp eax,0\n");
        tag = getNewTag();
        sprintf(temp,"je %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("mov eax,1\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("cmp ebx,0\n");
        tag = getNewTag();
        sprintf(temp,"je %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("mov ebx,1\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("or eax,ebx\n");
        p.push_back("push eax\n");
        break;
    case T_AND:
        p.push_back("and eax,ebx\n");
        p.push_back("push eax\n");
        break;
    case T_OR:
        p.push_back("or eax,ebx\n");
        p.push_back("push eax\n");
        break;
    case T_XOR:
        p.push_back("xor eax,ebx\n");
        p.push_back("push eax\n");
        break;
    case T_LNOT:
        p.push_back("mov ebx,1\n");
        tag = getNewTag();
        p.push_back("cmp eax,0\n");
        sprintf(temp,"je %s\n",tag.c_str());
        p.push_back(temp);
        p.push_back("mov ebx,0\n");
        sprintf(temp,"%s:\n",tag.c_str());
        p.push_back(temp);
        p.push_back("push ebx\n");
        break;
    case T_BNOT:
        p.push_back("not eax\n");
        p.push_back("push eax\n");
        break;
    case T_NEG:
        p.push_back("mov ebx,-1\n");
        p.push_back("mul ebx\n");
        p.push_back("push eax\n");
        break;
    default:
        break;
    };
}
bool T(){
    std::cout << "T" << std::endl;
    int lcount = 0;
    int idcount = 0;
    std::stack<std::pair<int,int>> stk;
    while(cur < tokens.size()){
        if(tokens[cur].id == T_END
            ||(tokens[cur].id == T_RPAREN && lcount==0)){
            while(!stk.empty()){
                OPoutput(stk.top().first);
                stk.pop();
            }
            return true;
        }
        if(tokens[cur].id == T_IDENTIFIER){
            if(cur+1 < tokens.size() && tokens[cur+1].id == T_LPAREN){
                if(!Pp())return false;
                printf("push eax\n");
            }else{
                TokenInfo info = tokens[cur++];
                std::cout << info.word << std::endl;
                SInfo sinfo = mytab.acc(info.word);
                if(sinfo.type == "global"){
                    printf("mov eax,[%s]\n",sinfo.name.c_str());
                    printf("push eax\n");
                }else if(sinfo.type == "local"){
                    printf("mov eax,DWORD PTR[ebp-%d]\n",sinfo.p*4);
                    printf("push eax\n");
                }else if(sinfo.type == "arg"){
                    printf("mov eax,DWORD PTR[ebp+%d]\n",(sinfo.p+1)*4);
                    printf("push eax\n");
                }
            }
        }else{
            TokenInfo info = tokens[cur++];
            if(info.id == T_CONST){
                printf("push %s\n",info.word.c_str());
            }else{
                if(info.id == T_SUB){
                    if(tokens[cur-2].id == T_RPAREN 
                    || tokens[cur-2].id == T_IDENTIFIER){
                        stk.push({T_NEG,lev[T_NEG]});
                    }else{
                        while(!stk.empty()){
                            if(stk.top().second > lev[T_SUB])break;
                            OPoutput(stk.top().first);
                            stk.pop();
                        }
                        stk.push({T_SUB,lev[T_SUB]});
                    }
                }else if(info.id == T_LPAREN){
                    lcount++;
                    stk.push({T_LPAREN,lev[T_RPAREN]});
                }else if(info.id == T_RPAREN){
                    lcount--;
                    while(!stk.empty()){
                        if(stk.top().first == T_LPAREN){
                            stk.pop();
                            break;
                        }
                        OPoutput(stk.top().first);
                        stk.pop();
                    }
                }else if(info.id == T_LNOT || info.id == T_BNOT){
                    stk.push({info.id,lev[info.id]});
                }else{
                    if(lev[info.id] == 0)return false;
                    while(!stk.empty()){
                        if(stk.top().second > lev[info.id])break;
                        OPoutput(stk.top().first);
                        stk.pop();
                    }
                    stk.push({info.id,lev[info.id]});
                }
            }
        }
    }
    return false;
}
bool Args(){
    std::cout << "Args" << std::endl;
    nowType = "arg";
    if(tokens[cur].id == T_INT){
        return D();
    }else if(tokens[cur].id == T_RPAREN){
        return true;
    }
    return false;
}
bool Pp(){
    std::cout << "Pp" << std::endl;
    TokenInfo info = tokens[cur++];
    if(info.id != T_IDENTIFIER)return false;
    info = tokens[cur++];
    if(info.id != T_LPAREN)return false;
    if(!Tp())return false;
    while(!argStack.empty()){
        while(!argStack.top().empty()){
            blockcache.push_back(argStack.top().front());
            argStack.top().pop_front();
        }
        argStack.pop();
    }
    info = tokens[cur++];
    if(info.id != T_RPAREN)return false;
    return true;
}
bool Tp(){
    std::cout << "Tp" << std::endl;
    nowType = "ARG";
    argStack.push(std::deque<std::string>());
    if(!T())return false;
    if(!Tp1())return false;
    nowType = "";
    return true;
}
bool Tp1(){
    std::cout << "Tp1" << std::endl;
    std::cout << tokens[cur].word << std::endl;
    if(tokens[cur].id == T_RPAREN){
        return true;
    }
    TokenInfo info = tokens[cur++];
    if(info.id != T_COMMA)return false;
    if(!Tp())return false;
    return true;
}
bool D(){
    std::cout << "D" << std::endl;
    TokenInfo info = tokens[cur++];
    if(info.id != T_INT)return false;
    info = tokens[cur++];
    if(info.id != T_IDENTIFIER) return false;
    if(nowType == "arg"){
        mytab.reg(info.word,nowType);
    }
    if(!D1())return false;
    return true;
}
bool D1(){
    std::cout << "D1" << std::endl;
    if(tokens[cur].id == T_RPAREN){
        return true;
    }
    TokenInfo info = tokens[cur++];
    if(info.id != T_COMMA)return false;
    if(!D())return false;
    return true;
}
int main(){
    //初始化
    for(int i = 0; i < 40; i++){
        lev[i] = 0;
    }
    lev[T_LPAREN] = 1;
    lev[T_LNOT] = lev[T_BNOT] = lev[T_NEG] = 2;
    lev[T_DIVIDE] = lev[T_MULTIPLY] = lev[T_MOD] = 3;
    lev[T_PLUS] = lev[T_SUB] = 4;
    lev[T_LT] = lev[T_GT] = lev[T_LE] = lev[T_GE] = 5;
    lev[T_EQ] = lev[T_NE] = 6;
    lev[T_AND] = 7;
    lev[T_XOR] = 8;
    lev[T_OR] = 9;
    lev[T_LAND] = 10;
    lev[T_LOR] = 11;
    opnum[T_DIVIDE] = opnum[T_MULTIPLY] = opnum[T_MOD] = opnum[T_PLUS]
    =opnum[T_SUB] = opnum[T_LT] = opnum[T_GT] = opnum[T_LE] = opnum[T_GE]
    =opnum[T_EQ] = opnum[T_NE] = opnum[T_AND] = opnum[T_XOR] = opnum[T_OR]
    =opnum[T_LAND] = opnum[T_LOR] = 2;
    opnum[T_LNOT] = opnum[T_BNOT] = opnum[T_NEG] = 1;
    //词法分析
    int i = 0;
    MyLexer lexer;
    do{
        i = lexer.yylex();
        if(i != 0){
            tokens.push_back({
                lexer.getYYText(),
                i
            });
            std::cout << lexer.getYYText() << " " << i << std::endl;
        }
    }while(i != 0);
    //语法分析
    try{
        if(!start()){
            printf("ERR: failed!\n");
        }else{
            printf(".intel_syntax noprefix\n");
            printf(".data\n");
            printf("format_str:\n.asciz \"%%d\\n\"\n.extern printf\n");
            printf(".text\n");
            printf(".global main\n");
            for(auto x: codeseg){
                printf("%s\n",x.c_str());
            }
        }
    }catch(std::runtime_error){
        printf("ERR: failed!\n");
    }
    return 0;
}