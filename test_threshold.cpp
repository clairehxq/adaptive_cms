//#include "tcs.hpp"
//#include "cms.hpp"
#include "adaptive_cms.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>

int main(){
  std::string line;
  std::ifstream counts_file ("data/count_k1024_1048576");
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
  for (int l=0; l<15; l++){
    std::ifstream syn_file ("data/stream_k1024_1048576");
    //adaptive_cms cms(1<<8, 1<<1, 1, 1<<10, 1000);
    adaptive_cms cms2(1<<8, 1<<1, 1, 1<<10, 1<<l);
    //cms.check_config();
    cms2.check_config();
    int cnum =0;
    
    
    while (syn_file>>k){
      //std::cout<<k<<std::endl;
      key = kw2num(k);
      //cms.addDeep(key);
      cms2.add(key);
      cnum ++;
    }

    
    int n=0;
    for (auto& p: counts){
      std::string key = num2kw(p.first);
      //uint32_t estimate_deep = cms.countMinDeep(p.first);
      //uint32_t estimate = cms.countMin(p.first);
      uint32_t estimate_big = cms2.countMin(p.first);
      int true_count = p.second;
      printf("estimate big %u, true count %u", estimate_big, true_count);
      if (n>10)
	break;
      n++;
    }
    
    //cms.del();
    cms2.del();
  }
  //printf("%d\n", cms.countMin(key));
  return 1;
}
    
    
