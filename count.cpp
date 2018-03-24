#include<stdio.h>
#include<string.h> 
#include<math.h>
#include<iostream>
#include<cstdlib>
#include<algorithm>

using namespace std;

extern int characters = 0, words = 0, lines = 0;
struct word 
{ 
    char c[30]; 
    int n; 
}word[10000]; 



void Count_Characters_Lines(void);
void CountWords(void);



int main()
{
	CountWordsFrequency();
}

//×Óº¯Êý 


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



