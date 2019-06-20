#ifndef ADAPTIVE_HPP
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <climits>
#include <random>
#include <algorithm> 

static inline std::string num2kw(uint64_t value)
{
  const char base36[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
  char buffer[14];
  int offset = sizeof(buffer);

  buffer[--offset] = '\0';
  for (; value; value/=36)
    buffer[--offset] = base36[value % 36];
  return std::string(&buffer[offset]);
}

static inline uint64_t base36dec(const char *text)
{
  char buf[13];
  strncpy(buf, text, 12);
  buf[12] = '\0';
  return strtoul(buf, NULL, 36);
}

inline uint64_t kw2num(const std::string &kw) {
  return kw2num(kw.c_str());
}

inline uint64_t kw2num(const char *cur) {
  uint64_t result = 0;
  for (int digitCnt=0; *cur; ++cur) {
    int digit = (*cur>'9')?(tolower(*cur)+10-(int)'a'):(*cur-'0');
    if (digit<0 || digit>35)
      break;
    else if (++digitCnt<13)
      result = result*36UL + digit;
  }
  return result;
}

//template<unsigned W, unsigned D, unsigned R, unsigned P, unsigned T>
class adaptive_cms{
  
private:
  unsigned W, D, R, P, T;
  uint32_t *__dataDeep,*__data;
  uint32_t *a, *b, *c, *bb, *cd;
  void init(unsigned W, unsigned D, unsigned R, unsigned P, unsigned T){
    this->__dataDeep = new uint32_t[D * W * R];
    this->__data = new uint32_t[D * (3+W)];
    this->a = this->__data; //&(this->__data[0]);
    this->b = this->a + D;
    this->bb = this->b+D;
    this->c = this->bb + D;
    this->cd = this->__dataDeep; //&(this->__dataDeep[0]);
    this->W = W;
    this->D = D;
    this->R = R;
    this->P = P;
    this->T = T;
    std::mt19937 gen(100284+W+D+P+T);
    std::uniform_int_distribution<> dist(1, P);

    for (auto i=0; i<D; ++i) {
      this->a[i] = dist(gen);
      this->b[i] = dist(gen);
      this->bb[i] = dist(gen);
    }
    this->clear(W, D, R);
  }

public:
  int global_c = 0;
  uint32_t max_v=0;
  adaptive_cms(unsigned W, unsigned D, unsigned R, unsigned P, unsigned T){
    this->init(W, D, R, P, T);
  }

  void clear(int W, int D, int R){
    std::fill(this->c, this->c+W*D, 0);
    std::fill(this->cd, this->cd+W*D*R, 0);
  }
  void del(){
    delete[] this->__data;
    delete[] this->__dataDeep;
  }

  int at_capacity(uint64_t value, int i){
    int j = this->hash(value, i);
    if ( this->c[i*this->W + j] > this->T)
      return 1;
    else
      return 0;
  }

  int at_capacity(uint64_t value){
    int c=0;
    for (int i=0; i<this->D; i++)
      c += this->at_capacity(value, i);
    if (c >= this->D)
      return 1;
    else
      return 0;
  }

  void check_config(){
    printf("Width %u, height %u, rows %u, p %u, threshold %u\n", this->W, this->D, this->R, this->P, this->T);
  }

  void add(uint64_t value) {
    uint32_t v;
    unsigned W,D,R,T;
    W = this->W;
    D = this->D;
    R = this->R;
    T = this->T;
    for (auto i=0; i<D; ++i){
      ++ this->c[i*W + this->hash(value, i)];
    }
    this->global_c++;
  }

  void addDeep(uint64_t value) {
    unsigned W,D,R,T, v;
    W = this->W;
    D = this->D;
    R = this->R;
    T = this->T;
    for (auto i=0; i<D; ++i){
      uint32_t j = this->hash(value, i);
      ++ this->c[i*W + j];
      v = this->c[i*W + j];
      if (this->c[i*W + j] > T){
	++ this->cd[i*W*R + j*R + this->hashDeep(value, i, j)];
      }
    }
    this->global_c++;
  }
  
  void printCounts(uint64_t value) const {
    uint32_t W, D, R, T;
    W = this->W;
    D = this->D;
    R = this->R;
    T = this->T;

    for (auto i=0; i<D; ++i)
      fprintf(stderr, "%u=%u ", this->hash(value, i), this->c[i*W + this->hash(value, i)]);
    fprintf(stderr, "\n");
  }

  void printCountsDeep(uint64_t value) const {
    int W = this->W;
    int D = this->D;
    int R = this->R;
    for (auto i=0; i<D; ++i){
      uint32_t j = this->hash(value, i);
      for (int l=0; l<this->R; l++){
	printf("%u ", this->cd[i*W*R + j*R + l]);
      }
      fprintf(stderr, "\n");
    }
  }

  void remove(uint64_t value) {
    uint32_t W, D, R, T;
    W = this->W;
    D = this->D;
    R = this->R;

    for (auto i=0; i<D; ++i)
      --this->c[i*W + this->hash(value, i)];
  }

  uint32_t hash(uint64_t value, int hi) const {
    uint32_t W, D, R, T, P;
    W = this->W;
    D = this->D;
    R = this->R;
    P = this->P;
    return ((value*this->a[hi] + this->b[hi]) % P) % W;
  }

  uint32_t hashDeep(uint64_t value, uint32_t i, uint32_t j) const{
    uint32_t W, D, R, T, P;
    W = this->W;
    D = this->D;
    R = this->R;
    P = this->P;
    return ((value * this->bb[i] + a[i]) % (P-1) % R);
  }
  
  uint32_t countMin(uint64_t value) const {
    uint32_t W, D, R;
    W = this->W;
    D = this->D;
    R = this->R;

    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i)
      cnt = std::min(cnt, this->c[i*W + this->hash(value, i)]);
    return cnt;
  }

  uint32_t countMinDeep(uint64_t value) const {
    uint32_t W, D, R, T;
    W = this->W;
    D = this->D;
    R = this->R;
    T = this->T;

    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i){
      uint32_t j = hash(value, i);
      if (this->c[i*W + j] <= T){
	cnt = std::min(cnt, this->c[i*W + j]);
      }
      else{
	uint32_t bv = this->cd[i*W*R + j*R + this->hashDeep(value, i, j)];
	cnt = std::min(cnt, T+bv);
      }
    }
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
