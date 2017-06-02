#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


struct tel{ //정보를 받을 구조체
	char *name;
	char *tel_no;
	char *birth;
};
// e-mail, age, address

void readfile();
void insert (struct tel **tel, int *count);
void print_all (struct tel **tel, int *count);
void modify(struct tel **tel, int *count);
void delete_tel (struct tel **tel, int *count);
void free_all(struct tel **tel, int *count);



int MAX_NUM=0;



int main()
{
	struct tel **tel_list;
	int count = 0;
	int select=0; 
	
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);

	//최대 입력가능한 정보 갯수
	printf("Max_num:");
	scanf("%d", &MAX_NUM);

	readfile();

	tel_list = (struct tel**)calloc((MAX_NUM+1), sizeof(struct tel*));


	while (select != 5) //5번을 선택할 때까지 반복
	{
		//시작화면 출력
		printf("******************** Menu ***********************\n");
		printf("<1.Register><2.List><3.Delete><4.Modify><5.Exit>\n");
		printf("Select_menu >");
		
		//메뉴선택	
		scanf("%d", &select);
			
		switch (select){
		case 1:
			insert(tel_list, &count);
			break;
		case 2:
			print_all(tel_list, &count);
			break;
		case 3:
			delete_tel(tel_list, &count);
			break;
		case 4:
			modify(tel_list, &count);
			break;
		case 5:
			free_all(tel_list, &count);
			free(tel_list);
			tel_list = NULL;
			break;
		default:
			break;
		}
	}


	return 0;
}


void readfile()
{
	char str[500];
	FILE *fp;
	fp = fopen("phone.txt", "rb");
	
	memset(str, 0, 500);
	fread(str, 500, 1, fp);
	printf("%s\n", str);
	
	fclose(fp);
}



/* 두개의 구조체 포인터를 서로 바꿔주는 함수 */
void swap(struct tel **a, int j)
{
	struct tel *c = a[j];
	a[j] = a[j-1];	
	a[j-1] = c;
}
	


/* 1 : 입력받고 이름순으로 정리하는 함수 */ 
void insert (struct tel **tel, int *count)
{
	int size=0, len=0;
	int i, j;
	char str[64];

	//가득 차있는지 체크
	if (*count == MAX_NUM)
	{
		printf("OVERFLOW\n");
		return;
	}
	
	//입력할 공간 할당
	tel[*count] = (struct tel*)calloc(1, sizeof(struct tel));


	//정보입력 & 동적할당후 저장
	printf("Name : "); 
	memset (str, 0, 64);
	read(0, str, 64);
	size = strlen(str); 
	len = size>40?40:size+1;
	//tel[*count]->name = (char*)calloc(len, sizeof(char));
	tel[*count]->name = (char*)malloc(sizeof(char)*len);
	//
	//memset(tel[*count]->name, 0, 16);
	//
	memcpy(tel[*count]->name, str, len+1);

	printf("Phone_number : ");
	memset (str, 0, 64);
	read(0, str, 64);
	size = strlen(str); 
	len = size>40?40:size+1;
	//tel[*count]->tel_no = (char*)calloc(len, sizeof(char));
	tel[*count]->tel_no = (char*)malloc(sizeof(char)*len);
	//memset(tel[*count]->name, 0, 16);
	memcpy(tel[*count]->tel_no, str, len+1);

	printf("Birth : "); 
	memset (str, 0, 64);
	read(0, str, 64);
	size = strlen(str); 
	len = size>40?40:size+1;
	//tel[*count]->birth = (char*)calloc(len, sizeof(char));
	tel[*count]->birth = (char*)malloc(sizeof(char)*len);
	//memset(tel[*count]->name, 0, 16);
	memcpy(tel[*count]->birth, str, len+1);
	

	//이름순으로 정리
	for (i=0 ; i<*count ; i++)
		if (strcmp(tel[i]->name, tel[*count]->name) > 0)
		{
			for (j=(*count) ; j>i ; j--)
				swap(tel, j);
			break;
		}
	
	//저장된 연락저 갯수 출력
	(*count)++;
	printf("\n");

	return ;
}



