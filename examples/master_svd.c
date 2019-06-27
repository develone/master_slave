#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

pthread_t tidsvd1,tidsvd2,tidsvd3;
FILE *inptr,*outptr;
int *inbuf, *inbuffr;

extern int dsvd(float **a, int m, int n, float *w, float **v);

extern int trans(float **a,float **b,int m,int n);
extern int disp(float **a,int m,int n);
extern int mul(float **a,float **b,float **c,int m,int n,int p,int q);	

int m=256,n=256,i,j,p=256,q=256,result,len1,len2,len3,len4;

//float w[m],*pw;

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
float *pv, **ppv, **ppvfr;
float *puds, **ppuds, **ppudsfr;
float *pa,**ppa, **ppafr;
float *pds, **ppds, **ppdsfr;
//8 x 9 arrays
float *pvt, **ppvt, **ppvtfr;
//9 x 9 arrays
float *pudsvt, **ppudsvt, **ppudsvtfr;

int *ps, **pps, **ppsfr;

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

void *hello(void *input) {
    printf("name: %s\n", ((struct args*)input)->name);
    printf("age: %d\n", ((struct args*)input)->age);
}

void free_mem() {
	free(ppsfr);
	free(inbuffr);
	free(ppvfr);
	free(ppudsfr);
	free(ppafr);
	free(ppdsfr);
	free(ppvtfr);
	free(ppudsvtfr);
}

