#include "DataStream.hpp"

#include <unistd.h>

#include <cstring>

DataStream::DataStream()
    : seq_(0), totalRead_(0), totalWrite_(0), isEOF_(false) {}
DataStream::~DataStream() {
  std::list<Chunk *>::iterator it;
  for (it = list_.begin(); it != list_.end(); it++) {
    delete *it;
  }
}
DataStream::Chunk::Chunk(int seq, int size)
    : seq_(seq), size_(size), offset_(0) {
  buffer_ = new char[size];
}

DataStream::Chunk::~Chunk() { delete[] buffer_; }

int DataStream::readStr(const std::string &str) {
  Chunk *chunk = new Chunk(seq_++, str.size());
  std::memcpy(chunk->buffer_, str.c_str(), str.size());
  totalRead_ += str.size();
  list_.push_back(chunk);
  return str.size();
}

int DataStream::readFile(int fd) {
  if (isEOF_) {
    return 0;
  }
  if (list_.size() == CHUNK_LIST_SIZE) {
    return DELAYED_FILE_READ;
  }
  Chunk *chunk = new Chunk(seq_++, BODY_CHUNK_DEFAULT_SIZE);
  int size = read(fd, chunk->buffer_, BODY_CHUNK_DEFAULT_SIZE);
  isEOF_ = size == 0 || (size > 0 && size < BODY_CHUNK_DEFAULT_SIZE);
  if (size <= 0) {
    delete chunk;
    return size;
  }
  totalRead_ += size;
  chunk->size_ = size;
  list_.push_back(chunk);
  return size;
}

int DataStream::writeToClient(int fd) {
  if (list_.size() == 0) {
    return 0;
  }
  Chunk *chunk = list_.front();
  int left = chunk->size_ - chunk->offset_;
  int size = write(fd, chunk->buffer_ + chunk->offset_, left);
  if (size <= 0) {
    return size;
  }
  totalWrite_ += size;
  chunk->offset_ += size;
  if (chunk->size_ - chunk->offset_ == 0) {
    list_.pop_front();
    delete chunk;
  }
  return size;
}

int DataStream::getTotalRead() const { return totalRead_; }

int DataStream::getTotalWrite() const { return totalWrite_; }

bool DataStream::isEOF() const { return list_.size() == 0 && isEOF_; }

void DataStream::setEof(bool eof) { isEOF_ = eof; }
