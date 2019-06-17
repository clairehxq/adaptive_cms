#include "tcs.hpp"
#include "cms.hpp"
#include "adaptive_cms.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>

int main(){
  std::string line;
  std::ifstream counts_file ("zipf_counts.txt");
  std::ifstream syn_file ("zipf_stream.txt");
  std::vector<std::pair<uint64_t, int>> counts;
  char k[20];
  int c;
  uint64_t key;
  //counts_file.open();
  while (counts_file >> k >> c ){
    if (k == "stream")
      break;
    std::pair<uint64_t, int> p (kw2num(k), c);
    counts.push_back(p);
  }


  //CountMinSketch<100, 10, 1<<5> cms;
  adaptive_cms<1<<4, 1<<2, 1<<18, 1000> cms;
  adaptive_cms<1<<7, 1<<2, 1<<18, 1000> cms2;
  int cnum =0;
  while (syn_file>>k){
    //std::cout<<k<<std::endl;
    key = kw2num(k);
    cms.addDeep(key);
    cms2.add(key);
    cnum ++;
  }


  for (auto& p: counts){
    std::string key = num2kw(p.first);
    uint32_t estimate_deep = cms.countMinDeep(p.first);
    uint32_t estimate = cms.countMin(p.first);
    uint32_t estimate_big = cms2.countMin(p.first);
    int true_count = p.second;
    printf("%u, %u, %u, %d\n", estimate_deep, estimate_big, estimate, true_count);
  }
    
  cms.del();
  cms2.del();
  //printf("%d\n", cms.countMin(key));
  return 1;
}
    
    
