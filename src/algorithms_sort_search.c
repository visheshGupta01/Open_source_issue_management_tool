#include "issueforge/algorithms.h"
#include <stdlib.h>
static void merge_ranges(int v[],int t[],size_t l,size_t m,size_t r){
 size_t i=l,j=m,k=l;
 while(i<m&&j<r)t[k++]=v[i]<=v[j]?v[i++]:v[j++];
 while(i<m)t[k++]=v[i++]; while(j<r)t[k++]=v[j++];
 for(i=l;i<r;++i)v[i]=t[i];
}
static void merge_range(int v[],int t[],size_t l,size_t r){
 if(r-l<2)return; size_t m=l+(r-l)/2;
 merge_range(v,t,l,m);merge_range(v,t,m,r);merge_ranges(v,t,l,m,r);
}
void if_merge_sort(int v[],size_t n){
 if(!v||n<2||n>SIZE_MAX/sizeof(int))return;
 int*t=malloc(n*sizeof(*t));if(!t)return;merge_range(v,t,0,n);free(t);
}
static void quick_range(int v[],ptrdiff_t lo,ptrdiff_t hi){
 while(lo<hi){
  int p=v[lo+(hi-lo)/2];ptrdiff_t i=lo,j=hi;
  while(i<=j){while(v[i]<p)++i;while(v[j]>p)--j;if(i<=j){int x=v[i];v[i]=v[j];v[j]=x;++i;--j;}}
  if(j-lo<hi-i){if(lo<j)quick_range(v,lo,j);lo=i;}
  else{if(i<hi)quick_range(v,i,hi);hi=j;}
 }
}
void if_quick_sort(int v[],size_t n){if(!v||n<2||n>(size_t)PTRDIFF_MAX)return;quick_range(v,0,(ptrdiff_t)n-1);}
ptrdiff_t if_binary_search_iterative(const int v[],size_t n,int target){
 if(!v||n>(size_t)PTRDIFF_MAX)return -1;size_t l=0,r=n;
 while(l<r){size_t m=l+(r-l)/2;if(v[m]==target)return (ptrdiff_t)m;if(v[m]<target)l=m+1;else r=m;}return -1;
}
static ptrdiff_t binary_range(const int v[],ptrdiff_t l,ptrdiff_t r,int target){
 if(l>r)return -1;ptrdiff_t m=l+(r-l)/2;
 if(v[m]==target)return m;return v[m]<target?binary_range(v,m+1,r,target):binary_range(v,l,m-1,target);
}
ptrdiff_t if_binary_search_recursive(const int v[],size_t n,int target){
 if(!v||n==0||n>(size_t)PTRDIFF_MAX)return -1;return binary_range(v,0,(ptrdiff_t)n-1,target);
}
