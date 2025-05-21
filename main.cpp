#include <algorithm>
#include <atomic>
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
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

/**
 * @brief Penerapan Linked List menggunakan struct / class
 *
 * @tparam T = tipe data yang akan disimpan dalam linkedlist
 */
template <typename T>
struct Node {
  T data;
  Node<T>* previous;
  Node<T>* next;

  Node() = default;
  Node(const T& data) : data(data), next(nullptr), previous(nullptr) {}
};

template <typename T>
class LinkedList {
 public:
  LinkedList() : head_(nullptr), tail_(nullptr), node_counter_(0) {}

  const int count() const { return node_counter_; }
  Node<T>* head() const { return head_; }
  Node<T>* tail() const { return tail_; }

  /**
   * @brief Push digunakan untuk menyisipkan data di belakang (sisip belakang)
   *
   * @param data merupakan tipe data yang akan disimpan
   */
  void push(const T& data) {
    Node<T>* new_node = new Node<T>(data);
    if (isEmpty()) {
      node_counter_++;
      head_ = tail_ = new_node;
      return;
    }

    tail_->next = new_node;
    new_node->previous = tail_;
    tail_ = new_node;

    node_counter_++;
  }

  /**
   * @brief Insert digunakan untuk sisip depan (default bila tidak memberikan
   * position) dan sisip tengah jika mengisi parameter position.
   *
   * @param data
   * @param position
   */
  void insert(const T& data, int position = 0) {
    Node<T>* new_node = new Node<T>(data);
    node_counter_++;

    if (isEmpty()) {
      head_ = tail_ = new_node;
      return;
    }

    if (position <= 0) {
      new_node->next = head_;
      head_->previous = new_node;

      head_ = new_node;
      return;
    }

    Node<T>* iterator = head_;

    for (size_t i = 0; i < position - 1 && isNotNull(iterator); i++) {
      iterator = iterator->next;
    }

    if (isNull(iterator)) {
      push(data);
      delete new_node;
      return;
    }

    new_node->next = iterator->next;
    new_node->previous = iterator;

    if (isNotNull(new_node->next)) {
      new_node->next->previous = new_node;
    } else {
      tail_ = new_node;
    }

    iterator->next = new_node;
  }

  /**
   * @brief deleteNode -- fungsi yang digunakan untuk menghapus node berdasarkan
   * data yang sama
   *
   * @param data
   */
  void deleteNode(const T& data) {
    if (isEmpty()) return;

    Node<T>* target_node = head_;
    while (isNotNull(target_node) && target_node->data != data) {
      target_node = target_node->next;
    }

    if (isNull(target_node)) return;

    if (isNotNull(target_node->previous)) {
      target_node->previous->next = target_node->next;
    } else {
      head_ = target_node->next;
    }

    if (target_node->next) {
      target_node->next->previous = target_node->previous;
    } else {
      tail_ = target_node->previous;
    }

    delete target_node;

    node_counter_--;
  }

  /**
   * @brief reverse digunakan untuk memutarbalikkan linked list atau mengubah
   * urutan linkedlist dari belakang ke depan
   *
   */
  void reverse() {
    if (isEmpty()) return;

    Node<T>* previous_node = nullptr;
    Node<T>* current_node = head_;
    Node<T>* next_node = nullptr;

    tail_ = head_;

    while (isNotNull(current_node)) {
      next_node = current_node->next;
      current_node->next = previous_node;
      current_node->previous = next_node;

      previous_node = current_node;
      current_node = next_node;
    }

    head_ = previous_node;
  }

  /**
   * @brief clear digunakan untuk menghapus seluruh isi linked list tanpa
   * menyebabkan memory leak
   *
   */
  void clear() {
    Node<T>* current{head_};

    while (isNotNull(current)) {
      Node<T>* next_node = current->next;
      delete current;
      current = next_node;
    }

    head_ = tail_ = nullptr;
    node_counter_ = 0;
  }

  Node<T>* head_;

  bool isEmpty() { return !head_; }

 private:
  Node<T>* tail_;
  size_t node_counter_;

