#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_t tidsvd1,tidsvd2,tidsvd3;

struct args {
    char* name;
    int age;
};


struct FILEs {
	char *input_file;
	char *first_output;
	char *second_output;
};

void *hello(void *input) {
    printf("name: %s\n", ((struct args*)input)->name);
    printf("age: %d\n", ((struct args*)input)->age);
}


void *mysvd(void *pp) {
	pthread_t id = pthread_self();
	if(pthread_equal(id,tidsvd1)) {
		printf("\n 1st thread processing\n");
		printf("In mysvd input_file: %s\n", ((struct FILEs*)pp)->input_file);
		printf("In mysvd first_output: %s\n", ((struct FILEs*)pp)->first_output);
		printf("In mysvd second_output: %s\n", ((struct FILEs*)pp)->second_output);
		printf("\n");
	}
	
	if(pthread_equal(id,tidsvd2)) {
		printf("\n 2nd thread processing\n");
		printf("In mysvd input_file: %s\n", ((struct FILEs*)pp)->input_file);
		printf("In mysvd first_output: %s\n", ((struct FILEs*)pp)->first_output);
		printf("In mysvd second_output: %s\n", ((struct FILEs*)pp)->second_output);
		printf("\n");
	}

	if(pthread_equal(id,tidsvd3)) {
		printf("\n 3rd thread processing\n");
		printf("In mysvd input_file: %s\n", ((struct FILEs*)pp)->input_file);
		printf("In mysvd first_output: %s\n", ((struct FILEs*)pp)->first_output);
		printf("In mysvd second_output: %s\n", ((struct FILEs*)pp)->second_output);
		printf("\n");
	}

 
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

    printf("In main %s %s %s \n",files1->input_file,files1->first_output,files1->second_output);
    printf("In main %s %s %s \n",files2->input_file,files2->first_output,files2->second_output);
    printf("In main %s %s %s \n",files3->input_file,files3->first_output,files3->second_output);

    
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
    
    free(files1);
    free(files2);
    free(files3);
    free(Allen);
    
    return 0;
}
