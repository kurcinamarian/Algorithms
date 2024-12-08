#include <stdio.h>

int find_max(int n,int arr[]){
    int max = 0;
    for(int i=0; i<n; i++){
        if(arr[i]>max){
            max = arr[i];
        }
    }
    return max;
}

void schedule(int n, int deadline[], int jobs_inx[],int jobs_profit[]){
    int i=0;
    int max_deadline = find_max(n,deadline);
    int jobs_sequence[max_deadline];
    int total_profit = 0;
    for (int i = 0; i < max_deadline; ++i){
        jobs_sequence[i] = 0;
    }
    for(i; i<n; i++){
        if(jobs_sequence[deadline[i]-1]==0){
            jobs_sequence[deadline[i]-1] = jobs_inx[i];
            total_profit += jobs_profit[i];
        }
        else{
            int j = deadline[i]-1;
            while(j>0){
                j--;
                if(jobs_sequence[j]==0){
                    jobs_sequence[j] = jobs_inx[i];
                    total_profit += jobs_profit[i];
                    break;
                }
            }
        }
    }
    printf("Best job sequence:\n");
    for(i=0; i<max_deadline; i++){
        if(jobs_sequence[i]!=0){
            printf("%d\n", jobs_sequence[i]);
        }
    }
    printf("Total Profit: %d\n", total_profit);
}

int main(){
    int jobs_inx[] = {1,2,3,4,5,6,7};
    int deadline[] = {2,4,3,2,3,1,3};
    int jobs_profit[] = {40,15,60,20,10,45,55};
    int n =sizeof(jobs_inx)/sizeof(jobs_inx[0]);
    int i = 0;
    for(i=0; i<n-1; i++){
        for(int j=i+1; j<n; j++){
            if(jobs_profit[i]<jobs_profit[j]){
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
    schedule(n,deadline,jobs_inx,jobs_profit);  
}