  bool isNull(Node<T>* node) { return !node; }
  bool isNotNull(Node<T>* node) { return node; }
};

/**
 * @brief Implementasi FILE* menggunakan FileManager digunakan untuk menyimpan
 * data playlist yang ingin dibuat dan juga daftar lagu yang tersedia
 */
class FileManager {
 public:
  static std::string kDatabase;
  static std::string kPlaylist;

  // implementasi save and load untuk vector atau array
  template <typename T>
  static bool save(const std::string& filename, const std::vector<T>& data) {
    file_ptr = fopen(filename.c_str(), "wb");
    if (!file_ptr) return false;

    for (T x : data) {
      if (!x.serialize()) {
        fclose(file_ptr);
        return false;
      }
    }

    fclose(file_ptr);
    return true;
  }

  template <typename T>
  static bool load(const std::string& filename, std::vector<T>& data) {
    file_ptr = fopen(filename.c_str(), "rb");
    data.clear();
    if (!file_ptr) return false;

    while (true) {
      T x;
      if (!x.deserialize()) {
        break;
      }
      data.emplace_back(x);
    }

    fclose(file_ptr);
    return true;
  }

  // implementasi save and load untuk LinkedList
  template <typename T>
  static bool save(const std::string& filename, const LinkedList<T>& data) {
    file_ptr = fopen(filename.c_str(), "wb");
    if (!file_ptr) return false;

    Node<T>* node = data.head();
    while (node) {
      if (!node->data.serialize()) {
        fclose(file_ptr);
        return false;
      }

      node = node->next;
    }

    fclose(file_ptr);
    return true;
  }

  template <typename T>
  static bool load(const std::string& filename, LinkedList<T>& data) {
    file_ptr = fopen(filename.c_str(), "rb");
    if (!file_ptr) return false;

    while (!feof(file_ptr)) {
      T x;
      if (!x.deserialize()) {
        break;
      }
      data.push(x);
    }

    fclose(file_ptr);
    return true;
  }

  // Implementasi fungsi bantuan untuk mencegah kesalahan saat menulis dan
  // membaca data serta digunakan untuk meningkatkan readabilitas
  template <typename T>
  static bool read(T& data, size_t length = 1) {
    return fread(&data, sizeof(T), length, file_ptr) == length;
  }

  static bool read(char* str, size_t length) {
    return fread(str, sizeof(char), length, file_ptr) == length;
  }

  template <typename T>
  static bool write(const T& data) {
    return fwrite(&data, sizeof(T), 1, file_ptr) == 1;
  }

  static bool write(const char* str, size_t length) {
    return fwrite(str, sizeof(char), length, file_ptr) == length;
  }

 private:
  static inline FILE* file_ptr{nullptr};
};

std::string FileManager::kDatabase = "DatabaseLagu.dat";
std::string FileManager::kPlaylist = "DatabasePlaylist.dat";

/**
 * @brief Song adalah data yang digunakan pada project RAiVFY
 *
 */
struct Song {
  static size_t id_counter;
  size_t id;
  std::string title, artist, genre;
  size_t duration, playCount{0};
  int release_year;

  // Constructor
  Song() : id(id_counter) {}
  Song(const std::string& title, const std::string& artist,
       const std::string& genre, int release_year, size_t duration,
       size_t playCount = 0)
      : id(id_counter++),
        title(title),
        artist(artist),
        genre(genre),
        release_year(release_year),
        duration(duration) {}

  // Operator Overloading digunakan untuk membantu membandingkan apakah dua lagi
  // sama atau tidak
  bool operator==(const Song& rhs) const {
    return title == rhs.title && artist == rhs.artist;
  }

  bool operator!=(const Song& rhs) const { return !(*this == rhs); }

  // serialize dan deserialize digunakan agar FileManager dapat digunakan untuk
  // lebih dari satu type data
  bool serialize() const {
    size_t title_length = title.size();
    size_t genre_length = genre.size();
    size_t artist_length = artist.size();

    FileManager::write<size_t>(id);
    FileManager::write<size_t>(title_length);
    FileManager::write(title.c_str(), title_length);
    FileManager::write<size_t>(genre_length);
    FileManager::write(genre.c_str(), genre_length);
    FileManager::write<size_t>(artist_length);
    FileManager::write(artist.c_str(), artist_length);
    FileManager::write<int>(release_year);
    FileManager::write<size_t>(duration);
    FileManager::write<size_t>(playCount);

    return true;
  }

