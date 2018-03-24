#include<stdio.h>
#include<string.h> 
#include<math.h>
#include<iostream>
#include<cstdlib>
#include<algorithm>

using namespace std;

int characters = 0, words = 0, lines = 0;
struct word 
{ 
    char c[30]; 
    int n; 
}word[10000]; 



void Count_Characters_Lines(void);
void CountWords(void);
int JudgeEqual(char a[], char b[]);
void CountWordsFrequency(void);
void HeapAdjust(int *a,int i,int size);
void BuildHeap(int *a,int size);
void HeapSort(int *a,int size);


int main()
{
	CountWordsFrequency();
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

int JudgeEqual(char a[], char b[]) 
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
	int i=0, j=0, flag = 4, mark=1, m=1, k=0, a[10000];
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
                if( JudgeEqual(b, word[j].c) ==1 )
				{ 
				    m=1; break;
				} 
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
            if(JudgeEqual(b, word[j].c) ==1){ m=1; break;} 
        } 
        if(m) word[j].n++;
        else{
            word[k].n=1; strcpy(word[k].c,b); k++;
		}
	}
	
	printf("words: %d\n", words);
	
	for(i=1; i<=k; i++) a[i]=i;
	HeapSort(a,k);
	
	fclose(fp);
} 

void HeapAdjust(int *a,int i,int size)  //调整堆 
{
    int lchild=2*i;       //i的左孩子节点序号 
    int rchild=2*i+1;     //i的右孩子节点序号 
    int max=i;            //临时变量 
    if(i<=size/2)          //如果i不是叶节点就不用进行调整 
    {
        if(lchild<=size&&word[a[lchild]-1].n >word[a[max]-1].n )
        {
            max=lchild;
        }    
        if(rchild<=size&&word[a[rchild]-1].n >word[a[max]-1].n )
        {
            max=rchild;
        }
        if(max!=i)
        {
            swap(a[i],a[max]);
            HeapAdjust(a,max,size);    //避免调整之后以max为父节点的子树不是堆 
        }
    }        
}

void BuildHeap(int *a,int size)    //建立堆 
{
    int i;
    for(i=size/2;i>=1;i--)    //非叶节点最大序号值为size/2 
    {
        HeapAdjust(a,i,size);    
    }    
} 

void HeapSort(int *a,int size)    //堆排序 
{
    int i,j,temp;
    BuildHeap(a,size);
    for(i=size;i>=1;i--)
    {
          temp=a[1]; a[1]=a[i]; a[i]=temp;         //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
          //BuildHeap(a,i-1);        //将余下元素重新建立为大顶堆 
          HeapAdjust(a,1,i-1);      //重新调整堆顶节点成为大顶堆
          if(i==size-9 || i==1 )
          {
		       for(j=size; j >=( size>10? (size-9) : 1) ; j--) printf("<%s> :%d\n", word[a[j]-1].c, word[a[j]-1].n );  return;   
		  }
	}
} 







