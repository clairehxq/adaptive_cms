//#include "tcs.hpp"
#include "adaptive_cms.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>
#include <stdlib.h>
#include <sstream>

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
  file_name += "res/";
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
    if (k == "stream")
      break;
    std::pair<uint64_t, int> p (kw2num(k), c);
    counts.push_back(p);
  }
  // Width, Height, Rows, Hash_index_mean, threshold
  for (int fold=0; fold<10; fold++){
    std::ifstream syn_file (argv[6]); //("data/zipf_stream.txt");
    adaptive_cms cms(W, H, R, W*R+333+fold , T);
    adaptive_cms cms2(W*R, H, 1, W*(H+3)+111+fold, T);
    float over_capacity=0;
    cms.check_config();
    cms2.check_config();

    int cnum =0;
    while (syn_file>>k){
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
      over_capacity += cms.at_capacity(p.first);
      output_file << estimate_deep << ", " << estimate_big << "," << estimate << "," << true_count << std::endl;
    }
    std::cout<<"reached capacity proportion "<<over_capacity/counts.size()<<std::endl;
    cms.del();
    cms2.del();
    syn_file.close();

  }
  output_file.close();
  counts_file.close();
  return 1;
}
    
    