  bool deserialize() {
    size_t title_length{0}, genre_length{0}, artist_length{0};

    if (!FileManager::read(id)) return false;

    if (!FileManager::read(title_length)) return false;
    title.resize(title_length);
    if (title_length > 0 && !FileManager::read(&title[0], title_length))
      return false;

    if (!FileManager::read(genre_length)) return false;
    genre.resize(genre_length);
    if (genre_length > 0 && !FileManager::read(&genre[0], genre_length))
      return false;

    if (!FileManager::read(artist_length)) return false;
    artist.resize(artist_length);
    if (artist_length > 0 && !FileManager::read(&artist[0], artist_length))
      return false;

    if (!FileManager::read(release_year)) return false;
    if (!FileManager::read(duration)) return false;
    if (!FileManager::read(playCount)) return false;

    Song::id_counter++;
    return true;
  }
};

/**
 * @brief User dan UserType merupakan data user yang digunakan untuk fitur login
 *
 */
enum UserType { LISTENER, ADMIN };
struct User {
  size_t id;
  std::string username, password;
  UserType type;
};

/**
 * @brief Implementasi Algoritma Sorting Bubble Sort dan Quick Sort
 *
 */
class SongSorter {
 public:
  // Lambda function untuk membuat bubbleSort dan quickSort dapat digunakan
  // secara flexible untuk mengurutkan data berdasarkan field tertentu
  static inline auto by_title = [](const Song& a, const Song& b) {
    return a.title <= b.title;
  };

  static inline auto by_id = [](const Song& a, const Song& b) {
    return a.id <= b.id;
  };

  static inline auto by_play_count = [](const Song& a, const Song& b) {
    return a.playCount <= b.playCount;
  };

  static inline auto by_release_year = [](const Song& a, const Song& b) {
    return a.release_year <= b.release_year;
  };

  static void ensureSortedByID(std::vector<Song>& data) {
    if (data.size() > 30) {
      quickSort(data, 0, data.size() - 1, by_id);
    } else {
      bubbleSort(data, by_id);
    }
  }

  static void bubbleSort(
      std::vector<Song>& data,
      std::function<bool(const Song&, const Song&)> comparator) {
    if (data.empty()) return;

    for (size_t i = 0; i < data.size(); ++i) {
      for (size_t j = 0; j < data.size() - i - 1; ++j) {
        if (comparator(data[j + 1], data[j])) {
          swap(&data[j], &data[j + 1]);
        }
      }
    }
  }

  static void quickSort(
      std::vector<Song>& data, size_t awal, size_t akhir,
      std::function<bool(const Song&, const Song&)> comparator) {
    if (awal < akhir) {
      int pivot = partition(data, awal, akhir, comparator);
      if (pivot > awal) quickSort(data, awal, pivot - 1, comparator);
      if (pivot < akhir) quickSort(data, pivot + 1, akhir, comparator);
    }
  }

  // reverseOrder digunakan untuk melakukan pengurutan secara descending
  template <typename T>
  static std::function<bool(const T&, const T&)> reverseOrder(
      std::function<bool(const T&, const T&)> comparator) {
    return [comparator](const T& a, const T& b) { return comparator(b, a); };
  }

 private:
  static size_t partition(
      std::vector<Song>& data, size_t index_awal, size_t index_akhir,
      std::function<bool(const Song&, const Song&)> comparator) {
    Song pivot = data[index_akhir];
    size_t i = index_awal - 1;

    for (size_t j = index_awal; j < index_akhir; j++) {
      if (comparator(data[j], pivot)) {
        i++;
        swap(&data[j], &data[i]);
      }
    }

    swap(&data[i + 1], &data[index_akhir]);

    return (i + 1);
  }

  static void swap(Song* data1, Song* data2) {
    Song temporary = *data1;
    *data1 = *data2;
    *data2 = temporary;
  }
};

