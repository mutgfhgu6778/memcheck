#ifndef CS100_MEMCHECK_H
#define CS100_MEMCHECK_H 1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RECORDS 1000

struct Record {
  void *ptr;              // address of the allocated memory
  size_t size;            // size of the allocated memory
  int line_no;            // line number, at which a call to malloc or calloc happens
  const char *file_name;  // name of the file, in which the call to malloc or calloc happens
};

struct Record records[MAX_RECORDS];

void *recorded_malloc(size_t size, int line, const char *file) {
	printf("Summary:\n");
  void *ptr = malloc(size);
  if (ptr != NULL) {
    // YOUR CODE HERE
	  int i;
	  for(i=0;i<MAX_RECORDS;i++)
	  {
		  if(records[i].ptr == NULL)
		  { 
			  records[i].ptr = ptr;
			  records[i].size = size;
			  records[i].line_no = line;
			  records[i].file_name = file;
			  break;
		  }
	  }
	  if(i >= MAX_RECORDS)
	  {
		  free(ptr);
		  printf("%d malloc failed in file %s ,line %d\n",(int)size,file,line);
		  return  NULL;
	  }
  }
  else
  {
	  printf("%d not malloc in file %s ,line %d\n",(int)size,file,line);
  }
  return ptr;
}

void *recorded_calloc(size_t cnt, size_t each_size, int line, const char *file) {
	printf("Summary:\n");
  void *ptr = calloc(cnt, each_size);
  if (ptr != NULL) {
    // YOUR CODE HERE
	  int i;
	  for(i=0;i<MAX_RECORDS;i++)
	  {
		  if(records[i].ptr == NULL)
		  { 
			  records[i].ptr = ptr;
			  records[i].size = each_size;
			  records[i].line_no = line;
			  records[i].file_name = file;
			  break;
		  }
	  }
	  if(i >= MAX_RECORDS)
	  {
		  free(ptr);
		  printf("%d malloc failed in file %s ,line %d\n",(int)each_size,file,line);
		  return  NULL;
	  }
  }
  else
  {
	  printf("%d not malloc in file %s ,line %d\n",(int)each_size,file,line);
  }
  return ptr;
}

void recorded_free(void *ptr, int line, const char *file) {
	printf("Summary:\n");
  if (ptr != NULL) {
	  int i;
	  for(i=0;i<MAX_RECORDS;i++)
	  {
		  if(records[i].ptr == ptr)
		  {
			  free(ptr);
			  records[i].ptr = NULL;
			  records[i].size = 0;
			  records[i].line_no = 0;
			  records[i].file_name = NULL;
			  break;
		  }
	  }
	  if(i >= MAX_RECORDS)
	  {
		  printf(" bytes memory not freed (allocated in file %s, line %d)\n",file,line);
	  }
 } 
}

void check_leak(void) __attribute__((destructor));

void check_leak(void) {
  // YOUR CODE HERE
	int  count = 0;
	int  totalsize = 0;
	printf("Summary:\n");
	for(int i=0;i<MAX_RECORDS;i++)
	{
		if(records[i].ptr != NULL)
		{
			count ++;
			totalsize = totalsize+records[i].size;

		}
	}
	if(count > 0)
	{
		printf("%d allocation records not freed (%d bytes in total).\n",count,totalsize);
	}

}

#define malloc(SIZE) recorded_malloc(SIZE, __LINE__, __FILE__)
#define calloc(CNT, EACH_SIZE) recorded_calloc(CNT, EACH_SIZE, __LINE__, __FILE__)
#define free(PTR) recorded_free(PTR, __LINE__, __FILE__)

#endif // CS100_MEMCHECK_H
