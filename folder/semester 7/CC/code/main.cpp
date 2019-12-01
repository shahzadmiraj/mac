#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string.h>
#include<cctype>
using namespace std;
#define totalAscci 256
#define identifyCol 26
#define transitionRow 45
#define tableRows 100 //tabel depend speed
#define totalTokens 29 //29 without keywords
struct node
{
public:
    node *next;
    int scope;
    int index;
    char *lexiem;
    node()
    {
        index=-1;
        next=NULL;
        lexiem=NULL;
        scope=0;
    }
    void setLexiem(char *array)
    {
//        int length=int(size_t(array));
//        lexiem=new char[length];
//        strcpy(lexiem, array);
        lexiem=array;
    }
};
class hashing
{
public:
    node rows[tableRows];
    int hashingfunction(char array[])
    {
        int index=0;
        for(int i=0;array[i]!='\0';i++)
        {
            index=index+i+array[i];
        }
        return index%tableRows;
    }
    void display()
    {
        node *temp;
        for(int i=0;i<tableRows;i++)
        {
            
            temp=&rows[i];
            if(temp->lexiem!=NULL)
            {
                cout<<"\n";
            }
            while(temp->lexiem!=NULL)
            {
                cout<<"  "<<temp->lexiem<<temp->index;
                temp=temp->next;
                if(temp==NULL)
                {
                    break;
                }
            }
        }
    }
    int search(char array[])
    {
        int index=0;
        index=hashingfunction(array);
        node *temp=&rows[index];
        while (1)
        {
            
            if(temp->lexiem!=NULL)
            {
                if(strcmp(temp->lexiem, array)==0)
                {
                    //match arrays
                    return temp->index;
                }
            }
            temp=temp->next;
            if(temp==NULL)
            {
                return 0;
            }
        }
        return 0;
    }
    void add(char array[],int value)
    {
    
        if(!search(array))
        {
            //lexim not present so push
            int index=hashingfunction(array);
            node *temp=&rows[index];
            if(rows[index].lexiem==NULL)
            {
                //first row exist
                temp->setLexiem(array);
                temp->index=value;
            }
            else
            {
                //after first row new create row
                while(temp->next!=NULL)
                {
                    temp=temp->next;
                }
                node *newtemp=new node;
                newtemp->setLexiem(array);
                newtemp->index=value;
                temp->next=newtemp;
            }
            
        }
        else
        {
            delete array;
        }
    }
    
};

hashing symTable;
hashing keywords;

int readKeywordFromFile()
{
    ifstream inFile;
    inFile.open("keyword.txt");
    if (!inFile)
    {
        cout << "keyword file Unable to open file";
        return 1;
    }
    else
    {
        int num;
        string keyword;
        char *temp;
        while (inFile>>keyword)
        {
            temp=new char[keyword.length()+1];
            strcpy(temp, keyword.c_str());
            inFile>>num;
            keywords.add(temp,num);
        }
        return 0;
    }
}
ofstream *tokenFile;
ofstream *errorFile;
void initializeArray(int *array, int size)
{
    for (int i = 0; i<size; i++)
    {
        array[i] = -1;
    }
}
void initializeAcceptarray(int *array, int size)
{
    for (int i = 0; i<size; i++)
    {
        array[i] = 0;
    }
}

void display(int *array, int size)
{
    for (int i = 0; i<size; i++)
    {
        cout << array[i] << " ";
    }
}
int readIdentityfile(int *identityArray)
{
    
    char ch;
    int index;
    ifstream inFile;
    inFile.open("identity.txt");
    if (!inFile) {
        cout << "identity file Unable to open file";
        return 1; // terminate with error
    }
    initializeArray(identityArray, totalAscci);
    while (inFile >> ch)
    {
        inFile >> index;
        identityArray[int(ch)] = index;
    }
    //    " 36 bcz not read this exist "
    ch = '"';
    identityArray[int(ch)] = 12;
      //    space 25 bcz not read this "
    ch = ' ';
    identityArray[int(ch)] = 24;
    //    \n 26 bcz not read this "
    ch = '\n';
    identityArray[int(ch)] = 25;
    inFile.close();
    return 0;
}
int initiallize2d(int **transitionArray)
{
    for (int r = 0; r < transitionRow; r++)
    {
        transitionArray[r] = new int[identifyCol];
        for (int c = 0; c < identifyCol; c++)
        {
            transitionArray[r][c] = -1;
        }
    }
    return 0;
}
int display2d(int **transitionArray)
{
    cout << "\n";
    for (int r = 0; r < transitionRow; r++)
    {
        for (int c = 0; c < identifyCol; c++)
        {
            cout << transitionArray[r][c] << " ";
        }
        cout << "\n";
    }
    return 0;
}
int readTransitionfile(int **transitionArray)
{
    initiallize2d(transitionArray);
    int number;
    ifstream inFile;
    inFile.open("transition.txt");
    if (!inFile)
    {
        cout << "transition file Unable to open file";
        return 1; // terminate with error
    }
    int r = -100;
    int c=0;
    while (inFile>>number)
    {
        if(number==-100)
        {
            inFile>>number;
            c=0;
            r=number;
        }
        else if(r==-100)
        {
            r=number;
            c=0;
        }
        else
        {
            transitionArray[r][c]=number;
            c++;
        }
    }
    
    inFile.close();

    return 0;
}
int openFile()
{
    
    tokenFile=new ofstream;
    errorFile=new ofstream;
    tokenFile->open("token.txt");
    if (!tokenFile)
    {
        cout <<"token file is Unable to open file";
        return 1;
    }
    errorFile->open("error.txt");
    if (!errorFile)
    {
        cout <<"error file is Unable to open file";
        return 1;
    }
    
    return 0;
}

