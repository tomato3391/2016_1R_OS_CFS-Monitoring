#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define faster 328
#define slower 329
typedef struct node
{
	int pid;
	char pname[100];
	bool fast;
	bool slow;
	struct node* next;
} NODE;

void main(void)
{
	clock_t init;
	clock_t end;
	FILE* fp;
	char dir[100];
	NODE* new = NULL;
	NODE* temp = NULL;
	NODE* start = NULL;
	while(1)
	{
		init = clock();
		for (int i = 1; i < 32768; i++)
		{
			sprintf(dir, "/proc/%d/stat", i);
			fp = fopen(dir, "r");
			if (fp == NULL)
				continue;
			int pid;
			char name[100];
			fscanf(fp, "%d", &pid);
			fscanf(fp, " (%s ", name);
			name[strlen(name) - 1] = '\0';
			if (start != NULL)
			{
				for (temp = start; ; temp = temp -> next)
				{
					if (!strcmp(temp -> pname, name))
					{
						if (temp -> fast && temp -> pid != pid)
						{
							syscall(faster, i);
							temp -> pid = pid;
							printf("fast identified\n");
						}
						else if (temp -> slow && temp -> pid != pid)
						{
							syscall(slower, i);
							temp -> pid = pid;
							printf("slow identified\n");
						}
						break;
					}
					if (temp -> next == NULL)
						break;
				}
				if (!strcmp(temp -> pname, name))
					continue;
			}
			if (!strcmp(name, "benchmark1") || !strcmp(name, "benchmark2"))
			{
				if (start == NULL)
				{
					new = (NODE*)malloc(sizeof(NODE));
					start = new;
				}
				else
				{
					temp = new;
					new = (NODE*)malloc(sizeof(NODE));
				}
				new -> fast = false;
				new -> slow = false;
				new -> next = NULL;
				new -> pid = pid;
				strcpy(new -> pname, name);
				printf("Process insertion - PID : %d\n", new -> pid);
				printf("Process insertion - PNAME : %s\n", new -> pname);
				if (!strcmp(new -> pname, "benchmark1"))
					new -> slow = true;	
				else
					new -> fast = true;
				if (temp != NULL)
					temp -> next = new;
			}
			fclose(fp);
		}
		end = clock();
		usleep(500000 - end + init);
	}
}
