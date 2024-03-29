//#include "tcs.hpp"
#include "adaptive_cms.hpp"
#include "cms.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>
#include <stdlib.h>
#include <sstream>

float error_rel(uint32_t estimate, uint32_t tr){
  if (tr==0){
    printf("true is zero\n");
  
    return 0;
  }
  else{
    return (float)(estimate-tr) / (float)tr;
  }
}

uint32_t error_abs(uint32_t estimate, uint32_t tr){
  return estimate-tr;
}

int main(int argc, char** argv){
  std::cout<<"usage: width, height, rows, threshold, counts_file_path, stream_file_path"<<std::endl;
  std::string line;
  std::ifstream counts_file (argv[5]); //("data/zipf_counts.txt");
  std::vector<std::pair<uint64_t, int> > counts;
  char k[20];
  int c;
  uint64_t key;
  unsigned W, H, R, T;
  
  W = atoi(argv[1]);
  H = atoi(argv[2]);
  R = atoi(argv[3]);
  T = atoi(argv[4]);
  //output file
  std::string file_name;
  file_name += "res/big/";
  for (int i = 1; i < 5; i++){
    file_name += argv[i];
    file_name += "_";
  }
  std::stringstream m;
  m << W*H*R;
  file_name += m.str();
  std::ofstream output_file (file_name.c_str());
  std::cout<<"output to file "<< file_name <<std::endl;

  while (counts_file >> k >> c ){
    if (c>0){
      std::pair<uint64_t, int> p (kw2num(k), c);
      counts.push_back(p);
    }    
  }

  printf("counts has %u items", counts.size());
  // Width, Height, Rows, Hash_index_mean, threshold
  float over_capacity=0;
  int log_r = (int) std::log2(R);
  printf("log r is %d\n", log_r);
  printf("fix r=4\n");
  for (int fold=0; fold<10; fold++){
    adaptive_cms acms(W, H, R, fold+(W*R)<<2 , T);
    cms scms(W, H, fold+(W*R)<<2);    
    if (fold==0){
      acms.check_config();
      scms.check_config();
    }
    std::vector<cms> bcmss;
    bcmss.reserve(log_r);
    for (int i=0; i<log_r; i++){
      cms bbcms(W*(1<<i), H<<(log_r-i), fold+(W*R)<<2);
      bcmss.push_back(bbcms);
      printf("all bcmss generated\n");
    }

    for (int i=0; i<log_r; i++){
      bcmss.at(i).check_config();
    }
    
    int cnum =0;
    std::ifstream syn_file (argv[6]); //("data/zipf_stream.txt");
    while (syn_file>>k){
      key = kw2num(k);
      acms.addDeep(key);
      scms.add(key);
      for (int i=0; i<log_r; i++)
	bcmss.at(i).add(key);
      cnum ++;
    }

    for (auto& p: counts){
      std::string key = num2kw(p.first);
      uint32_t estimate_deep = acms.countMinDeep(p.first);
      uint32_t estimate = scms.countMin(p.first);
      int true_count = p.second;
      over_capacity += acms.at_capacity(p.first);
      output_file << error_abs(estimate_deep, true_count);
      for (int i=0; i<log_r; i++){
	uint32_t estimate_another = bcmss.at(i).countMin(p.first);
	output_file << ", " << error_abs(estimate_another, true_count);
      }
      output_file << std::endl;

    }
    acms.del();
    scms.del();
    for (int i=0; i<log_r; i++){
      bcmss.at(i).del();
    }
    syn_file.close();

  }
  std::cout<<"reached capacity proportion "<<over_capacity/(10*counts.size())<<std::endl;  
  output_file.close();
  counts_file.close();
  return 1;

}
    
    
