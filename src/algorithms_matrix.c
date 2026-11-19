#include "issueforge/algorithms.h"
#include <stdlib.h>
#include <string.h>
static void classical(const int64_t*a,const int64_t*b,int64_t*c,size_t n){
 for(size_t i=0;i<n;++i)for(size_t j=0;j<n;++j){int64_t s=0;for(size_t k=0;k<n;++k)s+=a[i*n+k]*b[k*n+j];c[i*n+j]=s;}
}
int if_matrix_multiply_classical(const int64_t*a,const int64_t*b,int64_t*out,size_t n){
 if(!a||!b||!out||!n||n>SIZE_MAX/n||n*n>SIZE_MAX/sizeof(int64_t))return -1;
 classical(a,b,out,n);return 0;
}
static void prepare_product(const int64_t*a,const int64_t*b,int64_t*s,int64_t*t,size_t n,int mode){
 size_t h=n/2;
 for(size_t i=0;i<h;++i)for(size_t j=0;j<h;++j){
  size_t q=i*h+j,x=i*n+j,y=x+h,z=(i+h)*n+j,w=z+h;
  switch(mode){
   case 1:s[q]=a[x]+a[w];t[q]=b[x]+b[w];break;
   case 2:s[q]=a[z]+a[w];t[q]=b[x];break;
   case 3:s[q]=a[x];t[q]=b[y]-b[w];break;
   case 4:s[q]=a[w];t[q]=b[z]-b[x];break;
   case 5:s[q]=a[x]+a[y];t[q]=b[w];break;
   case 6:s[q]=a[z]-a[x];t[q]=b[x]+b[y];break;
   default:s[q]=a[y]-a[w];t[q]=b[z]+b[w];break;
  }
 }
}
static void strassen(const int64_t*a,const int64_t*b,int64_t*c,size_t n){
 if(n<=32){classical(a,b,c,n);return;}
 size_t h=n/2,z=h*h;
 if(z>SIZE_MAX/(11*sizeof(int64_t))){classical(a,b,c,n);return;}
 int64_t*q=calloc(11*z,sizeof(int64_t));
 if(!q){classical(a,b,c,n);return;}
 int64_t*s=q,*t=s+z,*p1=t+z,*p2=p1+z,*p3=p2+z,*p4=p3+z,*p5=p4+z,*p6=p5+z,*p7=p6+z;
 int64_t*products[]={p1,p2,p3,p4,p5,p6,p7};
 for(int mode=1;mode<=7;++mode){prepare_product(a,b,s,t,n,mode);strassen(s,t,products[mode-1],h);}
 for(size_t i=0;i<h;++i)for(size_t j=0;j<h;++j){
  size_t u=i*h+j,x=i*n+j,y=x+h,v=(i+h)*n+j,w=v+h;
  c[x]=p1[u]+p4[u]-p5[u]+p7[u];c[y]=p3[u]+p5[u];c[v]=p2[u]+p4[u];c[w]=p1[u]-p2[u]+p3[u]+p6[u];
 }
 free(q);
}
int if_matrix_multiply_strassen(const int64_t*a,const int64_t*b,int64_t*out,size_t n){
 if(!a||!b||!out||!n||n>SIZE_MAX/n||n*n>SIZE_MAX/sizeof(int64_t))return -1;
 size_t m=1;while(m<n){if(m>SIZE_MAX/2)return -1;m*=2;}
 if(m>SIZE_MAX/m||m*m>SIZE_MAX/sizeof(int64_t))return -1;
 size_t cells=m*m;int64_t*pa=calloc(cells,sizeof(int64_t)),*pb=calloc(cells,sizeof(int64_t)),*pc=calloc(cells,sizeof(int64_t));
 if(!pa||!pb||!pc){free(pa);free(pb);free(pc);return -1;}
 for(size_t i=0;i<n;++i){memcpy(pa+i*m,a+i*n,n*sizeof(int64_t));memcpy(pb+i*m,b+i*n,n*sizeof(int64_t));}
 strassen(pa,pb,pc,m);for(size_t i=0;i<n;++i)memcpy(out+i*n,pc+i*m,n*sizeof(int64_t));
 free(pa);free(pb);free(pc);return 0;
}
