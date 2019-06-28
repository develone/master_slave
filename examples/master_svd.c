#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
pthread_t th_id[2];

 


extern int dsvd(float **a, int m, int n, float *w, float **v);

extern int trans(float **a,float **b,int m,int n);
extern int disp(float **a,int m,int n);
extern int mul(float **a,float **b,float **c,int m,int n,int p,int q);	




/*	
 *  a 9 x 8
 *  u 9 x 8
 *  v 9 x 8 
 *  ds 9 x 8 
 *  vt 8 x 9
 *  uds 9 x 8
 *  udsvt 9 X 9
*/

//Several of the arrays use 2 pointers to allocate memory. 
//9 x 8 arrays
 

struct args {
    char* name;
    int age;
};


struct FILEs {
	char *input_file;
	char *first_output;
	char *second_output;
	int status;
	//status 0 nothing done
	//status 1 input_file read
	//status 2 svd done
	//status 3 Sx.bin written
	//status 4 reconst file written
	int num_bytes_rd;
	
	
};

struct th_var {
	float w[256], *pw;
	int m,n,i,j,p,q,result,len1,len2,len3,len4;
	//Several of the arrays use 2 pointers to allocate memory. 
	//9 x 8 arrays
	float *pv, **ppv, **ppvfr;
	float *puds, **ppuds, **ppudsfr;
	float *pa,**ppa, **ppafr;
	float *pds, **ppds, **ppdsfr;
	//8 x 9 arrays
	float *pvt, **ppvt, **ppvtfr;
	//9 x 9 arrays
	float *pudsvt, **ppudsvt, **ppudsvtfr;

	int *ps, **pps, **ppsfr;
	int *inbuf, *inbuffr;
	
	FILE *inptr,*outptr;

};
struct th_var th0;
struct th_var th1;
struct th_var th2;
void *hello(void *input) {
    printf("name: %s\n", ((struct args*)input)->name);
    printf("age: %d\n", ((struct args*)input)->age);
}

