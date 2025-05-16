#include <iostream>

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

  static void lihatPlaylist(){
    std::cout << "\n========================================\n";
    std::cout << "            DAFTAR PLAYLIST           \n";
    std::cout << "========================================\n";
  }
  static void daftarLagu(){
    std::cout << "\n===========================================\n";
    std::cout << "               DAFTAR LAGU               \n";
    std::cout << "===========================================\n";
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
        case 2: lihatPlaylist();
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