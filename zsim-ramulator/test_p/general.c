#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "/root/ndp_hooks.h"

#define A_ARRAY_SIZE 4096 
#define B_ARRAY_SIZE 1024 
#define C_ARRAY_SIZE 2048 
#define D_ARRAY_SIZE 512 

typedef struct Inputs{
    int *a;
    int *b;
}Input;

void *func_offload1(void *argv){
    ndp_roi_begin(); 
    ndp_func_begin(); 
    ndp_set_monitoring_thread();
    Input* in = (Input*) argv;
    int *a = in->a;
    ndp_point(2<<26);
    int *b = in->b;
    int *c = (int *) malloc(sizeof(int)*B_ARRAY_SIZE);
    for(int i = 0 ; i < 150 ; i++){
        if((i %50)==0){
            a[i] = b[i]*c[i];
        } else {
            c[i] = a[i] * b[i];
        }
        sleep(0.001);
    }

    ndp_func_end(); 
    ndp_roi_end(); 
}
void *func_offload2(void *argv){
    ndp_roi_begin(); 
	ndp_func_begin(); 
    ndp_set_monitoring_thread();
    ndp_point(5<<26);
    Input* in = (Input*) argv;
    int *a = in->a;
    int *b = in->b;
    int *c = (int *) malloc(sizeof(int)*B_ARRAY_SIZE);
    for(int i = 0 ; i < 250 ; i++){
        if((i %60)==0){
            a[i] = b[i]*c[i];
        } else {
            c[i] = a[i] * b[i];
        }
        sleep(0.001);
    }
    ndp_func_end(); 
    ndp_roi_end(); 

}

void *func_offload3(void *argv){
    ndp_roi_begin(); 
	ndp_func_begin(); 
    ndp_set_monitoring_thread();
    Input* in = (Input*) argv;
    ndp_point(27<<26);
    int *a = in->a;
    int *b = in->b;
    int *c = (int *) malloc(sizeof(int)*B_ARRAY_SIZE);
    for(int i = 0 ; i < 100 ; i++){
        if((i %20)==0){
            a[i] = b[i]*c[i];
        } else {
            c[i] = a[i] * b[i];
        }
        sleep(0.001);
    }
    ndp_func_end(); 
    ndp_roi_end(); 


}

void *func_offload4(void *argv){
    ndp_roi_begin(); 
	ndp_func_begin(); 
    ndp_set_monitoring_thread();
    Input* in = (Input*) argv;
    int *a = in->a;
    int *b = in->b;
    ndp_point(2<<26);
    int *c = (int *) malloc(sizeof(int)*B_ARRAY_SIZE);
    for(int i = 0 ; i < 500 ; i++){
        if((i %30)==0){
            a[i] = b[i]*c[i];
        } else {
            c[i] = a[i] * b[i];
        }
        sleep(0.001);
    }
    ndp_func_end(); 
    ndp_roi_end(); 

}