/**
 * @brief Implementasi Algoritma Searching Binary Search dan Linear Search
 *
 */
class SongSearcher {
 public:
  // Algoritma binarySearch digunakan untuk menemukan lagu dengan id tertentu
  static int result_index;
  static bool binarySearch(size_t target_id, std::vector<Song>& data) {
    if (data.empty()) return false;

    SongSorter::ensureSortedByID(data);

    int start{0}, mid{0}, end{static_cast<int>(data.size()) - 1};
    while (start <= end) {
      mid = (start + end) / 2;

      if (target_id == data[mid].id) {
        result_index = mid;
        return true;
      }

      if (target_id > data[mid].id) {
        start = mid + 1;
      } else if (target_id < data[mid].id) {
        end = mid - 1;
      }
    }
    result_index = -1;
    return false;
  }

  // Fungsi untuk mencari lagu berdasarkan field tertentu
  static std::vector<Song> searchById(int target_id,
                                      const std::vector<Song>& data) {
    return linearSearch(
        data, [target_id](const Song& song) { return song.id == target_id; });
  }

  static std::vector<Song> searchByGenre(const std::string& genre,
                                         const std::vector<Song>& data) {
    std::string normalized_genre = normalizeString(genre);

    return linearSearch(data, [normalized_genre](const Song& song) {
      std::string song_genre = normalizeString(song.genre);
      return song_genre.find(normalized_genre) != std::string::npos;
    });
  }

  static std::vector<Song> searchByArtist(const std::string& artist,
                                          const std::vector<Song>& data) {
    std::string normalized_artist = normalizeString(artist);

    return linearSearch(data, [normalized_artist](const Song& song) {
      std::string song_artist = normalizeString(song.artist);
      return song_artist.find(normalized_artist) != std::string::npos;
    });
  }

  static std::vector<Song> searchByTitle(const std::string& title,
                                         const std::vector<Song>& data) {
    std::string normalized_title = normalizeString(title);

    return linearSearch(data, [normalized_title](const Song& song) {
      std::string song_title = normalizeString(song.title);
      return song_title.find(normalized_title) != std::string::npos;
    });
  }

 private:
  // linearSearch digunakan untuk proses filtering lagu berdasarkan field
  // tertentu
  static std::vector<Song> linearSearch(
      const std::vector<Song>& data,
      std::function<bool(const Song&)> comparator) {
    size_t index{0};
    std::vector<Song> filtered_library;
    while (index < data.size()) {
      if (comparator(data[index])) {
        filtered_library.emplace_back(data[index]);
      }
      index++;
    }
    return filtered_library;
  }

  static std::string normalizeString(const std::string& string) {
    std::string normalized = string;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return normalized;
  }
};

class SongLibrary {
 public:
  std::vector<Song>& database() { return database_; }

  void addToLibrary(const Song& song) {
    if (!SongSearcher::binarySearch(song.id, database_)) {
      database_.push_back(song);
    }

    SongSorter::ensureSortedByID(database_);
  }

  void removeSongById(int target_id) {
    if (SongSearcher::binarySearch(target_id, database_)) {
      database_.erase(std::remove_if(database_.begin(), database_.end(),
                                     [target_id](const Song& song) {
                                       return song.id == target_id;
                                     }),
                      database_.end());
    }
  }

  Song& getSongById(int target_id) {
    if (SongSearcher::binarySearch(target_id, database_)) {
      int result = SongSearcher::result_index;
      SongSearcher::result_index = -1;
      return database_[result];
    }
  }

 private:
  std::vector<Song> database_;
};

class Playlist {
 public:
  static size_t id_counter;
  Playlist() : id_(id_counter) {}
  Playlist(std::string name) : name_(name), id_(id_counter) { play = false; }

  const size_t id() { return id_; }
  const std::string& name() { return name_; }
  const std::string& description() { return description_; }
  LinkedList<Song>& list() { return list_; }

  void changeName(const std::string& name) {
    if (name != name_) {
      name_ = name;
    }
  }

