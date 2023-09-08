#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define FILE_DIR "config.cfg"

typedef struct tag_rules
{
    uint32_t left_board;
    uint32_t right_board;
    int tag;
} tag_rules;

int tag_rules_init(tag_rules** tag_rules_obj, int size)
{
	*tag_rules_obj = malloc(size * sizeof * *tag_rules_obj);
	if(*tag_rules_obj == NULL)
	{
		printf("Error");
		return 1;
	}
	
	return 0;
}

void tag_rules_clear(tag_rules** tag_rules_obj)
{
	free(*tag_rules_obj);
}

int config_file_check(void)
{
	FILE* pFile;
	if((pFile = fopen(FILE_DIR, "r")) == NULL)
	{
		printf("Error\n");
		return 1;
	}

	if(fclose(pFile) > 0)
	{
		printf("Error\n");
		return 2;
	}
	
	return 0;
}

int ip_converting(uint32_t* ip_uint, char* ip_str)
{
	struct in_addr in_addr_obj;

	if(inet_aton(ip_str, &in_addr_obj) == 0)
	{
		printf("Error\n");
		return 1;
	}

	*ip_uint = in_addr_obj.s_addr;
	return 0;
}

int tag_converting(int* tag_int, char* tag_str)
{
	int tag_int_tmp;
	tag_int_tmp = atoi(tag_str);
	if(tag_int_tmp == 0 && tag_str[0] != '0')
	{
		printf("Error\n");
		return 1;
	}

	*tag_int = tag_int_tmp;
	return 0;
}

int config_file_read(tag_rules* tag_rules_obj)
{
	FILE* pfile;
	if((pfile = fopen(FILE_DIR, "r")) == NULL)
	{
		printf("Error\n");
		return 1;
	}
	
	for(int i = 0;; i++)
	{
		char rule[37];
		char* prule;
		char* rule_part;
		char* pfull_stop;
		
		prule = fgets(rule, sizeof(rule), pfile);
		if(prule == NULL)
		{
			if(feof(pfile) != 0)
			{
				break;
			}
			else
			{
				printf("Error\n");
				return 2;
			}
		}
		
		rule_part = strtok(rule, "-");
		if(rule_part == NULL)
		{
			printf("Error\n");
			return 3;
		}
		
		if(ip_converting(&tag_rules_obj[i].left_board, rule_part) > 0)
		{
			return 4;
		}
		
		rule_part = strtok(rule, "-");
		if(rule_part == NULL)
		{
			printf("Error\n");
			return 3;
		}
		
		pfull_stop = strchr(rule_part, '.');
		if(pfull_stop == NULL)
		{
			tag_rules_obj[i].right_board = tag_rules_obj[i].left_board;
			if(tag_converting(&tag_rules_obj[i].tag, rule_part) > 0)
			{
				return 5;
			}
		}
		else
		{
			if(ip_converting(&tag_rules_obj[i].right_board, rule_part) > 0)
			{
				return 4;
			}
			
			rule_part = strtok(rule, "-");
			if(rule_part == NULL)
			{
				printf("Error\n");
				return 3;
			}
			
			tag_rules_obj[i].right_board = tag_rules_obj[i].left_board;
			if(tag_converting(&tag_rules_obj[i].tag, rule_part) > 0)
			{
				return 5;
			}
		}
	}

	if(fclose(pfile) > 0)
	{
		printf("Error\n");
		return 5;
	}
	
	return 0;
}
