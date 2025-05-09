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
  static std::string bold(std::string_view text) {
    return "\033[1m" + std::string(text) + "\033[0m";
  }

  static std::string faintOff(std::string_view text) {
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

// TODO: Buat Tampilan yang Cantik (gunain Text buat warna, bold, atau agak
// transparant)
int main() {
  std::cout << "Musik Player Project Algoritma\n";
  std::cout << Text::bold("Ivan & Rafid");
  return 0;
}