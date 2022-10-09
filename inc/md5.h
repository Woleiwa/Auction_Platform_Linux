#pragma once
#ifndef _MD5H_
#define _MD5H_
#include <math.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

void ROL(unsigned int& s, unsigned short cx);
void ltob(unsigned int& i); 
unsigned int* MD5(const char* mStr); 
unsigned int* MD5_2(const char* mStr);
#endif

//�����https://blog.csdn.net/Oliver_xpl/article/details/90214896?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166304927816782391834263%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=166304927816782391834263&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-90214896-null-null.142^v47^pc_rank_34_default_3,201^v3^control_1&utm_term=md5&spm=1018.2226.3001.4187