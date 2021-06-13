# soal-shift-sisop-modul-4-A06-2021
Jawaban Soal Shift Sistem Operasi Kelas A Kelompok A-06


**[Source Code](https://github.com/husinassegaff/soal-shift-sisop-modul-4-A06-2021/blob/main/SinSeiFS_A06.c)**

## Soal 1
**Deskripsi:**\
Mengencode sebuah folder yang mengandung kata "Atoz_" dengan atbash chiper

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