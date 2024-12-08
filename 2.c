#include <stdio.h>
#include <stdlib.h>

//ste struct 
struct job {
    struct job* parent;  
    int depth;    
    int smallest; 
};

//array of sets
struct job* U;

//function to make a set
void makeset(int i) {
    U[i].parent = &U[i];  
    U[i].depth = 0;    
    U[i].smallest = i; 
}

//find the the latest place to schedule
int small(int i) {
    //get to the root
    if (U[i].parent != &U[i]) {
        U[i].parent = &U[small(U[i].parent - U)];
    }
    return U[i].parent->smallest;
}

//merge two sets
void merge(int P, int Q) {
    struct job* rootP = U[P].parent;
    struct job* rootQ = U[Q].parent;

    if (rootP != rootQ) {
        rootQ->parent = rootP;
        if (rootQ->smallest < rootP->smallest) {
            rootP->smallest = rootQ->smallest;
        }
    }
}

//find maximum deadline
int find_max(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main() {
    int jobs_inx[] = {1,2,3,4,5,6,7};
    int deadline[] = {2,4,3,2,3,1,1};
    int jobs_profit[] = {40,15,60,20,10,45,55};
    //find the number of jobs
    int n = sizeof(jobs_inx)/sizeof(jobs_inx[0]);
    //sort jobs based on profit
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (jobs_profit[i] < jobs_profit[j]) {
                int temp_profit = jobs_profit[i];
                jobs_profit[i] = jobs_profit[j];
                jobs_profit[j] = temp_profit;

                int temp_deadline = deadline[i];
                deadline[i] = deadline[j];
                deadline[j] = temp_deadline;

                int temp_jobs_inx = jobs_inx[i];
                jobs_inx[i] = jobs_inx[j];
                jobs_inx[j] = temp_jobs_inx;
            }
        }
    }
    //find the maximum deadline
    int max_deadline = find_max(deadline, n);
    //initialize the array of  sets to represent all posible places for scheduling jobs
    U = (struct job*)malloc((max_deadline + 1) * sizeof(struct job));
    //make sets
    for (int i = 0; i <= max_deadline; i++) {
        makeset(i);
    }
    //array for schedule
    int schedule[max_deadline + 1];
    for (int i = 0; i <= max_deadline; i++) {
        schedule[i] = -1;
    }
    int total_profit = 0;
    //try to schedule all jobs
    for (int j = 0; j < n; j++) {
        //find the lattest time to schedule 
        int available_time = small(deadline[j]);
        //if its larger then 0 it is possible to schedule
        if (available_time > 0) {
            schedule[available_time] = jobs_inx[j];
            //merge sets
            merge(available_time, available_time - 1);
            total_profit += jobs_profit[j];
        }
    }
    
    printf("Best job sequence:\n");
    for (int i = 1; i <= max_deadline; i++) {
        if (schedule[i] != -1) {
            printf("%d\n", schedule[i]);
        }
    }
    printf("Total Profit: %d\n", total_profit);
    free(U);
    return 0;
}
