//#include "tcs.hpp"
//#include "cms.hpp"
#include "adaptive_cms.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>
#include <set>
#include <cstdio>


int main(){
  printf("hello world\n");
  
  adaptive_cms acms(200, 5, 4, 3216, 0);
  printf("constructed\n");
  acms.check_config();
  
  std::set<uint32_t> hvalues;
  for (uint64_t i=0; i<1531712; i++){
    for (int j=0; j<5; j++){
      hvalues.insert(acms.hash(i, j));
    }
  }

  for (auto& i : hvalues){
    printf("%u ", i);
  }
  printf("length %d\n", hvalues.size());
  return 1;
}

  
	
