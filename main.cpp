#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

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

 private:
  Node<T>* tail_;
  size_t node_counter_;

  bool isNull(Node<T>* node) { return !node; }
  bool isNotNull(Node<T>* node) { return node; }
  bool isEmpty() { return !head_; }
};

class FileManager {
 public:
  template <typename T>
  static bool save(const std::string& filename, const std::vector<T>& data) {
    file_ptr = fopen(filename.c_str(), "wb");
    if (!file_ptr) return false;

    for (T x : data) {
      if (!x.serialize(file_ptr)) {
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

    T x;
    while (true) {
      if (!x.deserialize(file_ptr)) {
        break;
      }
      data.push_back(x);
    }

    fclose(file_ptr);
    return true;
  }

  template <typename T>
  static bool save(const std::string& filename, const LinkedList<T>& data) {
    file_ptr = fopen(filename.c_str(), "wb");
    if (!file_ptr) return false;

    Node<T>* node = data.head();
    while (node) {
      if (!node->data.serialize(file_ptr)) {
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

    T x;
    while (!feof(file_ptr)) {
      if (!x.deserialize(file_ptr)) {
        break;
      }
      data.push(x);
    }

    fclose(file_ptr);
    return true;
  }

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

// Data Lagu
struct Song {
  static size_t id_counter;
  size_t id;
  std::string title, artist, genre;
  size_t duration, playCount;
  int release_year;

  Song() = default;
  Song(const std::string& title, const std::string& artist,
       const std::string& genre, int release_year, size_t duration,
       size_t playCount = 0)
      : title(title),
        artist(artist),
        genre(genre),
        release_year(release_year),
        duration(duration) {
    id = Song::id_counter;
    Song::id_counter++;
  }

  bool operator==(const Song& rhs) const {
    return title == rhs.title && artist == rhs.artist;
  }

  bool operator!=(const Song& rhs) const { return !(*this == rhs); }

  bool serialize(FILE* file_ptr) const {
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

  bool deserialize(FILE* file_ptr) {
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

// Data User
enum UserType { LISTENER, ADMIN };
struct User {
  size_t id;
  std::string username, password;
  UserType type;
};

class SongSorter {
 public:
  static inline auto by_title = [](const Song& a, const Song& b) {
    return a.title < b.title;
  };

  static inline auto by_id = [](const Song& a, const Song& b) {
    return a.id < b.id;
  };

  static inline auto by_play_count = [](const Song& a, const Song& b) {
    return a.playCount < b.playCount;
  };

  static inline auto by_release_year = [](const Song& a, const Song& b) {
    return a.release_year < b.release_year;
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

class SongSearcher {
 public:
  static int result_index;
  static bool binarySearch(size_t target_id, std::vector<Song>& data) {
    if (data.empty()) return false;

    SongSorter::ensureSortedByID(data);

    size_t start{0}, mid{0}, end{data.size() - 1};
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
  static std::vector<Song> linearSearch(
      const std::vector<Song>& data,
      std::function<bool(const Song&)> comparator) {
    size_t index{0};
    std::vector<Song> filtered_library;
    while (index < data.size()) {
      if (comparator(data[index])) {
        filtered_library.push_back(data[index]);
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
  Playlist(std::string name) : name_(name) { play = false; }

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
    }
  }

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

  void removeSong(const Song& song) { list_.deleteNode(song); }

  bool pausePlay() { return !play; }

 private:
  LinkedList<Song> list_;
  Node<Song>* current_song_;
  bool play;
  std::string name_;
  std::string description_;
};

class ConsoleUI {
 public:
  static void clearScreen() {
#ifdef __WIN32__
    system("cls");
#else
    system("clear");
#endif
  }
};

size_t Song::id_counter = 1;
int SongSearcher::result_index = 0;

int main() {
  ConsoleUI::clearScreen();
  SongLibrary library;
  std::vector<Playlist> playlist_user;

  return 0;
}