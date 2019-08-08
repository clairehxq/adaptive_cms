//#include "tcs.hpp"
//#include "cms.hpp"
#include "hashutil.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>
#include <set>
#include <cstdio>


int main(){
  printf("hello world\n");

  uint64_t key=0;
  for (int i=0; i<5; i++){
    uint32_t hvalue1 = MurmurHash64A(&key, sizeof(key), 123);
    uint32_t hvalue2 = MurmurHash64A(&key, 24, 124);
    uint32_t hvalue3 = MurmurHash64A(&key, 24, 125);
    printf("hvalue is %u\n", hvalue1);
    printf("hvalue is %u\n", hvalue2);
    printf("hvalue is %u\n", hvalue3);
  }

  return 1;
}

  
	
