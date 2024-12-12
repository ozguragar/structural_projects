#include <stdio.h>

int main() {

    int arr[20], n, i, del, j;

    printf("size of arr[] : ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("element %d of arr[] : ", i);
        scanf("%d", &arr[i]);
    }

    printf("delete element : ");
    scanf("%d", &del);

    i = 1;
    while(i <= n && arr[i] < del){
        i++;
    }
    if(del == arr[i]){
        for(j = i; j < n-1; j++){
            arr[j] = arr[j+1];
        }
    }

    for(i = 0; i < n-1; i++) {
        printf("%d ",arr[i]);
    }

    return 0;
}

