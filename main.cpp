#include <chrono>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

/**
 * @class Text
 * @brief Kelas untuk memformat teks pada terminal menggunakan ANSI (American
 * National Standards Institute) escape code
 *
 * Kelas ini menyediakan static method untuk memberikan efek visual text serta
 * mengatur posisi kursor di terminal
 */
class Text {
 public:
  static constexpr int kAnsi =
      8; /**< Nilai tetap untuk penggunaan std::setw pada teks terformat */

  /**
   * @brief Memberikan efek bold pada teks
   *
   * @param text teks yang ingin diformat
   * @return std::string dengan format ANSI bold
   */
  static std::string bold(std::string text) {
    return "\033[1m" + std::string(text) + "\033[0m";
  }

  /**
   * @brief Memberikan efek redup / faint pada teks
   *
   * @param text Teks yang ingin diformat
   * @return std::string dengan dengan format ANSI faint
   */
  static std::string faintOff(std::string text) {
    return "\033[2m" + std::string(text) + "\033[0m";
    ;
  }

  /**
   * @brief Memberikan efek italic pada teks
   *
   * @param text Teks yang ingin diformat
   * @return std::string dengan format ANSI italic
   */
  static std::string italic(std::string text) {
    return "\033[2m" + std::string(text) + "\033[0m";
  }

  /**
   * @brief Memberikan efek underline pada teks
   *
   * @param text Teks yang ingin diformat
   * @return std::string dengan format ANSI underline
   */
  static std::string underline(std::string text) {
    return "\033[4m" + std::string(text) + "\033[0m";
  }

  /**
   * @brief Memindahkan posisi cursor ke posisi tertentu di terminal
   *
   * @param line Baris tujuan (mulai dari 1)
   * @param column Kolom tujuan (mulai dari 1)
   */
  static void moveCursor(int line, int column) {
    std::cout << "\033[" << line << ";" << column << "H";
  }
};

/**
 * @brief Array class
 *
 *
 */
template <typename T>
class ArrayList {
 public:
  T arr[1000]{};

  ArrayList() { size_ = 0; }
  size_t size() { return size_; }
  bool empty() { return size_ == 0; }

  void add(T& new_data) {
    if (size_ < 1000) {
      arr[size_] = new_data;
      ++size_;
    }
  }

  void clear() {
    for (size_t i = 0; i < size_; i++) {
      arr[i] = T();
    }
    size_ = 0;
  }

  void decreaseSize() {
    arr[size_ - 1] = T();
    size_--;
  }

 private:
  size_t size_{0};
};

/**
 * @class Node
 * @brief Struktur Node untuk Implementasi Linked List Ganda.
 *
 * @tparam T Tipe data yang akan disimpan dalam node
 */
template <typename T>
struct Node {
  T data;            /**< Data yang akan disimpan dalam node */
  Node<T>* previous; /**< Pointer ke node sebelumnya */
  Node<T>* next;     /**< Pointer ke node selanjutnya */

  Node() = default;

  /**
   * @brief Constructor Node baru dengan data
   *
   * @param data Nilai awal yang akan disimpan dalam node
   *
   * Pointer node sebelumnya dan selanjutnya akan otomatis memiliki nilai
   * nullptr
   */
  Node(const T& data) : data(data), next(nullptr), previous(nullptr) {}
};

/**
 * @class LinkedList
 * @brief Implementasi Linked List Ganda
 *
 * @tparam T Tipe data yang akan disimpan dalam list
 */
template <typename T>
class LinkedList {
 public:
  /**
   * @brief Constructor default untuk membuat Linked List Ganda
   *
   * Pointer awal dan akhir (head and tail) akan otomatis memiliki nilai nullptr
   * artinya Linked List dalam kondisi kosong
   */
  LinkedList() : head_(nullptr), tail_(nullptr), node_counter_(0) {}

  /**
   * @brief Mengambil jumlah node dalam linked list
   *
   * @return const int jumlah node saat ini
   */
  int count() { return node_counter_; }

  /**
   * @brief Mengambil pointer ke node kepala atau awal
   *
   * @return Node<T>* Pointer ke kepala / awal
   */
  Node<T>* head() const { return head_; }

  /**
   * @brief Mengambil pointer ke node ekor atau akhir
   *
   * @return Node<T>* Pointer ke ekor / akhir
   */
  Node<T>* tail() const { return tail_; }

  /**
   * @brief Menambahkan elemen ke bagian belakang list
   *
   * @param data Nilai yang akan ditambahkan
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
   * @brief Menambahkan elemen ke bagian depan atau tengah list
   *
   * @param data Nilai yang akan ditambahkan
   * @param position Urutan dimana node akan ditambahkan (0 = depan)
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
   * @brief Menghapus node berdasarkan isi data
   *
   * @param data data yang akan dihapus
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
   * @brief Membalik urutan elemen dalam Linked List
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
   * @brief Menghapus seluruh elemen dari linked list
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

  /**
   * @brief Mengecek apakah linked list kosong
   *
   * @return false jika linked list memiliki node / elemen
   * @return true jika linked list kosong
   */
  bool isEmpty() { return !head_; }

 private:
  Node<T>* head_;       /**< Pointer ke elemen kepala / awal */
  Node<T>* tail_;       /**< Pointer ke elemen ekor / akhir */
  size_t node_counter_; /**< Jumlah elemen dallam list */

  /**
   * @brief Mengecek apakah node kosong
   *
   * @param node Pointer node yang ingin dicek
   * @return true jika elemen kosong
   * @return false jika elemen memiliki nilai / isi
   */
  bool isNull(Node<T>* node) { return !node; }
  bool isNotNull(Node<T>* node) { return node; }
};

/**
 * @class FileManager
 * @brief Kelas berisi static method untuk melakukan operasi penyimpanan dan
 * pembacaan data dari file
 *
 * Penyimpanan dan pembacaan secara keseluruhan dilakukan untuk binary file
 */
class FileManager {
 public:
  static constexpr const char* kDatabase{
      "DatabaseLagu.dat"}; /**< Nama file default untuk database lagu */
  static constexpr const char* kPlaylist{
      "DatabasePlaylist.dat"}; /**< Nama file default untuk database playlist */

  /**
   * @brief Menyimpan data bertipe ArrayList ke dalam file biner
   *
   * @tparam T Tipe data dalam ArrayList, harus memiliki method serialize() yang
   * mengatur proses penyimpanan file
   * @param filename Nama file tujuan penyimpanan (termasuk ekstensi)
   * @param data ArrayList berisi elemen yang ingin disiapkan
   * @return true jika semua elemen berhasil disimpan
   * @return false jika terjadi error saat penulisan atau serialisasi
   */
  template <typename T>
  static bool save(const std::string& filename, ArrayList<T>& data) {
    file_ptr = fopen(filename.c_str(), "wb");

    if (!file_ptr) {
      std::cerr << "Tidak dapat membuka file: " << Text::bold(filename)
                << "!\n";
      return false;
    }

    for (size_t i = 0; i < data.size(); i++) {
      T& x = data.arr[i];
      if (!x.serialize()) {
        fclose(file_ptr);
        return false;
      }
    }

    fclose(file_ptr);
    return true;
  }

