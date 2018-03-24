#include<stdio.h>
#include<string.h> 
#include<math.h>
#include<iostream>
#include<cstdlib>
#include<algorithm>

using namespace std;

int characters = 0, words = 0, lines = 0;
struct Word 
{ 
    char c[30]; 
    int n; 
}word[10000]; 

struct Column
{
	char c1[30];
	char c2[30];
};

struct Phrase 
{ 
    Column p;
    int n; 
}phrase[10000];

//函数声明 功能基本如函数名所示 

void Count_Characters_Lines(void);             //统计字符数和行数 
void CountWords(void);                         //统计单词总数 
int  JudgeWordEqual(char a[], char b[]);       //判断两个单词是否等价 
void CountWordsFrequency(void);                //统计各单词频率及排序 

void HeapAdjustWord(int *a,int i,int size);    //调整堆 
void BuildHeapWord(int *a,int size);           //建堆 
void HeapSortWord(int *a,int size);            //针对单词频率统计的堆排序 

void HeapAdjustPhrase(int *a,int i,int size);
void BuildHeapPhrase(int *a,int size);
void HeapSortPhrase(int *a,int size);          //针对词组频率统计的堆排序 

void CountPhrasesFrequency(void);              //统计各词组频率及排序 
int JudgePhraseEqual(Column p, char a[], char b[]);          //判断后两个单词组成的词组是否与第一个词组等价 


int main()
{
	CountPhrasesFrequency();
}

//子函数 


void Count_Characters_Lines() 
{
	FILE *fp;
	char ch;
	if((fp=fopen("01.txt","r"))==0) return;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch >= 0 && ch <= 127) characters++;
		if (ch == '\n') lines++;
	}
	printf("characters: %d\nlines: %d\n", characters, lines);
	fclose(fp);
}

void CountWords()
{
	FILE *fp;
	char ch;
	int flag = 4, mark=1 ;
	if((fp=fopen("000.txt","r"))==0) return;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (flag>0 && mark==1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{		
			flag--;
			continue;
		}
		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; mark=1; words++;
			continue;
		}
		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z')) 
		{ 
		    flag=4; 
		    if(ch >= '0' && ch <= '9') mark=0;
		    continue;
		}
		if(flag==4 && mark==0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark=1;
	}
	if(flag==0) words++;
	printf("words: %d\n", words);
	fclose(fp);
}