void free_mem() {
	free(th0.ppsfr);
	free(th0.inbuffr);
	free(th0.ppvfr);
	free(th0.ppudsfr);
	free(th0.ppafr);
	free(th0.ppdsfr);
	free(th0.ppvtfr);
	free(th0.ppudsvtfr);
}

 void *mysvd(void *strptr) {
	pthread_t id = pthread_self();
	int sw;
	if(pthread_equal(id,th_id[0])!=0) {
		sw = 0;
		
		th0.m = 256;
		th0.n = 256;
		th0.p = 256;
		th0.q = 256;
		
		th0.inbuf = (int *)malloc(sizeof(int)*th0.m*th0.n);
		th0.inbuffr = th0.inbuf;
	}
	if(pthread_equal(id,th_id[1])!=0) {
		sw = 1;
		th1.m = 256;
		th1.n = 256;
		th1.p = 256;
		th1.q = 256;
		
		th1.inbuf = (int *)malloc(sizeof(int)*th1.m*th1.n);
		th1.inbuffr = th1.inbuf;
	}
	if(pthread_equal(id,th_id[2])!=0) {
		sw = 2;
		th2.m = 256;
		th2.n = 256;
		th2.p = 256;
		th2.q = 256;
		
		th2.inbuf = (int *)malloc(sizeof(int)*th2.m*th2.n);
		th2.inbuffr = th2.inbuf;
	}
	switch(sw) {
		case 0:
			

			printf("\n 1st thread processing th_id[1] 0x%x\n",th_id[0]);
			printf("In mysvd input_file: %s\n", ((struct FILEs*)strptr)->input_file);
			printf("In mysvd first_output: %s\n", ((struct FILEs*)strptr)->first_output);
			printf("In mysvd second_output: %s\n", ((struct FILEs*)strptr)->second_output);
			printf("In mysvd status: %d\n", ((struct FILEs*)strptr)->status);
			printf("In mysvd num_bytes_rd: %d\n", ((struct FILEs*)strptr)->num_bytes_rd);
			printf("\n");
			th0.inptr = fopen(((struct FILEs*)strptr)->input_file,"r");
			if (th0.inptr == 0) printf("file not found\n");
			th0.len1 = fread(th0.inbuf,sizeof(int),th0.m*th0.n,th0.inptr);
			fclose(th0.inptr);
			((struct FILEs*)strptr)->num_bytes_rd = th0.len1;
			((struct FILEs*)strptr)->status = 1;
			printf("In mysvd status input file read: %d num_bytes_rd %d\n", ((struct FILEs*)strptr)->status,((struct FILEs*)strptr)->num_bytes_rd);
			printf("grn.bin th0.len1 = %d \n",th0.len1);
			th0.len1 = sizeof(float *) * th0.m + sizeof(float) * th0.n * th0.m;
			th0.len2 = sizeof(float *) * th0.n + sizeof(float) * th0.m * th0.n;
			th0.len3 = sizeof(float *) * th0.p + sizeof(float) * th0.p * th0.q;
			th0.len4 = sizeof(int *) * th0.n + sizeof(int) * th0.m * th0.n;
			printf("len = %d th0.len2 = %d th0.len3 = %d th0.len4 = %d\n",th0.len1, th0.len2,th0.len3,th0.len4);		
			printf("setting up ptrs with malloc\n");
			th0.ppv = (float **)malloc(th0.len1);
			th0.ppvfr = th0.ppv;
			th0.ppuds = (float **)malloc(th0.len1);
			th0.ppudsfr = th0.ppuds;
			th0.ppa = (float **)malloc(th0.len1);
			th0.ppafr = th0.ppa;
			th0.ppds = (float **)malloc(th0.len1);
			th0.ppdsfr = th0.ppds;
			th0.ppvt = (float **)malloc(th0.len2);
			th0.ppvtfr = th0.ppvt;
			th0.pps = (int **)malloc(th0.len4);
			th0.ppsfr = th0.ppsfr;
			th0.ppudsvt = (float **)malloc(th0.len3);
			th0.ppudsvtfr = th0.ppudsvt;
			// pv, puds, pa, pds, pvt, and pudsvt are now pointing to the first elements of 2D arrays 
			th0.pv = (float *)(th0.ppv + th0.m);
			th0.puds = (float *)(th0.ppuds + th0.m);
			th0.pa = (float *)(th0.ppa + th0.m);
			th0.pds = (float *)(th0.ppds + th0.m);
			th0.pvt = (float *)(th0.ppvt + th0.n);
			th0.pudsvt = (float *)(th0.ppudsvt + th0.p);
			th0.ps = (int *)(th0.pps + th0.m);
			printf("pa 0x%x ppa 0x%x  \n",th0.pa,th0.ppa);
			printf("pv = 0x%x ppv = 0x%x \n",th0.pv,th0.ppv);
			printf("pvt = 0x%x ppvt = 0x%x \n",th0.pvt,th0.ppvt);
			printf("pds = 0x%x ppds = 0x%x \n",th0.pds,th0.ppds);
			printf("puds = 0x%x ppuds = 0x%x \n",th0.puds,th0.ppuds);
			printf("pudsvt = 0x%x ppudsvt = 0x%x \n",th0.pudsvt,th0.ppudsvt);
	
			for(th0.i = 0; th0.i < th0.m; th0.i++) {
				th0.ppa[th0.i] = (th0.pa + th0.n * th0.i);
				th0.ppuds[th0.i] = (th0.puds + th0.n * th0.i);
				th0.ppv[th0.i] = (th0.pv + th0.n * th0.i);
				th0.ppds[th0.i] = (th0.pds + th0.n * th0.i);
	

			}	

			for(th0.i = 0; th0.i < th0.m; th0.i++) {
				th0.ppvt[th0.i] = (th0.pvt + th0.n * th0.i);
				th0.pps[th0.i] = (th0.ps + th0.n * th0.i);
			}
			for(th0.i = 0; th0.i < th0.m; th0.i++) th0.ppudsvt[th0.i] = (th0.pudsvt + th0.q * th0.i); 
	
			for(th0.i=0;th0.i<th0.m;th0.i++) {
				for(th0.j=0;th0.j<th0.n;th0.j++) {
					th0.ppa[th0.i][th0.j]=(float)*th0.inbuf;
					//printf("%d %d %5.1f \n",th0.i,th0.j,th0.ppa[th0.i][th0.j]);
					th0.inbuf++;
				}
			}
			th0.pw=(float *)&th0.w;
			th0.result = dsvd(th0.ppa,th0.m,th0.n,th0.pw,th0.ppv);
			for(th0.i=0;th0.i<th0.m;th0.i++) printf("%5.8f \n",th0.w[th0.i]);
			((struct FILEs*)strptr)->status = 2;
			th0.outptr = fopen(((struct FILEs*)strptr)->first_output,"w");
			if (th0.outptr == 0) printf("can not open file S.bin for writing\n");
			th0.result = fwrite(th0.pw,sizeof(float),th0.m,th0.outptr);
			fclose(th0.outptr);
			printf("U row = %d col = %d \n",th0.m,th0.n);
			//result = disp(th0.ppa,th0.m,th0.n);
			printf("Singular Values\n");
			//clear the S diagonal matrix
			for(th0.i=0;th0.i<th0.m;th0.i++) { 
				for(th0.j=0;th0.j<th0.n;th0.j++) {
					th0.ppds[th0.i][th0.j] = 0;
				}	
			}	
			//create the S diagonal matrix from w
			th0.j=0;
			for(th0.i=0;th0.i<th0.m;th0.i++) { 
				th0.ppds[th0.i][th0.j] = th0.w[th0.i];
				th0.j++;
			}
			
			printf("V row = %d col = %d \n",th0.m,th0.n);
			//th0.result = disp(th0.ppv,th0.m,th0.n);

			printf("V' row = %d col = %d \n",th0.m,th0.n);
 
			th0.result = trans(th0.ppv,th0.ppvt,th0.m,th0.n);
			//result = disp(ppvt,n,m);
			printf("Call mul u * s  \n");
			th0.result = mul(th0.ppa,th0.ppds,th0.ppuds,th0.m,th0.n,th0.p,th0.q);
			printf("UDS row = %d col = %d \n",th0.m,th0.n);
			//th0.result = disp(th0.ppuds,th0.m,th0.n);
			printf("Call mul u * ds * vt \n");

			th0.result = mul(th0.ppuds,th0.ppvt,th0.ppudsvt,th0.m,th0.n,th0.m,th0.n);
			printf("USDVT row = %d col = %d \n",th0.p,th0.q);
			for(th0.i=0;th0.i<th0.m;th0.i++) {
				for(th0.j=0;th0.j<th0.n;th0.j++) {
					//th0.pps[th0.i][th0.j]=(int)th0.ppudsvt[th0.i][th0.j];
					th0.pps[th0.i][th0.j]=FLOAT_TO_INT(th0.ppudsvt[th0.i][th0.j]);
					//printf("%d ",pps[i][j]);
		
				}
			}
			th0.outptr = fopen(((struct FILEs*)strptr)->second_output,"w"); 
			printf("ps converted from float to int 0x%x \n",th0.outptr);
			
			if (th0.outptr == 0) printf("can not open file reconst.bin for writing\n");
			th0.result = fwrite(th0.ps,sizeof(int),th0.m*th0.n,th0.outptr);
			fclose(th0.outptr);
			((struct FILEs*)strptr)->status = 3;
			printf("# of data written 0x%x \n",th0.result);

			break;
		case 1:
			printf("\n 2nd thread processing th_id[1] 0x%x\n",th_id[1]);
			printf("In mysvd input_file: %s\n", ((struct FILEs*)strptr)->input_file);
			printf("In mysvd first_output: %s\n", ((struct FILEs*)strptr)->first_output);
			printf("In mysvd second_output: %s\n", ((struct FILEs*)strptr)->second_output);
			printf("In mysvd status: %d\n", ((struct FILEs*)strptr)->status);
			printf("In mysvd num_bytes_rd: %d\n", ((struct FILEs*)strptr)->num_bytes_rd);
			printf("\n");
			th1.inptr = fopen(((struct FILEs*)strptr)->input_file,"r");
			if (th1.inptr == 0) printf("file not found\n");
			th1.len1 = fread(th1.inbuf,sizeof(int),th1.m*th1.n,th1.inptr);
			fclose(th1.inptr);
			((struct FILEs*)strptr)->num_bytes_rd = th1.len1;
			((struct FILEs*)strptr)->status = 1;
			printf("In mysvd status input file read: %d num_bytes_rd %d\n", ((struct FILEs*)strptr)->status,((struct FILEs*)strptr)->num_bytes_rd);
			printf("grn.bin th1.len1 = %d \n",th1.len1);
			th1.len1 = sizeof(float *) * th1.m + sizeof(float) * th1.n * th1.m;
			th1.len2 = sizeof(float *) * th1.n + sizeof(float) * th1.m * th1.n;
			th1.len3 = sizeof(float *) * th1.p + sizeof(float) * th1.p * th1.q;
			th1.len4 = sizeof(int *) * th1.n + sizeof(int) * th1.m * th1.n;
			printf("len = %d th1.len2 = %d th1.len3 = %d th1.len4 = %d\n",th1.len1, th1.len2,th1.len3,th1.len4);		
			printf("setting up ptrs with malloc\n");
			th1.ppv = (float **)malloc(th1.len1);
			th1.ppvfr = th1.ppv;
			th1.ppuds = (float **)malloc(th1.len1);
			th1.ppudsfr = th1.ppuds;
			th1.ppa = (float **)malloc(th1.len1);
			th1.ppafr = th1.ppa;
			th1.ppds = (float **)malloc(th1.len1);
			th1.ppdsfr = th1.ppds;
			th1.ppvt = (float **)malloc(th1.len2);
			th1.ppvtfr = th1.ppvt;
			th1.pps = (int **)malloc(th1.len4);
			th1.ppsfr = th1.ppsfr;
			th1.ppudsvt = (float **)malloc(th1.len3);
			th1.ppudsvtfr = th1.ppudsvt;
			// pv, puds, pa, pds, pvt, and pudsvt are now pointing to the first elements of 2D arrays 
			th1.pv = (float *)(th1.ppv + th1.m);
			th1.puds = (float *)(th1.ppuds + th1.m);
			th1.pa = (float *)(th1.ppa + th1.m);
			th1.pds = (float *)(th1.ppds + th1.m);
			th1.pvt = (float *)(th1.ppvt + th1.n);
			th1.pudsvt = (float *)(th1.ppudsvt + th1.p);
			th1.ps = (int *)(th1.pps + th1.m);
			printf("pa 0x%x ppa 0x%x  \n",th1.pa,th1.ppa);
			printf("pv = 0x%x ppv = 0x%x \n",th1.pv,th1.ppv);
			printf("pvt = 0x%x ppvt = 0x%x \n",th1.pvt,th1.ppvt);
			printf("pds = 0x%x ppds = 0x%x \n",th1.pds,th1.ppds);
			printf("puds = 0x%x ppuds = 0x%x \n",th1.puds,th1.ppuds);
			printf("pudsvt = 0x%x ppudsvt = 0x%x \n",th1.pudsvt,th1.ppudsvt);
	
			for(th1.i = 0; th1.i < th1.m; th1.i++) {
				th1.ppa[th1.i] = (th1.pa + th1.n * th1.i);
				th1.ppuds[th1.i] = (th1.puds + th1.n * th1.i);
				th1.ppv[th1.i] = (th1.pv + th1.n * th1.i);
				th1.ppds[th1.i] = (th1.pds + th1.n * th1.i);
	

			}	

			for(th1.i = 0; th1.i < th1.m; th1.i++) {
				th1.ppvt[th1.i] = (th1.pvt + th1.n * th1.i);
				th1.pps[th1.i] = (th1.ps + th1.n * th1.i);
			}
			for(th1.i = 0; th1.i < th1.m; th1.i++) th1.ppudsvt[th1.i] = (th1.pudsvt + th1.q * th1.i); 
	
			for(th1.i=0;th1.i<th1.m;th1.i++) {
				for(th1.j=0;th1.j<th1.n;th1.j++) {
					th1.ppa[th1.i][th1.j]=(float)*th1.inbuf;
					printf("%d %d %5.1f \n",th1.i,th1.j,th1.ppa[th1.i][th1.j]);
					th1.inbuf++;
				}
			}
			th1.pw=(float *)&th1.w;
			th1.result = dsvd(th1.ppa,th1.m,th1.n,th1.pw,th1.ppv);
			for(th1.i=0;th1.i<th1.m;th1.i++) printf("%5.8f \n",th1.w[th1.i]);
			((struct FILEs*)strptr)->status = 2;
			th1.outptr = fopen(((struct FILEs*)strptr)->first_output,"w");
			if (th1.outptr == 0) printf("can not open file S.bin for writing\n");
			th1.result = fwrite(th1.pw,sizeof(float),th1.m,th1.outptr);
			fclose(th1.outptr);
			printf("U row = %d col = %d \n",th1.m,th1.n);
			//result = disp(th1.ppa,th1.m,th1.n);
			printf("Singular Values\n");
			//clear the S diagonal matrix
			for(th1.i=0;th1.i<th1.m;th1.i++) { 
				for(th1.j=0;th1.j<th1.n;th1.j++) {
					th1.ppds[th1.i][th1.j] = 0;
				}	
			}	
			//create the S diagonal matrix from w
			th1.j=0;
			for(th1.i=0;th1.i<th1.m;th1.i++) { 
				th1.ppds[th1.i][th1.j] = th1.w[th1.i];
				th1.j++;
			}
			
			printf("V row = %d col = %d \n",th1.m,th1.n);
			//th1.result = disp(th1.ppv,th1.m,th1.n);

			printf("V' row = %d col = %d \n",th1.m,th1.n);
 
			th1.result = trans(th1.ppv,th1.ppvt,th1.m,th1.n);
			//result = disp(ppvt,n,m);
			printf("Call mul u * s  \n");
			th1.result = mul(th1.ppa,th1.ppds,th1.ppuds,th1.m,th1.n,th1.p,th1.q);
			printf("UDS row = %d col = %d \n",th1.m,th1.n);
			//th1.result = disp(th1.ppuds,th1.m,th1.n);
			printf("Call mul u * ds * vt \n");

			th1.result = mul(th1.ppuds,th1.ppvt,th1.ppudsvt,th1.m,th1.n,th1.m,th1.n);
			printf("USDVT row = %d col = %d \n",th1.p,th1.q);
			for(th1.i=0;th1.i<th1.m;th1.i++) {
				for(th1.j=0;th1.j<th1.n;th1.j++) {
					//th1.pps[th1.i][th1.j]=(int)th1.ppudsvt[th1.i][th1.j];
					th1.pps[th1.i][th1.j]=FLOAT_TO_INT(th1.ppudsvt[th1.i][th1.j]);
					//printf("%d ",pps[i][j]);
		
				}
			}
			th1.outptr = fopen(((struct FILEs*)strptr)->second_output,"w"); 
			printf("ps converted from float to int 0x%x \n",th1.outptr);
			
			if (th1.outptr == 0) printf("can not open file reconst.bin for writing\n");
			th1.result = fwrite(th1.ps,sizeof(int),th1.m*th1.n,th1.outptr);
			fclose(th1.outptr);
			printf("# of data written 0x%x \n",th1.result);
			((struct FILEs*)strptr)->status = 3;
			break;		
			case 2:
 			printf("\n 3rd thread processing th_id[1] 0x%x\n",th_id[2]);
			printf("In mysvd input_file: %s\n", ((struct FILEs*)strptr)->input_file);
			printf("In mysvd first_output: %s\n", ((struct FILEs*)strptr)->first_output);
			printf("In mysvd second_output: %s\n", ((struct FILEs*)strptr)->second_output);
			printf("In mysvd status: %d\n", ((struct FILEs*)strptr)->status);
			printf("In mysvd num_bytes_rd: %d\n", ((struct FILEs*)strptr)->num_bytes_rd);
			printf("\n");
			th2.inptr = fopen(((struct FILEs*)strptr)->input_file,"r");
			if (th2.inptr == 0) printf("file not found\n");
			th2.len1 = fread(th2.inbuf,sizeof(int),th2.m*th2.n,th2.inptr);
			fclose(th2.inptr);
			((struct FILEs*)strptr)->num_bytes_rd = th2.len1;
			((struct FILEs*)strptr)->status = 1;
			printf("In mysvd status input file read: %d num_bytes_rd %d\n", ((struct FILEs*)strptr)->status,((struct FILEs*)strptr)->num_bytes_rd);
			printf("grn.bin th2.len1 = %d \n",th2.len1);
			th2.len1 = sizeof(float *) * th2.m + sizeof(float) * th2.n * th2.m;
			th2.len2 = sizeof(float *) * th2.n + sizeof(float) * th2.m * th2.n;
			th2.len3 = sizeof(float *) * th2.p + sizeof(float) * th2.p * th2.q;
			th2.len4 = sizeof(int *) * th2.n + sizeof(int) * th2.m * th2.n;
			printf("len = %d th2.len2 = %d th2.len3 = %d th2.len4 = %d\n",th2.len1, th2.len2,th2.len3,th2.len4);		
			printf("setting up ptrs with malloc\n");
			th2.ppv = (float **)malloc(th2.len1);
			th2.ppvfr = th2.ppv;
			th2.ppuds = (float **)malloc(th2.len1);
			th2.ppudsfr = th2.ppuds;
			th2.ppa = (float **)malloc(th2.len1);
			th2.ppafr = th2.ppa;
			th2.ppds = (float **)malloc(th2.len1);
			th2.ppdsfr = th2.ppds;
			th2.ppvt = (float **)malloc(th2.len2);
			th2.ppvtfr = th2.ppvt;
			th2.pps = (int **)malloc(th2.len4);
			th2.ppsfr = th2.ppsfr;
			th2.ppudsvt = (float **)malloc(th2.len3);
			th2.ppudsvtfr = th2.ppudsvt;
			// pv, puds, pa, pds, pvt, and pudsvt are now pointing to the first elements of 2D arrays 
			th2.pv = (float *)(th2.ppv + th2.m);
			th2.puds = (float *)(th2.ppuds + th2.m);
			th2.pa = (float *)(th2.ppa + th2.m);
			th2.pds = (float *)(th2.ppds + th2.m);
			th2.pvt = (float *)(th2.ppvt + th2.n);
			th2.pudsvt = (float *)(th2.ppudsvt + th2.p);
			th2.ps = (int *)(th2.pps + th2.m);
			printf("pa 0x%x ppa 0x%x  \n",th2.pa,th2.ppa);
			printf("pv = 0x%x ppv = 0x%x \n",th2.pv,th2.ppv);
			printf("pvt = 0x%x ppvt = 0x%x \n",th2.pvt,th2.ppvt);
			printf("pds = 0x%x ppds = 0x%x \n",th2.pds,th2.ppds);
			printf("puds = 0x%x ppuds = 0x%x \n",th2.puds,th2.ppuds);
			printf("pudsvt = 0x%x ppudsvt = 0x%x \n",th2.pudsvt,th2.ppudsvt);
	
			for(th2.i = 0; th2.i < th2.m; th2.i++) {
				th2.ppa[th2.i] = (th2.pa + th2.n * th2.i);
				th2.ppuds[th2.i] = (th2.puds + th2.n * th2.i);
				th2.ppv[th2.i] = (th2.pv + th2.n * th2.i);
				th2.ppds[th2.i] = (th2.pds + th2.n * th2.i);
	

			}	

			for(th2.i = 0; th2.i < th2.m; th2.i++) {
				th2.ppvt[th2.i] = (th2.pvt + th2.n * th2.i);
				th2.pps[th2.i] = (th2.ps + th2.n * th2.i);
			}
			for(th2.i = 0; th2.i < th2.m; th2.i++) th2.ppudsvt[th2.i] = (th2.pudsvt + th2.q * th2.i); 
	
			for(th2.i=0;th2.i<th2.m;th2.i++) {
				for(th2.j=0;th2.j<th2.n;th2.j++) {
					th2.ppa[th2.i][th2.j]=(float)*th2.inbuf;
					printf("%d %d %5.1f \n",th2.i,th2.j,th2.ppa[th2.i][th2.j]);
					th2.inbuf++;
				}
			}
			th2.pw=(float *)&th2.w;
			th2.result = dsvd(th2.ppa,th2.m,th2.n,th2.pw,th2.ppv);
			for(th2.i=0;th2.i<th2.m;th2.i++) printf("%5.8f \n",th2.w[th2.i]);
			((struct FILEs*)strptr)->status = 2;
			th2.outptr = fopen(((struct FILEs*)strptr)->first_output,"w");
			if (th2.outptr == 0) printf("can not open file S.bin for writing\n");
			th2.result = fwrite(th2.pw,sizeof(float),th2.m,th2.outptr);
			fclose(th2.outptr);
			printf("U row = %d col = %d \n",th2.m,th2.n);
			//result = disp(th2.ppa,th2.m,th2.n);
			printf("Singular Values\n");
			//clear the S diagonal matrix
			for(th2.i=0;th2.i<th2.m;th2.i++) { 
				for(th2.j=0;th2.j<th2.n;th2.j++) {
					th2.ppds[th2.i][th2.j] = 0;
				}	
			}	
			//create the S diagonal matrix from w
			th2.j=0;
			for(th2.i=0;th2.i<th2.m;th2.i++) { 
				th2.ppds[th2.i][th2.j] = th2.w[th2.i];
				th2.j++;
			}
			
			printf("V row = %d col = %d \n",th2.m,th2.n);
			//th2.result = disp(th2.ppv,th2.m,th2.n);

			printf("V' row = %d col = %d \n",th2.m,th2.n);
 
			th2.result = trans(th2.ppv,th2.ppvt,th2.m,th2.n);
			//result = disp(ppvt,n,m);
			printf("Call mul u * s  \n");
			th2.result = mul(th2.ppa,th2.ppds,th2.ppuds,th2.m,th2.n,th2.p,th2.q);
			printf("UDS row = %d col = %d \n",th2.m,th2.n);
			//th2.result = disp(th2.ppuds,th2.m,th2.n);
			printf("Call mul u * ds * vt \n");

			th2.result = mul(th2.ppuds,th2.ppvt,th2.ppudsvt,th2.m,th2.n,th2.m,th2.n);
			printf("USDVT row = %d col = %d \n",th2.p,th2.q);
			for(th2.i=0;th2.i<th2.m;th2.i++) {
				for(th2.j=0;th2.j<th2.n;th2.j++) {
					//th2.pps[th2.i][th2.j]=(int)th2.ppudsvt[th2.i][th2.j];
					th2.pps[th2.i][th2.j]=FLOAT_TO_INT(th2.ppudsvt[th2.i][th2.j]);
					//printf("%d ",pps[i][j]);
		
				}
			}
			th2.outptr = fopen(((struct FILEs*)strptr)->second_output,"w"); 
			printf("ps converted from float to int 0x%x \n",th2.outptr);
			
			if (th2.outptr == 0) printf("can not open file reconst.bin for writing\n");
			th2.result = fwrite(th2.ps,sizeof(int),th2.m*th2.n,th2.outptr);
			fclose(th2.outptr);
			((struct FILEs*)strptr)->status = 3;
			printf("# of data written 0x%x \n",th2.result);

			break;		
		}

			 
	return NULL;
}
 

