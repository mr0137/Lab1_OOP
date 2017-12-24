/************************************************************************
*file: mainf.c
*purpose: only for tests
*author: Hnatenko Vitaliy 
*written: 19/10/2017
*last modified: 19/10/2017
*************************************************************************/
#include "file.h"

int main()
{
	char *argz, *entry=NULL;
	size_t argz_len;
	int i=0;

	if (argz_create_sep("Hello;my;little", ';', &argz, &argz_len)==OK)
	{
			printf("---- argz_create_sep('Hello;my;little', ';', &argz, &argz_len):\n");
	}

	argz_print(argz, argz_len);

	printf("---- argz_add(&argz, &argz_len, 'honey'):\n");
	argz_add(&argz, &argz_len, "honey");

	argz_print(argz, argz_len);

	printf("---- argz_delete(&argz, &argz_len, argz+9):\n");
	argz_delete(&argz, &argz_len, argz+9);

	argz_print(argz, argz_len);

	printf("---- argz_insert(&argz, &argz_len, argz + 6, 'beautiful'):\n");
	argz_insert(&argz, &argz_len, argz+6, "beautiful");

	argz_print(argz, argz_len);

	printf("---- argz_next: \n");
	entry="beautiful";
	entry = argz_next(argz, argz_len, entry);
	
	printf("%s\n", entry);
	printf("\n");

	printf("---- argz_replace(&argz, &argz_len, 'my', 'my'):\n");
	argz_replace(&argz, &argz_len, "beautiful", "wonderfull");

	argz_print(argz, argz_len);

	system("pause");
}