int JudgeWordEqual(char a[], char b[]) 
{
	int i=0, j, la=strlen(a), lb=strlen(b);
	char a1[30], b1[30];
	if(la < lb)
	{
		for(i=lb-1; i> la -1; i--) if(!(b[i] >= '0' && b[i] <= '9')) return 0;
		while((a[i] >= '0' && a[i] <= '9') && (b[i] >= '0' && b[i] <= '9')) i--;
		if(((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')))
		{
			j=i;
			for(; i>=0; i--) if(a[i] != b[i]) return 0;
			if(a[j+1]<b[j+1]) strcpy(b,a);
			return 1;
		}
		return 0;
	}
	else if(la > lb)
	{
		for(i=la-1; i> lb -1; i--) if(!(a[i] >= '0' && a[i] <= '9')) return 0;
		while((a[i] >= '0' && a[i] <= '9') && (b[i] >= '0' && b[i] <= '9')) i--;
		if(((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')))
		{
			j=i;
			for(; i>=0; i--) if(a[i] != b[i]) return 0;
			if(a[j+1]<b[j+1]) strcpy(b,a);
			return 1;
		}
		return 0;
	}
	else{
		i= la-1;
		while((a[i] >= '0' && a[i] <= '9') && (b[i] >= '0' && b[i] <= '9')) i--;
		if(((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')))
		{
			j=i;
			for(; i>=0; i--) if(a[i] != b[i]) return 0;
			if(a[j+1]<b[j+1]) strcpy(b,a);
			return 1;
		}
		return 0;
	}
}

void CountWordsFrequency()
{
	FILE *fp;
	int i=0, j=0, flag = 4, mark=1, m=1, k=1, a[10000];
	char ch,b[30];
	if((fp=fopen("000.txt","r"))==0) return;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (flag>0 && mark==1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{	
			b[i]=ch; i++; 
			flag--;
			continue;
		}
		
		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; words++; mark=1;
		    b[i]='\0'; i=0;  m=0; 
		    
		    for(j=0;j < k;j++) 
            {  
                if( JudgeWordEqual(b, word[j].c) ==1 )	{  m=1; break;	} 
                
            } 
            if(m) word[j].n++;
            else{
            	word[k].n=1; strcpy(word[k].c,b); k++;
			}
			continue;
		}
		
		if (flag==0) { b[i]=ch; i++; continue;}
		
		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z')) 
		{
			flag=4;  i=0; memset(b,'\0',sizeof(b)); 
			if(ch >= '0' && ch <= '9') mark=0; continue;
		}
		
		if(flag==4 && mark==0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark=1;
	}
	if(flag==0)
	{
		words++;
		b[i]='\0'; m=0;
		for(j=0; j < k; j++) 
        { 
            if(JudgeWordEqual(b, word[j].c) ==1){ m=1; break;} 
        } 
        if(m) word[j].n++;
        else{
            word[k].n=1; strcpy(word[k].c,b); k++;
		}
	}
	
	printf("words: %d\n", words);
	
	for(i=1; i<=k; i++) a[i]=i;
	HeapSortWord(a,k-1);
	
	fclose(fp);
} 

void CountPhrasesFrequency()
{
	FILE *fp;
	int i=0, j=0, l=1, flag = 4, mark=1, m=1, n=1, k=1, a[10000];
	char ch,b[30], temp[30];
	if((fp=fopen("61.txt","r"))==0) return;
	while ((ch = fgetc(fp)) != EOF)
	{
		if (flag>0 && mark==1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{	
			b[i]=ch; i++; 
			flag--;
			continue;
		}
		
		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; words++; mark=1;
		    b[i]='\0'; i=0;  m=0; n=0;
		    
		    for(j=1;j < k;j++) 
            {  
                if( JudgeWordEqual(b, word[j].c) ==1 )	
				{  
				    m=1; 
					break;	
				} 
            } 
            if(m) word[j].n++;
            else{
            	word[k].n=1; strcpy(word[k].c,b); k++;			
			}
			
			
            for(j=1; j<l; j++)
            {
            	if( JudgePhraseEqual(phrase[j].p, temp, b )==1 )
                {
                   n=1; break;
			    }
			}
			if(n) phrase[j].n ++;
			else{
				phrase[l-1].n =1; strcpy(phrase[l-1].p.c1 ,temp); strcpy(phrase[l-1].p.c2 ,b); l++;
			}
			
			strcpy(temp,b);
			continue;
		}
		
		if (flag==0) { b[i]=ch; i++; continue;}
		
		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z')) 
		{
			flag=4;  i=0; memset(b,'\0',sizeof(b)); 
			if(ch >= '0' && ch <= '9') mark=0; continue;
		}
		
		if(flag==4 && mark==0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark=1;
	}
	if(flag==0)
	{
		words++;
		b[i]='\0'; m=0; n=0;
		for(j=1;j < k;j++) 
            {  
                if( JudgeWordEqual(b, word[j].c) ==1 )	
				{  
				    m=1; 
					break;	
				} 
            } 
            if(m) word[j].n++;
            else{
            	word[k].n=1; strcpy(word[k].c,b); k++;			
			}
			
			
            for(j=1; j<l; j++)
            {
            	if( JudgePhraseEqual(phrase[j].p, temp, b )==1 )
                {
                   n=1; break;
			    }
			}
			if(n) phrase[j].n ++;
			else{
				phrase[l].n =1; strcpy(phrase[l].p.c1 ,temp); strcpy(phrase[l].p.c2 ,b); l++;
			}
	}
	
	printf("words: %d\n", words);
	
	for(i=1; i<=l; i++) a[i]=i;
	HeapSortPhrase(a,l-1);
	
	fclose(fp);
} 

int JudgePhraseEqual(Column p, char a[], char b[])
{
	if(JudgeWordEqual(p.c1,a)==1 && JudgeWordEqual(p.c2,b)==1) return 1;
	return 0;
}

void HeapAdjustWord(int *a,int i,int size)  //调整堆 
{
    int lchild=2*i;       //i的左孩子节点序号 
    int rchild=2*i+1;     //i的右孩子节点序号 
    int max=i;            //临时变量 
    if(i<=size/2)          //如果i不是叶节点就不用进行调整 
    {
        if(lchild<=size&&word[a[lchild]].n >word[a[max]].n ) max=lchild;   
        if(rchild<=size&&word[a[rchild]].n >word[a[max]].n ) max=rchild;
        if(max!=i)
        {
            swap(a[i],a[max]);
            HeapAdjustWord(a,max,size);    //避免调整之后以max为父节点的子树不是堆 
        }
    }        
}

void BuildHeapWord(int *a,int size)    //建立堆 
{
    int i;
    for(i=size/2;i>=1;i--)    //非叶节点最大序号值为size/2 
    {
        HeapAdjustWord(a,i,size);    
    }    
} 

void HeapSortWord(int *a,int size)    //堆排序 
{
    int i,j,temp;
    BuildHeapWord(a,size);
    for(i=size;i>=1;i--)
    {
          temp=a[1]; a[1]=a[i]; a[i]=temp;         //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
          //BuildHeap(a,i-1);        //将余下元素重新建立为大顶堆 
          HeapAdjustWord(a,1,i-1);      //重新调整堆顶节点成为大顶堆
          if(i==size-9 || i==1 )
          {
		       for(j=size; j >=( size>10? (size-9) : 1) ; j--) printf("<%s> :%d\n", word[a[j]].c, word[a[j]].n );  return;   
		  }
	}
} 

void HeapAdjustPhrase(int *a,int i,int size)  //调整堆 
{
    int lchild=2*i;       //i的左孩子节点序号 
    int rchild=2*i+1;     //i的右孩子节点序号 
    int max=i;            //临时变量 
    if(i<=size/2)          //如果i不是叶节点就不用进行调整 
    {
        if(lchild<=size&&phrase[a[lchild]].n >phrase[a[max]].n ) max=lchild;   
        if(rchild<=size&&phrase[a[rchild]].n >phrase[a[max]].n ) max=rchild;
        if(max!=i)
        {
            swap(a[i],a[max]);
            HeapAdjustPhrase(a,max,size);    //避免调整之后以max为父节点的子树不是堆 
        }
    }        
}

void BuildHeapPhrase(int *a,int size)    //建立堆 
{
    int i;
    for(i=size/2;i>=1;i--)    //非叶节点最大序号值为size/2 
    {
        HeapAdjustPhrase(a,i,size);    
    }    
} 

void HeapSortPhrase(int *a,int size)    //堆排序 
{
    int i,j,temp;
    BuildHeapPhrase(a,size);
    for(i=size;i>=1;i--)
    {
          temp=a[1]; a[1]=a[i]; a[i]=temp;         //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
          //BuildHeap(a,i-1);        //将余下元素重新建立为大顶堆 
          HeapAdjustPhrase(a,1,i-1);      //重新调整堆顶节点成为大顶堆
          if(i==size-9 || i==1 )
          {
		       for(j=size; j >=( size>10? (size-9) : 1) ; j--) printf("<%s %s> :%d\n", phrase[a[j]].p.c1, phrase[a[j]].p.c2, phrase[a[j]].n);  return;   
		  }
	}
} 