/* 2 : 모든 연락처를 순서대로 출력하는 함수 */
void print_all (struct tel **tel, int *count)
{
	int i;
	for (i=0 ; i<*count ; i++){
		printf("<<%d>>\n", i+1);
		printf("Name : %s", tel[i]->name);
		printf("Phone_number : %s", tel[i]->tel_no);
		printf("Birth : %s\n", tel[i]->birth);
	}
}


/* 3 : 이름을 통해 연락처를 삭제하고 정리하는 함수 */
void delete_tel (struct tel **tel, int *count)
{
	int i;

	if (*count == 0)
	{
		printf("NO MEMBER\n");
		return;
	}
	
	printf("Index : ");
	scanf("%d", &i);

	if (i<1 || i>*count)
	{
		printf("Wrong...\n");
		return;
	}
	i--;

	//연락처 삭제 (뒤의 정보를 앞으로 옮긴다)
	for (; i<*count-1 ; i++)
	{
		memcpy(tel[i]->name, tel[i+1]->name, strlen(tel[i+1]->name));
		memcpy(tel[i]->tel_no, tel[i+1]->tel_no, strlen(tel[i+1]->tel_no));
		memcpy(tel[i]->birth, tel[i+1]->birth, strlen(tel[i+1]->birth));
	}

	//마지막 구조체의 메모리 해제
	free(tel[*count-1]->birth);
	free(tel[*count-1]->tel_no);
	free(tel[*count-1]->name);
	free(tel[*count-1]);
	(*count)--;
}

int checksec(struct tel **tel, char *str)
{
	if (((int)tel>>24)!=((int)str>>24))
		return 1;
	return 0;
}

void modify_name(struct tel **tel, int i)
{
	char str[64];
	int len;

	if (checksec(tel, tel[i]->name))
	{
		printf("Wrong...\n");
		return;
	}
	printf("Name : ");
	memset(str, 0, 40);
	len = strlen(tel[i]->name)>40?40:strlen(tel[i]->name)+1;
	read(0, str, len);
	memcpy(tel[i]->name, str, strlen(str)+1);
}


void modify_tel(struct tel **tel, int i)
{
	char str[64];
	int len;

	if (checksec(tel, tel[i]->tel_no))
	{
		printf("Wrong...\n");
		return;
	}
	printf("Phone_number : ");
	memset(str, 0, 40);
	len = strlen(tel[i]->tel_no)>40?40:strlen(tel[i]->tel_no)+1;
	read(0, str, len);
	memcpy(tel[i]->tel_no, str, strlen(str)+1);
}

void modify_birth(struct tel **tel, int i)
{
	char str[64];
	int len;

	if (checksec(tel, tel[i]->birth))
	{
		printf("Wrong...\n");
		return;
	}
	printf("Birth : ");
	memset(str, 0, 40);
	len = strlen(tel[i]->birth)>40?40:strlen(tel[i]->birth)+1;
	read(0, str, len);
	memcpy(tel[i]->birth, str, strlen(str)+1);
}


void modify(struct tel **tel, int *count)
{
	int len, i;
	int select;

	if (*count == 0)
	{
		printf("NO MEMBER\n");
		return ;
	}
	
	printf("Index : ");
	scanf("%d", &i);
	
	i--;
	if (!(i>=0 && i<=*count)){
		printf("Wrong...\n");
		return;
	}

	printf("Which one do you want to modify?\n");
	printf("1.Name\n");
	printf("2.Phone_number\n");
	printf("3.Birth\n");
	printf(" >");
	scanf("%d", &select);

	switch(select){
	case 1:
		modify_name(tel,i);
		break;
	case 2:
		modify_tel(tel,i);
		break;
	case 3:
		modify_birth(tel,i);
		break;
	default:
		printf("Wrong...\n");
		break;
	}
}



/* 5 : 모든 메모리를 해제시킨다. */
void free_all(struct tel **tel, int *count)
{
	int i;

	for (i=*count-1 ; i>=0 ; i--)
	{
		free(tel[i]->birth);
		free(tel[i]->tel_no);
		free(tel[i]->name);
		free(tel[i]);
		tel[i] = NULL;
	}
}

