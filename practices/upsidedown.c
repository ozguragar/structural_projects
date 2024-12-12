#include <stdio.h>

int main(){
    int i, j, n, m, tmp, arr[50][50];

    //input
    scanf("%d %d", &n, &m);
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            scanf("%d", &arr[i][j]);
        }
    }

    //code
    for(i = 0; i < n/2; i++){
        for(j = 0; j < m; j++){
            tmp = arr[i][j];
            arr[i][j] = arr[n-i-1][m-j-1];
            arr[n-i-1][m-j-1] = tmp;
        }
    }

    for(i = 0; i < n; i++){
        printf("\n");
        for(j = 0; j < m; j++){
            printf("%d", arr[i][j]);
        }
    }
    return 0;
}