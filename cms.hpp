#ifndef CMS_HPP
#define CMS_HPP
#include "tcs.hpp"
#include <cstdint>
#include <climits>
#include <random>
#include <algorithm>

template<unsigned W, unsigned D, unsigned P>
class CountMinSketch {
public:
  CountKeywordList candidate;
  
  CountMinSketch()
  {
    this->init();
  }
  
  void init() {
    this->__data.resize(D*(2+W));
    this->__ider.resize(D*W);
    this->a = &(this->__data[0]);
    this->b = this->a + D;
    this->c = this->b + D;

    this->cc = &(this->__ider[0]);
    
    //this->lhh = &(this->__ider[0]);
    std::mt19937 gen(100284);
    std::uniform_int_distribution<> dist(1, P);
    for (auto i=0; i<D; ++i) {
      this->a[i] = dist(gen);
      this->b[i] = dist(gen);
    }
    this->clear();
  }

  void clear() {
    std::fill(this->c, this->c+W*D, 0);
    std::fill(this->cc, this->cc+W*D, CountKeyword(0, 0));
    this->global_c = 0;
    //fprintf(stderr, "%u, %u\n", cc[0].first, cc[0].second);
    //fprintf(stderr, "%u, %u\n", cc[D].first, cc[D].second);
    //fprintf(stderr, "%u, %u\n", cc[2*D].first, cc[2*D].second);
    //fprintf(stderr, "%u, %u\n", cc[2*D+10].first, cc[2*D+10].second);
  }

  void add(uint64_t value) {
    for (auto i=0; i<D; ++i)
      ++this->c[i*W + this->hash(value, i)];
    this->global_c++;
  }

  uint32_t addAndCount(uint64_t value) {
    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i){
      uint32_t hv = this->hash(value, i);
      cnt = std::min(cnt, ++this->c[i*W+hv]); //increment cms
      CountKeyword l = this->cc[i*W+hv];
      if (l.second == value) //lhh key matches the value
	this->cc[W*i+hv].first ++;
      else{
	this->cc[W*i+hv].first --;
	if (this->cc[W*i+hv].first <=0){
	  this->cc[W*i+hv].second = value;
	  this->cc[W*i+hv].first = 1;
	}
      }
    }
    //++this->c[i*W + this->hash(value, i)];
    this->global_c ++;
    return cnt;
  }

  uint32_t addAndCount(uint64_t value, uint32_t delta) {
    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i){
      uint32_t hv = this->hash(value, i);
      cnt = std::min(cnt, (this->c[i*W+hv] += delta)); //increment cms
      CountKeyword l = this->cc[i*W+hv];
      if (l.second == value) //lhh key matches the value
	this->cc[i*W+hv].first += delta;
      else{
	if (this->cc[i*W+hv].first > delta)
	  cc[i*W+hv].first-=delta;
	else{
	  this->cc[i*W+hv].second = value;
	  this->cc[i*W+hv].first = delta;
	}
      }

    }
    this->global_c ++;
    return cnt;
  }

  void printCounts(uint64_t value) const {
    for (auto i=0; i<D; ++i)
      fprintf(stderr, "%u=%u ", this->hash(value, i), this->c[i*W + this->hash(value, i)]);
    fprintf(stderr, "\n");
  }

  void remove(uint64_t value) {
    for (auto i=0; i<D; ++i)
      --this->c[i*W + this->hash(value, i)];
  }

  uint32_t hash(uint64_t value, int hi) const {
    return ((value*this->a[hi] + this->b[hi]) % P) % W;
  }

  uint32_t countMin(uint64_t value) const {
    uint32_t cnt = UINT_MAX;
    for (auto i=0; i<D; ++i)
      cnt = std::min(cnt, this->c[i*W + this->hash(value, i)]);
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

  void topk_query(int k, CountKeywordList &candidate){
    candidate.clear();
    int threshold = this->global_c / (20*D);
    fprintf(stderr, "threshold is %d", threshold);
    CountKeyword p;
    for (int i=0; i<W; ++i){
      for (int j=0; j<1; ++j){
	//if ((this->cc[i].first) & (this->c[j*W + this->hash(this->cc[i].second, j)] > threshold)){
	if (this->cc[i].first){
	  uint32_t v = this->countMin(this->cc[i].second);
	  if (v > threshold){
	      p.second = this->cc[i].second;
	      p.first = v;
	  
	      candidate.push_back(p);
	  }
	}
      }
    }
    std::sort(candidate.begin(), candidate.end(),  CountKeywordSort);
    fprintf(stderr, "check candidate, candidate has %zu items\n", candidate.size());
    //check heavy hitter the
    //uint64_t t = kw2num("the");
    //fprintf(stderr, "check word the, the has %d items\n", this->countMin(t));

  }
  
private:
  uint32_t *a, *b, *c, global_c;
  std::vector<uint32_t> __data;
  std::vector<CountKeyword> __ider;
  CountKeyword *cc;
};

#endif

