#include<stdio.h>
#include<string.h>

char keywords[][20]={"auto","enum","register","typedef","const","short","unsigned","signed","sizeof","volatile","static","extern","void","main","char","int","double","long","struct","if","while","for","do","else","return","break","continue","goto","switch","case","default","float"};
int read_data(char *input_name,char *data){
    FILE *fp;
    fp=fopen(input_name,"r");
    int size=0;
    int pre=0;
    int scom=0;
    while (!feof(fp)){
        char ch=(char)fgetc(fp);
        //directives excluded
        if(ch=='#'){
            pre=1;
        }
        else if(pre==1 && ch=='\n'){
            pre=0;
        }
        char ch3;
        if(ch=='/'){
            ch3=(char)fgetc(fp);
           
            if(ch3=='/'){
                while(ch3!='\n'&&!feof(fp)){
                ch3=(char)fgetc(fp);}
                }
            else if(ch3=='*'){
                char ch1=(char)fgetc(fp);
                char ch2=(char)fgetc(fp);
                while(ch1!='*'&&ch2!='/'){
                    ch1=(char)fgetc(fp);
                 ch2=(char)fgetc(fp);
                }
                
            }
            else{
                if(pre!=1&&ch!='\n'&&ch!=EOF)
                    data[size++]=ch;
                if(pre!=1&&ch!='\n'&&ch!=EOF)
                    data[size++]=ch3;
            }

            ch=(char)fgetc(fp);

        }
            
        

        if(pre!=1&&ch!='\n'&&ch!=EOF)
            data[size++]=ch;
        
        
    }
    
    fclose(fp);
    return size;
}
int isdelimeter(char ch){
    if(ch!='\r'&&ch!=' '&&ch!='\t'&&ch!='{'&&ch!='}'&&ch!='('&&ch!=')'&&ch!=';'&&ch!='['&&ch!=']'&&ch!=',')
        return 0;
    return 1;
}
int isoperator(char ch){
    if(ch=='+'||ch=='-'||ch=='='||ch=='/'||ch=='*'||ch=='%'||ch=='&'||ch=='^'||ch=='|'||ch=='?'||ch==':')
        return 1;
    return 0;
}
int iskeyword(char key[]){
    //printf("%s",key);
    for(int i=0;i<32;i++){
        if(strcmp(key,keywords[i])==0){
            //printf("%s",key);
            return 1;
        }
    }
    return 0;
}
int isnumber(char tok[]){
    int i=0;
    char ch=tok[i++];
    
    while(ch!='\0'){
        if((ch<48 || ch>57)&&ch!='.')
            return 0;
        ch=tok[i++];
    }
    return 1;

}
void validate_identifier(char tok[]){
    if(tok[0]>=48&&tok[0]<=57){
        printf("Syntax Error: invalid identifier %s\n",tok);
    }
    for(int i=0;i<strlen(tok);i++){
        if((tok[i]>='A'&&tok[i]<='Z')||(tok[i]>='a'&&tok[i]<='z')||tok[i]=='_'||(tok[i]>='0'&&tok[i]<='9')){
            ;
        }
        else
        {
            printf("Syntax Error: invalid identifier %s\n",tok);
            break;
        }
        
    }
}
void write_output(char tok_tab[][60],int size,char argv[]){
    FILE *fp;
    char outname[100];
    int j=0;

    while(j<strlen(argv)){
        if(argv[j]=='.')
            break;
        outname[j++]=argv[j];
    }
    outname[j++]='.';
    outname[j++]='t';
    outname[j++]='o';
    outname[j++]='k';
    outname[j++]='\0';
    fp=fopen(outname,"w");
    for(int i=0;i<size;i++){
        fwrite(tok_tab[i],strlen(tok_tab[i]),1,fp);
        fwrite("\n",1,1,fp);
    }
    fclose(fp);

}
void get_tokens(char *data,int size,char argv[]){
    
    char token_table[100][60];
    int tok_tab_i=0;
    char cur_buf[32];//max token size 32
    int cur_buf_i=0;
    for(int i=0;i<size;i++){
        int ch=data[i];

        
        if(!isdelimeter(ch)&&!isoperator(ch)&&ch!='"'){
            cur_buf[cur_buf_i++]=ch;
            //printf("%s\n",cur_buf);
        }
        
        else{
            cur_buf[cur_buf_i++]='\0';
            if(cur_buf[0]!='\0'&&iskeyword(cur_buf)){
                char token[60]="<keyword,";
                //cur_buf[cur_buf_i++]='\0';
                
                strcat(token,cur_buf);
                token[8+cur_buf_i]='>';

                strcpy(token_table[tok_tab_i++],token);
                cur_buf_i=0;
            }
            else{
                if(cur_buf[0]!='\0'&&isnumber(cur_buf)){
                    char token[60]="<constant,";
                
                
                strcat(token,cur_buf);
                token[9+cur_buf_i]='>';

                strcpy(token_table[tok_tab_i++],token);
                cur_buf_i=0;
                }
                else if(cur_buf[0]!='\0'){

                validate_identifier(cur_buf);
                char token[60]="<identifier,";
                
                
                strcat(token,cur_buf);
                token[11+cur_buf_i]='>';

                strcpy(token_table[tok_tab_i++],token);
                cur_buf_i=0;
                }
                
                
            }
            cur_buf_i=0;
            cur_buf[0]='\0';



            if(isdelimeter(ch)&&ch!=' '){
                char token[60]="<delimeter,";
                token[11]=ch;
                token[12]='>';
                strcpy(token_table[tok_tab_i++],token);
            }

            if(isoperator(ch)){
                char token[60]="<operator,";
                int jj=10;
                token[jj++]=ch;
                if(isoperator(data[i+1])){
                        
                        token[jj++]=data[i+1];
                        i++;
                }
                
                token[jj++]='>';
                strcpy(token_table[tok_tab_i++],token);
            }
            if(ch=='"'){
                char token[60]="<literal,";
                int jj=9;
                token[jj++]=ch;
                i++;
                while(data[i]!='"'){
                        
                        token[jj++]=data[i];
                        i++;
                }
                token[jj++]='"';
                token[jj++]='>';
                strcpy(token_table[tok_tab_i++],token);
            }
            
        }
    }
    // for(int s=0;s<tok_tab_i;s++){
    //     printf("%s\n",token_table[s]);
    // }
    write_output(token_table,tok_tab_i,argv);

}

void main(char argc, char *argv[]){
    
    char  data[1000];
    if(argc>1){
        int size=read_data(argv[1],data);
        get_tokens(data,size,argv[1]);
    }
    else{
        printf("Please input a file name...");
    }
    //printf("%s",data);

}