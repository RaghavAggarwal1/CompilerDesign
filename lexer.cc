
/*
 * 
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stack>


#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE", "PUBLIC", "PRIVATE", 
    "SEMICOLON", "LBRACE", "RBRACE", "ID", "ERROR",
    "EQUAL", "COLON", "COMMA"
};
// TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!

#define KEYWORDS_COUNT 2
string keyword[] = { 
    "public", "private" };

int allowed_is = 0;

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

char* char_scope = "::";

//***************************************************************
struct raghavTable //mTableItem
{
    char* name;
    char* scope;
    int allowed_are;

};

struct agg_Table  //myItem
{
    raghavTable* item;
    agg_Table* before;
    agg_Table* after;
};

struct agg_Table* table_letter;

struct mem_save
{
    string rhs;
    string lhs;
    string s_print1;
    string s_print2;

};

struct agg_Table* table_node_1;

vector <struct mem_save *> mem_note;

struct agg_Table* table_node;



//***************************************************************

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.token_type = ERROR;
    tmp.line_no = 1;
    tmp.lexeme = "";
}

LexicalAnalyzer lexer;
Token token1;
Token token2;
Token token3;

int LexicalAnalyzer::parse_program(void)
{
    token1 = lexer.GetToken();

    if(token1.token_type == ID)
    {
        token2 = lexer.GetToken();
        
        if(token2.token_type == SEMICOLON || 
            token2.token_type == COMMA)
        {
            lexer.UngetToken(token2);
            lexer.UngetToken(token1);
            lexer.parse_global_vars();
            lexer.parse_scope();

        }
        else if (token2.token_type == LBRACE)
        {
            lexer.UngetToken(token2);
            lexer.UngetToken(token1);
            lexer.parse_scope();
        }
        else{
            cout<<"Syntax Error"<<endl;
            exit(1);

        }
    }
    else{

        cout<<"Syntax Error"<<endl;
        exit(1);
    }
    return 0;
}


void list_terminate(char* lexeme)
{

    if(table_letter == NULL)
    {
        return;
    }

    while(table_node->item->scope == lexeme)
    {
        table_node->before = NULL;
        table_node_1->after = NULL;
        if (table_node_1 == table_node)
        {
            table_node_1 = NULL;
            table_node = NULL;
            return;
        }
        else if(table_node_1->before != NULL)
        {
            table_node_1 = table_node_1->before;
            table_node = table_node_1->after;
            table_node->after = NULL;
            return;
        }
        else
        {
            table_node_1->after = NULL;
            table_node->before = NULL;
            table_node = table_node_1;
            table_node_1->after = NULL;
        }
    }

    char_scope = table_node->item->scope;
}

int LexicalAnalyzer::parse_global_vars(void){
    token1 = lexer.GetToken();
    if(token1.token_type != ID){
        cout<<"Syntax Error"<<endl;
        exit(1);
    }
    else{
        lexer.UngetToken(token1);
        lexer.parse_varlist();
        token1 = lexer.GetToken();
        if(token1.token_type == SEMICOLON){

        }
        else{
            cout<<"Syntax Error"<<endl;
            exit(1);
        }
    }
    return 0;
}


int LexicalAnalyzer::parse_scope(void)
{
    token1 = lexer.GetToken();
    if(token1.token_type == ID)
    {
        string rexlex = token1.lexeme;
        char_scope = const_cast<char *>(rexlex.c_str());

        token1 = lexer.GetToken();

        if(token1.token_type == LBRACE)
        {
            lexer.parse_public_vars();
            lexer.parse_private_vars();
            lexer.parse_stmt_list();

            token1 = lexer.GetToken();

            if(token1.token_type == RBRACE)
            {
                list_terminate(char_scope);
                token1 = lexer.GetToken();
                if(token1.token_type == END_OF_FILE)
                {
                    list_terminate(char_scope);
                }

                else
                {
                    UngetToken(token1);
                }

                return 0;
            }

            else
            {
                cout<<"Syntax Error"<<endl;
                exit(1);
            }
        }

        else
        {
            cout<<"Syntax Error"<<endl;
            exit(1);
        }
    }

    else
    {
        cout<<"Syntax Error"<<endl;
        exit(1);
    }

    return 0;
}

int LexicalAnalyzer::parse_private_vars(void)
{
    token1 = lexer.GetToken();
    if(token1.token_type == PRIVATE)
    {
        allowed_is = 2;
        token1 = lexer.GetToken();
        if(token1.token_type == COLON)
        {
            token1 = lexer.GetToken();
            if(token1.token_type == ID)
            {
                lexer.UngetToken(token1);
                lexer.parse_varlist();
                token1 = lexer.GetToken();

                if(token1.token_type != SEMICOLON)
                {
                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }
            }
            else
            {
                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }
        }
        else
        {
                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }

    }
    else if (token1.token_type == ID)
    {
        lexer.UngetToken(token1);
    }

    else
    {
        cout<<"Syntax Error"<<endl;

    }
        return 0;

}

//*******************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************
//*******************************************************************************************************************************************************************
void list_insertion(char* lexeme){
    if(table_letter != NULL)
    {
        table_node_1 = table_letter;

        while(table_node_1->after != NULL)
        {
            table_node_1 = table_node_1->after;
        }

        struct raghavTable* newItem = (raghavTable*) malloc (sizeof(raghavTable));

        table_node = (agg_Table*) malloc (sizeof(agg_Table));

        table_node->before = table_node_1;
        table_node->after = NULL;
        table_node->item = newItem;
        table_node_1->after = table_node;
        int len = strlen(lexeme);

        table_node->item->name = new char[len+1];

        strcpy(table_node->item->name,lexeme);

        table_node->item->scope = char_scope;
        table_node->item->allowed_are = allowed_is;
        table_node->item->name[len] = '\0';
    }

    else
    {
        table_letter = (agg_Table*) malloc (sizeof(agg_Table));

        struct raghavTable* newItem = (raghavTable*) malloc (sizeof(raghavTable));

        table_letter->item = newItem;
        table_letter->before = NULL;
        table_letter->after = NULL;


        int len = strlen(lexeme);

        table_letter->item->name = new char[len+1];

        strcpy(table_letter->item->name,lexeme);
        table_letter->item->allowed_are = allowed_is;
        table_letter->item->scope = char_scope;
        table_letter->item->name[len] = '\0';
        table_node_1 = table_letter;
        table_node = table_letter;
    }
}


int LexicalAnalyzer::parse_public_vars(void)
{
    token1 = lexer.GetToken();
    
    if(token1.token_type == PUBLIC)
    {
        allowed_is = 1;

        token1 = lexer.GetToken();

        if(token1.token_type == COLON)
        {
            token1 = lexer.GetToken();
            if(token1.token_type == ID)
            {
                lexer.UngetToken(token1);
                lexer.parse_varlist();
                token1 = lexer.GetToken();

                if(token1.token_type != SEMICOLON)
                {
                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }

            }
            else
            {
                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }
        }
        else
        {
                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }
    }
    else if (token1.token_type == ID || token1.token_type == PRIVATE)
    {
        lexer.UngetToken(token1);
    }
    else
    {
        cout<<"Syntax Error"<<endl;
        exit(1);
    }

        return 0;
}

int LexicalAnalyzer::parse_varlist(void)
{
    token1 = lexer.GetToken();
    char* lexeme = (char*) malloc (sizeof (token1.lexeme));

    memcpy(lexeme,(token1.lexeme).c_str(),sizeof(token1));

    list_insertion(lexeme);

    agg_Table* jack = table_letter;

    while(jack != NULL)
    {
        jack = jack->after;
    }

    if(token1.token_type != ID)
    {
        cout<<"Syntax Error"<<endl;
        exit(1);
    }
    else{
        token1 = lexer.GetToken();
        if (token1.token_type == SEMICOLON)
        {
            UngetToken(token1);
            return 0;
        }
        else if(token1.token_type == COMMA)
        {
            lexer.parse_varlist();
            return 0;
        }
        else{
            cout<<"Syntax Error"<<endl;
            exit(1);
        }
    }
}

void output_file(Token tok1, Token tok2)
{
    char* s_print2 = (char*)malloc(sizeof(char)*20);

    struct agg_Table* jack = table_node;

    char* s_print1 = (char*)malloc(sizeof(char)*20);

    struct agg_Table* jacky = table_node;


    while(jacky != NULL){
        if(jacky->item->name == tok1.lexeme)
        {
            if(char_scope != jacky->item->scope && jacky->item->allowed_are == 2){

                jacky = jacky->before;

                continue;

            }
            
            else{

                s_print1 = jacky->item->scope;

                break;

            }

        }

        jacky = jacky->before;
    }

    while(jack != NULL)
    {
        if(jack->item->name == tok2.lexeme)
        {
            if(char_scope != jack->item->scope && jack->item->allowed_are == 2)
            {
                jack = jack->before;
                continue;
            }
            else{
                s_print2 = jack->item->scope;
                break;
            }
        }
        jack = jack->before;       
    }
    if(strlen(s_print2)==0){
        s_print2 = new char[2];
        s_print2[0] = '?';
        s_print2[1] = '\0';
    }

    if(strlen(s_print1)==0){
        s_print1 = new char[2];
        s_print1[0]='?';
        s_print1[1]='\0';
    }

    struct mem_save * jacky_node = new mem_save;
    jacky_node->rhs = tok2.lexeme;
    jacky_node->s_print2 = s_print2;
    jacky_node->lhs = tok1.lexeme;
    jacky_node->s_print1 = s_print1;
    mem_note.push_back(jacky_node);

}

int LexicalAnalyzer::parse_stmt(void)
{
    token1 = lexer.GetToken();

    if(token1.token_type != ID)
    {
        cout<<"Syntax Error"<<endl;
        exit(1);   
    } 
    else
    {
        token2 = lexer.GetToken();
        if(token2.token_type == LBRACE){
            char_scope == const_cast<char*>((token1.lexeme).c_str());
            lexer.UngetToken(token2);
            lexer.UngetToken(token1);
            lexer.parse_scope();
        }
        else if(token2.token_type == EQUAL)
        {
            token3= lexer.GetToken();
            if(token3.token_type == ID)
            {
                output_file(token1, token3);
                token1 = lexer.GetToken();
                if(token1.token_type == SEMICOLON)
                {
                    return 0;
                }
                else{

                    cout<<"Syntax Error"<<endl;
                    exit(1);
                }
            }
            else
            {
                cout<<"Syntax Error"<<endl;
                exit(1);
            }
        }
        else
        {
            cout<<"Syntax Error"<<endl;
            exit(1);
        }
    }
    return 0;
}

int LexicalAnalyzer::parse_stmt_list(void)
{
    token1 = lexer.GetToken();
    if(token1.token_type != ID)
    {
        cout<<"Syntax Error"<<endl;
        exit(1);
    }
    else{
        lexer.UngetToken(token1);
        lexer.parse_stmt();
        token2= lexer.GetToken();
        if(token2.token_type == RBRACE)
        {
            lexer.UngetToken(token2);
            return 0;
        }
        else if(token2.token_type == ID)
        {
            lexer.UngetToken(token2);
            lexer.parse_stmt_list();
            return 0;
        }
        else{
            cout<<"Syntax Error"<<endl;
            exit(1);
        }
    }
}

bool LexicalAnalyzer::SkipComment(){
    char c;
    bool comment = false;
    input.GetChar(c);

    if(input.EndOfInput())
    {
        input.UngetChar(c);
        return comment;
    }

    if(c!='/'){
        input.UngetChar(c);
        return comment;
    }
    else{
        input.GetChar(c);
        if(c!='/')
        {
            comment = false;
            exit(1);
        }
        else{
            comment = true;
            while(c!='\n')
            {
                comment = true;
                input.GetChar(c);
            }
            line_no+=1;
            SkipComment();
        }
    }
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

// Token LexicalAnalyzer::ScanNumber()
// {
//     char c;
//     char a,b,d,e,f,g,h,t;
//     char table[100];
//     //*******
//     int flag0= 0;
//     int flag8 = 0;
//     int fb =0;

//     //*****
//     input.GetChar(c);
//     if (isdigit(c)) {
//         if (c == '0') {
//             tmp.lexeme = "0";
//             flag0 = 1;
//         } else {
//             tmp.lexeme = "";
//             while (!input.EndOfInput() && isdigit(c)) {
//                 tmp.lexeme += c;
//                 input.GetChar(c);
//                 if(c == '0'){
//                     fb = 0;
//                 }
//                 if( c == '9' || c == '8'){
//                     flag8 = 1;
//                 }
//             }
//             if (!input.EndOfInput()) {
//                 input.UngetChar(c);
//             }
//         }

//         input.GetChar(a);
//         if(a == 'x'){
//             input.GetChar(b);
//             if(b == ' '){
//                 input.UngetChar(b);
//             }
//             if(b == '0'){
//                 input.GetChar(d);
//                 if(flag8 == 0 && d == '8'){
                    
//                     tmp.lexeme = tmp.lexeme+a+b+d;
//                     tmp.token_type = BASE08NUM;
//                     tmp.line_no = line_no;
//                     return tmp;
//                 }
//                 else{
//                     input.UngetChar(d);
//                 }
//                 input.UngetChar(b);
                
//             }
//             else if (b == '1')
//             {
//                 input.GetChar(d);
//                 if(d == '6')
//                 {
                    
//                     tmp.lexeme = tmp.lexeme+a+b+d;
//                     tmp.token_type = BASE16NUM;
//                     tmp.line_no = line_no;
//                     return tmp;
//                 }
//                 else{
//                     input.UngetChar(d);
//                 }   
//                 input.UngetChar(b);
//             }
//             input.UngetChar(a);
//         }
//         else if (a == '.')
//         {
//             int num1 = 0, num2 = 0;
//             string tem1;
//             input.GetChar(b);
//             if(isdigit(b)){
//                 while(isdigit(b) && !input.EndOfInput()){
//                     num1 = num1 +1;
//                     if(b == '0'){
//                         num2 = num2 +1;
//                     }
//                     tem1 = tem1 + b;
//                     input.GetChar(b);
//                 }
//                 if (!input.EndOfInput()){
//                     input.UngetChar(b);
//                 }
//                 if(flag0 == 1 && num1 == num2){
//                     input.UngetString(tem1);
//                 }
//                 else{
//                     input.UngetString(tem1);
//                     input.GetChar(b);
//                     tmp.lexeme = tmp.lexeme + '.';
//                     while ((isdigit(b)) && !input.EndOfInput()){
//                         tmp.lexeme = tmp.lexeme + b;
//                         input.GetChar(b);
//                     }
//                     if(!input.EndOfInput()){
//                         input.UngetChar(b);
//                     }

//                     tmp.token_type = REALNUM;
//                     tmp.line_no = line_no;
//                     return tmp;
//                 }

//             }
//             else{
//                 input.UngetChar(b);
//             }
//             input.UngetChar(a);
//         }
//         else if(a == 'F' || a == 'E' || a == 'D' || a == 'C' || a == 'B' || a == 'A'){
//             int num1 = 0, num2 =0;

//             input.UngetChar(a);
//             input.GetChar(table[num1]);

//             while((!input.EndOfInput() && table[num1] == 'B' || table[num1] == 'C' || table[num1] == 'A' || table[num1] == 'E' || table[num1] == 'F' || table[num1] == 'D') || isdigit(table[num1])){
//                 num1 = num1 + 1;
//                 num2 = num2 + 1;
//                 input.GetChar(table[num1]);
//             }
//             if(table[num1] == 'x'){
//                 input.GetChar(table[num1+1]);
//                 if(table[num1+1] == '1'){
//                     input.GetChar(table[num1+2]);
//                     if(table[num1+2] == '6'){
//                         while(num1 > -3){
//                             input.UngetChar(table[num1+2]);
//                             num1 = num1 - 1;
//                         }
//                         while(num2 > -3){
//                             num2 = num2 -1;
//                             input.GetChar(c);
//                             tmp.lexeme = tmp.lexeme + c;
//                         }
//                         tmp.token_type = BASE16NUM;
//                         tmp.line_no = line_no;
//                         return tmp;
//                     }
//                     else{
//                         input.UngetChar(table[num1+2]);
//                         input.UngetChar(table[num1+1]);
//                     }
//                 }
//                 else{
//                     input.UngetChar(table[num1+1]);
//                 }
//             }

//             while (num1 > 0)
//             {
//                 input.UngetChar(table[num1]);
//                 num1 = num1 -1;
//             }
//             input.UngetChar(a);
//         }
//         else{
//             input.UngetChar(a);
//         }

//         // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!

// //***************************************************************************************
//         tmp.token_type = NUM;
//         tmp.line_no = line_no;
//         return tmp;
//     } else {
//         if (!input.EndOfInput()) {
//             input.UngetChar(c);
//         }
//         tmp.lexeme = "";
//         tmp.token_type = ERROR;
//         tmp.line_no = line_no;
//         return tmp;
//     }
// }

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were beforeiously
    // mem_saved due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    SkipComment();
    SkipSpace();

    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '{':
            tmp.token_type = LBRACE;
            return tmp;
        case '}':
            tmp.token_type = RBRACE;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                tmp.token_type = ERROR;
                return tmp;
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else{
                tmp.token_type = ERROR;
            }
                
            return tmp;
    }
}

void list_output(){

    int i=0;
    while(i < mem_note.size())
    {
        cout<< mem_note[i]->s_print1;

        if(mem_note[i]->s_print1 != "::")
            cout<<".";

        cout<<mem_note[i]->lhs<<" = "<< mem_note[i]->s_print2;

        if(mem_note[i]->s_print2 != "::")
            cout<<".";

        cout<<mem_note[i]->rhs<<endl;
        i++;
    }
}

int main()
{
    // LexicalAnalyzer lexer;
    // Token token;

    // token = lexer.GetToken();
    // token.Print();
    // while (token.token_type != END_OF_FILE)
    // {
    //     token = lexer.GetToken();
    //     token.Print();
    // }
    int i;
    i = lexer.parse_program();
    list_output();
}