int main(){
    ndp_start_analysis();
    int thread_count = 10;
    pthread_t **threads = (pthread_t **) malloc(sizeof(pthread_t*)*thread_count);
    Input ** threads_input = (Input **) malloc(sizeof(Input*)*thread_count);
    for(int i = 0 ; i < thread_count; i++){
        threads[i] = (pthread_t*) malloc(sizeof(pthread_t));
        threads_input[i] = (Input*) malloc(sizeof(Input));
    }

    int * a = (int *) malloc(sizeof(int) * A_ARRAY_SIZE);
    int * b = (int *) malloc(sizeof(int) * B_ARRAY_SIZE);
    int * c = (int *) malloc(sizeof(int) * C_ARRAY_SIZE);
    int * d = (int *) malloc(sizeof(int) * D_ARRAY_SIZE);
    
    threads_input[0]->a = a;
    threads_input[0]->b = c;

    threads_input[1]->a = b;
    threads_input[1]->b = c;

    threads_input[2]->a = b;
    threads_input[2]->b = a;

    threads_input[3]->a = a;
    threads_input[3]->b = b;
    
    int thread_index = 0;
    pthread_create(threads[1], NULL, func_offload2, (void *) threads_input[1]);
    pthread_create(threads[2], NULL, func_offload3, (void *) threads_input[2]);
    pthread_create(threads[3], NULL, func_offload4, (void *) threads_input[3]);


    for(int i = 0; i<1000;i++){
        if(i == 10){
            pthread_create(threads[thread_index], NULL, func_offload1, (void *) threads_input[0]);
            thread_index++;
        }

        if (i == 35){
            pthread_create(threads[thread_index], NULL, func_offload3, (void *) threads_input[2]);
            thread_index++;

        }

        if (i == 5){
            pthread_create(threads[thread_index], NULL, func_offload2, (void *) threads_input[1]);
            thread_index++;

        }
        if (i == 500){
            pthread_create(threads[thread_index], NULL, func_offload2, (void *) threads_input[1]);
            thread_index++;

        }
        if (i == 530){
            pthread_create(threads[thread_index], NULL, func_offload3, (void *) threads_input[2]);
            thread_index++;

        }
        if (i == 350){
            pthread_create(threads[thread_index], NULL, func_offload1, (void *) threads_input[0]);
            thread_index++;

        }
        if (i == 100){
            pthread_create(threads[thread_index], NULL, func_offload3, (void *) threads_input[2]);
            thread_index++;

        }
        if (i == 1000){
            pthread_create(threads[thread_index], NULL, func_offload1, (void *) threads_input[0]);
            thread_index++;

        }
        if(i%2== 0){
            a[i] = b[i] * c[i];
        }
        if(i%3== 0){
            b[i] = d[i] + c[i];
        }
        if(i%13== 0){
            d[i] = d[i] + c[i];
        }
        if(i%5== 0){
            c[i] = a[i] + c[i];
        }
        sleep(0.001);
    }
    for(int i = 0 ; i < thread_index; i++){
        pthread_join(*threads[i], NULL);
    }
	ndp_end_analysis();

ndp_start_analysis();

    threads = (pthread_t **) malloc(sizeof(pthread_t*)*thread_count);
    threads_input = (Input **) malloc(sizeof(Input*)*thread_count);
    for(int i = 0 ; i < thread_count; i++){
        threads[i] = (pthread_t*) malloc(sizeof(pthread_t));
        threads_input[i] = (Input*) malloc(sizeof(Input));
    }

    a = (int *) malloc(sizeof(int) * A_ARRAY_SIZE);
    b = (int *) malloc(sizeof(int) * B_ARRAY_SIZE);
    c = (int *) malloc(sizeof(int) * C_ARRAY_SIZE);
    d = (int *) malloc(sizeof(int) * D_ARRAY_SIZE);
    
    threads_input[0]->a = a;
    threads_input[0]->b = c;

    threads_input[1]->a = b;
    threads_input[1]->b = c;

    threads_input[2]->a = b;
    threads_input[2]->b = a;

    threads_input[3]->a = a;
    threads_input[3]->b = b;
    
    thread_index = 0;
    pthread_create(threads[1], NULL, func_offload2, (void *) threads_input[1]);
    pthread_create(threads[2], NULL, func_offload3, (void *) threads_input[2]);
    pthread_create(threads[3], NULL, func_offload4, (void *) threads_input[3]);


    for(int i = 0; i<1000;i++){
        if(i == 10){
            pthread_create(threads[thread_index], NULL, func_offload1, (void *) threads_input[0]);
            thread_index++;
        }

        if (i == 35){
            pthread_create(threads[thread_index], NULL, func_offload3, (void *) threads_input[2]);
            thread_index++;

        }

        if (i == 5){
            pthread_create(threads[thread_index], NULL, func_offload2, (void *) threads_input[1]);
            thread_index++;

        }
        if (i == 500){
            pthread_create(threads[thread_index], NULL, func_offload2, (void *) threads_input[1]);
            thread_index++;

        }
        if (i == 530){
            pthread_create(threads[thread_index], NULL, func_offload3, (void *) threads_input[2]);
            thread_index++;

        }
        if (i == 350){
            pthread_create(threads[thread_index], NULL, func_offload1, (void *) threads_input[0]);
            thread_index++;

        }
        if (i == 100){
            pthread_create(threads[thread_index], NULL, func_offload3, (void *) threads_input[2]);
            thread_index++;

        }
        if (i == 1000){
            pthread_create(threads[thread_index], NULL, func_offload1, (void *) threads_input[0]);
            thread_index++;

        }
        if(i%2== 0){
            a[i] = b[i] * c[i];
        }
        if(i%3== 0){
            b[i] = d[i] + c[i];
        }
        if(i%13== 0){
            d[i] = d[i] + c[i];
        }
        if(i%5== 0){
            c[i] = a[i] + c[i];
        }
        sleep(0.001);
    }
    for(int i = 0 ; i < thread_index; i++){
        pthread_join(*threads[i], NULL);
    }
	ndp_end_analysis();

}
