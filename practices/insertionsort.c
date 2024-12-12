#include <stdio.h>

int main()
{
    int i, j, n, min, tmp, arr[20];

    // input
    printf("Dizi Boyutu :\t");
    scanf("%d", &n);
    printf("Dizi Elemanlari :\t");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    // sort
    for (i = 0; i < n; i++)
    {
        min = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min])
            {
                min = j;
            }
        }
        tmp = arr[i];
        arr[i] = arr[min];
        arr[min] = tmp;
    }

    // output
    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}