  void changeDescription(const std::string& description) {
    if (description != description_) {
      description_ = description;
    }
  }

  void addSong(const Song& song) {
    list_.push(song);
    if (current_song_ == nullptr) {
      current_song_ = list_.head();
      play = true;
    }
  }

  void removeSong(const Song& song) { list_.deleteNode(song); }

  bool pausePlay() {
    play = !play;
    return play;
  }

  bool serialize() const {
    size_t name_length = name_.size();

    FileManager::write<size_t>(name_length);
    FileManager::write(name_.c_str(), name_length);

    return true;
  }

  bool deserialize() {
    size_t name_length{0};
    if (!FileManager::read(name_length)) return false;

    name_.resize(name_length);
    if (name_length > 0 && !FileManager::read(&name_[0], name_length))
      return false;

    Playlist::id_counter++;
    return true;
  }

  void displayList() {
    Node<Song>* x = list_.head();

    std::cout << "\n\n";
    std::cout << std::left << std::setw(5 + Text::kAnsi) << Text::bold("No")
              << std::setw(22 + Text::kAnsi) << Text::bold("Judul")
              << std::setw(22 + Text::kAnsi) << Text::bold("Artis")
              << std::setw(8 + Text::kAnsi) << Text::bold("Tahun")
              << std::setw(12 + Text::kAnsi) << Text::bold("Diputar") << "\n";
    std::cout << std::setfill('-') << std::setw(61) << "-" << std::setfill(' ')
              << "\n";

    while (x != nullptr) {
      std::cout << std::left << std::setw(5) << x->data.id << std::setw(18)
                << x->data.title.substr(0, 18) << std::setw(18)
                << x->data.artist.substr(0, 18) << std::setw(8)
                << x->data.release_year << std::setw(12) << x->data.playCount
                << "\n";
      x = x->next;
    }
    std::cout << "\n\n";
  }

  void playbackLoop() {
    current_song_ = list_.head();
    play = true;

    while (current_song_ && play) {
      int remaining = current_song_->data.duration / 10;
      std::string title = current_song_->data.title + "         ";
      size_t move_print = 0;

      int tick;
      while (remaining > 0 && play) {
        std::cout << "\033[s";
        std::cout << "\033[2;24H";

        std::string scrolling_title = title.substr(move_print, 8);
        if (scrolling_title.length() < 8) {
          scrolling_title += title.substr(0, 8 - scrolling_title.length());
        }

        std::cout << "Now Playing: " << scrolling_title
                  << " | Remaining: " << std::setw(3) << remaining << "s"
                  << std::flush;

        std::cout << "\033[u" << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        if (tick % 5 == 0) {
          --remaining;
          tick = 0;
        }

        tick++;
        move_print = (move_print + 1) % title.length();  // wrap scroll
      }

      nextSong();
    }
  }

 private:
  LinkedList<Song> list_;
  Node<Song>* current_song_;
  bool play;

  size_t id_;
  std::string name_;
  std::string description_;

  void nextSong() {
    if (current_song_ && current_song_->next) {
      current_song_ = current_song_->next;
    }
  }

  void previousSong() {
    if (current_song_ && current_song_->previous) {
      current_song_ = current_song_->previous;
    }
  }
};

size_t Song::id_counter = 1;
size_t Playlist::id_counter = 1;
int SongSearcher::result_index = 0;

class RAiVFY {
 public:
  void ignite() {
    if (!playlist_library.empty()) {
      std::thread playback(&Playlist::playbackLoop, &playlist_library[0]);
      playback.detach();
    }

    while (true) {
      mainMenu();
    }
  }

  void load() {
    FileManager::load<Song>(FileManager::kDatabase, database.database());
    FileManager::load<Playlist>(FileManager::kPlaylist, playlist_library);

    for (auto& p : playlist_library) {
      FileManager::load<Song>(p.name() + ".dat", p.list());
    }
  }

 private:
  SongLibrary database;
  std::vector<Playlist> playlist_library;

