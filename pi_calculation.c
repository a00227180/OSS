/* 
    Link to the graph: https://docs.google.com/document/d/1Fk7t5A5I7RXQbY7RUeEZfj48ekqlb2LNHli8H4IPW_U/edit?usp=sharing
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_RESET    "\x1b[0m"

double points = 10000;
double circle_count = 0;

int threads_number;                       
int thread_work;                                                     
unsigned int seed;                         


double generateNumber();
void * monte_carlo_method(void *thread_id);

int main(int argc, const char* argv[]){
    //clock_t start, end;
    seed = time(NULL);
    if(argc != 2){
        printf(ANSI_COLOR_RED "Error:" ANSI_COLOR_RESET "Invalid number of arguments\n");
        exit(1);
    }
    threads_number = atoi(argv[1]);
    if(threads_number == 0){
            printf(ANSI_COLOR_RED "Error:" ANSI_COLOR_RESET " number of threads must be greater than 0\n");
            exit(1);
        }
    
    double  result;
    void    *count_inside_thread;
    int     thread_creation;
   
    pthread_t threads[threads_number];                 
    thread_work = points / threads_number;
    
    unsigned int i;
    //start = clock();
    for(i=0; i<threads_number; i++){
        thread_creation = pthread_create(&threads[i], NULL, monte_carlo_method, (void *)(intptr_t) i);
        seed++;
    }
    for(i=0; i<threads_number; i++){
        pthread_join(threads[i], &count_inside_thread);
        circle_count += * (long *) count_inside_thread;
    }

    result = (4.00 * circle_count) / points;
    //end = clock();
    //double time = (double) (end - start) / CLOCKS_PER_SEC;
    //printf("time %f",time)
    printf("%f\n",result);
    exit(0);
}

double generateNumber(){
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX + 1);
}

void *monte_carlo_method(void *thread_id){
    double x, y;
    int *count_in = (int *)malloc(sizeof(int));
    *count_in = 0; 
    for(int i=0; i<thread_work; i++){
        x = generateNumber();
        y = generateNumber();
        if(sqrt(pow((x-.5),2)+ pow((y-.5),2)) <= .5)
            *count_in += 1;
    }
    pthread_exit((void *)count_in);
}