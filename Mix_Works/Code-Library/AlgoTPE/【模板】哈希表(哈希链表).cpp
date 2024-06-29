#define MAXSIZE 20000
struct HashData {
	int key;
	char filePathName[2000];
	char fileInfo[1000];
	struct HashData* next;
};

typedef struct {
	int hashWid;
	struct HashData** data;
}HashTbl;

void HashInit(HashTbl *h, int wid)
{
	struct HashData** tmp = (struct HashData**)malloc(sizeof(struct HashData*) * wid);
	memset(tmp, 0, sizeof(struct HashData*) * wid);
	h->hashWid = wid;
	h->data = tmp;
}

int HashFun(char *s, int wid)
{
	int seed = 131313131;
	long long hashNum = 0;
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		hashNum = hashNum * seed + s[i];
		hashNum = hashNum % wid;
	}

	return (int)hashNum & 0x7ffffffff;
}

void HashInsert(HashTbl *h, char *pathname, char *info)
{
	int key = HashFun(info, h->hashWid);
	struct HashData* tmp = (struct HashData*)malloc(sizeof(struct HashData));
	tmp->key = key;
	info[strlen(info) - 1] = '\0';
	memcpy(tmp->filePathName, pathname, (strlen(pathname) + 1));
	memcpy(tmp->fileInfo, info, (strlen(info) + 1));

	if (h->data[key] == NULL) {
		h->data[key] = tmp;
		tmp->next = NULL;
	} else {
		struct HashData* p = h->data[key];
		if (strcmp(p->fileInfo, info) == 0) {
			h->data[key] = tmp;
			tmp->next = p;
		} else {
			while (h->data[key] != NULL) {
				if (strcmp(h->data[key]->fileInfo, info) == 0) {
					break;
				}
				key++;
				if (key >= h->hashWid) {
					key = key % h->hashWid;
				}
			}
			p = h->data[key];
			h->data[key] = tmp;
			tmp->next = p;
		}
	}
}

void SplitStr(char *s, char str[1000][1000], int *len)
{
	int strNum = *len;
	while (*s != '\0') {
		int num = 0;
		while (*s != ' ' && *s != '\0') {
			str[strNum][num] = *s;
			num++;
			s++;
		}

		if (strNum == *len) {
			str[strNum][num] = '/';
			num++;
		}
		str[strNum][num] = '\0';
		num++;
		strNum++;
		if (*s == '\0') {
			break;
		}
		s++;
	}
	*len = strNum;
}

void findNameInfo(char *s, char *name, char *info)
{
	int num = 0;
	while (*s != '(') {
		name[num] = *s;
		num++;
		s++;
	}

	name[num] = '\0';
	num = 0;
	while (*s != '\0') {
		info[num] = *s;
		num++;
		s++;
	}
	info[num] = '\0';
}

void GetPathName(char *path, char *name, char *pathname)
{
	int num = 0;
	while (*path != '\0') {
		pathname[num] = *path;
		num++;
		path++;
	}

	while (*name != '\0') {
		pathname[num] = *name;
		num++;
		name++;
	}
	pathname[num] = '\0';
}

char *** findDuplicate(char ** paths, int pathsSize, int* returnSize, int** returnColumnSizes)
{
	if (pathsSize == 0 || paths == NULL) {
		*returnSize = 0;
		*returnColumnSizes = NULL;
		return NULL;
	}

	HashTbl h;
	HashInit(&h, MAXSIZE);

	for (int i = 0; i < pathsSize; i++) {
		char *p = paths[i];
		int n = 0;
		char strArr[1000][1000] = { {'\0'} };
		SplitStr(p, strArr, &n);
		for (int j = 1; j < n; j++) {
			char name[1000] = { '\0' };
			char info[1000] = { '\0' };
			char pathname[2000] = { '\0' };
			findNameInfo(strArr[j], &(name[0]), &(info[0]));
			GetPathName(strArr[0], &(name[0]), &(pathname[0]));
			HashInsert(&h, pathname, &(info[1]));
		}
	}

	char ***ret = (char ***)malloc(sizeof(char**) * MAXSIZE);
	int retNum = 0;
	int *retCol = (int *)malloc(sizeof(int) * MAXSIZE);

	for (int i = 0; i < h.hashWid; i++) {
		struct HashData* p = h.data[i];
		if (p == NULL || p->next == NULL) {
			continue;
		}

		char **ret1 = (char **)malloc(sizeof(char*) * MAXSIZE);
		int ret1Num = 0;
		memset(ret1, 0, sizeof(char*) * MAXSIZE);

		while (p != NULL) {
			char *ret2 = (char *)malloc(sizeof(char) * 1000);
			memset(ret2, 0, sizeof(char) * 1000);
			memcpy(ret2, p->filePathName, (strlen(p->filePathName) + 1));
			ret1[ret1Num] = ret2;
			ret1Num++;
			p = p->next;
		}
		retCol[retNum] = ret1Num;
		ret[retNum] = ret1;
		retNum++;
	}

	if (retNum == 0) {
		*returnSize = 0;
		*returnColumnSizes = NULL;
		return NULL;
	}

	*returnSize = retNum;
	*returnColumnSizes = retCol;
	return ret;
}
609. 在系统中查找重复文件