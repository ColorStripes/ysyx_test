#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

word_t expr(char *e, bool *success);
void print_wp();
void delete_wp(int n);
void set_wp(char *expre,bool *successed);
void scan_wp(bool *isdebug);

#endif
