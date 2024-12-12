#include <stdio.h>
#include <time.h>

int main()
{
    int apsis, i, j, k, m, n, blocknum, rotation, rot[3][3], temp[3][3], x, y, sif, sif0, sif1, sif2, game, score, hscore = 0, clear, fin = 0;
    char matrix[100][50];

    srand(time(NULL));

    // Oyunda Kullanilacak Bloklarin Kolay Erişilebilmesi İcin Tanimli Olduğu 3 Boyutlu Dizi
    int block[9][3][3] = {{{0, 0, 0}, {0, 1, 0}, {0, 0, 0}},  // 1-I
                          {{0, 0, 0}, {0, 1, 0}, {0, 1, 0}},  // 2-I
                          {{0, 0, 0}, {1, 1, 0}, {1, 1, 0}},  // O
                          {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}},  // I-H
                          {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},  // I-V
                          {{0, 1, 0}, {0, 1, 0}, {0, 1, 1}},  // L
                          {{0, 1, 0}, {0, 1, 0}, {1, 1, 0}},  // J
                          {{0, 0, 0}, {0, 1, 1}, {1, 1, 0}},  // S
                          {{0, 0, 0}, {1, 1, 0}, {0, 1, 1}}}; // Z

    while (fin == 0) // Menu Ekraninda 0'a Basilma Kontrolu
    {
        printf("\n|''||''| '||''''| |''||''| '||'''|, |''||''| .|'''|  \n   ||     ||   .     ||     ||   ||    ||    ||      \n   ||     ||'''|     ||     ||...|'    ||    `|'''|, \n   ||     ||         ||     || \\\\      ||     .   || \n  .||.   .||....|   .||.   .||  \\\\. |..||..|  |...|' \n"); // Oyun Menusu Başlik
        printf("\t1 - Oyuna Basla\n\t2 - En Yuksek Puani Goster\n\t3 - Emegi Gecenler\n\n\tOyundan cikmak icin oyun icerisinde 0'a basabilirsiniz.");                                                                                                                                                      // Oyun Menusu
        do                                                                                                                                                                                                                                                                                                 // Oyun Menusu Kullanici Girdisi
        {
            scanf("%d", &game);
        } while (game < 0 || game > 3); // Gecerli Girdi Kontrolu
        if (game == 0)                  // Menu Ekraninda 0'a Basilma Kontrolu
        {
            fin = 1;
        }

        if (game == 1) // Menuden Oyuna Giris
        {
            printf("Oyun Alani Boyutlarini Once Satir Sonra Sutun Olmak Uzere Giriniz :");
            do
            {
                scanf("%d%d", &n, &m);
            } while (m < 0 || m > 100 || n < 0 || n > 50); // Gecerli Girdi Kontrolu

            for (i = 0; i < m; i++) // Oyun Alani Matris Sifirlama
            {
                for (j = 0; j < n; j++)
                {
                    matrix[i][j] = 0;
                }
                score = 0;
            }
        }
        while (game == 1) // Menuden Oyuna Giriş
        {

            blocknum = rand() % 9; // Rastgele Parca Secme
            // printf("%d", blocknum);
            if (blocknum == 0 || blocknum == 1)
            {blocknum = 5;}
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    rot[i][j] = block[blocknum][i][j]; // Veri Kaybetmemek İcin Farkli Bir Diziye Kopyaliyorum
                }
            }

            printf("\n\n\tOyun Alani: \n\n\t");

            for (i = 1; i <= m; i++)
            {
                printf("%d\t", i);
            }

            for (i = 0; i < m; i++)
            {
                clear = 0;
                printf("\n");
                for (j = 0; j < n; j++)
                {
                    clear += matrix[i][j];

                    if (clear == n) // blok silme
                    {
                        for (i = i; i > 0; i--)
                        {
                            for (j = 0; j < n; j++)
                            {
                                matrix[i][j] = matrix[i - 1][j];
                            }
                        }
                        score += 100;
                        if (score > hscore)
                        {
                            hscore = score;
                        }
                        printf("\n");
                    }

                    printf("\t%d", matrix[i][j]);
                }

                printf("\n");
            }
            printf("\n");

            printf("\nKullanilacak Parca:\n");
            for (i = 0; i < 3; i++) // Rastgele Parca Yazdirma
            {
                for (j = 0; j < 3; j++)
                {

                    printf("\t%d", block[blocknum][i][j]);
                }
                printf("\n\n");
            }

            do // Parca Döndurme
            {
                printf("\t1 - Devam\n\t2 - Saat Yonunde Dondur\n\t3 - Saat Yonunun Tersinde Dondur");

                do
                {
                    scanf("%d", &rotation);
                } while (rotation < 0 || rotation > 3);

                if (rotation == 0)
                {
                    game = 0; // Oyunu Bitir
                    printf("\n\n\tOyun Sonlandirilacaktir...\n\n");
                }

                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        temp[i][j] = rot[i][j]; // Veri Kaybetmemek İcin Farkli Bir Diziye Kopyaliyorum
                    }
                }
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {

                        if (rotation == 1) // Devam Etme
                        {
                            x = i;
                            y = j;
                        }
                        else if (rotation == 2) // Saat Yönu Döndurme
                        {
                            x = 2 - j;
                            y = i;
                        }
                        else if (rotation == 3) // Saat Yönunun Tersi Döndurme
                        {
                            x = j;
                            y = 2 - i;
                        }

                        rot[x][y] = temp[i][j]; // Gecici Değer uzerinden Parcanin Döndurulmesi
                    }
                }

                printf("\n");
                if (rotation != 1)
                {
                    for (i = 0; i < 3; i++)
                    {
                        for (j = 0; j < 3; j++)
                        {

                            printf("\t%d", rot[i][j]); // Döndurulen Parcanin Yazdirilmasi
                        }
                        printf("\n\n");
                    }
                }

            } while (rotation != 1); // Devam Kontrolu

            printf("Puan: %d\t\t\n\tBlogun sol ucunu yerlestirmek istediginiz x-koordinatini seciniz : ", score);
            do
            {
                scanf("%d", &apsis);
            } while (apsis < 0 || apsis > n); // Gecerli Girdi Kontrolu

            if (apsis == 0)
            {
                game = 0; // Oyunu Bitir
                printf("\n\n\tOyun Sonlandirilacaktir...\n\n");
            }
            printf("\n\t");

            sif0 = m, sif1 = m, sif2 = m;

            for (i = 2; i >= 0; i--) // Blok Matrisinin En Altindaki 0 Adetini Bulma
            {
                if (rot[i][0] == 1 && sif0 == m)
                {
                    sif0 = i;
                }

                if (rot[i][1] == 1 && sif1 == m)
                {
                    sif1 = i;
                }

                if (rot[i][2] == 1 && sif2 == m)
                {
                    sif2 = i;
                }
            }

            i = 1;
            while (matrix[i][apsis - 1] == 0 && i < m) // Matriste Blogun Yerleşeceği İlk Satir Boş Alan Kontrolu
            {
                i++;
            }
            sif0 = i - sif0; // Blok Matrisindeki İlk Sutun Dikkate Alinarak Blogun Ekrana Yazdirilabilecegi Satir
            if (i == 1)
            {
                game = 0; // Oyunu Kaybetme
                printf("Oyun bitti. Puaniniz: %d", score);
            }
            i = 1;
            while (matrix[i][apsis] == 0 && i < m) // Matriste Blogun Yerleşeceği İkinci Satir Bos Alan Kontrolu
            {
                i++;
            }
            sif1 = i - sif1; // Blok Matrisindeki İkinci Sutun Dikkate Alinarak Blogun Ekrana Yazdirilabilecegi Satir
            if (i == 1)
            {
                game = 0; // Oyunu Kaybetme
                printf("Oyun bitti. Puaniniz: %d", score);
            }
            i = 1;
            while (matrix[i][apsis + 1] == 0 && i < m) // Matriste Blogun Yerleşeceği Ucuncu Satir Bos Alan Kontrolu
            {
                i++;
            }
            sif2 = i - sif2; // Blok Matrisindeki Ucuncu Sutun Dikkate Alinarak Blogun Ekrana Yazdirilabilecegi Satir

            if (i == 1)
            {
                game = 0; // Oyunu Kaybetme
                printf("Oyun bitti. Puaniniz: %d", score);
            }

            if (sif0 > sif1)
            {
                sif = sif1;
            }
            else
            {
                sif = sif0;
            }
            if (sif2 < sif)
            {
                sif2 = sif;
            }

            for (i = 0; i < 3; i++)
            { // Blok Yerleştirme
                for (j = 0; j < 3; j++)
                {
                    matrix[sif + i -  1][apsis + j - 1] += rot[i][j];
                }
            }
        }
        if (game == 2) // Menuden En Yuksek Puan Gösterme
        {
            printf("En Yuksek Puan: %d", hscore);
        }
        if (game == 3)
        {
            printf("\n\t Yapan:\t Ozgur Agar\n\n\tYapilma Talimati Veren:\tM. Amac Guvensan\n\n");
        }
    }
    return 0;
}
