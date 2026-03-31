# PLTDV Solver - Optimasi Program Linear C++

Sebuah aplikasi berbasis terminal Interaktif (CLI) yang dinamis menggunakan bahasa C++ untuk menyelesaikan **Sistem Pertidaksamaan Linear Dua Variabel (PLTDV / SPtLDV)**. Program ini dirancang khusus untuk mempermudah perhitungan Matematika dalam mencari titik sudut (perpotongan) serta melakukan optimasi (mencari nilai maksimum/minimum) dari suatu daerah penyelesaian fungsi objektif.

Dibuat oleh **Ivan Aryasatya**.

---

## 🚀 Fitur Utama

- **Parsing Interaktif Otomatis:** Memasukkan pertidaksamaan dengan format koefisien yang otomatis dibaca dengan 5 Operator Matematika (`<=`, `>=`, `<`, `>`, `=`). Tampilan ke layar akan selalu dirapikan.
- **Pendeteksi Titik Sudut/Ekstrem Cerdas:** Menghitung perpotongan antar garis menggunakan Metode D / Eliminasi, kemudian memfilternya dengan ketat untuk memastikan titik potong tersebut masuk ke dalam Daerah Penyelesaian / DP (Valid).
- **Kalkulator Fungsi Objektif:** Hanya perlu memasukkan fungsi target `Z = px + qy` dan program akan otomatis mengevaluasi *Z* terhadap semua titik potong valid untuk menampilkan **Nilai Maksimal & Minimal**.
- **Syarat Non-Negatif Instan:** Menyelesaikan soal dunia nyata? Gunakan fitur sekali klik untuk otomatis membatasi garis pada kuadran I (`x >= 0` dan `y >= 0`).
- **UI Terminal Berwarna:** Antarmuka disajikan dengan apik dan ramah menggunakan kode ANSI Color supaya tabel dan hasil operasi matematika Anda lebih mudah dibaca (Mendukung CMD dan Terminal modern).
- **Standalone Exe:** Telah dikompilasi secara *Statically Linked* sehingga program dapat dicopy-paste ke komputer/laptop Windows manapun dan tetap berjalan tanpa error "Missing DLL".

---

## 🛠️ Cara Menginstall & Menjalankan

### Cara 1: Menjalankan Langsung (Termudah)
Jika Anda hanya ingin menggunakan aplikasinya:
1. Buka folder aplikasi ini (`pltdvSolver`).
2. Cari file bernama `pltdvSolver.exe`.
3. Klik ganda pada file `pltdvSolver.exe`, atau buka Command Prompt di folder tersebut dan ketikkan `pltdvSolver.exe`. Program akan langsung terbuka.

### Cara 2: Menggunakan Script `run.bat` (Untuk Modifikasi Code / Re-compile)
Jika Anda memodifikasi kode di `main.cpp` dan ingin melakukan *build* ulang:
1. Pastikan Anda memiliki *compiler* C++ (seperti MinGW gcc/g++) yang sudah dipasang dan jalurnya masuk ke *System Environment Variables (PATH)*.
2. Klik ganda file `run.bat` di folder aplikasi Anda.
3. Script otomatis akan mem-*build* ulang `main.cpp` secara statik dan menggantikan `pltdvSolver.exe` lama Anda dengan versi yang baru, lalu membukanya untuk Anda.

---

## 📖 Panduan Penggunaan Menu Utama

Saat program berjalan, Anda akan disambut 7 navigasi menu utama:

| Nomor Menu | Nama Menu | Deskripsi & Kegunaan |
| :---: | :--- | :--- |
| **`1`** | Tambah Pertidaksamaan | Anda akan diminta memasukkan nilai `a`, `b`, opsi operator, dan `c`. (Contoh: untuk membuat pertidaksamaan `2x + 3y <= 12`, masukkan 2, lalu 3, pilih opsi `<=`, lalu 12). |
| **`2`** | Tambah Syarat Non-Negatif | Tombol ini menambahkan 2 garis batas otomatis (`x >= 0` dan `y >= 0`) ke dalam sistem tanpa perlu input manual. Sangat menghemat waktu. |
| **`3`** | Hapus Pertidaksamaan | Salah ketik nomor saat input? Cukup cek nomor urut pertidaksamaan yang salah di layar, lalu hapus menggunakan nomor ini. |
| **`4`** | Set Fungsi Objektif | Menentukan persamaan linear penargetan `Z = px + qy` (Misalnya dari soal cerita untuk mencari Keuntungan Maksimum). |
| **`5`** | Selesaikan & Cari Ekstrem | Inti dari program ini; memproses semua persamaan, menghitung persilangan garis membuang intersection yang salah, serta mencetak kesimpulan titik sudut di Tabel. |
| **`6`** | Reset Sistem | Membuang seluruh persamaan dalam memori. Gunakan bila Anda ingin mengerjakan soal Matematika yang baru. |
| **`0`** | Keluar | Mengakhiri eksekusi program dan keluar dari terminal. |

---

## 💡 Contoh Praktik Soal Matematika

**Contoh Soal:**
Carilah Nilai Maksimum dan Minimum dari Sistem Pertidaksamaan berikut:
* `3x + 2y <= 6`
* `x >= 0`
* `y >= 0`
Dengan fungsi sasaran / objektif `Z = 5x + 3y`.

**Langkah penyelesaian dengan PLTDV Solver:**
1. Di Menu Utama, ketik `1`, masukkan `a=3`, `b=2`, pilih Operator `1` (`<=`), dan `c=6`.
2. Di Menu Utama lagi, ketik `2` untuk memasukkan `x>=0` dan `y>=0` secara otomatis dan instan.
3. Di Menu Utama, ketik `4`, lalu masukkan koefisien fungsi `p=5`, dan `q=3`.
4. Di Menu Utama, ketik `5` untuk mengeksekusi Program Linear Anda!
5. Program akan otomatis menampilkan tabel seluruh **Titik Sudut (x, y)** yang valid beserta nilai `Z`-nya, lalu memberikan kesimpulan ber-Highlight mana nilai **Maksimum** dan **Minimum**.