  /**
   * @brief Membaca data dari file dan disimpan ke dalam ArrayList
   *
   * @tparam T Tipe data yang akan dibaca, harus memiliki method deserialize()
   * yang mengatur proses pembacaan file
   * @param filename Nama file sumber data (termasuk ekstensi)
   * @param data ArrayList tujuan untuk menyimpan hasil pembacaan
   * @return true jika seluruh operasi pembacaan dan deserialisasi berhasil
   * @return false jika terjadi error saat pembacaan atau pembukaan file
   */
  template <typename T>
  static bool load(const std::string& filename, ArrayList<T>& data) {
    FileManager::file_ptr = fopen(filename.c_str(), "rb");

    data.clear(); /**< Memastikan vector tujuan dalma keadaan kosong untuk
                     menghindari duplikasi */

    if (!file_ptr) {
      std::cerr << "Tidak dapat membuka file: " << Text::bold(filename)
                << "!\n";
      return false;
    }

    while (true) {
      T x;
      if (!x.deserialize()) {
        break;
      }
      data.add(x);
    }

    fclose(FileManager::file_ptr);
    return true;
  }

  /**
   * @brief Menyimpan data bertipe LinkedList ke dalam file biner
   *
   * @tparam T tipe elemen dalma linked list, harus memiliki serialize()
   * @param filename Nama file tujuan
   * @param data LinkedList yang akan disimpan
   * @return true jika semua elemen berhasil disimpan
   * @return false jika terjadi error saat penulisan atau serialisasi
   */
  template <typename T>
  static bool save(const std::string& filename, const LinkedList<T>& data) {
    file_ptr = fopen(filename.c_str(), "wb");

    if (!file_ptr) {
      std::cerr << "Tidak dapat membuka file: " << Text::bold(filename)
                << "!\n";
      return false;
    }

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

  /**
   * @brief Membaca data dari file ke dalam LinkedList.
   *
   * @tparam T Tipe data yang akan dimasukkan ke dalam list.
   * @param filename Nama file sumber data.
   * @param data LinkedList yang akan diisi dengan hasil pembacaan.
   * @return true Jika semua data berhasil dibaca dan dideserialisasi.
   * @return false Jika ada kesalahan pembacaan file atau deserialisasi.
   */
  template <typename T>
  static bool load(const std::string& filename, LinkedList<T>& data) {
    file_ptr = fopen(filename.c_str(), "rb");
    if (!file_ptr) {
      std::cerr << "Tidak dapat membuka file: " << Text::bold(filename)
                << "!\n";
      return false;
    }

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

  /**
   * @brief Membaca data dari file.
   *
   * @tparam T Tipe data yang ingin dibaca.
   * @param data Referensi variabel tempat menyimpan hasil pembacaan.
   * @param length Jumlah elemen yang ingin dibaca (default = 1).
   * @return true Jika jumlah data yang dibaca sesuai dengan yang diminta.
   */
  template <typename T>
  static bool read(T& data, size_t length = 1) {
    return fread(&data, sizeof(T), length, file_ptr) == length;
  }

  /**
   * @brief Membaca string (char array) dari file.
   *
   * @param str Buffer tempat menyimpan hasil pembacaan.
   * @param length Panjang karakter yang akan dibaca.
   * @return true Jika seluruh karakter berhasil dibaca.
   */
  static bool read(char* str, size_t length) {
    return fread(str, sizeof(char), length, file_ptr) == length;
  }

  /**
   * @brief Menulis data ke file.
   *
   * @tparam T Tipe data yang akan ditulis.
   * @param data Data yang ingin disimpan.
   * @return true Jika penulisan berhasil.
   */
  template <typename T>
  static bool write(const T& data) {
    return fwrite(&data, sizeof(T), 1, file_ptr) == 1;
  }

  /**
   * @brief Menulis string (char array) ke file.
   *
   * @param str String yang ingin ditulis.
   * @param length Panjang string yang akan ditulis.
   * @return true Jika seluruh karakter berhasil ditulis.
   */

  static bool write(const char* str, size_t length) {
    return fwrite(str, sizeof(char), length, file_ptr) == length;
  }

 private:
  static inline FILE* file_ptr{
      nullptr}; /**< Pointer file global yang digunakan untuk seluruh operasi */
};

/**
 * @class Song
 * @brief Struct Song merepresentasikan satu entitas lagu dalam sistem.
 *
 * Struct ini menyimpan informasi seperti judul lagu, artis, genre, durasi,
 * tahun rilis, dan jumlah pemutaran. Struct ini juga menyediakan fungsi
 * untuk serialisasi dan deserialisasi sehingga dapat disimpan atau dibaca dari
 * file biner.
 */
struct Song {
  static size_t id_counter; /**< variabel static untuk auto-increment ID */
  size_t id;                /**< ID yang nilainya berdasarkan id_counter */
  char title[255];          /**< Judul Lagu */
  char artist[255];         /**< Nama penyanyi / pencipta lagu */
  char genre[255];          /**< Genre lagu */
  int release_year;         /**< Tahun rilis lagu */
  size_t duration;          /**< Durasi lagu dalam detik */
  size_t play_count;        /**< Jumlah pemutaran lagu */

  /**
   * @brief Constructor default, mengatur id berdasarkan id_counter
   *
   */
  Song() : id(id_counter) {
    std::strcpy(title, "");
    std::strcpy(artist, "");
    std::strcpy(genre, "");
  }

  /**
   * @brief Constructor berparameter untuk membuat Song berdasarkan nilai
   * spesifik
   *
   * @param title Judul lagu
   * @param artist Nama penyanyi / pencipta
   * @param genre Genre lagu
   * @param release_year Tahun rilis lagu
   * @param duration Durasi waktu dalam detik
   * @param play_count jumlah pemutaran awal (opsional, default = 0)
   */
  Song(const char* _title, const char* _artist, const char* _genre,
       int release_year, size_t duration, size_t play_count = 0)
      : id(id_counter++),
        release_year(release_year),
        duration(duration),
        play_count(play_count) {
    std::strncpy(title, _title, 254);
    std::strncpy(artist, _artist, 254);
    std::strncpy(genre, _genre, 254);
  }

  /**
   * @brief Operator pembanding untuk mengetahui apakah dua lagu sama.
   *
   * Dua lagu dianggap sama jika `title` dan `artist` nya sama.
   *
   * @param rhs Lagu lain yang ingin dibandingkan.
   * @return true jika kedua lagu memiliki judul dan artis yang sama.
   */
  bool operator==(const Song& rhs) const {
    // return std::memcmp(title, rhs.title, sizeof(title) == 0) &&
    //        memcmp(artist, rhs.artist, sizeof(title) == 0);

    return id == rhs.id;
  }

  /**
   * @brief Operator pembanding tidak sama.
   *
   * @param rhs Lagu lain yang ingin dibandingkan.
   * @return true jika lagu berbeda (berdasarkan title dan artist).
   */
  bool operator!=(const Song& rhs) const { return !(*this == rhs); }

  /**
   * @brief Operasi penyimpanan seluruh atribut lagu ke dalam file biner.
   *
   * Fungsi ini digunakan oleh FileManager untuk menyimpan data `Song`
   * menggunakan fungsi `write()` yang sesuai tipe datanya.
   *
   * @return true jika seluruh proses penulisan berhasil.
   */
  bool serialize() const {
    return FileManager::write<size_t>(id) && FileManager::write(title, 255) &&
           FileManager::write(genre, 255) && FileManager::write(artist, 255) &&
           FileManager::write<int>(release_year) &&
           FileManager::write<size_t>(duration) &&
           FileManager::write<size_t>(play_count);
  }

  /**
   * @brief Operasi pembacaan atribut lagu dari file biner.
   *
   * Fungsi ini digunakan oleh FileManager untuk memuat data `Song`
   * menggunakan fungsi `read()` yang sesuai tipe datanya.
   * Panjang string dibaca terlebih dahulu, lalu isinya.
   *
   * @return true jika seluruh proses pembacaan berhasil.
   */
  bool deserialize() {
    if (!FileManager::read<size_t>(id)) return false;

    if (!FileManager::read(&title[0], 255)) return false;
    if (!FileManager::read(&genre[0], 255)) return false;
    if (!FileManager::read(&artist[0], 255)) return false;

    title[254] = genre[254] = artist[254] = '\0';

    if (!FileManager::read<int>(release_year)) return false;
    if (!FileManager::read<size_t>(duration)) return false;
    if (!FileManager::read<size_t>(play_count)) return false;

    Song::id_counter++;
    return true;
  }
};

/**
 * @class SongSorter
 * @brief Implementasi algoritma sorting (pengurutan) untuk objek Song.
 *
 * Kelas ini menyediakan metode Bubble Sort dan Quick Sort yang dapat digunakan
 * secara fleksibel dengan comparator untuk berbagai field (judul, id, dll).
 */

class SongSorter {
 public:
  /**
   * @brief Comparator untuk mengurutkan berdasarkan judul lagu (ascending).
   *
   * Comparator digunakan untuk membuat algoritma pengurutan dapat digunakan
   * secara fleksibel untuk mengurutkan data berdasrkan field tertentu
   */
  static inline auto by_title = [](const Song& a, const Song& b) {
    return std::strcmp(a.title, b.title) <= 0;
  };

  /**
   * @brief Comparator untuk mengurutkan berdasarkan ID lagu (ascending).
   */
  static inline auto by_id = [](const Song& a, const Song& b) {
    if (a.id == 0) return false;
    if (b.id == 0) return true;
    return a.id < b.id;
  };

  /**
   * @brief Comparator untuk mengurutkan berdasarkan jumlah pemutaran lagu
   * (ascending).
   */
  static inline auto by_play_count = [](const Song& a, const Song& b) {
    return a.play_count < b.play_count;
  };

  /**
   * @brief Comparator untuk mengurutkan berdasarkan tahun rilis lagu
   * (ascending).
   */
  static inline auto by_release_year = [](const Song& a, const Song& b) {
    return a.release_year < b.release_year;
  };

  /**
   * @brief Memastikan ArrayList Song telah terurut berdasarkan ID.
   *
   * Memilih algoritma quickSort untuk data > 30 elemen, bubbleSort jika ≤ 30.
   * @param data ArrayList lagu yang akan diurutkan.
   */
  static void ensureSortedByID(ArrayList<Song>& data) {
    if (data.size() > kQuickSortThreshold) {
      quickSort(data, 0, data.size() - 1, by_id);
    } else {
      bubbleSort(data, by_id);
    }
  }

  /**
   * @brief Mengurutkan data menggunakan Bubble Sort.
   *
   * @param data ArrayList data lagu.
   * @param comparator Fungsi pembanding untuk menentukan urutan.
   */
  static void bubbleSort(
      ArrayList<Song>& data,
      std::function<bool(const Song&, const Song&)> comparator) {
    if (data.empty()) return;

    for (size_t i = 0; i < data.size() - 1; ++i) {
      bool swapped = false;

      for (size_t j = 0; j < data.size() - i - 1; ++j) {
        if (comparator(data.arr[j + 1], data.arr[j])) {
          swap(&data.arr[j], &data.arr[j + 1]);
          swapped = true;
        }
      }

      if (!swapped) break;
    }
  }

  /**
   * @brief Mengurutkan data menggunakan Quick Sort.
   *
   * @param data ArrayList data lagu.
   * @param awal Indeks awal.
   * @param akhir Indeks akhir.
   * @param comparator Fungsi pembanding untuk menentukan urutan.
   */
  static void quickSort(
      ArrayList<Song>& data, size_t awal, size_t akhir,
      std::function<bool(const Song&, const Song&)> comparator) {
    if (awal < akhir) {
      size_t pivot = partition(data, awal, akhir, comparator);
      if (pivot > awal) quickSort(data, awal, pivot - 1, comparator);
      if (pivot < akhir) quickSort(data, pivot + 1, akhir, comparator);
    }
  }

  /**
   * @brief Membalikkan urutan comparator menjadi descending.
   *
   * @tparam T Tipe data (misalnya: Song).
   * @param comparator Fungsi pembanding ascending.
   * @return Comparator yang hasilnya dibalik.
   */
  template <typename T>
  static std::function<bool(const T&, const T&)> reverseOrder(
      std::function<bool(const T&, const T&)> comparator) {
    return [comparator](const T& a, const T& b) { return comparator(b, a); };
  }

 private:
  static constexpr size_t kQuickSortThreshold = 30;
  /**
   * @brief Fungsi partisi pada algoritma quickSort.
   *
   * @param data ArrayList lagu.
   * @param index_awal Indeks awal.
   * @param index_akhir Indeks akhir.
   * @param comparator Comparator untuk sorting.
   * @return Indeks pivot setelah partisi.
   */
  static size_t partition(
      ArrayList<Song>& data, size_t index_awal, size_t index_akhir,
      std::function<bool(const Song&, const Song&)> comparator) {
    Song pivot = data.arr[index_akhir];
    size_t i = index_awal;

    for (size_t j = index_awal; j < index_akhir; j++) {
      if (comparator(data.arr[j], pivot)) {
        i++;
        swap(&data.arr[j], &data.arr[i]);
      }
    }

    swap(&data.arr[i + 1], &data.arr[index_akhir]);

    return (i + 1);
  }

  /**
   * @brief Menukar posisi dua elemen dalam array lagu.
   *
   * @param data1 Pointer ke elemen pertama.
   * @param data2 Pointer ke elemen kedua.
   */
  static void swap(Song* data1, Song* data2) {
    Song temporary = *data1;
    *data1 = *data2;
    *data2 = temporary;
  }
};

/**
 * @class SongSearcher
 * @brief Implementasi algoritma searching (penarian) untuk objek Song.
 *
 * Kelas ini mendukung pencarian berdasarkan genre, artis, dan judul lagu
 * menggunakan algoritma Binary Search atau Linear Search
 */
class SongSearcher {
 public:
  static int result_index; /**< Indeks hasil dari pencarian binarySearch */

  /**
   * @brief Mencari lagu berdasarkan ID menggunakan algoritma Binary Search.
   *
   * Akan mengurutkan data terlebih dahulu jika belum terurut.
   *
   * @param target_id ID lagu yang dicari.
   * @param data ArrayList lagu.
   * @return true jika ditemukan, false jika tidak.
   */
  static bool binarySearch(size_t target_id, ArrayList<Song>& data) {
    if (data.empty()) return false;

    SongSorter::ensureSortedByID(data);

    int start{0}, mid{0}, end{static_cast<int>(data.size()) - 1};
    while (start <= end) {
      mid = (start + end) / 2;

      if (target_id == data.arr[mid].id) {
        result_index = mid;
        return true;
      }

      if (target_id > data.arr[mid].id) {
        start = mid + 1;
      } else if (target_id < data.arr[mid].id) {
        end = mid - 1;
      }
    }
    result_index = -1;
    return false;
  }

  /**
   * @brief Mencari lagu berdasarkan genre (case-insensitive & substring).
   *
   * @param genre Genre yang dicari.
   * @param data ArrayList lagu.
   * @return ArrayList lagu yang cocok.
   */
  static ArrayList<Song> searchByGenre(const std::string& genre,
                                       ArrayList<Song>& data) {
    char normalized_genre[255];
    std::strncpy(normalized_genre, normalizeString(genre).c_str(), 254);

    return linearSearch(data, [normalized_genre](const Song& song) {
      char song_genre[255];
      std::strncpy(song_genre, normalizeString(song.genre).c_str(), 254);
      return std::strcmp(song_genre, normalized_genre) != 0 ? false : true;
    });
  }

  /**
   * @brief Mencari lagu berdasarkan artis (case-insensitive & substring).
   *
   * @param artist Nama artis.
   * @param data ArrayList lagu.
   * @return ArrayList lagu yang cocok.
   */
  static ArrayList<Song> searchByArtist(const std::string& artist,
                                        ArrayList<Song>& data) {
    char normalized_artist[255];
    std::strncpy(normalized_artist, normalizeString(artist).c_str(), 254);

    return linearSearch(data, [normalized_artist](const Song& song) {
      char song_artist[255];
      std::strncpy(song_artist, normalizeString(song.artist).c_str(), 254);
      return std::strcmp(song_artist, normalized_artist) != 0 ? false : true;
    });
  }

  /**
   * @brief Mencari lagu berdasarkan judul (case-insensitive & substring).
   *
   * @param title Judul lagu.
   * @param data ArrayList lagu.
   * @return ArrayList lagu yang cocok.
   */
  static ArrayList<Song> searchByTitle(const std::string& title,
                                       ArrayList<Song>& data) {
    char normalized_title[255];
    std::strncpy(normalized_title, normalizeString(title).c_str(), 254);

    return linearSearch(data, [normalized_title](const Song& song) {
      char song_title[255];
      std::strncpy(song_title, normalizeString(song.title).c_str(), 254);
      return std::strcmp(song_title, normalized_title) != 0 ? false : true;
    });
  }

 private:
  /**
   * @brief Implementasi pencarian linear (filter) berdasarkan comparator.
   *
   * @param data ArrayList lagu.
   * @param comparator Fungsi yang menentukan apakah lagu cocok.
   * @return ArrayList lagu yang memenuhi kondisi comparator.
   */
  static ArrayList<Song> linearSearch(
      ArrayList<Song>& data, std::function<bool(const Song&)> comparator) {
    size_t index{0};
    ArrayList<Song> filtered_library;
    while (index < data.size()) {
      if (comparator(data.arr[index])) {
        filtered_library.add(data.arr[index]);
      }
      index++;
    }
    return filtered_library;
  }

  /**
   * @brief Menormalisasi string menjadi huruf kecil semua.
   *
   * @param string String asli.
   * @return String hasil normalisasi.
   */
  static std::string normalizeString(const std::string& string) {
    std::string normalized = string;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return normalized;
  }
};

/**
 * @class SongLibrary
 * @brief Kelas yang mengatur seluruh operasi terhadap Database Lagu
 *
 */
class SongLibrary {
 public:
  /**
   * @brief Mengambil data lagu yang ada di database
   *
   * @return ArrayList<Song>& Vector berisi lagu dalam database
   */
  ArrayList<Song>& database() { return database_; }

  /**
   * @brief Menambahkan lagu ke dalam database
   *
   * @param song Data yang akan ditambahkan
   */
  void addToLibrary(Song song) {
    if (!SongSearcher::binarySearch(song.id, database_)) {
      database_.add(song);
    }

    SongSorter::ensureSortedByID(database_);
  }

  /**
   * @brief Menghapus lagu dari database berdasarkan ID
   *
   * @param target_id ID lagu yang akan dihapus
   */
  void removeSongById(size_t target_id) {
    if (SongSearcher::binarySearch(target_id, database_)) {
      for (size_t i = 0; i < database_.size() - 1; i++) {
        if (database_.arr[i].id == target_id) {
          for (size_t j = i; j < database_.size() - 1; j++) {
            database_.arr[j] = database_.arr[j + 1];
          }
          database_.decreaseSize();
          break;
        }
      }
    }
  }

  /**
   * @brief Mengambil data dari database berdasarkan ID
   *
   * @param target_id ID lagu yang akan diambil
   * @return Song& Referensi Lagu dari hasil pencarian
   */
  Song& getSongById(int target_id) {
    if (SongSearcher::binarySearch(target_id, database_)) {
      int result = SongSearcher::result_index;
      SongSearcher::result_index = -1;
      return database_.arr[result];
    }
  }

 private:
  ArrayList<Song>
      database_; /**< Vector berisi seluruh lagu yang ada (database) */
};

/**
 * @class Playlist
 * @brief Kelas yang merepresentasikan sebuah daftar putar (playlist) berisi
 * lagu-lagu.
 *
 * Kelas ini memungkinkan pengguna untuk menambahkan, menghapus, memutar,
 * menjeda, dan menampilkan lagu-lagu dalam playlist. Lagu-lagu disimpan dalam
 * struktur data LinkedList.
 */
class Playlist {
 public:
  static size_t
      id_counter; /**< Variable untuk auto-increment ID setiap playlist */

  /**
   * @brief Constructor default, menginisialisasi playlist dengan ID unik
   * berdasarkan id_counter
   *
   */
  Playlist() : id_(id_counter) {}

  /**
   * @brief Constructor dengan nama playlist
   *
   * @param name Nama dari playlist
   */
  Playlist(std::string name) : id_(id_counter) {
    std::strcpy(name_, name.c_str());
  }

  /**
   * @brief Mengambil data ID dari playlist
   *
   * @return const size_t ID unik playlist
   */
  size_t id() const { return id_; }

  /**
   * @brief Mengambil nama playlist
   *
   * @return const std::string& Nama playlist
   */
  std::string name() { return std::string(name_); }

  /**
   * @brief Mengambil daftar lagu dalam playlist
   *
   * @return LinkedList<Song>& Referensi ke LinkedList berisi lagu-lagu
   */
  LinkedList<Song>& list() { return list_; }

  /**
   * @brief Menambahkan lagu ke dalam playlist
   *
   * @param song Lagu yang ingin ditambahkan
   */
  void addSong(const Song& song) {
    list_.push(song);
    if (current_song_ == nullptr) {
      current_song_ = list_.head();
    }
  }

  /**
   * @brief Menghapus lagu dari playlist
   *
   * @param song Lagu yang akan dihapus
   */
  void removeSong(const Song& song) { list_.deleteNode(song); }

  /**
   * @brief Operasi penyimpanan atribut playlist ke dalam file
   *
   * @return true jika playlist berhasil disimpan
   */
  bool serialize() const { return FileManager::write(name_, 255); }

  /**
   * @brief Operasi pembacaan atribut playlist dari file
   *
   * @return true jika seluruh pembacaan berjalan dengan baik
   * @return false jika terjadi kegagalan dalam pembacaan file
   */
  bool deserialize() {
    if (!FileManager::read(&name_[0], 255)) return false;
    name_[254] = '\0';

    Playlist::id_counter++;
    return true;
  }

  /**
   * @brief Menampilkan daftar lagu dalam bentuk tabel yang terformat
   *
   */
  void displayList() {
    Node<Song>* x = list_.head();

    std::cout << "\n\n"
              << std::setfill('-') << std::setw(61) << "-" << std::setfill(' ')
              << "\n";
    ;
    std::cout << std::left << std::setw(5 + Text::kAnsi) << Text::bold("ID")
              << std::setw(18 + Text::kAnsi) << Text::bold("Judul")
              << std::setw(18 + Text::kAnsi) << Text::bold("Artis")
              << std::setw(8 + Text::kAnsi) << Text::bold("Tahun")
              << std::setw(8 + Text::kAnsi) << Text::bold("Durasi")
              << std::setw(15 + Text::kAnsi) << Text::bold("Diputar") << "\n";

    std::cout << std::setfill('-') << std::setw(61) << "-" << std::setfill(' ')
              << "\n";

    while (x != nullptr) {
      std::cout << std::left << std::setw(5) << x->data.id << std::setw(18)
                << std::string(x->data.title).substr(0, 17) << std::setw(18)
                << std::string(x->data.artist).substr(0, 17) << std::setw(8)
                << x->data.release_year << std::setw(8) << x->data.duration
                << std::setw(15) << x->data.play_count << "\n";
      x = x->next;
    }
    std::cout << "\n\n";
  }

  /**
   * @brief Menampilkan animasi seolah sedang memutar lagu dari playlist
   *
   */
  void playbackLoop(bool& display) {
    current_song_ = list_.head();

    while (current_song_) {
      int remaining = current_song_->data.duration / 10;
      std::string title = std::strcat(current_song_->data.title, "        ");
      size_t move_print = 0;

      int tick{0};
      while (remaining > 0) {
        std::cout << "\033[s";
        std::cout << "\033[2;24H";

        std::string scrolling_title = title.substr(move_print, 8);
        if (scrolling_title.length() < 8) {
          scrolling_title += title.substr(0, 8 - scrolling_title.length());
        }

        if (display) {
          std::cout << "Now Playing: " << scrolling_title
                    << " | Remaining: " << std::setw(3) << remaining << "s"
                    << std::flush;
        }

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
  char name_[255];           /**< Nama playlist */
  size_t id_;                /**< ID unik dari playlist */
  LinkedList<Song> list_;    /**< Daftar lagu dalam bentuk LinkedList */
  Node<Song>* current_song_; /**< Pointer ke lagu yang sedang diputar */

  /**
   * @brief Mengganti current_song ke lagu selanjutnya dalam list
   *
   */
  void nextSong() {
    if (current_song_ && current_song_->next) {
      current_song_ = current_song_->next;
    } else {
      current_song_ = list_.head();
    }
  }
};

/// Inisialisasi static variabel
size_t Song::id_counter = 1;
size_t Playlist::id_counter = 1;
int SongSearcher::result_index = -1;

/**
 * @class RAiVFY
 * @brief Kelas utama yang mengatur seluruh fungsionalitas aplikasi RAiVFY.
 *
 * Kelas ini menangani antarmuka pengguna, pemutaran playlist, pengelolaan lagu,
 * pencarian lagu, serta manajemen file untuk penyimpanan data.
 */
class RAiVFY {
 public:
  /**
   * @brief Memulai aplikasi RAiVFY.
   *
   * Jika terdapat playlist, lagu akan langsung diputar di thread terpisah.
   * Setelah itu, program menampilkan menu utama.
   */
  void ignite() {
    size_t index = 0;
    isLogin = isAdmin = isPlaying = false;
    bool display = false;

    while (true) {
      if (!isLogin) {
        isLogin = login();
        continue;
      }

      if (isAdmin) {
        display = false;
        menuAdmin();
        continue;
      }

      if (!playlist_library.empty() && !isPlaying) {
        display = true;

        while (index < playlist_library.size() - 1 &&
               playlist_library.arr[index].list().isEmpty()) {
          index++;
        }

        std::thread playback(&Playlist::playbackLoop,
                             &playlist_library.arr[index], std::ref(display));
        playback.detach();
        isPlaying = true;
      }

      mainMenu();
    }
  }

  /**
   * @brief Memuat data dari file eksternal ke dalam memori.
   *
   * Memuat daftar lagu dan playlist dari file menggunakan FileManager.
   */
  void load() {
    FileManager::load<Song>(FileManager::kDatabase, library.database());
    FileManager::load<Playlist>(FileManager::kPlaylist, playlist_library);

    for (size_t i = 0; i < playlist_library.size(); i++) {
      FileManager::load<Song>(playlist_library.arr[i].name() + ".dat",
                              playlist_library.arr[i].list());
    }
  }

 private:
  SongLibrary library; /**< Objek untuk mengatur seluruh operasi database */
  ArrayList<Playlist>
      playlist_library; /** Koleksi playlist yang dimiliki user */
  bool isAdmin,
      isLogin;    /**< Menentukan user login atau belum & admin atau bukan */
  bool isPlaying; /**< Playback status */
  /**
   * @brief Menampilkna menu utama kepada pengguna
   *
   */
  void mainMenu() {
    clearScreen();
    printBorder(Text::bold(" RAiVFY "), 11);
    std::cout << "\n";
    std::cout << " 1.  Buat Playlist\n";
    std::cout << " 2.  Lihat Playlist\n";
    std::cout << " 3.  Daftar Lagu\n";
    std::cout << " 4.  Cari Lagu\n";
    std::cout << " 0.  Keluar\n\n";

    enum mainMenu { KELUAR, BUAT, LIHAT, LIST, SEARCH };

    int choice = getNumberInput<int>(Text::bold(" > Pilih Menu : "));

    switch (choice) {
      case KELUAR: {
        std::cout << "\n\n Pilihan: \n";
        std::cout << " 1. Login\n";
        std::cout << " 0. Exit\n";

        enum menuKeluar { EXIT, LOGIN };
        int choice = getNumberInput<int>("\n > Ketik input: ");
        switch (choice) {
          case LOGIN:
            isLogin = false;
            break;
          case EXIT:
            exit(0);
            break;
        }
        break;
      }
      case BUAT:
        buatPlaylist();
        break;
      case LIHAT:
        daftarPlaylist();
        break;
      case LIST:
        daftarLagu(library.database());
        break;
      case SEARCH:
        searchingLagu();
        break;
      default:
        std::cout << " Pilihan menu tidak tersedia!\n";
    }
  }

  /**
   * @brief Membuat playlist baru berdasarkan input pengguna
   *
   */
  void buatPlaylist() {
    clearScreen();
    std::string namaPlaylist;

    printBorder(Text::bold(" Buat Playlist "), 10);
    std::cout << "\n Masukkan nama playlist baru kamu: ";

    std::getline(std::cin >> std::ws, namaPlaylist);
    bool isValid = true;

    for (size_t i = 0; i < playlist_library.size(); i++) {
      if (playlist_library.arr[i].name() == namaPlaylist) {
        isValid = false;
      }
    }

    if (!isValid) {
      std::cout << " Playlist [" << Text::bold(namaPlaylist)
                << "] sudah ada!\n";
    } else {
      Playlist new_playlist(namaPlaylist);
      playlist_library.add(new_playlist);
      FileManager::save(FileManager::kPlaylist, playlist_library);
      FileManager::save(namaPlaylist + ".dat", new_playlist.list());

      std::cout << "\n Playlist [" << Text::bold(namaPlaylist)
                << "] berhasil dibuat!\n";
      std::cout << " Saatnya mengisi playlist ini dengan lagu favoritmu!\n\n";
    }

    waitForInput();
    playlistAction(playlist_library.size() - 1);
  }

  /**
   * @brief Menampilkan seluruh playlist yang tersedia
   *
   */
  void daftarPlaylist() {
    clearScreen();
    printBorder(Text::bold(" Buat Playlist "), playlist_library.size() + 7);

    std::cout << "\n";
    for (size_t i = 0; i < playlist_library.size(); i++) {
      Playlist& playlist = playlist_library.arr[i];
      std::cout << " " << playlist.id() << ". " << std::left << std::setw(15)
                << playlist.name().substr(0, 14) << "("
                << playlist.list().count() << " songs)"
                << "\n";
    }
    std::cout << "\n 0. Kembali ke Main Menu\n";

    size_t choice = getNumberInput<size_t>(Text::bold("\n > Pilih Menu: "));

    if (choice <= 0 || choice > playlist_library.size()) {
      if (choice != 0) std::cout << " Tidak ada playlist yang sesuai!\n";
      waitForInput();
      return;
    }

    playlistAction(choice - 1);
  }

  /**
   * @brief Menangani aksi-aksi yang dapat dilakukan pengguna terhadap
   * playlist
   *
   * @param index playlist yang dipilih atau akan diinteraksi
   */
  void playlistAction(int index) {
    clearScreen();
    if (index < 0) {
      std::cout << "Input index tidak valid!\n";
      return;
    }

    printBorder(
        " Playlist [" + Text::bold(playlist_library.arr[index].name()) + "] ",
        11);
    std::cout << "\n" << Text::underline(" Pilihan Menu:\n");
    std::cout << " 1. Lihat isi playlist\n";
    std::cout << " 2. Tambah lagu ke playlist\n";
    std::cout << " 3. Hapus lagu dari playlist\n";
    std::cout << " 4. Hapus playlist\n";
    std::cout << " 5. Reverse playlist\n";
    std::cout << " 0. Kembali ke menu utama\n\n";

    enum action { KEMBALI, LIHAT, TAMBAH, HAPUS_LAGU, HAPUS_PLAYLIST, REVERSE };

    int choice = getNumberInput<int>(" > Pilih menu: ");
    switch (choice) {
      case KEMBALI: {
        return;
      }
      case LIHAT: {
        clearScreen();

        printBorder(" Playlist [" +
                        Text::bold(playlist_library.arr[index].name()) + "] ",
                    playlist_library.arr[index].list().count() + 3);

        if (!playlist_library.arr[index].list().isEmpty()) {
          playlist_library.arr[index].displayList();
        } else {
          std::cout << " Playlist masih kosong!\n";
        }

        std::cin.ignore();
        waitForInput();
        break;
      }
      case TAMBAH: {
        isPlaying = true;
        ArrayList<Song> filtered = searchingLagu();

        size_t selected_id = getNumberInput<size_t>(" Masukan ID lagu : ");
        if (SongSearcher::binarySearch(selected_id, filtered)) {
          playlist_library.arr[index].addSong(
              filtered.arr[SongSearcher::result_index]);
        }

        FileManager::save(playlist_library.arr[index].name() + ".dat",
                          playlist_library.arr[index].list());

        playlist_library.arr[index].list().clear();
        FileManager::load(playlist_library.arr[index].name() + ".dat",
                          playlist_library.arr[index].list());
        break;
      }
      case REVERSE: {
        playlist_library.arr[index].list().reverse();
        if (!playlist_library.arr[index].list().isEmpty()) {
          playlist_library.arr[index].displayList();
        } else {
          std::cout << "\n Playlist masih kosong!\n";
        }

        std::cin.ignore();
        waitForInput();
        break;
      }
      case HAPUS_LAGU: {
        if (playlist_library.arr[index].list().isEmpty()) {
          std::cout << "\n Playlist masih kosong!\n";
          std::cin.ignore();
          waitForInput();
          break;
        }

        playlist_library.arr[index].displayList();

        size_t selected_id = getNumberInput<size_t>(" Masukan ID lagu : ");
        if (SongSearcher::binarySearch(selected_id, library.database())) {
          playlist_library.arr[index].removeSong(
              library.database().arr[SongSearcher::result_index]);
        }

        FileManager::save(playlist_library.arr[index].name() + ".dat",
                          playlist_library.arr[index].list());

        playlist_library.arr[index].list().clear();
        FileManager::load(playlist_library.arr[index].name() + ".dat",
                          playlist_library.arr[index].list());
        break;
      }
      case HAPUS_PLAYLIST: {
        deleteFilePlaylist(index);

        size_t target = playlist_library.arr[index].id();

        for (size_t i = 0; i < playlist_library.size(); i++) {
          if (playlist_library.arr[i].id() == target) {
            for (size_t j = i; j < playlist_library.size(); j++) {
              playlist_library.arr[j] = playlist_library.arr[j + 1];
            }
            playlist_library.decreaseSize();
          }
        }

        FileManager::save(FileManager::kPlaylist, playlist_library);

        Playlist::id_counter = 1;
        FileManager::load(FileManager::kPlaylist, playlist_library);
        break;
      }
      default:
        std::cout << " Pilihan menu tidak tersedia!\n";
    }

    clearScreen();
  }

  /**
   * @brief Menampilkan daftar lagu dalam bentuk tabel.
   *
   * @param data ArrayList berisi lagu-lagu yang ingin ditampilkan.
   */
  void daftarLagu(ArrayList<Song>& data) {
    clearScreen();

    printBorder(Text::bold(" Daftar Lagu "), data.size() + 3);
    if (library.database().empty()) {
      std::cout << "Database masih kosong!\n";
      std::cin.ignore();
      waitForInput();
      return;
    }

    tabelLagu(data, false);

    std::cout << "\n Ingin mengurutkan berdasarkan:\n";
    std::cout << " 1. Tahun Rilis\n";
    std::cout << " 2. Most Played\n";
    std::cout << " 3. Judul\n";
    std::cout << " 0. Kembali\n\n";

    int isDescending{0};

    int pilihan = getNumberInput<int>(" > Pilih menu: ");
    switch (pilihan) {
      case 1:
        isDescending = opsiSorting() - 1;

        SongSorter::quickSort(
            library.database(), 0, library.database().size() - 1,
            (isDescending)
                ? SongSorter::reverseOrder<Song>(SongSorter::by_release_year)
                : SongSorter::by_release_year);

        daftarLagu(data);
        break;
      case 2:
        isDescending = opsiSorting() - 1;

        SongSorter::quickSort(
            library.database(), 0, library.database().size() - 1,
            (isDescending)
                ? SongSorter::reverseOrder<Song>(SongSorter::by_play_count)
                : SongSorter::by_play_count);

        daftarLagu(data);
        break;
      case 3:
        isDescending = opsiSorting() - 1;

        SongSorter::quickSort(
            library.database(), 0, library.database().size() - 1,
            (isDescending)
                ? SongSorter::reverseOrder<Song>(SongSorter::by_title)
                : SongSorter::by_title);

        daftarLagu(data);
        break;
      case 0:
        return;
        break;
      default:
        std::cout << " Pilihan menu tidak tersedia!\n";
        break;
    }
  }

  /**
   * @brief Menampilkna daftar lagu dalam bentuk tabel terformat
   *
   * @param data
   */
  void tabelLagu(ArrayList<Song>& data, bool display_id = true) {
    clearScreen();
    std::cout << "\n\n"
              << std::setfill('-') << std::setw(61) << "-" << std::setfill(' ')
              << "\n";
    std::cout << std::left << std::setw(5 + Text::kAnsi) << Text::bold("No")
              << std::setw(18 + Text::kAnsi) << Text::bold("Judul")
              << std::setw(18 + Text::kAnsi) << Text::bold("Artis");
    if (display_id) std::cout << std::setw(5 + Text::kAnsi) << Text::bold("ID");
    std::cout << std::setw(8 + Text::kAnsi) << Text::bold("Tahun")
              << std::setw(8 + Text::kAnsi) << Text::bold("Durasi")
              << std::setw(15 + Text::kAnsi) << Text::bold("Diputar") << "\n";
    std::cout << std::setfill('-') << std::setw(61) << "-" << std::setfill(' ')
              << "\n";

    for (size_t index = 0; index < data.size(); index++) {
      auto& song = data.arr[index];
      std::cout << std::left << std::setw(5) << index + 1 << std::setw(18)
                << std::string(song.title).substr(0, 17) << std::setw(18)
                << std::string(song.artist).substr(0, 17);
      if (display_id) std::cout << std::setw(5) << song.id;
      std::cout << std::setw(8) << song.release_year << std::setw(8)
                << song.duration << std::setw(12) << song.play_count << "\n";
    }

    std::cout << "\n\n";
  }

  /**
   * @brief Menampilkan opsi pengurutan (asceing / descending)
   *
   * @return int Pilihan pengguna (1 = ascending, 2 = descending)
   */
  int opsiSorting() {
    std::cout << "\n Mau Urutan Berdasarkan :\n";
    std::cout << " 1. Ascending\n";
    std::cout << " 2. Descending\n";
    std::cout << " Pilihan: ";

    return getNumberInput<int>();
  }

  /**
   * @brief Menampilkan menu pencarian lagu
   *
   * @return ArrayList<Song> ArrayList berisi lagu hasil pencarian
   */
  ArrayList<Song> searchingLagu() {
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
    ArrayList<Song> filtered;
    switch (choice) {
      case JUDUL: {
        std::cout << " Masukkan judul: ";

        std::string kata_kunci;
        std::cin.ignore();
        std::getline(std::cin, kata_kunci);
        filtered = SongSearcher::searchByTitle(kata_kunci, library.database());

        printBorder(Text::bold(" Search by [Title] "), filtered.size() + 5);
        if (!filtered.empty()) {
          tabelLagu(filtered);
        } else {
          std::cout << " Lagu dengan judul " << kata_kunci
                    << " tidak ditemukan!\n";
        }
        break;
      }
      case ARTIST: {
        std::cout << " Masukkan artis: ";

        std::string kata_kunci;
        std::cin.ignore();
        std::getline(std::cin, kata_kunci);
        filtered = SongSearcher::searchByArtist(kata_kunci, library.database());
        printBorder(Text::bold(" Search by [Artist] "), filtered.size() + 5);
        if (!filtered.empty()) {
          tabelLagu(filtered);
        } else {
          std::cout << " Lagu dengan artis " << kata_kunci
                    << " tidak ditemukan!\n";
          waitForInput();
        }
        break;
      }
      case GENRE: {
        std::cout << " Masukkan kata kunci: ";

        std::string kata_kunci;
        std::cin.ignore();
        std::getline(std::cin, kata_kunci);
        filtered = SongSearcher::searchByGenre(kata_kunci, library.database());
        printBorder(Text::bold(" Search by [Genre] "), filtered.size() + 5);
        if (!filtered.empty()) {
          tabelLagu(filtered);
        } else {
          clearScreen();
          std::cout << " Lagu dengan genre " << kata_kunci
                    << " tidak ditemukan!\n";
          waitForInput();
        }
        break;
      }
      case ALL:
        filtered = library.database();
        printBorder(Text::bold(" Show All Songs "), filtered.size() + 5);
        if (!filtered.empty()) {
          tabelLagu(library.database());
        } else {
          clearScreen();
          std::cout << "Database masih kosong!\n";
          waitForInput();
        }
        std::cin.ignore();
        break;
      default:
        std::cout << "Pilihan menu tidak tersedia!";
    }

    return filtered;
  }

  /**
   * @brief Membersihkan layar terminal sesuai sistem operasi
   *
   */
  void clearScreen() {
#ifdef __WIN32__
    system("cls");
#else
    system("clear");
#endif
  }

  /**
   * @brief Menampilkan border (garis atas dan bawah) serta judul halaman
   *
   * @param title Judul menu yang ingin ditampilkan di tengah
   * @param lines Jumlah baris konten untuk menentukan lokasi garis bawah
   */
  void printBorder(const std::string& title, int lines) {
    const int width = 61;
    Text::moveCursor(1, 1);
    std::cout << std::setfill('-') << std::setw(width) << '-';
    Text::moveCursor(lines, 1);
    std::cout << std::setfill('-') << std::setw(width) << '-'
              << std::setfill(' ');

    Text::moveCursor(1, width / 2 - 7);
    std::cout << title;

    Text::moveCursor(2, 1);
  }

  /**
   * @brief Menunggu input pengguna untu melanjutkan
   *
   */
  void waitForInput() {
#ifdef __WIN32__
    system("pause");
#else
    std::cout << "\033[K Tekan tombol enter untuk melanjutkan...";
    std::cin.get();
#endif
  }

  /**
   * @brief Memastikan input numerik yang valid dari user
   *
   * @tparam TypeTemplate tipe data numerik (int, size, etc)
   * @param prompt Teks prompt yang akan ditampilkan saat meminta input
   * @return TypeTemplate Nilai input numerik yang valid dari pengguna
   */
  template <typename TypeTemplate>
  static TypeTemplate getNumberInput(const std::string& prompt = "") {
    static_assert(std::is_arithmetic<TypeTemplate>::value,
                  "Hanya tipe data numerik!");

    TypeTemplate value{};

    do {
      std::cout << "\033[s";
      std::cout << prompt;
      std::cin >> value;

      if (!std::cin.fail()) {
        return value;
      }

      std::cout << "\033[u"; /**< resetore the saved position */
      std::cout << "\n Hanya menerima input angka!";
      std::cout << "\033[F"; /**< move cursor up one line */
      std::cout << "\033[K"; /**< Clear entire line */
      std::cin.clear();
      std::cin.ignore();
    } while (true);
  }

  void deleteFilePlaylist(int index) {
    std::string command;
#ifdef __WIN32__
    command = "delete '" + playlist_library.arr[index].name() + ".dat'";
#else
    command = "rm '" + playlist_library.arr[index].name() + ".dat'";
#endif
    system(command.data());
  }

  bool login() {
    clearScreen();
    printBorder("Selamat Datang di RAiVFY!", 7);
    std::string username;
    std::cout << "\nSilahkan masuk dengan username Anda!\n";
    std::cout << Text::bold("Username: ");

    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, username);

    if (username == "admin") {
      isAdmin = true;
    }

    return true;
  }

  void menuAdmin() {
    clearScreen();
    printBorder("Hapus Lagu", 10);
    std::cout << "Size: " << library.database().size();
    std::cout << "\n 1.  Tambah Lagu\n";
    std::cout << " 2.  Hapus Lagu\n";
    std::cout << " 3.  Daftar Lagu\n";
    std::cout << " 4.  Hapus Database\n";
    std::cout << " 0.  Keluar\n\n";

    int choice = getNumberInput<int>(" Pilihan Menu : ");
    enum mainMenu { KELUAR, TAMBAH, HAPUS, DAFTAR, DELETE_ALL };
    switch (choice) {
      case KELUAR:
        isAdmin = false;
        isLogin = false;
        return;
        break;
      case TAMBAH:
        tambahLaguByAdmin();
        break;
      case HAPUS:
        hapusLagubyAdmin();
        break;
      case DAFTAR:
        daftarLagu(library.database());
        break;
      case DELETE_ALL:
        system("rm DatabaseLagu.dat");
        library.database().clear();
        break;
      default:
        std::cout << "Menu tidak tersedia!\n";
    }
  }

  void tambahLaguByAdmin() {
    clearScreen();
    printBorder("Tambah Lagu", 12);

    char judul[255], artis[255], genre[255];
    int tahun, diputar, durasi;

    std::cout << "\n Masukkan detail lagu di bawah ini:\n";

    std::cout << Text::bold(" Judul Lagu\t: ");
    std::cin.ignore();
    std::cin.getline(judul, 254);

    std::cout << Text::bold(" Nama Artis\t: ");
    std::cin.getline(artis, 254);

    std::cout << Text::bold(" Genre\t\t: ");
    std::cin.getline(genre, 254);

    tahun = getNumberInput<int>(Text::bold(" Tahun Rilis\t: "));
    durasi = getNumberInput<int>(Text::bold(" Durasi (detik)\t: "));

    diputar = getNumberInput<int>(Text::bold(" Play Count\t: "));

    library.addToLibrary(Song(judul, artis, genre, tahun, durasi, diputar));
    FileManager::save(FileManager::kDatabase, library.database());

    std::cout << "\nLagu berhasil ditambahkan ke database!\n";
  }

  void hapusLagubyAdmin() {
    clearScreen();
    printBorder("Hapus Lagu", library.database().size() + 8);
    if (library.database().empty()) {
      std::cout << "Database masih kosong!\n";
      std::cin.ignore();
      waitForInput();
      return;
    }

    tabelLagu(library.database());
    size_t idLagu =
        getNumberInput<size_t>("\nMasukkan ID lagu yang ingin dihapus: ");

    library.removeSongById(idLagu);

    Song::id_counter = 1;

    FileManager::save(FileManager::kDatabase, library.database());
    FileManager::load(FileManager::kDatabase, library.database());

    std::cout << "\nLagu Berhasil Di Hapus!\n";
  }
};

int main() {
  RAiVFY app;
  app.load();
  app.ignite();

  return 0;
}