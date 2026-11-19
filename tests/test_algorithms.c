#include "issueforge/algorithms.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
static void sorting(void){
 int x[]={9,-2,7,7,0,4,1},a[7],b[7],e[]={-2,0,1,4,7,7,9};memcpy(a,x,sizeof a);memcpy(b,x,sizeof b);
 if_merge_sort(a,7);if_quick_sort(b,7);assert(!memcmp(a,e,sizeof a));assert(!memcmp(b,e,sizeof b));
 assert(if_binary_search_iterative(a,7,4)==3);assert(if_binary_search_recursive(a,7,4)==3);
 assert(if_binary_search_iterative(a,7,99)==-1);assert(if_binary_search_recursive(a,7,-99)==-1);
}
static void maximum(void){
 int x[]={-2,1,-3,4,-1,2,1,-5,4},y[]={-8,-3,-6};IFSubarrayResult r;
 assert(if_maximum_subarray(x,9,&r)==0&&r.sum==6&&r.start==3&&r.end==6);
 assert(if_maximum_subarray(y,3,&r)==0&&r.sum==-3&&r.start==1&&r.end==1);
 assert(if_maximum_subarray(NULL,0,&r)!=0);
}
static void power_test(void){double r;assert(!if_fast_power(2,10,&r)&&fabs(r-1024)<1e-9);assert(!if_fast_power(2,-3,&r)&&fabs(r-.125)<1e-9);assert(if_fast_power(0,-1,&r));}
static void celebrity(void){
 const unsigned char k[]={0,1,1,0,0,1,0,0,0};assert(if_find_celebrity(k,3)==2);
 const unsigned char no[]={0,1,1,0};assert(if_find_celebrity(no,2)==-1);assert(if_find_celebrity(NULL,0)==-1);
}
static void matrices(void){
 const int64_t a[]={1,2,3,4,5,6,7,8,9},b[]={9,8,7,6,5,4,3,2,1},e[]={30,24,18,84,69,54,138,114,90};
 int64_t c[9],s[9];assert(!if_matrix_multiply_classical(a,b,c,3));assert(!if_matrix_multiply_strassen(a,b,s,3));
 assert(!memcmp(c,e,sizeof e));assert(!memcmp(s,e,sizeof e));assert(!memcmp(c,s,sizeof c));assert(if_matrix_multiply_strassen(NULL,b,s,3));
 /* 33 pads to 64 and enters the recursive Strassen path. */
 int64_t aa[33*33],bb[33*33],cc[33*33],ss[33*33];
 for(size_t i=0;i<33*33;++i){aa[i]=(int64_t)(i%7)-3;bb[i]=(int64_t)(i%5)-2;}
 assert(!if_matrix_multiply_classical(aa,bb,cc,33));assert(!if_matrix_multiply_strassen(aa,bb,ss,33));
 assert(!memcmp(cc,ss,sizeof cc));
}
int main(void){sorting();maximum();power_test();celebrity();matrices();puts("Phase 2 algorithm tests passed.");return 0;}
