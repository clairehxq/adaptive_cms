//#include "tcs.hpp"
//#include "cms.hpp"
#include "adaptive_cms.hpp"
#include "cms.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>

int main(){
  std::string line;
  std::ifstream counts_file ("data/count_k1024_1048576");
  std::vector<std::pair<uint64_t, int> > counts;
  char k[20];
  int c;
  uint64_t key;
  //counts_file.open();
  while (counts_file >> k >> c ){
    std::pair<uint64_t, int> p (kw2num(k), c);
    counts.push_back(p);
  }
  printf("counts has %u\n", counts.size());

  for (int l=0; l<10; l++){
    std::ifstream syn_file ("data/stream_k1024_1048576");
    adaptive_cms acms(1<<6, 1<<1, 1<<2, 1<<10, 1000);
    cms bcms(1<<8, 1<<1, 1<<10);
    acms.check_config();
    bcms.check_config();
    int cnum =0;
    
    
    while (syn_file>>k){
      //std::cout<<k<<std::endl;
      key = kw2num(k);
      acms.addDeep(key);
      bcms.add(key);
      cnum ++;
    }

    printf("all added\n");
    for (int i=0; i<3; i++){
      std::pair<uint64_t, int> p = counts.at(1+i);
      uint64_t key = p.first;
      acms.addDeep(p.first);
      bcms.add(key);
      acms.printCountsDeep(p.first);
      
      printf("count min is %u\n", acms.countMin(p.first));
      printf("count min Deep is %u\n", acms.countMinDeep(p.first));
      printf("count min2 is %u\n", bcms.countMin(p.first));
      
    }
    
    int n=0;
    for (auto& p: counts){
      std::string key = num2kw(p.first);
      uint32_t estimate_deep = acms.countMinDeep(p.first);
      uint32_t estimate = acms.countMin(p.first);
      uint32_t estimate_big = bcms.countMin(p.first);
      int true_count = p.second;
      
      if (n>10)
	break;
      n++;
    }
    
    acms.del();
    bcms.del();
  }
  //printf("%d\n", cms.countMin(key));
  return 1;
}
    
    