  void mainMenu() {
    clearScreen();
    printBorder(Text::bold(" RAiVFY "), 11);
    std::cout << "\n 1.  Buat Playlist\n";
    std::cout << " 2.  Lihat Playlist\n";
    std::cout << " 3.  Daftar Lagu\n";
    std::cout << " 4.  Cari Lagu\n";
    std::cout << " 0.  Keluar\n";

    int choice = getNumberInput<int>(Text::bold("\n > Pilih Menu : "));
    enum mainMenu { KELUAR, BUAT, LIHAT, LIST, SEARCH };
    switch (choice) {
      case KELUAR:
        exit(0);
        break;
      case BUAT:
        buatPlaylist();
        break;
      case LIHAT:
        daftarPlaylist();
        break;
      case LIST:
        daftarLagu(database.database());
        break;
      case SEARCH:
        searchingLagu();
        waitForInput();
        break;
      default:
        std::cout << " Menu tidak tersedia!\n";
    }
  }

  void buatPlaylist() {
    clearScreen();
    std::string namaPlaylist;

    printBorder(" Buat Playlist ", 10);
    std::cout << "\n Masukkan nama playlist baru kamu: ";

    std::getline(std::cin >> std::ws, namaPlaylist);
    bool isValid = true;

    for (auto& p : playlist_library) {
      if (p.name() == namaPlaylist) {
        isValid = false;
      }
    }

    if (!isValid) {
      std::cout << " Playlist [" << Text::bold(namaPlaylist)
                << "] sudah ada!\n";
    } else {
      Playlist p(namaPlaylist);
      playlist_library.emplace_back(p);
      FileManager::save(FileManager::kPlaylist, playlist_library);
      FileManager::save(namaPlaylist + ".dat", p.list());

      std::cout << "\n Playlist [" << Text::bold(namaPlaylist)
                << "] berhasil dibuat!\n";
      std::cout << " Saatnya mengisi playlist ini dengan lagu favoritmu!\n\n";
    }

    waitForInput();
    playlistAction(playlist_library.size() - 1);
  }

  void daftarPlaylist() {
    clearScreen();
    printBorder(Text::bold(" Buat Playlist "), playlist_library.size() + 7);

    std::cout << "\n";
    for (auto& p : playlist_library) {
      std::cout << " " << p.id() << ". " << std::left << std::setw(15)
                << p.name().substr(0, 14) << "(" << p.list().count()
                << " songs)"
                << "\n";
    }
    std::cout << "\n 0. Kembali ke Main Menu\n";

    int choice = getNumberInput<int>(Text::bold("\n > Pilih Menu: "));

    if (choice <= 0 || choice > playlist_library.size()) {
      if (choice != 0) std::cout << " Tidak ada playlist yang sesuai!\n";
      std::cin >> std::ws;
      waitForInput();
      mainMenu();
    }

    playlistAction(choice - 1);
  }

