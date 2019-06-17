#ifndef ADAPTIVE_HPP
#include "cms.hpp"
#include <iostream>
#include <vector>

template<unsigned W, unsigned D, unsigned P, unsigned T>
class adaptive_cms{
  //unsigned W, D, P, T;

private:
  uint32_t *__dataDeep,*__data;
  uint32_t *a, *b, *c, *bb, *cd;
  void init(){
    this->__dataDeep = new uint32_t[D * W * (1<<3)];
    this->__data = new uint32_t[D * (3+W)];
    this->a = this->__data; //&(this->__data[0]);
    this->b = this->a + D;
    this->bb = this->b+D;
    this->c = this->bb + D;
    this->cd = this->__dataDeep; //&(this->__dataDeep[0]);
    //printf("allocated\n");
    std::mt19937 gen(100284+W+D+P+T);
    std::uniform_int_distribution<> dist(1, P);

    for (auto i=0; i<D; ++i) {
      //printf("gen %d\n", i);
      this->a[i] = dist(gen);
      this->b[i] = dist(gen);
      this->bb[i] = dist(gen);
    }
    
    //printf("start clear\n");	
    this->clear();

  }

    
public:
  int global_c = 0;
  uint32_t max_v=0;
  adaptive_cms(){
    this->init();
  }

  void clear(){
    std::fill(this->c, this->c+W*D, 0);
    std::fill(this->cd, this->cd+W*D*(1<<3), 0);
  }
  void del(){
    delete[] this->__data;
    delete[] this->__dataDeep;
  }

  bool at_capacity(uint64_t value, int i){
    int j = this->hash(value, i);
    if ( this->c[i*W + j] > T)
      return 1;
    else
      return 0;
  }
  
  void add(uint64_t value) {
    uint32_t v;
    for (auto i=0; i<D; ++i){
      ++ this->c[i*W + this->hash(value, i)];
    }
    this->global_c++;
  }

  void addDeep(uint64_t value) {
    uint32_t v;
    for (auto i=0; i<D; ++i){
      uint32_t j = this->hash(value, i);
      ++ this->c[i*W + j];
      v = this->c[i*W + j];
      if (this->c[i*W + j] > T){
	++ this->cd[i*W*(1<<3) + j*(1<<3) + this->hashDeep(value, i, j)];
	//printf("location %u", i*W*(1<<3) + j*(1<<3) + this->hashDeep(value, i, j));
	//printf("%u hash deep to %u\n", i, this->hashDeep(value, i, j));
      }
      //if (this->c[i*W + this->hash(value, i)] > this->max_v)
      //this->max_v = this->c[i*W + this->hash(value, i)];
    }
    this->global_c++;
  }
  
  void printCounts(uint64_t value) const {
    for (auto i=0; i<D; ++i)
      fprintf(stderr, "%u=%u ", this->hash(value, i), this->c[i*W + this->hash(value, i)]);
    fprintf(stderr, "\n");
  }

  void printCountsDeep(uint64_t value) const {
    for (auto i=0; i<D; ++i){
      uint32_t j = this->hash(value, i);
    
      fprintf(stderr, "%u %u %u %u %u %u %u %u\n", this->cd[i*W*(1<<3) + j*(1<<3) + 0], \
	      this->cd[i*W*(1<<3) + j*(1<<3) + 1],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 2],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 3],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 4],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 5],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 6],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 7],\
	      this->cd[i*W*(1<<3) + j*(1<<3) + 8]);
    
    }
    fprintf(stderr, "\n");
  }

  void remove(uint64_t value) {
    for (auto i=0; i<D; ++i)
      --this->c[i*W + this->hash(value, i)];
  }

  uint32_t hash(uint64_t value, int hi) const {
    return ((value*this->a[hi] + this->b[hi]) % P) % W;
  }

  uint32_t hashDeep(uint64_t value, uint32_t i, uint32_t j) const{
    return ((value * this->bb[i] + a[i]) % (P-1) % (1<<3));
  }
  
  uint32_t countMin(uint64_t value) const {
    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i)
      cnt = std::min(cnt, this->c[i*W + this->hash(value, i)]);
    return cnt;
  }

  uint32_t countMinDeep(uint64_t value) const {
    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i){
      uint32_t j = hash(value, i);
      if (this->c[i*W + j] <= T){
	cnt = std::min(cnt, this->c[i*W + j]);
      	//printf("this blocked value is not at capacity %u\n", this->c[i*W+j]);
      }
      //if (cnt > T)
      else{
	uint32_t bv = this->cd[i*W*(1<<3) + j*(1<<3) + this->hashDeep(value, i, j)];
	//printf("this blocked value is %u\n", bv);
	cnt = std::min(cnt, T+bv);
      }
    }
    return cnt;
  }  

  uint32_t countMaxIder(uint64_t value) const {
    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i)
      cnt = std::min(cnt, this->cc[i*W + this->hash(value, i)].first);
    return cnt;
  }

  const uint32_t *rawCounts() const {
    return this->c;
  }

  void setRawCounts(const uint32_t *_c) {
    std::copy(_c, _c+W*D, this->c);
    for (int i=0; i<W; ++i)
      this->s += this->c[i];
  }

  void addRawCounts(const uint32_t *_c) {
    for (auto i=0; i<W*D; ++i)
      this->c[i] += _c[i];
  }
 
  
    
};

#endif
