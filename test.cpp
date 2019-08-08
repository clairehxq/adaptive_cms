//#include "tcs.hpp"
//#include "cms.hpp"
#include "adaptive_cms.hpp"
#include "cms.hpp"
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
  cms ccms(200, 5, 3216);
  printf("constructed\n");
  acms.check_config();
  
  std::set<uint32_t> hvalues;
  printf("add\n");
  for (uint64_t i=0; i<1; i++){
    for (uint32_t j=0; j<5; j++){
      printf("%u th add\n", j*3);
      acms.addDeep(i);
      acms.addDeep(i);
      acms.addDeep(i);
      ccms.add(i);
      ccms.add(i);
      ccms.add(i);
      printf("\n");
    }
  }
  
  printf("countmindeep\n");
  for(uint64_t i=0; i<1; i++){
    for(uint32_t j=0; j<5; j++) {
      uint32_t hvalue = acms.hash(i,j);
      printf("%u hashed to %u ", j, hvalue);
    }
    //printf("%d estimate count %ul\n", i, acms.countMinDeep(i));
    printf("%u estimate count %u\n", i, acms.countMinDeep(i));
    printf("%u ccms estimate count %u\n", i, ccms.countMin(i));
  }

  acms.del();
  ccms.del();
  return 1;
}

  
	