  void playlistAction(int index) {
    clearScreen();
    if (index < 0) return;

    printBorder(
        " Playlist [" + Text::bold(playlist_library[index].name()) + "] ", 12);
    std::cout << Text::underline("\n Pilihan Menu:");
    std::cout << "\n 1. Lihat isi playlist";
    std::cout << "\n 2. Tambah lagu ke playlist";
    std::cout << "\n 3. Reverse playlist";
    std::cout << "\n 4. Hapus lagu dari playlist";
    std::cout << "\n 5. Hapus playlist";
    std::cout << "\n 0. Kembali ke menu utama\n\n";

    int choice = getNumberInput<int>(" > Pilih menu: ");

    enum action { KEMBALI, LIHAT, TAMBAH, REVERSE, HAPUS_LAGU, HAPUS_PLAYLIST };
    switch (choice) {
      case KEMBALI:
        mainMenu();
        break;
      case LIHAT:
        clearScreen();
        printBorder(
            " Playlist [" + Text::bold(playlist_library[index].name()) + "] ",
            playlist_library[index].list().count() + 3);
        playlist_library[index].displayList();
        std::cin.ignore();
        waitForInput();
        break;
      case TAMBAH: {
        std::vector<Song> filtered = searchingLagu();
        size_t selected_id = getNumberInput<size_t>(" Masukan ID lagu : ");
        if (SongSearcher::binarySearch(selected_id, filtered)) {
          playlist_library[index].addSong(
              filtered.at(SongSearcher::result_index));
        }
        FileManager::save(playlist_library[index].name() + ".dat",
                          playlist_library[index].list());
        playlist_library[index].list().clear();
        FileManager::load(playlist_library[index].name() + ".dat",
                          playlist_library[index].list());
        break;
      }
      case REVERSE:
        playlist_library[index].list().reverse();
        playlist_library[index].displayList();
        std::cin.ignore();
        waitForInput();
        break;
      case HAPUS_LAGU: {
        playlist_library[index].displayList();
        size_t selected_id = getNumberInput<size_t>(" Masukan ID lagu : ");
        if (SongSearcher::binarySearch(selected_id, database.database())) {
          playlist_library[index].removeSong(
              database.database().at(SongSearcher::result_index));
        }
        FileManager::save(playlist_library[index].name() + ".dat",
                          playlist_library[index].list());
        playlist_library[index].list().clear();
        FileManager::load(playlist_library[index].name() + ".dat",
                          playlist_library[index].list());
        break;
      }
      case HAPUS_PLAYLIST: {
        std::string command;
#ifdef __WIN32__
        command = "delete '" + playlist_library[index].name() + ".dat'";
#else
        command = "rm '" + playlist_library[index].name() + ".dat'";
#endif
        system(command.data());

        int target = playlist_library[index].id();
        playlist_library.erase(
            std::remove_if(playlist_library.begin(), playlist_library.end(),
                           [&target](Playlist& p) { return p.id() == target; }),
            playlist_library.end());
        Playlist::id_counter = 1;
        FileManager::save(FileManager::kPlaylist, playlist_library);
        FileManager::load(FileManager::kPlaylist, playlist_library);
        break;
      }
      default:
        std::cout << " Menu tidak tersedia!\n";
    }
  }

  void daftarLagu(std::vector<Song>& data) {
    clearScreen();
    printBorder(Text::bold(" Daftar Lagu "), data.size() + 3);
    tabelLagu(data);

    std::cout << "\n Ingin mengurutkan berdasarkan:\n";
    std::cout << " 1. Tahun Rilis\n";
    std::cout << " 2. Most Played\n";
    std::cout << " 3. Judul\n";
    std::cout << " 0. Main Menu\n\n";

    int pilihan = getNumberInput<int>(" > Pilih menu: ");
    int isDescending;
    switch (pilihan) {
      case 1:
        isDescending = opsiSorting() - 1;
        SongSorter::quickSort(
            database.database(), 0, database.database().size() - 1,
            (isDescending)
                ? SongSorter::reverseOrder<Song>(SongSorter::by_release_year)
                : SongSorter::by_release_year);
        daftarLagu(data);
        break;
      case 2:
        isDescending = opsiSorting() - 1;
        SongSorter::quickSort(
            database.database(), 0, database.database().size() - 1,
            (isDescending)
                ? SongSorter::reverseOrder<Song>(SongSorter::by_play_count)
                : SongSorter::by_play_count);
        daftarLagu(data);
        break;
      case 3:
        isDescending = opsiSorting() - 1;
        SongSorter::quickSort(
            database.database(), 0, database.database().size() - 1,
            (isDescending)
                ? SongSorter::reverseOrder<Song>(SongSorter::by_title)
                : SongSorter::by_title);
        daftarLagu(data);
        break;
      case 0:
        mainMenu();
        break;
      default:
        std::cout << " Pilihan tidak valid!\n";
        break;
    }
  }

  void tabelLagu(std::vector<Song>& data) {
    clearScreen();
    std::cout << "\n\n";
    std::cout << std::left << std::setw(5 + Text::kAnsi) << Text::bold("No")
              << std::setw(22 + Text::kAnsi) << Text::bold("Judul")
              << std::setw(22 + Text::kAnsi) << Text::bold("Artis")
              << std::setw(8 + Text::kAnsi) << Text::bold("Tahun")
              << std::setw(12 + Text::kAnsi) << Text::bold("Diputar") << "\n";
    std::cout << std::setfill('-') << std::setw(61) << "-" << std::setfill(' ')
              << "\n";

    for (auto& song : data) {
      std::cout << std::left << std::setw(5) << song.id << std::setw(18)
                << song.title.substr(0, 18) << std::setw(18)
                << song.artist.substr(0, 18) << std::setw(8)
                << song.release_year << std::setw(12) << song.playCount << "\n";
    }

    std::cout << "\n\n";
  }

