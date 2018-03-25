#include<stdio.h>
#include<string.h> 
#include<math.h>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<io.h>
#include<vector>


using namespace std;

int characters = 0, words = 0, lines = 0, k=1, l=1;  // k表示word频率  l表示phrase频率 
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

void getFiles(string folderpath, vector<string>&filespath); //文件句柄/文件信息  
int JudgeOrder(char a[], char b[]);                              //判断字典顺序 
void DictOrder(void);                          //字典排序 
void Count_Characters_Lines(void);             //统计字符数和行数 
void CountWords(void);                         //统计单词总数 
int  JudgeWordEqual(char a[], char b[]);      //判断两个单词是否等价 
void CountWordsFrequency(void);                //统计各单词频率及排序 

void HeapAdjustWord(int *a, int i, int size);    //调整堆 
void BuildHeapWord(int *a, int size);           //建堆 
void HeapSortWord(int *a, int size);            //针对单词频率统计的堆排序 

void HeapAdjustPhrase(int *a, int i, int size);
void BuildHeapPhrase(int *a, int size);
void HeapSortPhrase(int *a, int size);          //针对词组频率统计的堆排序 

void CountPhrasesFrequency(void);              //统计各词组频率及排序 
int JudgePhraseEqual(Column p, char a[], char b[]);          //判断后两个单词组成的词组是否与第一个词组等价 
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

//子函数 

void getFiles(string folderpath, vector<string>& filespath)
{
	long   hFile = 0;
	struct _finddata_t *fileinfo = new _finddata_t;
	string pathPrefix;

	if ((hFile = _findfirst(pathPrefix.assign(folderpath).append("\\*").c_str(), fileinfo)) != -1) {
		do {
			if ((fileinfo->attrib &  _A_SUBDIR)) { //如果是子目录,递归查找子文件夹
				if (strcmp(fileinfo->name, ".") != 0 && strcmp(fileinfo->name, "..") != 0)
					getFiles(pathPrefix.assign(folderpath).append("\\").append(fileinfo->name), filespath);
			}
			else { //如果是文件，则加入查找结果
				filespath.push_back(pathPrefix.assign(folderpath).append("\\").append(fileinfo->name));
			}
		} while (_findnext(hFile, fileinfo) == 0); //通过句柄关闭当前目录查找       
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

void HeapAdjustWord(int *a, int i, int size)  //调整堆 
{
	int lchild = 2 * i;       //i的左孩子节点序号 
	int rchild = 2 * i + 1;     //i的右孩子节点序号 
	int max = i;            //临时变量 
	if (i <= size / 2)          //如果i不是叶节点就不用进行调整 
	{
		if (lchild <= size && word[a[lchild]].n >word[a[max]].n) max = lchild;
		if (rchild <= size && word[a[rchild]].n >word[a[max]].n) max = rchild;
		if (max != i)
		{
			swap(a[i], a[max]);
			HeapAdjustWord(a, max, size);    //避免调整之后以max为父节点的子树不是堆 
		}
	}
}

void BuildHeapWord(int *a, int size)    //建立堆 
{
	int i;
	for (i = size / 2; i >= 1; i--)    //非叶节点最大序号值为size/2 
	{
		HeapAdjustWord(a, i, size);
	}
}

void HeapSortWord(int *a, int size)    //堆排序 
{
	int i, j, t, h, temp, min;
	BuildHeapWord(a, size);
	for (i = size; i >= 1; i--)
	{
		temp = a[1]; a[1] = a[i]; a[i] = temp;         //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
													   //BuildHeap(a,i-1);        //将余下元素重新建立为大顶堆 
		HeapAdjustWord(a, 1, i - 1);      //重新调整堆顶节点成为大顶堆
		if (i == size - 9 || i == 1)
		{
			for (j = size; j >= (size>10 ? (size - 9) : 1); j--) fprintf(fout, "<%s> :%d\n", word[a[j]].c, word[a[j]].n);
			fprintf(fout, "\n"); return;
		}

	}

}

void HeapAdjustPhrase(int *a, int i, int size)  //调整堆 
{
	int lchild = 2 * i;       //i的左孩子节点序号 
	int rchild = 2 * i + 1;     //i的右孩子节点序号 
	int max = i;            //临时变量 
	if (i <= size / 2)          //如果i不是叶节点就不用进行调整 
	{
		if (lchild <= size && phrase[a[lchild]].n >phrase[a[max]].n) max = lchild;
		if (rchild <= size && phrase[a[rchild]].n >phrase[a[max]].n) max = rchild;
		if (max != i)
		{
			swap(a[i], a[max]);
			HeapAdjustPhrase(a, max, size);    //避免调整之后以max为父节点的子树不是堆 
		}
	}
}

void BuildHeapPhrase(int *a, int size)    //建立堆 
{
	int i;
	for (i = size / 2; i >= 1; i--)    //非叶节点最大序号值为size/2 
	{
		HeapAdjustPhrase(a, i, size);
	}
}

void HeapSortPhrase(int *a, int size)    //堆排序 
{
	int i, j, temp;
	BuildHeapPhrase(a, size);
	for (i = size; i >= 1; i--)
	{
		temp = a[1]; a[1] = a[i]; a[i] = temp;         //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
													   //BuildHeap(a,i-1);        //将余下元素重新建立为大顶堆 
		HeapAdjustPhrase(a, 1, i - 1);      //重新调整堆顶节点成为大顶堆
		if (i == size - 9 || i == 1)
		{
			for (j = size; j >= (size>10 ? (size - 9) : 2); j--) fprintf(fout, "<%s %s> :%d\n", phrase[a[j]].p.c1, phrase[a[j]].p.c2, phrase[a[j]].n);
			return;
		}
	}
}

unsigned int Address(unsigned long long a) //平方取中得哈希地址 
{	
	int m;
    unsigned long long key;
    key=a*a; key=key<<20;
    key=key>>40;
    m=key;
    return m;
}

unsigned int H(unsigned long long x) //开放定址处理冲突 
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