int main() {
	
	
	
	char *inp1, *inp2, *inp3;
	struct FILEs *files1 = (struct FILEs *)malloc(sizeof(struct FILEs));
    struct FILEs *files2 = (struct FILEs *)malloc(sizeof(struct FILEs));
    struct FILEs *files3 = (struct FILEs *)malloc(sizeof(struct FILEs));
    
    inp1 = "red.bin";
    inp2 = "grn.bin";
    inp3 = "blu.bin";
    
    files1->input_file = inp1;
    files2->input_file = inp2;
    files3->input_file = inp3;

    inp1 = "Sred.bin";
    inp2 = "Sgrn.bin";
    inp3 = "Sblu.bin";    
 
    
	files1->first_output = inp1;
	files2->first_output = inp2;
	files3->first_output = inp3;
	
	inp1 = "rcred.bin";
    inp2 = "rcgrn.bin";
    inp3 = "rcblu.bin";    
 
    
	files1->second_output = inp1;
	files2->second_output = inp2;
	files3->second_output = inp3;
	
    files1->status = 0;
    files2->status = 0;
    files3->status = 0;
    
    files1->num_bytes_rd = 0;
    files2->num_bytes_rd = 0;
    files3->num_bytes_rd = 0;
    printf("In main %s %s %s %d\n",files1->input_file,files1->first_output,files1->second_output,files1->status);
    printf("In main %s %s %s %d\n",files2->input_file,files2->first_output,files2->second_output,files2->status);
    printf("In main %s %s %s %d\n",files3->input_file,files3->first_output,files3->second_output,files3->status);

    
    struct args *Allen = (struct args *)malloc(sizeof(struct args));
    char f1[] = "files1";
    char allen[] = "Allen";
    Allen->name = allen;
    Allen->age = 20;
    

    

    pthread_t tid;
    th_id[0] = 0;
    th_id[1] = 0;
    th_id[2] = 0;
    pthread_create(&tid, NULL, hello, (void *)Allen);
    pthread_join(tid, NULL);
    printf("0x%x\n",th_id[0]);
    pthread_create(&th_id[0], NULL, mysvd, (void *)files1);
    
	pthread_join(th_id[0], NULL);
	
	pthread_create(&th_id[1], NULL, mysvd, (void *)files2);
	
	pthread_join(th_id[1], NULL);

	pthread_create(&th_id[2], NULL, mysvd, (void *)files3);

	pthread_join(th_id[2], NULL);
	printf("all threads joined\n");
	sleep (15);

		printf("In main status %d num_bytes_rd %d \n",files1->status,files1->num_bytes_rd);
		printf("In main status %d num_bytes_rd %d \n",files2->status,files2->num_bytes_rd);
		printf("In main status %d num_bytes_rd %d \n",files3->status,files3->num_bytes_rd);
	
    /*
    free(files1);
    free(files2);
    free(files3);
    */
    //free(Allen);
    
    return 0;
}