  int opsiSorting() {
    std::cout << "Mau Urutan Berdasarkan :\n";
    std::cout << "1. Ascending\n";
    std::cout << "2. Descending\n";
    std::cout << "Pilihan: ";

    return getNumberInput<int>();
  }

  std::vector<Song> searchingLagu() {
    clearScreen();
    SongSearcher::result_index = -1;
    printBorder(Text::bold(" Search Lagu "), 11);
    std::cout << "\n Opsi Searching: \n";
    std::cout << " 1. Judul\n";
    std::cout << " 2. Artist\n";
    std::cout << " 3. Genre\n";
    std::cout << " 4. Show All\n\n";

    int choice = getNumberInput<int>(Text::bold(" > Pilih metode : ")) - 1;
    enum search { JUDUL, ARTIST, GENRE, ALL };
    std::vector<Song> filtered;
    switch (choice) {
      case JUDUL: {
        std::cout << " Masukkan kata kunci: ";

        std::string kata_kunci;
        std::cin.ignore();
        std::getline(std::cin, kata_kunci);
        filtered = SongSearcher::searchByTitle(kata_kunci, database.database());
        tabelLagu(filtered);
        break;
      }
      case ARTIST: {
        std::cout << " Masukkan kata kunci: ";

        std::string kata_kunci;
        std::cin.ignore();
        std::getline(std::cin, kata_kunci);
        filtered =
            SongSearcher::searchByArtist(kata_kunci, database.database());
        tabelLagu(filtered);
        break;
      }
      case GENRE: {
        std::cout << " Masukkan kata kunci: ";

        std::string kata_kunci;
        std::cin.ignore();
        std::getline(std::cin, kata_kunci);
        filtered = SongSearcher::searchByGenre(kata_kunci, database.database());
        tabelLagu(filtered);
        break;
      }
      case ALL:
        filtered = database.database();
        tabelLagu(filtered);
        std::cin.ignore();
        break;
      default:
        std::cout << "Menu tidak tersedia!";
    }

    return filtered;
  }

  void clearScreen() {
#ifdef __WIN32__
    system("cls");
#else
    system("clear");
#endif
  }

  void printBorder(const std::string& title, int lines) {
    const int width = 61;
    moveCursor(1, 1);
    std::cout << std::setfill('-') << std::setw(width) << '-';
    moveCursor(lines, 1);
    std::cout << std::setfill('-') << std::setw(width) << '-'
              << std::setfill(' ');

    moveCursor(1, width / 2 - 7);
    std::cout << title;

    moveCursor(2, 1);
  }

  void moveCursor(int line, int column) {
    std::cout << "\033[" << line << ";" << column << "H";
  }

  void waitForInput() {
#ifdef __WIN32__
    system("pause");
#else
    std::cout << " Tekan tombol enter untuk melanjutkan...";
    std::cin.get();
#endif
  }

  // Error Handling untuk memastikan tidak ada infinite loop
  // ketika user menginputkan huruf pada variable numerik
  template <typename TypeTemplate>
  static TypeTemplate getNumberInput(const std::string& prompt = "") {
    static_assert(std::is_arithmetic<TypeTemplate>::value,
                  "Hanya tipe data numerik!");

    TypeTemplate value{};
    do {
      std::cout << prompt;
      std::cin >> value;

      if (!std::cin.fail()) {
        return value;
      }

      std::cin.clear();
      std::cin.ignore();
      std::cout << "Harap hanya input angka!\n";
    } while (true);
  }
};

// TODO: ADD MORE ERROR HANDLING!
// OPTIONAL TODO: multithread for playing music
int main() {
  RAiVFY app;
  app.load();
  app.ignite();

  return 0;
}