#ifndef BodyStream_HPP_
#define BodyStream_HPP_

#include <list>
#include <string>

#define BODY_CHUNK_DEFAULT_SIZE 65536
#define CHUNK_LIST_SIZE 4
#define DELAYED_FILE_READ -2

class ResponseStream {
 public:
  ResponseStream();
  ~ResponseStream();
  int readStr(const std::string &str);
  int readFile(int fd);
  int writeToClient(int fd);
  int getTotalRead() const;
  int getTotalWrite() const;
  bool isEOF() const;
  void setEof(bool eof);

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
