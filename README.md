# soal-shift-sisop-modul-4-A06-2021

**Anggota kelompok**:

```
- Richard Asmarakandi       05111940000017
- Muhammad Rafki Mardi      05111940000054
- Husin Muhammad Assegaff   05111940000127
```

---

**[Source Code](https://github.com/husinassegaff/soal-shift-sisop-modul-4-A06-2021/blob/main/SinSeiFS_A06.c)**

## Fungsi Dasar
**Berikut beberapa fungsi dasar yang bakal sering digunakan,**

1. getFile()
```CPP
char *getFile(char *old_name){
    char *e = strrchr (old_name,'/');
    if (e == NULL)
        e = "";
    return e;
}
```

Fungsi ini digunakan untuk mencari lokasi karakter terakhir dari string `old_name` yang berupa `/`

2. getExt()
```CPP
char *getExt(char *old_name){
    char *e = strrchr (old_name,'.');
    if (e == NULL)
        e = "";
    return e;
}
```

Fungsi ini dipakai untuk memastikan adanya ekstensi file dengan melihat dari lokasi karakter terakhir dari string `old_name` yang berbentuk `.`

3. getName()
```CPP
char *getName(char *old_name){
    int nameLen;
    int extLen;
    int len;

    nameLen = strlen(old_name);
    extLen =  strlen(getExt(old_name));
    len = (nameLen-extLen);
    
    char *name = (char *) malloc(len);


    for(int i = 0;i<len;i++){
        name[i]=old_name[i];
    }

    strcpy(old_name,name);

    free(name);
    
    return old_name;
}
```

Fungsi ini digunakan untuk memperbarui isi dari string `old_name`

4. enFilesRecusively()

Dalam modul 4 digunakan libfuse (file system in userspace) yang kebanyakan mengadopsi fungsi [pada laman berikut](https://libfuse.github.io/doxygen/example_2passthrough_8c_source.html)


```CPP
void enFilesRecursively(char *basePath){
    int res;
    char fpath[1000],tpath[1000];
    char path[1000];
    char file[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;


    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(fpath, basePath);
            strcat(fpath, "/");
            strcat(fpath, dp->d_name);
            enFilesRecursively(fpath);

            strcpy(path,basePath);
            strcat(path,"/");

            strcpy(tpath,dp->d_name);
            strcpy(file,tpath);
            if(strstr(fpath,atozTag)!= NULL){
                printf("atoz\n");
                if(strstr(dp->d_name,atozTag)==NULL){
                    atoz(file);
                    strcat(path, file);
                    printf("%s\n%s\n",fpath,path);
                    res = rename(fpath, path);
                }
                if(res == -1) printf("%d\n",errno);
            }else if(strstr(fpath,rxTag)!= NULL){
                printf("rx\n");
                if(strstr(dp->d_name,rxTag)==NULL){
                    rxRnEn(file);
                    strcat(path, file);
                    printf("%s\n%s\n",fpath,path);
                    res = rename(fpath, path);
                }
                if(res == -1) printf("%d\n",errno);
            }else if(strstr(fpath,aisaTag)!= NULL){
                printf("aisa\n");
                if(strstr(dp->d_name,aisaTag)==NULL){
                    aIsaEn(file);
                    strcat(path, file);
                    printf("%s\n%s\n",fpath,path);
                    res = rename(fpath, file);
                }
                if(res == -1) printf("%d\n",errno);
            }

            char desc[100];
            sprintf(desc,"%s::%s",fpath,tpath);
            logFile("INFO","RENAME",desc);

        }
    }
    closedir(dir);
}
```

- Fungsi menerima sebuah `path` yang kemudian akan masuk kedalam loop `while` guna dilakukan rekursi.
- File `path` akan dibuka dengan `openddir()` guna looping file yang berada didalammnya.
- Fungsi dugunakan dalam melakukan `rename` dengan bantuan `if else` statement.
- `Tag(s)` berupa string yang menandakan enrkipsi apa yang harus dijalankan terhadap file tersebut.

## Soal 1

**Deskripsi:**\
Mengencode sebuah folder yang mengandung kata "Atoz\_" dengan atbash chiper

### Atbash Chiper + ROT 13

**Deskripsi:**\
Jika parameter mode bernilai 0, maka folder dienkripsi dengan Atbash chiper, yaitu mencerminkan sebuah huruf, awal menjadi akhir, awal+1 menjadi akhir-1. Contohnya A menjadi Z, B menjadi Y, C menjadi X dan seterusnya. Jika parameter mode bernilai 1, maka folder dienkripsi dengan ROT13. ROT 13 menggeser setiap huruf sebanyak 13 pergeseran ke kanan. apabila huruf telah sampai pada `z` maka pergeseran dilanjutkan dengan huruf kembali ke `a`. Misal kita akan menggeser huruf `o`. huruf `o` memiliki urutan ke-15 pada alphabet. Karena 15+13 adalah 28, sedangkan alphabet terakhir yaitu `z` memiliki urutan ke-26, maka urutan ke-27 akan kembali pada urutan ke-1 yaitu huruf `a` sehingga huruf ke-28 sama dengan huruf ke-28 dikurangi 26, yaitu huruf ke-2 atau huruf `b`.

**Pembahasan:**

```CPP
for(int i = 0; i < strlen(name);i++){
    if('a'<=name[i] && name[i]<='z'){
        if(mode == 0)
            name[i]= 'z' - name[i] + 'a';
        else
            name[i]= 'a' + (name[i] - 'a' + 13) % 26;
    }else if('A'<=name[i] && name[i]<='Z'){
        if(mode == 0)
            name[i]= 'Z' - name[i] + 'A';
        else
            name[i]= 'A' + (name[i] - 'A' + 13) % 26;
    }
}
```

- Kita loop dari huruf pertama hingga huruf terakhir sebanyak `strlen(name)` untuk mencerminkan setiap huruf pada nama folder.
- Setiap loop, kita cek apakah huruf tersebut adalah huruf kapital atau huruf kecil, karena jangkauan nilai ASCII keduanya berbeda.
- Jika parameter mode bernilai 0, maka kita mencerminkan setiap huruf pada nama folder (Atbash Chiper).
- Ide pencerminan yang kami pakai, adalah dengan mengurangkan huruf pada urutan terakhir yaitu `z` dengan huruf yang akan dicerminkan atau mudahnya kita mencari jarak antara huruf yang akan dicerminkan dengan huruf terakhir dari alphabet. Lalu jarak tersebut kita jumlahkan dengan huruf pada urutan paling awal yaitu `a`. dengan begitu huruf akan tercermin.
- Sebagai contoh kita ambil huruf `x`. Huruf `x` memiliki jarak 2 ke kiri dari huruf `z`. maka pencerminannya kita ambil huruf yang memiliki jarak 2 ke kanan dari huruf `a` yaitu, huruf `c`.
- Sedangkan jika parameter mode bernilai 1, maka kita mengenkripsi folder tersebut dengan ROT13.
- Idenya adalah dengan menambahkan huruf sekarang dengan angka 13. Namun karena ditakutkan akan melebihi batas alphabet, maka hasilnya kita modulo dengan 26 (Jumlah keseluruhan alphabet).

**Bukti :**

**Kesulitan :**

- Belum bisa mengimplementasikan pada saat mkdir.

## Soal 2

<!-- **2.A** -->

**Deskripsi:**\
Jika suatu direktori dilakukan `rename` yang pada penamaannya terdapat string awalan "RX\_" maka isi dari direktori akan dilakukan enkripsi berupa atbash + vigenere cipher.

### Atbash Chiper + vigenere cipher

**Deskripsi:**

- Atbash Chiper mengikuti cara yang sama dengan penjelasan nomor 1.
- Vinegere cipher merupakan metode enkripsi dengan dilakukanya pementaan suatu huruf kesebuah string yang merupakan kunci enkripsi, yang mana pada kasus ini adalah `sisop` dengan case sensitive dan tanpa dikenakan pada ekstensi file.

![Vinegere cipher](https://cdn.britannica.com/50/7850-050-219843C0/letter-plaintext-table-Vigenere-cipher-intersection-row.jpg)

- Pada contoh diatas, huruf `T` akan dipetakan terhadap kunci huruf `C` interseksi keduanya akan mendapatkan huruf `V` dan seterusnya.

- Enkripsi hanya dilakukan jika pada direktori tersebut dilakukan `rename` bukan `mkdir`

```c
// rename RX_ en
char *rxRnEn(char *old_name){
    return toVig(atBash(old_name,0),0);
}

// rename RX_ de
char *rxRnDe(char *old_name){
    return atBash(atBash(old_name,1),0);
}
```

- fungsi `rxRnEn()` berfungsi untuk menerima nama file yang akan dikenakan enkripsi atbash + vigenere cipher.
- fungsi `rxRnDe()` berfungsi sebaliknya.
- `atBash()` dengan mode `0` mengikuti penjelasan yang sama dengan nomor 1.

```c
// enkripsi RZ_ (mv atau rename)

char *toVig(char *old_name, int mode){

    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(ext,getExt(old_name));
    strcpy(name,getName(old_name));

    int nameLen = strlen(old_name);
    char keys[nameLen];

    key(keys,name);

    for(int i = 0; i<nameLen;i++){
        if(name[i]==keys[i]&&('a'>=name[i] && name[i]>='z')&&('A'>=name[i] && name[i]>='Z')){
            continue;
        }else if(mode == 0){
            if('a'<=name[i] && name[i]<='z'){
                name[i] = (name[i] + keys[i] - 2 * 'a') % 26 + 'a';
            }else if('A'<=name[i] && name[i]<='Z'){
                name[i] = (name[i] + keys[i] - 2 * 'A') % 26 + 'A';
            }
        }else if(mode == 1){
            if('a'<=name[i] && name[i]<='z'){
                name[i] = ((name[i] - keys[i] + 26)%26) + 'a';
            }else if('A'<=name[i] && name[i]<='Z'){
                name[i] = ((name[i] - keys[i] + 26)%26) + 'A';
            }
        }
    }

    strcpy(old_name,name);
    strcat(old_name,ext);

    free(name);
    free(ext);

    return old_name;
}
```

- fungsi `toVig()` akan mengolah string `old_name` menjadi string yang sudah dienkripsi dengan basis Vinegere cypher.
- fungsi `toVig()` memiliki dua mode, mode `0` untuk enkripsi dan mode `1` dekripsi.

```
Enkripsi:
for deHuruf in namaFile
    namaFile[i] = (deHuruf[i] + keyHuruf[i]) mod 26

Dekripsi:
for enHuruf in namaFile
    namaFile[i] = (enHuruf[i] - keyHuruf[i]) mod 26
```

```c
// pembuatan kunci toVig

char *key(char *keys, char *name){
    int i=0;
    char code[] = "sisop";

    while(name[i]){
        if('a'<=name[i] && name[i]<='z'){
            keys[i] = tolower(code[i%5]);
        }else if('A'<=name[i] && name[i]<='Z'){
            keys[i] = toupper(code[i%5]);
        }else{
            keys[i] = name[i];
        }
        i++;
    }
    return keys;
}
```

- fungsi `key()` akan mengambil paramenter berupa nama `file` serta string kosong `keys` yang memiliki panjang sama dengan string `file`.
- loop `while` akan mengisi keys dengan key yang case sensitive untuk membantu enkripsi dan dekripsi.
- jika sudah melakukan traverse sepanjang `file` maka akan dikembalikan keys ke `toVig()`.

**Bukti :**

**Kesulitan :**

- Belum bisa mengimplementasikan pada saat mkdir dan belum mengimplementasikan enkripsi saat mkdir.
- Belum bisa mengimplementasikan `link` guna partifile file.

## Soal 3

**Deskripsi:**\
Jika suatu direktori dilakukan `rename` yang pada penamaannya terdapat string awalan "A*is_a*" maka isi dari direktori akan dilakukan enkripsi dengan format

```
lowercase_namafile.ekstensi.desimal_dari_binary_casehuruf
```

**Deskripsi:**

- `lowercase_namafile` hanya meminta agar nama file dijadikan lowercase (tanpa ekstensi).
- `desimal_dari_binary_casehuruf` yang dimaksut adalah, setelah meruba string nama file menjadi lowercase, perubahan di huruf tertentu menandakan bilangan binary `1` pada posisi char tersebut, contoh:

```
mUlOk_senI.txt ---(tolower)---> mulok_seni.txt

mUlOk_senI
mulok_seni

0101000001  ---(bin to dec)---> 321
```

- Maka, jika file `mUlOk_senI.txt` dienkripsi, akan menjadi `mulok_seni.txt.321`.

```c
// mkdir atau renam A_is_a En
char *aIsaEn(char *old_name){
    return toLow(old_name);
}

// mkdir atau renam A_is_a De
char *aIsaDe(char *old_name){
    return toNor(old_name);
}
```

- Dalam penyelesaiaan masalah ini, dibuat sebuah fungsi `toLow()` untuk melakukan enrkipsi dan `toNor()` untuk melakukan dekripsi dengan funsi transisi `aisaEn()` dan `aisaDe()`.

```c
// enkripsi file folder A_is_a

char *toLow(char *old_name){
    int i = 0;

    char *bin = (char *) malloc(128);
    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(ext,getExt(old_name));
    strcpy(name,getName(old_name));

    while(name[i]){
        if('a'<=name[i] && name[i]<='z'){
            strcat(&bin[i],"0");
            name[i] = tolower(name[i]);
        }else if('A'<=name[i] && name[i]<='Z'){
            strcat(&bin[i],"1");
            name[i] = tolower(name[i]);
        }else{
            strcat(&bin[i],"0");
            name[i] = name[i];
        }
        i++;
    }

    sprintf(bin,".%d", decDif(bin));

    strcpy(old_name,name);
    strcat(old_name,ext);
    strcat(old_name,bin);

    free(name);
    free(ext);
    free(bin);

    return old_name;
}
```

- Variabel `bin` untuk menampung nilai binary dari beda case huruf
- Loop `while` pada fungsi akan menjalankan fungsi `tolower` terhadap char pada posisi tersebut. Pada saat itu juga, jika terdeteksi huruf besar, maka akan di-`strcat` ke `bin`.
- Kemudian akan dipanggin fungsi `decDif()`.

```c
// nilai desimal dari binary beda huruf

int decDif(char *bin){
    return (int) strtol(bin, 0, 2);
}
```

- Pada fungsi ini digunakan `strtol()` yang mengubah nilai string menjadi nilai `int` hingga bertemu parameter berupa `0` atau `NULL`.
- Nilai desimal akan dikembalikan ke fungsi `toLow()` dan komponen komponen string akan disatukan dengan `strcat` dan `strcpy` yang dilanjutkan dengan `free()`.

```c
// dekripsi file folder A_is_a

char *toNor(char *old_name){
    int i = 0;

    char *dec = (char *) malloc(128);
    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(dec,getExt(old_name));
    strcpy(ext,getExt(getName(old_name)));
    strcpy(name,getName(old_name));

    memmove(dec, dec+1, strlen(dec));

    dec = binDif(dec);

    dec = trueBin(dec,strlen(name));

    while(dec[i]){
        if(dec[i]=='1'){
            name[i] = toupper(name[i]);
        }else{
            name[i] = name[i];
        }
        i++;
    }

    strcpy(old_name,name);
    strcat(old_name,ext);

    free(name);
    free(ext);
    free(dec);

    return old_name;
}
```

- Fungsi `toNor()` digunakan untuk dekripsi folder `A_is_a_`.
- Mirip dengan `toLow()` namun bedanya tidak memanggil `decDif()` namun `binDif()` dan `trueBin()`.

```c
// nilai binary dari decimal beda huruf

char *binDif(char *dec){
   int decimal = atoi(dec);
   long bno=0,rem,f=1;

   while(decimal != 0){
      rem = decimal % 2;
      bno = bno + rem * f;
      f = f * 10;
      decimal = decimal / 2;
   }

   sprintf(dec,"%ld",bno);
   return dec;
}
```

- Fungsi `binDif()` menerima string berupa angka desimal.
- Argumen lalu di-_cast_ ke `int` dengan bantuna `atoi()`.
- variable `bno` menyimpan nilai biner.
- Loop `while` melakukan pembagian terhadap `decimal`, dalam pembagian 2, `rem` (reminder) menjadi acuan. Jika tidak habis, maka nilainya akan dikali `f` yang merupakan eksponensial 10 yang berfugsi sebagai traking posisi digit.

```c
char *trueBin (char *bin, int len){
    char *binary = (char *) malloc(128);
    int length = len - strlen(bin);

    while(length!=0){
        strcat(binary,"0");
        length--;
    }

    strcat(binary,bin);
    strcpy(bin,binary);

    free(binary);

    return bin;
}
```

- Fungsi `trueBin()` sendiri bertujuan untuk memperpanjang nilai binary, karna hasil binary dari `difBin()` belum mewakili seluruh huruf pada nama file.
- `trueBin()` menerima argumen berupa `bin` dan `len` yang berturut turut adalah nilai binary dan panjang nama file.
- Singkatnya mulai dari selisih panjang file dengan binary, di `strcat` char `0` ke variabel `binary` yang kemudian akan di salin ke `bin` guna dikembalikan sehingga menghasilkan nilai binary yang sesuai jumlah digitnya.
- Hasil `trueBin()` dan `binDif()` akan digunakan oleh `toNor()` dalam loop `while`.
- Jika `bin` bernilai `1` maka char pada saat itu akan dikenakan `toupper()`.
- Hasil `while` akan digabungakn dengan ekstensi lalu dikembalikan.

**Bukti :**

**Kesulitan :**

- Belum bisa mengimplementasikan pada saat mkdir.

## Soal 4

**Deskripsi:**\
Membuat log system yang akan mendata setiap system call yang digunakan. Tujuannya untuk memonitor kegiatan pada filesystem.

**Pembahasan:**

```CPP
void logFile(char *level, char *cmd,char *desc) {
    FILE *fp = fopen(logpath, "a");
    time_t t;
    struct tm *tmp;
    char tmBuff[100];
    time(&t);
    tmp = localtime(&t);
    strftime(tmBuff, sizeof(tmBuff), "%y%m%d-%H:%M:%S", tmp);

    fprintf(fp, "%s::%s::%s::%s", level, tmBuff, cmd, desc);
    fprintf(fp, "\n");

    fclose(fp);
}
```

- Pada fungsi void ini terdapat variabel t_time untuk menunjukkan waktu pada log. Kemudian, struct tm yang diguanakn untuk menunjukkan waktu sekarang
- Kemudian untuk format loggingnya jika tujuan logging diketahui / tidak bernilai NULL adalah `[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]` yang pada fungsi kami dibuat menjadi `fprintf(fp, "%s::%s::%s::%s", level, tmBuff, cmd, desc);`

**Bukti :**

**Kesulitan :**

- Tidak Ada
