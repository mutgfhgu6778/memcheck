# 留学生日常作业/课程设计/代码辅导/CS/DS/商科，仅为漂洋过海的你❥
标签：Computer Science、Data Science、Business Administration，留学生编程作业代写&&辅导

## 个人介绍:
本人是一名资深码农，酷爱投资。

[为您提供 CS , DS , 商科 编程作业代写](http://dzuoye.work "编程代写")

<img src="design2023866.jpg"  width="200" />

Obtaining additional information
It is of great help if you can record not only the address of the allocated memory, but also some
other information, like the name of the source file and the line number. For example, you may
want to see an error message like
or
To achieve this, our array element should be a struct :

See this webpage for standard definitions of __LINE__ and __FILE__ .
Now here is how the magic happens: we use #define to replace the calls to malloc , calloc and
free , so that these operations can be captured by us:
100 bytes memory not freed (allocated in file hw3_problem1.c, line 32)
Invalid free in file hw3_problem1.c, line 49

#define MAX_RECORDS 1000
struct Record {
void *ptr; // address of the allocated memory
size_t size; // size of the allocated memory
int line_no; // line number, at which a call to malloc or calloc happens
const char *file_name; // name of the file, in which the call to malloc or calloc happens
};
struct Record records[MAX_RECORDS];


How do we obtain the file name and the line number? The C standard provides the macros
__LINE__ and __FILE__ . You can try on your own (in line_and_file.c ) to see what they represent:

```
#include <stdio.h>
int main(void) {
printf("%s\n", __FILE__);
printf("%d\n", __LINE__);
printf("%d\n", __LINE__);
printf("%d\n", __LINE__);
printf("%d\n", __LINE__);
return 0;
}

```
See this webpage for standard definitions of __LINE__ and __FILE__ .
Now here is how the magic happens: we use #define to replace the calls to malloc , calloc and
free , so that these operations can be captured by us:
```
void *recorded_malloc(size_t size, int line, const char *file) {
void *ptr = malloc(size);
if (ptr != NULL) {
// record this allocation
}
return ptr;
}
void *recorded_calloc(size_t cnt, size_t each_size, int line, const char *file) {
void *ptr = calloc(cnt, each_size);
if (ptr != NULL) {
// record this allocation
}
return ptr;
}
void recorded_free(void *ptr, int line, const char *file) {
// ...
}
#define malloc(SIZE) recorded_malloc(SIZE, __LINE__, __FILE__)
#define calloc(CNT, EACH_SIZE) recorded_calloc(CNT, EACH_SIZE, __LINE__, __FILE__)
#define free(PTR) recorded_free(PTR, __LINE__, __FILE__)
```

# Leak checking
In the end, we need to check whether all allocated blocks of memory have been free d. This
should be done when the program terminates. To force a function to be called on termination, we
need another "black magic":
void check_leak(void) __attribute__((destructor));
void check_leak(void) {
// Traverse your array to see whether any allocated memory is not freed.
}
With an additional declaration marked by __attribute__((destructor)) , this function will be called
automatically when the program terminates. You may try the following code (in destructor.c ) on
your own:

#include <stdio.h>
void fun(void) __attribute__((destructor));
void fun(void) {
printf("Goodbye world!\n");
}
int main(void) {}
Even though the main function is empty, this program will print Goodbye world! .
# Usage
This is what we want to achieve: Place the code you wrote in a header file memcheck.h :
```
// memcheck.h
#ifndef CS100_MEMCHECK_H
#define CS100_MEMCHECK_H 1
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_RECORDS 1000
struct Record { /* ... */ };
struct Record records[MAX_RECORDS];
void *recorded_malloc(/* ... */) {
// ...
}
void *recorded_calloc(/* ... */) {
// ...
}
void recorded_free(/* ... */) {
// ...
}
void check_leak(void) __attribute__((destructor));
void check_leak(void) {
// ...
}
#define malloc(SIZE) recorded_malloc(SIZE, __LINE__, __FILE__)
#define calloc(CNT, EACH_SIZE) recorded_calloc(CNT, EACH_SIZE, __LINE__, __FILE__)
#define free(PTR) recorded_free(PTR, __LINE__, __FILE__)
#endif // CS100_MEMCHECK_H

```
Then, #include "memcheck.h" in the file you want to check:
```
// Suppose this is my_code.c
#include "memcheck.h"
// Some code that may have memory problems:
int main(void) {
int **p = malloc(sizeof(int *) * 5);
for (int i = 0; i != 5; ++i)
p[i] = malloc(sizeof(int) * 3); // Suppose this is line 7
// do something
// ...
// ...
free(p[2] + 3); // Suppose this is line 11
free(p);
return 0;
}
```
Compile and run my_code.c . The output should be:
Invalid free in file my_code.c, line 11
Summary:
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
5 allocation records not freed (60 bytes in total).
# Detailed requirements
The detailed requirements on how you should deal with certain situations and what to print are
as follows.
free(NULL) should do nothing.
If malloc or calloc returns NULL (indicating failure), no memory is allocated and nothing
should be recorded.
When an invalid free happens, you should (in the function recorded_free )
detect it, and output
with a newline ( '\n' ) at the end, where FILENAME is replaced with the file name, and N is
replaced with the line number, and
prevent the call to the real free function, so that no invalid free s actually happen.
When the program terminates (i.e., in the function leak_check ), you should make a summary
of the memory usage of this program:
First, output
with a newline at the end.
Traverse your array to see whether any memory is not free d. For each such record,
output
// ...
// ...
free(p[2] + 3); // Suppose this is line 11
free(p);
return 0;
}
Invalid free in file my_code.c, line 11
Summary:
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
12 bytes memory not freed (allocated in file my_code.c, line 7)
5 allocation records not freed (60 bytes in total).
Invalid free in file FILENAME, line N
Summary:
M bytes memory not freed (allocated in file FILENAME, line N
with a newline at the end, where M is replaced with the size (in bytes) of that block of
memory, FILENAME is replaced with the file name, and N is replaced with the line
number. These lines can be printed in any order. You don't need to care about
singular/plural forms of "byte". Just print bytes even if M == 1 .
As a conclusion, if memory leak is detected, output
with a newline at the end, where N is replaced with the number of allocation records
that are not free d, and M is replaced with the total size (in bytes) of memory leaked.
You don't need to care about singular/plural forms of "record". Just print records even if
N == 1 .
If there is no memory leak, output
with a newline at the end.
Special things you should know
The behaviors of malloc(0) , calloc(0, N) and calloc(N, 0) are implementation-defined.
They may return a null pointer (with no memory allocated), but they may also allocate some
memory and return a pointer to it. Whenever the returned pointer is non-null, it should be
recorded, and it is also considered as memory leak if that memory is not free d.
Due to alignment requirements or some other possible reasons, malloc and calloc may
allocate more memory than you expect. For example, calloc(N, M) may allocate more than N
* M bytes of memory, and malloc(0) may allocate some memory, as has been mentioned.
But you don't need to consider these complex things - just count N for malloc(N) and N * M
for calloc(N, M) . In other words, the following is a possible summary output:
Notes
We have provided you with a template code memcheck.c that contains everything mentioned
above.
It is guaranteed that the total number of calls to malloc and calloc will not exceed
MAX_RECORDS , which is 1000 .
Feel free to add other helper functions, if you need. Contact us if you encounter name
collision problems.
You may try some fancy data structures, like linked-lists or hash-tables, to optimize the timeand space-complexity. But these will not give you additional (bonus) points.
Samples
N allocation records not freed (M bytes in total).
All allocations have been freed.
Sample program 1
sample1.c
Output:
Sample program 2
sample2.c
Possible output: (The line numbers may be different)
If malloc(0) allocates some memory and returns a non-null pointer, the output should be
#include "memcheck.h"
int main(void) {
int **matrix = malloc(sizeof(int *) * 10);
for (int i = 0; i != 10; ++i)
matrix[i] = malloc(sizeof(int) * 5);
// do some calculations
for (int i = 0; i != 10; ++i)
free(matrix[i]);
free(matrix);
matrix = NULL;
free(matrix); // free(NULL) should do nothing
return 0;
}

## 作业代写价格:
|类型|美元|人民币|
|-----:|-----:|-----:|
|Assignment|$40-$120|¥400-¥800|

### 为了方便快速定价和确定是否代做，麻烦提供私聊的时候提供以下信息：
如果是作业，提供本次作业要求文档；如果是考试，提供考试范围和考试时间
一两句话概括一下作业任务或者考试任务，比如”可以帮忙实现一个决策树算法吗？”、”网络安全选择题考试，范围是内网横向移动知识点”
### 作业定价有两种方式：
    - 根据定价标准进行
    - 通过微信我们一起协商
## 联系方式
[为您提供 CS , DS , 商科 编程作业代写](http://dzuoye.work "编程代写")
微信（WeChat）：design2023866

<img src="design2023866.jpg"  width="200" />


