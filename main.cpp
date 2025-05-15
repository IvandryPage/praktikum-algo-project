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

  static void mainMenu() {
    std::cout << "1. Buat Playlist\n";
    std::cout << "2. Lihat Playlist\n";
    std::cout << "3. Daftar Lagu\n";
    std::cout << "4. Cari Lagu\n";
    std::cout << "0. Keluar\n";
    std::cout << "Pilihan Menu : ";
    int pilihan;
    std::cin >> pilihan;
    switch (pilihan)
    {
    case 1:
      
      break;
    case 2:
      
      break;
    case 3:
      
      break;
    case 4:
      
      break;
    case 0:
      
      break;
    
    default:
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