#include<stdio.h>
#include<string.h> 
#include<math.h>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<io.h>
#include<vector>


using namespace std;

int characters = 0, words = 0, lines = 0, k=1, l=1;  // k��ʾwordƵ��  l��ʾphraseƵ�� 
FILE *fin, *fout;

struct Word 
{ 
    char c[200]; 
    int n; 
}word[16777216]; 

struct Column
{
	char c1[200];
	char c2[200];
};

struct Phrase 
{ 
    Column p;
    int n; 
}phrase[16777216];

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

	char folderpath[100] = "C:/Users/pc/Desktop/tell", q[200], *p;
	int a[10000000], j;
	vector<string> filepath;
	//gets(folderpath);
	getFiles(folderpath, filepath);
	int fpsize = filepath.size();
	fout = fopen("C:/Users/pc/Desktop/result.txt", "w");
	for (int i = 0; i < fpsize; i++)
	{
		strcpy(q, filepath[i].c_str()); p = q;
		while (*p != '\0')
		{
			if (*p == '\\') *p = '\/'; p++;
		}

		if ((fin = fopen(q, "r")) == 0) return 0;

		//CountWordsFrequency();
		//CountPhrasesFrequency();
		statistics();

		fclose(fin);

	}

	fprintf(fout, "characters: %d\nlines: %d\n", characters, lines);
	fprintf(fout, "words: %d\n\n", words);
	for (j = 1; j <= k; j++) a[j] = j; HeapSortWord(a, k - 1);
	for (j = 1; j <= l; j++) a[j] = j; HeapSortPhrase(a, l - 1);



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
	if (a[i] == NULL && b[i] != NULL) return -1;
	else if (a[i] != NULL && b[i] == NULL) return 1;
	else return 0;
}

void Count_Characters_Lines()
{
	char ch;
	while ((ch = fgetc(fin)) != EOF)
	{
		if (ch >= 0 && ch <= 127) characters++;
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
			if (a[i] == NULL && b[i] != NULL) strcpy(b, a);
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
			if (a[i] == NULL && b[i] != NULL) strcpy(b, a);
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
			if (a[i] == NULL && b[i] != NULL) strcpy(b, a);
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

unsigned int Address(unsigned long long a) //ƽ��ȡ�еù�ϣ��ַ 
{	
	int m;
    unsigned long long key;
    key=a*a; key=key<<20;
    key=key>>40;
    m=key;
    return m;
}

unsigned int H(unsigned long long x) //���Ŷ�ַ�����ͻ 
{
	if(x%2==1) return(x%16777216);
	else return((x+1)%16777216);
}

void statistics()
{
	int i = 0, j = 0, flag = 4, mark = 1, m = 1, n = 1;
	char ch, b[200], temp[200];
	while ((ch = fgetc(fin)) != EOF)
	{
		if (ch >= 0 && ch <= 127) characters++;
		if (ch == '\n') lines++;
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

		/*	for (j = 1; j < k; j++)
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
        */
		     if(b[0]>='a' && b[0]<='z')  b[0]-=32;
		     
		     
		     
		     
		     
		     
		     
		     
		     
		     
		     
		     
		     
		     
		     
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



}



