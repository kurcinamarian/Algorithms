 #include <stdio.h>

//queue
int heap[100];
int size;

//add file size to queue
void add(int file_size)
{
    int i = size;
    size++;
    //heapify the queue
    while (i <= size && i > 0 && heap[(i - 1)] > file_size)
    {
        heap[i] = heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap[i] = file_size;
}

//get minimum from queue
int get_min()
{
    int minNode = heap[0];
    size--;
    //heapify the queue
    if (size > 0)
    {
        int lastNode = heap[size];
        int i = 0;
        while (i * 2 + 1 < size)
        {
            int child = 2 * i + 1;
            if (child + 1 < size && heap[child + 1] < heap[child])
                child++;
            if (lastNode <= heap[child])
                break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = lastNode;
    }
    return minNode;
}

int main(){
    int file_sizes[] = {1,2,3,4,5,6,7,456,5132};
    //get number of files
    int n =sizeof(file_sizes)/sizeof(file_sizes[0]);
    size = 0;
    //add to queue
    for(int i=0; i<n; i++){
        add(file_sizes[i]);
    }
    //merge files until only one remains in the queue
    for(int i=n;1<i;i--){
        //get two smallest files from the queue
        int first_file = get_min();
        int second_file = get_min();
        //merge them and add the result back to the queue
        add(first_file+second_file);
        printf("Merged %d and %d, resulting in %d\n", first_file, second_file, first_file+second_file);
    }
    //print result
    int final_file = get_min();
    printf("%d", final_file);
}