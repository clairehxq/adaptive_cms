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
  std::ifstream counts_file ("counts.txt");
  std::ifstream syn_file ("syn_shuf.txt");
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
  adaptive_cms<1<<10, 1<<3, 1<<18, 1000> cms;
  adaptive_cms<1<<13, 1<<3, 1<<18, 1000> cms2;
  int cnum =0;

  
  while (syn_file>>k){
    //std::cout<<k<<std::endl;
    key = kw2num(k);
    cms.addDeep(key);
    cms2.add(key);
    cnum ++;
  }

  
  for (int i=0; i<3; i++){
    std::pair<uint64_t, int> p = counts.at(1);
    uint64_t key = p.first;
    cms.addDeep(p.first);
    cms2.add(key);
    cms.printCountsDeep(p.first);

    printf("count min is %u", cms.countMin(p.first));
    printf("count min Deep is %u", cms.countMinDeep(p.first));
    printf("count min2 is %u", cms2.countMin(p.first));

    p = counts.at(5);
    cms.addDeep(p.first);
    cms2.add(key);
    cms.printCountsDeep(p.first);

    printf("count min is %u", cms.countMin(p.first));
    printf("count min Deep is %u", cms.countMinDeep(p.first));
    printf("count min2 is %u", cms2.countMin(p.first));
  }
  
  int n=0;
  for (auto& p: counts){
    std::string key = num2kw(p.first);
    uint32_t estimate_deep = cms.countMinDeep(p.first);
    uint32_t estimate = cms.countMin(p.first);
    uint32_t estimate_big = cms2.countMin(p.first);
    int true_count = p.second;
    
    //printf("print count deep\n");
    //cms.printCountsDeep(p.first);
    //printf("print count\n");
    //cms.printCounts(p.first);
    //printf("print count big\n");
    //cms2.printCounts(p.first);
    //printf("eva %u, %u, %u, %d\n", estimate_deep, estimate_big, estimate, true_count);
    if (n>10)
      break;
    n++;
  }
    
  cms.del();
  cms2.del();
  //printf("%d\n", cms.countMin(key));
  return 1;
}
    
    
