#include <stdio.h>

int main() {

    int arr[20], n, i, ins;

    printf("size of arr[] : ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("element %d of arr[] : ", i);
        scanf("%d", &arr[i]);
    }

    printf("insert element : ");
    scanf("%d", &ins);

    i = n;
    while(ins < arr[i-1] && i != 0) {
        arr[i] = arr[i-1];
        i--;
    }

    arr[i] = ins;
    
    for(i = 0; i < n+1; i++) {
        printf("%d ",arr[i]);
    }

    return 0;
}

