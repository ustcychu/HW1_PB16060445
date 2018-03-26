#include<stdio.h>
#include<string.h> 
#include<math.h>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<io.h>
#include<vector>
#define NUM 500000 

using namespace std;

int characters = 0, words = 0, lines = 0,k,l;  
FILE *fin, *fout;

struct Word 
{ 
    char c[100]; 
    int n; 
}word[NUM]; 

struct Column
{
	char c1[100];
	char c2[100];
};

struct Phrase 
{ 
    Column p;
    int n; 
}phrase[NUM];

void getFiles(string folderpath, vector<string>&filespath); //�ļ����/�ļ���Ϣ  
int JudgeOrder(char a[], char b[]);                              //�ж��ֵ�˳�� 
void DictOrder(void);                          //�ֵ����� 
void Count_Characters_Lines(void);             //ͳ���ַ��������� 
void CountWords(void);                         //ͳ�Ƶ������� 
int  JudgeWordEqual(char a[], char b[]);      //�ж����������Ƿ�ȼ� 
void CountWordsFrequency(void);                //ͳ�Ƹ�����Ƶ�ʼ����� 

void HeapAdjustWord(int *a, int i, int size);    //������ 
void BuildHeapWord(int *a, int size);           //���� 
void HeapSortWord(int *a, int size);            //��Ե���Ƶ��ͳ�ƵĶ����� 

void HeapAdjustPhrase(int *a, int i, int size);
void BuildHeapPhrase(int *a, int size);
void HeapSortPhrase(int *a, int size);          //��Դ���Ƶ��ͳ�ƵĶ����� 

void CountPhrasesFrequency(void);              //ͳ�Ƹ�����Ƶ�ʼ����� 
int JudgePhraseEqual(Column p, char a[], char b[]);          //�жϺ�����������ɵĴ����Ƿ����һ������ȼ� 
void statistics(void);



int main()
{
	char folderpath[100] = "C:/Users/pc/Desktop/newsample", q[200], *p;
	int a[NUM], i, j, t=1, l=1;
	vector<string> filepath;
	//gets(folderpath);
	getFiles(folderpath, filepath);
	int fpsize = filepath.size();
	fout = fopen("C:/Users/pc/Desktop/result.txt", "w");
	for (i = 0; i < fpsize; i++)
	{
		strcpy(q, filepath[i].c_str()); p = q;
		while (*p != '\0')
		{
			if (*p == '\\') *p = '/'; p++;
		}

		if ((fin = fopen(q, "r")) == 0) return 0;

		//CountWordsFrequency();
		//CountPhrasesFrequency();
		statistics();
        //cout << q << endl;
 
		fclose(fin);

	}

	fprintf(fout, "characters: %d\nlines: %d\n", characters, lines);
	fprintf(fout, "words: %d\n\n", words);
	
	for(i=1; i<=52; i++)
	{
		for(j=i; word[j].n && j<NUM ; j+=52)
		{
			a[t]=j; t++;
		}
	}
	HeapSortWord(a, t - 1);
	
	for(i=1; i<=52; i++)
	{
		for(j=i; phrase[j].n && j<NUM ; j+=52)
		{
			a[l]=j; l++;
		}
	}
	HeapSortPhrase(a, l - 1);



	fclose(fout);
	return 0;
}

//�Ӻ��� 

void getFiles(string folderpath, vector<string>& filespath)
{
	long   hFile = 0;
	struct _finddata_t *fileinfo = new _finddata_t;
	string pathPrefix;

	if ((hFile = _findfirst(pathPrefix.assign(folderpath).append("\\*").c_str(), fileinfo)) != -1) {
		do {
			if ((fileinfo->attrib &  _A_SUBDIR)) { //�������Ŀ¼,�ݹ�������ļ���
				if (strcmp(fileinfo->name, ".") != 0 && strcmp(fileinfo->name, "..") != 0)
					getFiles(pathPrefix.assign(folderpath).append("\\").append(fileinfo->name), filespath);
			}
			else { //������ļ����������ҽ��
				filespath.push_back(pathPrefix.assign(folderpath).append("\\").append(fileinfo->name));
			}
		} while (_findnext(hFile, fileinfo) == 0); //ͨ������رյ�ǰĿ¼����       
		_findclose(hFile);
	}
}

