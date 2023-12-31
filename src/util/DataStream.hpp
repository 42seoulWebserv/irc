#ifndef BodyStream_HPP_
#define BodyStream_HPP_

#include <fstream>
#include <list>
#include <string>

#define BODY_CHUNK_DEFAULT_SIZE 65536
#define CHUNK_LIST_SIZE 4
#define DELAYED_FILE_READ -2

class DataStream {
 public:
  DataStream();
  ~DataStream();
  int push(int fd);
  int push(std::ifstream &file);
  int push(const std::string &str);
  int popToClient(int fd);
  int popToFile(std::ofstream &file);
  int popToFile(int fd);
  int getTotalRead() const;
  int getTotalWrite() const;
  bool isEOF() const;
  void markEOF();
  void writeTo(DataStream &data);

 private:
  class Chunk {
   public:
    Chunk(int seq, int size);
    ~Chunk();
    int seq_;
    int size_;
    int offset_;
    char *buffer_;

   private:
    Chunk(const Chunk &chunk);
    Chunk &operator=(const Chunk &chunk);
  };
  int seq_;
  int totalRead_;
  int totalWrite_;
  bool isEOF_;
  std::list<Chunk *> list_;
};

#endif
