#include <stdio.h>

int main() {

    int arr[20], n, i, min, max, mid, goal, found = 0, index;

    printf("size of array : ");
    scanf("%d", &n);

    min = 0;
    max = n;

    for(i = 0;  i < n; i++) {
        printf("element %d of array : ", i);
        scanf("%d", &arr[i]);
    }

    printf("find : ");
    scanf("%d", &goal);

    while (found == 0 && min <= max) {

        mid = (min + max) / 2;
        if (goal > arr[mid]) {
            min = mid + 1;
        }
        else {
            if (goal == arr[mid]) {
                found = 1;
                index = mid;
            }
            max = mid - 1;
        }
    }

    if(found == 1) {
        printf("Found in index of %d.", index);
    }
    else
        printf("Not found.");

    return 0;
}