int JudgeOrder(char a[], char b[])
{
	int i;
	for (i = 0; a[i] && b[i]; i++)
	{
		if (a[i] < b[i]) return -1;
		else if ((a[i] == b[i])) return 0;
		else return 1;
	}
	if (!a[i] && b[i]) return -1;
	else if (a[i] && !b[i] ) return 1;
	else return 0;
}

void Count_Characters_Lines()
{
	char ch;
	while ((ch = fgetc(fin)) != EOF)
	{
		if (ch >= 32 && ch <= 126) characters++;
		if (ch == '\n') lines++;
	}
	fprintf(fout, "characters: %d\nlines: %d\n", characters, lines);
}

void CountWords()
{
	char ch;
	int flag = 4, mark = 1;
	while ((ch = fgetc(fin)) != EOF)
	{
		if (flag>0 && mark == 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{
			flag--;
			continue;
		}
		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; mark = 1; words++;
			continue;
		}
		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z'))
		{
			flag = 4;
			if (ch >= '0' && ch <= '9') mark = 0;
			continue;
		}
		if (flag == 4 && mark == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark = 1;
	}
	if (flag == 0) words++;
	fprintf(fout, "words: %d\n", words);
}

int JudgeString(char a[], char b[], int n)
{
	int i = 0;
	for (; i<n; i++)
	{
		if (!(a[i] == b[i] || abs(a[i] - b[i]) == 32)) return 0;
	}
	return 1;
}

int JudgeWordEqual(char a[], char b[])
{
	int i = 0, j, la = strlen(a), lb = strlen(b), flag = 0;
	char a1[200], b1[200];
	if (la < lb)
	{
		for (i = lb - 1; i> la - 1; i--) if (!(b[i] >= '0' && b[i] <= '9')) return 0;
		while ((a[i] >= '0' && a[i] <= '9') && (b[i] >= '0' && b[i] <= '9')) i--;
		if (((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')))
		{
			j = i;
			for (; i >= 0; i--) if (!(a[i] == b[i]) && !(((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')) && abs(a[i] - b[i]) == 32)) return 0;
			for (i = 0; i <= j; i++)
			{
				if (a[i] < b[i]) { strcpy(b, a); break; }
				else if ((a[i] > b[i])) break;
			}
			if (!a[i] && b[i]) strcpy(b, a);
			return 1;
		}
		return 0;
	}
	else if (la > lb)
	{
		for (i = la - 1; i> lb - 1; i--) if (!(a[i] >= '0' && a[i] <= '9')) return 0;
		while ((a[i] >= '0' && a[i] <= '9') && (b[i] >= '0' && b[i] <= '9')) i--;
		if (((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')))
		{
			j = i;
			for (; i >= 0; i--) if (!(a[i] == b[i]) && !(((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')) && abs(a[i] - b[i]) == 32)) return 0;
			for (i = 0; i <= j; i++)
			{
				if (a[i] < b[i]) { strcpy(b, a); break; }
				else if ((a[i] > b[i])) break;
			}
			if (!a[i] && b[i]) strcpy(b, a);
			return 1;
		}
		return 0;
	}
	else {
		i = la - 1;
		while ((a[i] >= '0' && a[i] <= '9') && (b[i] >= '0' && b[i] <= '9')) i--;
		if (((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')))
		{
			j = i;
			for (; i >= 0; i--) if (!(a[i] == b[i]) && !(((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z')) && ((b[i] >= 'A' && b[i] <= 'Z') || (b[i] >= 'a' && b[i] <= 'z')) && abs(a[i] - b[i]) == 32)) return 0;
			for (i = 0; a[i] && b[i]; i++)
			{
				if (a[i] < b[i]) { strcpy(b, a); break; }
				else if ((a[i] > b[i])) break;
			}
			if (!a[i] && b[i]) strcpy(b, a);
			return 1;
		}
		return 0;
	}
}

void CountWordsFrequency()
{
	;
	int i = 0, j = 0, flag = 4, mark = 1, m = 1;
	char ch, b[200];
	while ((ch = fgetc(fin)) != EOF)
	{
		if (flag>0 && mark == 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{
			b[i] = ch; i++;
			flag--;
			continue;
		}

		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; words++; mark = 1;
			b[i] = '\0'; i = 0;  m = 0;

			for (j = 0; j < k; j++)
			{
				if (JudgeWordEqual(b, word[j].c) == 1) { m = 1; break; }

			}
			if (m) word[j].n++;
			else {
				word[k].n = 1; strcpy(word[k].c, b); k++;
			}
			continue;
		}

		if (flag == 0) { b[i] = ch; i++; continue; }

		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z'))
		{
			flag = 4;  i = 0; memset(b, '\0', sizeof(b));
			if (ch >= '0' && ch <= '9') mark = 0; continue;
		}

		if (flag == 4 && mark == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark = 1;
	}
	if (flag == 0)
	{
		words++;
		b[i] = '\0'; m = 0;
		for (j = 0; j < k; j++)
		{
			if (JudgeWordEqual(b, word[j].c) == 1) { m = 1; break; }
		}
		if (m) word[j].n++;
		else {
			word[k].n = 1; strcpy(word[k].c, b); k++;
		}
	}

	fprintf(fout, "words: %d\n", words);
}

void CountPhrasesFrequency()
{
	int i = 0, j = 0, flag = 4, mark = 1, m = 1, n = 1;
	char ch, b[200], temp[200];
	while ((ch = fgetc(fin)) != EOF)
	{
		if (flag>0 && mark == 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{
			b[i] = ch; i++;
			flag--;
			continue;
		}

		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; words++; mark = 1;
			b[i] = '\0'; i = 0;  m = 0; n = 0;

			for (j = 1; j < k; j++)
			{
				if (JudgeWordEqual(b, word[j].c) == 1)
				{
					m = 1;
					break;
				}
			}
			
			if (m) word[j].n++;
			else {
				word[k].n = 1; strcpy(word[k].c, b); k++;
			}

			if (temp[0])
			{
				for (j = 1; j<l; j++)
				{
					if (JudgePhraseEqual(phrase[j].p, temp, b) == 1)
					{
						n = 1; break;
					}
				}
				if (n) phrase[j].n++;
				else {
					phrase[l].n = 1; strcpy(phrase[l].p.c1, temp); strcpy(phrase[l].p.c2, b); l++;
				}
			}
			strcpy(temp, b);
			continue;
		}

		if (flag == 0) { b[i] = ch; i++; continue; }

		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z'))
		{
			flag = 4;  i = 0; memset(b, '\0', sizeof(b));
			if (ch >= '0' && ch <= '9')
			{
				memset(temp, '\0', sizeof(temp));
				mark = 0; continue;
			}
		}

		if (flag == 4 && mark == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark = 1;
	}
	if (flag == 0)
	{
		words++;
		b[i] = '\0'; m = 0; n = 0;
		
		for (j = 1; j < k; j++)
			{
				if (JudgeWordEqual(b, word[j].c) == 1)
				{
					m = 1;
					break;
				}
			}
			
			if (m) word[j].n++;
			else {
				word[k].n = 1; strcpy(word[k].c, b); k++;
			}
			
		if (temp[0])
		{
			for (j = 1; j<l; j++)
			{
				if (JudgePhraseEqual(phrase[j].p, temp, b) == 1)
				{
					n = 1; break;
				}
			}
			if (n) phrase[j].n++;
			else {
				phrase[l].n = 1; strcpy(phrase[l].p.c1, temp); strcpy(phrase[l].p.c2, b); l++;
			}
		}
	}

	fprintf(fout, "words: %d\n", words);

}

int JudgePhraseEqual(Column p, char a[], char b[])
{
	if (JudgeWordEqual(p.c1, a) == 1 && JudgeWordEqual(p.c2, b) == 1) return 1;
	return 0;
}

void HeapAdjustWord(int *a, int i, int size)  //������ 
{
	int lchild = 2 * i;       //i�����ӽڵ���� 
	int rchild = 2 * i + 1;     //i���Һ��ӽڵ���� 
	int max = i;            //��ʱ���� 
	if (i <= size / 2)          //���i����Ҷ�ڵ�Ͳ��ý��е��� 
	{
		if (lchild <= size && word[a[lchild]].n >word[a[max]].n) max = lchild;
		if (rchild <= size && word[a[rchild]].n >word[a[max]].n) max = rchild;
		if (max != i)
		{
			swap(a[i], a[max]);
			HeapAdjustWord(a, max, size);    //�������֮����maxΪ���ڵ���������Ƕ� 
		}
	}
}

void BuildHeapWord(int *a, int size)    //������ 
{
	int i;
	for (i = size / 2; i >= 1; i--)    //��Ҷ�ڵ�������ֵΪsize/2 
	{
		HeapAdjustWord(a, i, size);
	}
}

void HeapSortWord(int *a, int size)    //������ 
{
	int i, j, t, h, temp, min;
	BuildHeapWord(a, size);
	for (i = size; i >= 1; i--)
	{
		temp = a[1]; a[1] = a[i]; a[i] = temp;         //�����Ѷ������һ��Ԫ�أ���ÿ�ν�ʣ��Ԫ���е�����߷ŵ������ 
													   //BuildHeap(a,i-1);        //������Ԫ�����½���Ϊ�󶥶� 
		HeapAdjustWord(a, 1, i - 1);      //���µ����Ѷ��ڵ��Ϊ�󶥶�
		if (i == size - 9 || i == 1)
		{
			for (j = size; j >= (size>10 ? (size - 9) : 1); j--) fprintf(fout, "<%s> :%d\n", word[a[j]].c, word[a[j]].n);
			fprintf(fout, "\n"); return;
		}

	}

}

void HeapAdjustPhrase(int *a, int i, int size)  //������ 
{
	int lchild = 2 * i;       //i�����ӽڵ���� 
	int rchild = 2 * i + 1;     //i���Һ��ӽڵ���� 
	int max = i;            //��ʱ���� 
	if (i <= size / 2)          //���i����Ҷ�ڵ�Ͳ��ý��е��� 
	{
		if (lchild <= size && phrase[a[lchild]].n >phrase[a[max]].n) max = lchild;
		if (rchild <= size && phrase[a[rchild]].n >phrase[a[max]].n) max = rchild;
		if (max != i)
		{
			swap(a[i], a[max]);
			HeapAdjustPhrase(a, max, size);    //�������֮����maxΪ���ڵ���������Ƕ� 
		}
	}
}

void BuildHeapPhrase(int *a, int size)    //������ 
{
	int i;
	for (i = size / 2; i >= 1; i--)    //��Ҷ�ڵ�������ֵΪsize/2 
	{
		HeapAdjustPhrase(a, i, size);
	}
}

void HeapSortPhrase(int *a, int size)    //������ 
{
	int i, j, temp;
	BuildHeapPhrase(a, size);
	for (i = size; i >= 1; i--)
	{
		temp = a[1]; a[1] = a[i]; a[i] = temp;         //�����Ѷ������һ��Ԫ�أ���ÿ�ν�ʣ��Ԫ���е�����߷ŵ������ 
													   //BuildHeap(a,i-1);        //������Ԫ�����½���Ϊ�󶥶� 
		HeapAdjustPhrase(a, 1, i - 1);      //���µ����Ѷ��ڵ��Ϊ�󶥶�
		if (i == size - 9 || i == 1)
		{
			for (j = size; j >= (size>10 ? (size - 9) : 2); j--) fprintf(fout, "<%s %s> :%d\n", phrase[a[j]].p.c1, phrase[a[j]].p.c2, phrase[a[j]].n);
			return;
		}
	}
}

void statistics()
{
	int i = 0, j = 0, flag = 4, mark = 1, m = 1, n = 1, ad;
	char ch, ch1, b[200], temp[200];
	while ((ch = fgetc(fin)) != EOF)
	{
		if (ch >= 32 && ch <= 126) characters++;
		if (ch == '\n') lines++; ch1=ch;
		if (flag>0 && mark == 1 && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
		{
			b[i] = ch; i++;
			flag--;
			continue;
		}

		if (flag == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9'))
		{
			flag = 4; words++; mark = 1;
			b[i] = '\0'; i = 0;  m = 0; n = 0;

		    if(b[0]>='a' && b[0]<='z') ad= (int)b[0]-96;
			else ad = (int)b[0]-38;
			
			while(word[ad].n!=0 && JudgeWordEqual(b,word[ad].c)==0)
			{
				ad=(ad+52)%NUM;
			}
			if(word[ad].n==0)	strcpy(word[ad].c,b);
			word[ad].n++;
		     
			if (temp[0])
			{
				if(temp[0]>='a' && temp[0]<='z') ad= (int)temp[0]-96;
			    else if(temp[0]>='A' && temp[0]<='Z') ad = (int)temp[0]-38;
			    else ad=0;
			    while(phrase[ad].n!=0 && JudgePhraseEqual(phrase[ad].p, temp, b) == 0)
			    {
				    ad=(ad+52)%NUM;
			    }
			    if(phrase[ad].n==0)
			    {
			    	strcpy(phrase[ad].p.c1, temp); strcpy(phrase[ad].p.c2, b);
				}
			    phrase[ad].n++;
				
			}
			strcpy(temp, b);
			continue;
		}

		if (flag == 0) { b[i] = ch; i++; continue; }

		if (flag>0 && flag<4 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z'))
		{
			flag = 4;  i = 0; memset(b, '\0', sizeof(b));
			if (ch >= '0' && ch <= '9')
			{
				memset(temp, '\0', sizeof(temp));
				mark = 0; continue;
			}
		}

		if (flag == 4 && mark == 0 && !(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z') && !(ch >= '0' && ch <= '9')) mark = 1;
	}
	if(ch1 != '\n') lines++;
	if (flag == 0)
	{
		words++;
		b[i] = '\0'; m = 0; n = 0;
		
		if(b[0]>='a' && b[0]<='z') ad= (int)b[0]-96;
			else ad = (int)b[0]-38;
			while(word[ad].n!=0 && JudgeWordEqual(b,word[ad].c)==0)
			{
				ad=(ad+52)%NUM;
			}
			if(word[ad].n==0)	strcpy(word[ad].c,b);
			word[ad].n++;
			
		if (temp[0])
		{
			if(temp[0]>='a' && temp[0]<='z') ad= (int)temp[0]-96;
			    else if(temp[0]>='A' && temp[0]<='Z') ad = (int)temp[0]-38;
			    else ad=0;
			    while(phrase[ad].n!=0 && JudgePhraseEqual(phrase[ad].p, temp, b) == 0)
			    {
				    ad=(ad+52)%NUM;
			    }
			    if(phrase[ad].n==0)
			    {
			    	strcpy(phrase[ad].p.c1, temp); strcpy(phrase[ad].p.c2, b);
				}
			    phrase[ad].n++;
		}
	}



}



