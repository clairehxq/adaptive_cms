#ifndef CMS_HPP
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <climits>
#include <random>
#include <algorithm> 


//template<unsigned W, unsigned D, unsigned R, unsigned P, unsigned T>
class cms{
  
private:
  unsigned W, D, P;
  uint32_t *__dataDeep,*__data;
  uint32_t *a, *b, *c;
  void init(unsigned W, unsigned D, unsigned P){
    this->__data = new uint32_t[D * (2+W)];
    this->a = this->__data; //&(this->__data[0]);
    this->b = this->a + D;
    this->c = this->b + D;
    this->W = W;
    this->D = D;
    this->P = P;
    std::mt19937 gen(100284+W+D+P);
    std::uniform_int_distribution<> dist(1, P);

    for (auto i=0; i<D; ++i) {
      this->a[i] = dist(gen);
      this->b[i] = dist(gen);
    }
    this->clear(W, D);
  }

public:
  int global_c = 0;
  uint32_t max_v=0;
  cms(unsigned W, unsigned D, unsigned P){
    this->init(W, D, P);
  }

  void clear(int W, int D){
    std::fill(this->c, this->c+W*D, 0);
  }
  void del(){
    delete[] this->__data;
  }

  void check_config(){
    printf("Width %u, height %u, p %u\n", this->W, this->D, this->P);
  }

  void add(uint64_t value) {
    uint32_t v;
    unsigned W,D,R;
    W = this->W;
    D = this->D;
    for (auto i=0; i<D; ++i){
      ++ this->c[i*W + this->hash(value, i)];
    }
    this->global_c++;
  }

  void printCounts(uint64_t value) const {
    uint32_t W, D;
    W = this->W;
    D = this->D;

    for (auto i=0; i<D; ++i)
      fprintf(stderr, "%u=%u ", this->hash(value, i), this->c[i*W + this->hash(value, i)]);
    fprintf(stderr, "\n");
  }


  void remove(uint64_t value) {
    uint32_t W, D;
    W = this->W;
    D = this->D;

    for (auto i=0; i<D; ++i)
      --this->c[i*W + this->hash(value, i)];
  }

  uint32_t hash(uint64_t value, int hi) const {
    uint32_t W, D, P;
    W = this->W;
    D = this->D;
    P = this->P;
    return ((value*this->a[hi] + this->b[hi]) % P) % W;
  }

  uint32_t countMin(uint64_t value) const {
    uint32_t W, D;
    W = this->W;
    D = this->D;

    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i)
      cnt = std::min(cnt, this->c[i*W + this->hash(value, i)]);
    return cnt;
  }

  const uint32_t *rawCounts() const {
    return this->c;
  }

  void setRawCounts(const uint32_t *_c) {
    std::copy(_c, _c+W*D, this->c);
    for (int i=0; i<W; ++i)
      this->global_c += this->c[i];
  }

  void addRawCounts(const uint32_t *_c) {
    for (auto i=0; i<W*D; ++i)
      this->c[i] += _c[i];
  } 
    
};

#endif