void setup_memory() {
	ppv = (float **)malloc(len1);
	ppvfr = ppv;
	ppuds = (float **)malloc(len1);
	ppudsfr = ppuds;
	ppa = (float **)malloc(len1);
	ppafr = ppa;
	ppds = (float **)malloc(len1);
	ppdsfr = ppds;
	ppvt = (float **)malloc(len2);
	ppvtfr = ppvt;
	pps = (int **)malloc(len4);
	ppsfr = ppsfr;
	ppudsvt = (float **)malloc(len3);
	ppudsvtfr = ppudsvt;
	// pv, puds, pa, pds, pvt, and pudsvt are now pointing to the first elements of 2D arrays 
	pv = (float *)(ppv + m);
	puds = (float *)(ppuds + m);
	pa = (float *)(ppa + m);
	pds = (float *)(ppds + m);
	pvt = (float *)(ppvt + n);
	pudsvt = (float *)(ppudsvt + p);
	ps = (int *)(pps + m);
	printf("pa 0x%x ppa 0x%x  \n",pa,ppa);
	printf("pv = 0x%x ppv = 0x%x \n",pv,ppv);
	printf("pvt = 0x%x ppvt = 0x%x \n",pvt,ppvt);
	printf("pds = 0x%x ppds = 0x%x \n",pds,ppds);
	printf("puds = 0x%x ppuds = 0x%x \n",puds,ppuds);
	printf("pudsvt = 0x%x ppudsvt = 0x%x \n",pudsvt,ppudsvt);
}
void *mysvd(void *pp) {
	pthread_t id = pthread_self();
	if(pthread_equal(id,tidsvd1)) {
		printf("\n 1st thread processing\n");
		printf("In mysvd input_file: %s\n", ((struct FILEs*)pp)->input_file);
		printf("In mysvd first_output: %s\n", ((struct FILEs*)pp)->first_output);
		printf("In mysvd second_output: %s\n", ((struct FILEs*)pp)->second_output);
		printf("In mysvd status: %d\n", ((struct FILEs*)pp)->status);
		printf("In mysvd num_bytes_rd: %d\n", ((struct FILEs*)pp)->num_bytes_rd);
		printf("\n");
		inptr = fopen(((struct FILEs*)pp)->input_file,"r");
		if (inptr == 0) printf("file not found\n");
		len1 = fread(inbuf,sizeof(int),m*n,inptr);
		fclose(inptr);
		((struct FILEs*)pp)->num_bytes_rd = len1;
		((struct FILEs*)pp)->status = 1;
		printf("In mysvd status input file read: %d\n", ((struct FILEs*)pp)->status);
		printf("red.bin len1 = %d \n",len1);
		len1 = sizeof(float *) * m + sizeof(float) * n * m;
		len2 = sizeof(float *) * n + sizeof(float) * m * n;
		len3 = sizeof(float *) * p + sizeof(float) * p * q;
		len4 = sizeof(int *) * n + sizeof(int) * m * n;
		printf("len = %d len2 = %d len3 = %d len4 = %d\n",len1, len2,len3,len4);
		printf("setting up ptrs with malloc\n");
		setup_memory();
		/*
		for(i=0;i<m;i++) {
			for(j=0;j<n;j++) {
				ppa[i][j]=(float)*inbuf;
				printf("%d %d %5.1f \n",i,j,ppa[i][j]);
				inbuf++;
			}
		}
		*/
 //free_mem();
 	}
	
	if(pthread_equal(id,tidsvd2)) {
		printf("\n 2nd thread processing\n");
		printf("In mysvd input_file: %s\n", ((struct FILEs*)pp)->input_file);
		printf("In mysvd first_output: %s\n", ((struct FILEs*)pp)->first_output);
		printf("In mysvd second_output: %s\n", ((struct FILEs*)pp)->second_output);
		printf("In mysvd status: %d\n", ((struct FILEs*)pp)->status);
		printf("In mysvd num_bytes_rd: %d\n", ((struct FILEs*)pp)->num_bytes_rd);
		printf("\n");
		inptr = fopen(((struct FILEs*)pp)->input_file,"r");
		if (inptr == 0) printf("file not found\n");
		len1 = fread(inbuf,sizeof(int),m*n,inptr);
		fclose(inptr);
		((struct FILEs*)pp)->num_bytes_rd = len1;
		((struct FILEs*)pp)->status = 1;
		printf("In mysvd status input file read: %d\n", ((struct FILEs*)pp)->status);
		printf("grn.bin len1 = %d \n",len1);
		len1 = sizeof(float *) * m + sizeof(float) * n * m;
		len2 = sizeof(float *) * n + sizeof(float) * m * n;
		len3 = sizeof(float *) * p + sizeof(float) * p * q;
		len4 = sizeof(int *) * n + sizeof(int) * m * n;
		printf("len = %d len2 = %d len3 = %d len4 = %d\n",len1, len2,len3,len4);		
		printf("setting up ptrs with malloc\n");
		setup_memory();
		/*
		for(i=0;i<m;i++) {
			for(j=0;j<n;j++) {
				ppa[i][j]=(float)*inbuf;
				printf("%d %d %5.1f \n",i,j,ppa[i][j]);
				inbuf++;
			}
		}
		*/
 //free_mem();
 	}

	if(pthread_equal(id,tidsvd3)) {
		printf("\n 3rd thread processing\n");
		printf("In mkysvd input_file: %s\n", ((struct FILEs*)pp)->input_file);
		printf("In mysvd first_output: %s\n", ((struct FILEs*)pp)->first_output);
		printf("In mysvd second_output: %s\n", ((struct FILEs*)pp)->second_output);
		printf("In mysvd status: %d\n", ((struct FILEs*)pp)->status);
		printf("In mysvd num_bytes_rd: %d\n", ((struct FILEs*)pp)->num_bytes_rd);
		printf("\n");
		inptr = fopen(((struct FILEs*)pp)->input_file,"r");
		if (inptr == 0) printf("file not found\n");
		len1 = fread(inbuf,sizeof(int),m*n,inptr);
		fclose(inptr);
		((struct FILEs*)pp)->num_bytes_rd = len1;
		((struct FILEs*)pp)->status = 1;
		printf("In mysvd status input file read: %d\n", ((struct FILEs*)pp)->status);
		printf("blu.bin len1 = %d \n",len1);
		len1 = sizeof(float *) * m + sizeof(float) * n * m;
		len2 = sizeof(float *) * n + sizeof(float) * m * n;
		len3 = sizeof(float *) * p + sizeof(float) * p * q;
		len4 = sizeof(int *) * n + sizeof(int) * m * n;
		printf("len = %d len2 = %d len3 = %d len4 = %d\n",len1, len2,len3,len4);
		printf("setting up ptrs with malloc\n");
		setup_memory();
		/*
		for(i=0;i<m;i++) {
			for(j=0;j<n;j++) {
				ppa[i][j]=(float)*inbuf;
				printf("%d %d %5.1f \n",i,j,ppa[i][j]);
				inbuf++;
			}
		}
		*/ 
//free_mem();
	}

 
}

int main() {
	inbuf = (int *)malloc(sizeof(int)*m*n);
	inbuffr = inbuf;
	
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
    pthread_create(&tid, NULL, hello, (void *)Allen);
    pthread_join(tid, NULL);
    
    pthread_create(&tidsvd1, NULL, mysvd, (void *)files1);
	pthread_join(tidsvd1, NULL);
	
	pthread_create(&tidsvd2, NULL, mysvd, (void *)files2);
	pthread_join(tidsvd2, NULL);
	
	pthread_create(&tidsvd3, NULL, mysvd, (void *)files3);
	pthread_join(tidsvd3, NULL);
	printf("all threads joined\n");
	sleep (5);

		printf("In main status %d num_bytes_rd %d \n",files1->status,files1->num_bytes_rd);
		printf("In main status %d num_bytes_rd %d \n",files2->status,files2->num_bytes_rd);
		printf("In main status %d num_bytes_rd %d \n",files3->status,files3->num_bytes_rd);
	
    /*
    free(files1);
    free(files2);
    free(files3);
    */
    free(Allen);
    
    return 0;
}
