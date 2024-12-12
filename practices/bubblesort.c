#include <stdio.h>

int main()
{
    int i, j, n, arr[20], tmp;

    // input
    printf("Dizi Boyutu :\t");
    scanf("%d", &n);
    printf("Dizi Elemanlari :\t");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    // sort

    for (j = 0; j < n; j++)
    {
        for (i = 0; i < n - 1; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
            }
        }
    }

    // output
    for (i = 0; i < n; i++)
    {
        printf("%d", arr[i]);
    }
    return 0;
}