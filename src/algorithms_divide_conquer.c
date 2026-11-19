#include "issueforge/algorithms.h"
#include <limits.h>
static IFSubarrayResult crossing(const int v[],size_t l,size_t m,size_t r){
 int64_t sum=0,left=INT64_MIN,right=INT64_MIN;size_t start=m,end=m;
 for(size_t i=m+1;i-- >l;){sum+=v[i];if(sum>left){left=sum;start=i;}}
 sum=0;for(size_t i=m+1;i<=r;++i){sum+=v[i];if(sum>right){right=sum;end=i;}}
 IFSubarrayResult x={left+right,start,end};return x;
}
static IFSubarrayResult max_range(const int v[],size_t l,size_t r){
 if(l==r){IFSubarrayResult x={v[l],l,l};return x;}
 size_t m=l+(r-l)/2;IFSubarrayResult a=max_range(v,l,m),b=max_range(v,m+1,r),c=crossing(v,l,m,r);
 if(a.sum>=b.sum&&a.sum>=c.sum)return a;
 if(b.sum>=a.sum&&b.sum>=c.sum)return b;
 return c;
}
int if_maximum_subarray(const int v[],size_t n,IFSubarrayResult*out){
 if(!v||!n||!out)return -1;
 *out=max_range(v,0,n-1);return 0;
}
int if_fast_power(double base,int exponent,double*out){
 if(!out||(base==0.0&&exponent<0))return -1;
 unsigned int p=exponent<0?(unsigned int)(-(long long)exponent):(unsigned int)exponent;
 double ans=1.0;
 while(p){if(p&1U)ans*=base;base*=base;p>>=1U;}
 *out=exponent<0?1.0/ans:ans;return 0;
}
ptrdiff_t if_find_celebrity(const unsigned char*k,size_t n){
 if(!k||!n||n>(size_t)PTRDIFF_MAX||n>SIZE_MAX/n)return -1;
 size_t c=0;
 for(size_t i=1;i<n;++i)if(k[c*n+i])c=i;
 for(size_t i=0;i<n;++i)if(i!=c&&(k[c*n+i]||!k[i*n+c]))return -1;
 return (ptrdiff_t)c;
}
