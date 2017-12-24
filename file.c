/***********************************************************************
*file: file.c
*synopsis: The argz functions use malloc/realloc to allocate/grow argz vectors, *and so any argz vector creating using these functions may be freed by using *free; conversely, any argz function that may grow a string expects that string *to have been allocated *using malloc those argz functions that only examine *their arguments or modify them in place *will work on any sort of memory). All *argz functions that do memory allocation have a return type of error_t, and 
*return 0 for success, and ENOMEM if an allocation error occurs.
* These functions are declared in the include file "file.h".
*related files: none
*author: Hnatenko Vitaliy
*written: 19/10/2017
*last modified: 01/11/2017
************************************************************************/ 
#include "file.h"
#include <stdlib.h>

#define bool  int
#define false 0
#define true  1

void argz_print(const char *argz, size_t argz_len)
{
	size_t i=0;

	for (i; i <argz_len; i++)
		{
			if (*(argz + i)=='\0')
				printf("\\0\n");
			else
				printf("%c", *(argz + i));
		}
		printf("\n");
		printf("argz_count(argz, argz_len): %d\n\n", argz_count(argz, argz_len));
		printf("argz_length=%d\n\n", argz_len);
}

error_t argz_create_sep(const char *string, int sep, char **argz, size_t *argz_len)
{
	int i=0, length=strlen(string);

	if (length==0)
	{
		if ((sep>255) || (sep<0))
		{
			return BAD;
		}
	}

	if ((*argz=(char*)malloc(sizeof(char)*(strlen(string) + 1)))==NULL)
		exit(1);

	for (i; i<length; i++)
	{
		if (*(string+i)==sep)
		{
			*(*argz+i)='\0';
		}
		else
		{
			*(*argz+i)=*(string+i);
		}
	}
	
	*(*argz+strlen(string))='\0';
	*argz_len=strlen(string)+1;

	return OK;
}

size_t  argz_count(const char *argz, size_t argz_len)
{
	int i=0, length=argz_len, number=0;

	for (i; i<length; i++)
	{
		if(*(argz+i)=='\0')
		{
			number++;
		}
	}
	return number;
}

error_t argz_add(char **argz, size_t *argz_len, const char *str)
{
	int i=0, length=strlen(str);
	if (realloc(*argz, (*argz_len+strlen(str)+1)*sizeof(char))==NULL)
	{
		exit(1);
	}

	for (i; i<length+1; i++)
	{
		*(*argz+*argz_len+i)=*(str+i);
	}

	*argz_len+=length+1;

	return OK;
}

void argz_delete(char **argz, size_t *argz_len, char *entry)
{
	char field, *tmp=*argz, *new_argz;
	int fields_number, new_length, i=0, setoff=0;

	fields_number=argz_count(*argz,*argz_len);
	new_length=*argz_len-strlen(entry)-1;
	new_argz = (char*)malloc(new_length*sizeof(char));
	for (i; i<fields_number; i++)
	{
		if (strcmp(tmp,entry))
		{
			strcpy(new_argz+setoff,tmp);
			setoff+=strlen(tmp)+1;
		}
		tmp+=strlen(tmp)+1;
	}
	free(*argz);
	*argz=new_argz;
	*argz_len=new_length;
}

error_t argz_insert(char **argz, size_t *argz_len, char *before, const char *entry) 
{
	int i=0,length;
	size_t  k=0;
	char *new_argz, *ptr;

	ptr = (*argz);
	new_argz = (char*)malloc(((*argz_len) + strlen(entry))*sizeof(char));

	if (new_argz == NULL || before == NULL || entry == NULL)
	{
		return BAD;
	}

	length = argz_count((*argz), *argz_len);

	while (i < length) 
	{
		argz_add(&new_argz, &k, ptr);
		if (strcmp(ptr, before) == 0) 
		{
			argz_add(&new_argz, &k, entry);
		}
		ptr+=strlen(ptr)+1;
		i++;
	}

	(*argz_len) = k;
	(*argz) = new_argz;
	return OK;
}

char * argz_next(char *argz, size_t argz_len, const char *entry) 
{
	int i;
	if (NULL == argz) 
	{
			return NULL;
	}
	if (NULL == entry)
	{
		return argz;
	}
	
	for(i=0;i<argz_len;i+=strlen((argz+i))+1)
	{
		if (strcmp((argz+i),entry)==0)
		{
				break;
		}
	}

	i+=strlen(argz+i)+1;

	if (i>=argz_len) 
		return NULL;
	else
		return (argz+i);
}

error_t argz_replace(char **argz, size_t *argz_len,const char *str, const char *with) 
{
	int i, j;
	char *paste_argz, *copy_argz=*argz;
	
	i=strlen(str)+1;
	j=strlen(with)+1;
	
	if (argz==NULL || with==NULL || str==NULL)
	{
		return BAD;
	}
	for (copy_argz; copy_argz; copy_argz=argz_next(*argz, *argz_len, copy_argz))
	{
		if (strcmp(str,copy_argz)==0)
			{
				break;
			}
	}
	if (i==j)
	{
		strcpy(copy_argz,with);
		return OK;
	}
	paste_argz=(char*)malloc((*argz_len+(j-i))*sizeof(char));

	if (paste_argz==NULL)
	{
		exit(BAD);
	}
	if (copy_argz==NULL)
	{
		printf("\nElement, which must be replaced - not found\n\n");
		return BAD;
	}
		
	memcpy(paste_argz,*argz,copy_argz-*argz);
	memcpy(paste_argz+(copy_argz-*argz),with,j);
	memcpy(paste_argz+(copy_argz-*argz)+j,argz_next (*argz, *argz_len, copy_argz),*argz_len-(argz_next (*argz, *argz_len, copy_argz)-*argz));
	free(*argz);
	*argz = paste_argz;
	*argz_len-=(i-j);
		return OK;
}