void pasteInfile(int fileno,string sourceArr,int start,int end,int tokenNo)
{
    if(fileno==0)
    {
        //error file
        *errorFile<<" ";
        for(int i=start;i<=end;i++)
        {
            *errorFile<<sourceArr[i];
        }
        *errorFile<<" ";
    }
    else
    {
        
        //token file
        *tokenFile<<" <";
        for(int i=start;i<=end;i++)
        {
            *tokenFile<<sourceArr[i];
        }
        *tokenFile<<" , "<<tokenNo<<" >";
    }
}
int readAcceptStatefile(int acceptArray[])
{

    int index;
    int state;
    ifstream inFile;
    inFile.open("accept.txt");
    if (!inFile)
    {
        cout << "identity file Unable to open file";
        return 1; // terminate with error
    }
    initializeAcceptarray(acceptArray, transitionRow);
    while (inFile >> index)
    {
        inFile >> state;
        acceptArray[index]=state;
    }
    inFile.close();
    return 0;
}
string ReadSourceFile()
{
    string sourceArr;
    ifstream inFile;
    inFile.open("source.txt");
    if (!inFile)
    {
        cout << "source file Unable to open file";
        return NULL;
    }
    ostringstream ss;
    ss<<inFile.rdbuf();
    sourceArr=ss.str();
    inFile.close();
    return sourceArr;
}
char* getstring(string sourceArr,int start,int end)
{
    char *array;
    array=new char[(end-start)+2];
    int i=0;
    for (; start<=end; start++)
    {
        array[i]=sourceArr[start];
        i++;
    }
    array[i]='\0';
    return array;
}
void checkKeywordOrError(string sourceArr,int &start,int &end,int less)
{
    int index=keywords.search(getstring(sourceArr, start,end-less));
    if(index)
    {
        //keywords so paste on token file
         pasteInfile(1,sourceArr, start, end-less,index);
        
    }
    else
    {
        pasteInfile(0, sourceArr, start, end-less,0);
        //keywords so paste on token file
    }
    
}

int checkExponential(string sourceArr,int start,int end)
{
    for(int i=start;i<end;i++)
    {
        if((isdigit(sourceArr[i])==0)||(sourceArr[i]=='.'))
        {
            return 0;
        }
    }
    if((sourceArr[end+1]=='+')||(sourceArr[end+1]=='-'))
    {
        return 6;
    }
    return 0;
    
}
int GetToken(string sourceArr,int &start,int &end,int acceptArray[],int **transitionArray,int identityArray[])
{
    
    int exponent=0;
    int state=0;
    int accept,map,newstate=0;
        char ch;
    start=end;
    while ((sourceArr[start]==' ')||(sourceArr[start]=='\n')||(sourceArr[start]=='\t'))
    {
        start++;
    }
    end=start;
    ch=sourceArr[start];
    if(ch=='\0')
    {
        return 1;
    }
    accept=acceptArray[state];
    

    while(accept==0)
    {
        map=identityArray[ch];
        if(map==-1)
        {
            //error report
            checkKeywordOrError(sourceArr, start, end,0);
            end++;
            return 0;
        }
        if((map==0)&& (ch=='E'))
        {
            //check Exponential
            exponent=checkExponential(sourceArr, start, end);
            if(exponent!=0)
            {
                state=9;
                map=exponent;
            }
            
        }
        newstate=transitionArray[state][map];
        if(newstate==-1)
        {
            //error report
            checkKeywordOrError(sourceArr, start, end,1);
            return 0;
        }
        accept=acceptArray[newstate];
        if(ch=='\0')
        {
            return 1;
        }
        if(accept==0)
        {
            end++;
            ch=sourceArr[end];
        }
        state=newstate;
    }
    //token make
    if(accept<0)
    {
        accept=accept*-1;
        end--;
    }
    pasteInfile(1,sourceArr, start, end,accept);
    if(accept==27)
    {
        //only ID
        symTable.add(getstring(sourceArr, start, end),accept);
    }
    end++;
    return 0;
}





void Lexical(string sourceArr,int acceptArray[],int **transitionArray,int identityArray[])
{
    int start=0;
    int end=0;
    while(1)
    {
        if(GetToken(sourceArr,start,end,acceptArray,transitionArray,identityArray))
        {
                break;
        }
    }
}
int main()
{
    int** transitionArray = new int*[transitionRow];
    int identityArray[totalAscci];
    int acceptArray[transitionRow];
    string sourceArr;
    if (readIdentityfile(identityArray))
    {
        return 0;
    }
    if (readTransitionfile(transitionArray))
    {
        return 0;
    }
    if(readAcceptStatefile(acceptArray))
    {
        return 0;
    }
    sourceArr=ReadSourceFile();
    if(sourceArr=="\0")
    {
        return 0;
    }
    if(openFile())
    {
        return 0;
    }
    if (readKeywordFromFile())
    {
        //read keyword file
        return 0;
    }
   //display2d(transitionArray);
    
 Lexical(sourceArr,acceptArray,transitionArray,identityArray);

    cout<<"\n ..........Symbal Table .........\n";
    symTable.display();
    
    cout<<"\n ..........keywords Table .........\n";
    keywords.display();
    tokenFile->close();
    errorFile->close();
    return 0;
}
