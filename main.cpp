#include <iostream>
#include <iomanip>
#include <vector>

/*
 * @brief Class Text digunakan untuk memodifikasi atau memanipulasi
 *        tulisan pada console / terminal
 * @param string_view digunakan untuk melihat string (mengurangi overhead
 * program).
 */
class Text {
 public:
  static constexpr int kAnsi = 8;
  static std::string bold(std::string text) {
    return "\033[1m" + std::string(text) + "\033[0m";
  }

  static std::string faintOff(std::string text) {
    return "\033[2m" + std::string(text) + "\033[0m";
    ;
  }

  static std::string italic(std::string text) {
    return "\033[2m" + std::string(text) + "\033[0m";
  }

  static std::string underline(std::string text) {
    return "\033[4m" + std::string(text) + "\033[0m";
  }
};

class ConsoleUI {
  public: 
  static void clearScreen(){
    #ifdef __WIN32__
    system("cls");
    #else system ("clear");
    #endif
  }

  static void buatPlaylist(){
     std::string namaPlaylist;

    std::cout << "\n=====================================\n";
    std::cout << "          BUAT PLAYLIST BARU      \n";
    std::cout << "=====================================\n";
    std::cout << "\n Masukkan nama playlist baru mu: ";

    std::getline(std::cin >> std::ws, namaPlaylist); // Agar bisa input dengan spasi

    std::cout << "\n Playlist " << namaPlaylist <<  " berhasil dibuat!\n";
    std::cout << "Saatnya mengisi playlist ini dengan lagu favoritmu!\n";
    std::cout << "-------------------------------------\n\n";
  }

  static void daftarPlaylist(){
    std::cout << "\n========================================\n";
    std::cout << "            DAFTAR PLAYLIST           \n";
    std::cout << "========================================\n";
     std::cout << "\n" << Text::bold("Playlist yang Kamu Miliki:\n");
    std::cout << std::setfill('=') << std::setw(40) << "=" << std::setfill(' ') << "\n";

        std::cout << std::left
          << std::setw(5) << Text::bold("No")
          << std::setw(25) << Text::bold("Nama Playlist");
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::setfill(' ') << "\n";
    const auto;
        std::cout << std::left
                  << std::setw(5) // nomor
                  << std::setw(22) //nama playlist
                  << "\n";
    std::cout << "=================================================================\n";
        std::cout << "\nPilih playlist yang ingin kamu kelola: ";
    std::cout << "\n\n" << Text::underline("Pilihan Menu:");
    std::cout << "\n1. Lihat isi playlist";
    std::cout << "\n2. Tambah lagu ke playlist";
    std::cout << "\n3. Hapus lagu dari playlist";
    std::cout << "\n4. Hapus playlist";
    std::cout << "\n5. Kembali ke menu utama\n";


  }

  
static void tabelLagu() {
    std::cout << std::left
          << std::setw(5)  << Text::bold("No")
          << std::setw(22) << Text::bold("Judul")
          << std::setw(22) << Text::bold("Artis")
          << std::setw(8)  << Text::bold("Tahun")
          << std::setw(12) << Text::bold("Diputar") << "\n";
    std::cout << std::setfill('-') << std::setw(69) << "-" << std::setfill(' ') << "\n";
    
        const auto;
        std::cout << std::left
                  << std::setw(5) // nomor
                  << std::setw(22) //judul
                  << std::setw(22) //artis
                  << std::setw(8) //tahun
                  << std::setw(12) //diputar
                  << "\n";
    

    std::cout << "=================================================================\n";
  }
  static void menuSorting(){
    std::cout << "Mau Urutan Berdasarkan :\n";
    std::cout << "1. Ascending\n";
    std::cout << "2. Descending\n";
    std::cout << "Pilihan: ";
    int pilihan;
    std::cin >> pilihan;
  }

  static void daftarLagu(){
    std::cout << "\n===========================================\n";
    std::cout << "               DAFTAR LAGU               \n";
    std::cout << "===========================================\n";
    std::cout << "\n Daftar Semua Lagu:\n";
    tabelLagu(); 
    std::cout << "\nIngin mengurutkan berdasarkan:\n";
    std::cout << "1. Tahun\n";
    std::cout << "2. Most Played\n";
    std::cout << "3. Judul\n";
    std::cout << "Pilihan: ";
    int pilihan;
    std::cin >> pilihan;
    switch (pilihan)
    {
    case 1:
      menuSorting();
      break;
    case 2:
      menuSorting();
      break;
    case 3:
      menuSorting();
      break;
    default: std::cout << "Pilihan tidak valid!\n";
      break;
    }
  }

  static void searchingLagu(){
    std::cout << "\n=====================================\n";
    std::cout << "           CARI LAGU               \n";
    std::cout << "=====================================\n";
    std::cout << "Masukkan judul lagu: ";
    std::string judul;
    std::cin.ignore();
    std::getline(std::cin, judul);
  }

  static void mainMenu() {
    std::cout << "=====================================\n";
    std::cout << "              RAiVFY              \n";
    std::cout << "=====================================\n";
    std::cout << " 1.  Buat Playlist\n";
    std::cout << " 2.  Lihat Playlist\n";
    std::cout << " 3.  Daftar Lagu\n";
    std::cout << " 4.  Cari Lagu\n";
    std::cout << " 0.  Keluar\n";
    std::cout << "-------------------------------------\n";
    std::cout << " Pilihan Menu : ";

    int pilihan;
    std::cin >> pilihan;

    switch (pilihan)
    {
        case 1: buatPlaylist();
            // Buat Playlist
            break;
        case 2: daftarPlaylist();
            // Lihat Playlist
            break;
        case 3: daftarLagu();
            // Daftar Lagu
            break;
        case 4: searchingLagu();
            // Cari Lagu
            break;
        case 0: exit(0);
            // Keluar
            break;
        default:
            std::cout << "Pilihan tidak valid!\n";
            break;
    }
}

};

// TODO: Buat Tampilan yang Cantik (gunain Text buat warna, bold, atau agak
// transparant)
int main() {
  std::cout << "Musik Player Project Algoritma\n";
  std::cout << Text::bold("Ivan & Rafid");
 
  ConsoleUI::mainMenu();
  return 0;
}