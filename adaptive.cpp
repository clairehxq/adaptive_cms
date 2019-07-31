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
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <set>
#include <map>

float error(uint32_t e, uint32_t t){
  if (t==0)
    return 0;
  return (float)(e-t) / (float)t;
}

int main(int argc, char** argv){
  std::cout<<"usage: width, height, rows, threshold, data_file_path, counts_file, stream_file"<<std::endl;
  //d::string line;
  std::string counts_file_path = argv[5];
  std::string syn_file_path = argv[5];
  counts_file_path += argv[6];
  syn_file_path += argv[7];
  std::ifstream counts_file (counts_file_path); //("data/zipf_counts.txt");
  std::vector<std::pair<uint64_t, int> > counts;
  char k[20];
  int c;
  uint64_t key;
  unsigned W, H, R, T;
  double multiple_hh_p=0;
  double multiple_hh_p_topk=0;
  double multiple_hh_p_deep=0;
  double multiple_hh_p_topk_deep=0;
  int topkk=200;
  W = atoi(argv[1]);
  H = atoi(argv[2]);
  R = atoi(argv[3]);
  T = atoi(argv[4]);
  //output file
  std::string file_name;
  file_name += "rel_res/";
  file_name += argv[6];
  file_name += "/top200/";
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
      //sscanf(k, "%" SCNu64, &key);
      key = strtoull(k, NULL, 0);
      //std::pair<uint64_t, int> p (kw2num(k), c);
      std::pair<uint64_t, int> p (key, c);
      //std::cout<<p.first<<" "<<p.second<<std::endl;
      counts.push_back(p);
    }
  }
  printf("counts has %u items\n", counts.size());
  // Width, Height, Rows, Hash_index_mean, threshold
  double adaptive_time = 0;
  double bcms_time = 0;
  clock_t begin;
  clock_t end;
  int keys_at_capacity_200=0;
  int keys_at_capacity_all=0;
  for (int fold=0; fold<10; fold++){
    std::map<uint32_t, int > touched_blocks;
    std::map<uint32_t, int > touched_blocks_deep;
    adaptive_cms acms(W, H, R, H+fold+(W*R)<<2 , T);
    cms bcms(W*R, H, fold+(W*R)<<2);
    if (fold==0){
      acms.check_config();
      bcms.check_config();
    }

    //add to acms
    std::ifstream syn_file (syn_file_path ); //("data/zipf_stream.txt");
    begin = clock();
    
    while (syn_file>>k){
      key = strtoull(k, NULL, 0);
      acms.addDeep(key);
      bcms.add(key);
    }
    
    end = clock();
    adaptive_time = adaptive_time + (double) (end-begin) / CLOCKS_PER_SEC;
    syn_file.close();

    //construct naive cms using the same space
    double capacity_p = acms.at_capacity_proportion();
    int naive_width = (int) ceil((1 + capacity_p * (R-1)) * W);
    
    cms cms2(naive_width, H, fold+(W*R)<<2);
    cms2.check_config();

    std::ifstream syn_file2 (syn_file_path ); //("data/zipf_stream.txt");
    begin = clock();
    //add to bcms
    while (syn_file2>>k){
      key = strtoull(k, NULL, 0);
      cms2.add(key);
    }
    end = clock();
    bcms_time = bcms_time + (double)(end-begin) / CLOCKS_PER_SEC;
    syn_file2.close();	  

    int topk=0;
    uint32_t loc;
    uint32_t loc_deep;
    for (auto& p: counts){
      if (topk<topkk){
	std::string key = num2kw(p.first);
	uint32_t estimate_deep = acms.countMinDeep(p.first);
	for (int i=0; i<H; i++){
	  loc = (acms.hash(p.first, i) << 3) + i;
	  loc_deep = loc << 2 + acms.hashDeep(p.first, i, acms.hash(p.first, i));
	  if (touched_blocks.find(loc) != touched_blocks.end())
	    touched_blocks[loc] ++;
	  else
	    touched_blocks[loc] = 1;
	  if (touched_blocks_deep.find(loc_deep) != touched_blocks_deep.end())
	    touched_blocks_deep[loc_deep] ++;
	  else
	    touched_blocks_deep[loc_deep] = 1;
	}
	    
	int tmp = acms.at_capacity(p.first);
	keys_at_capacity_200 += tmp;
	keys_at_capacity_all += tmp;
	//uint32_t estimate_small = acms.countMin(p.first);
	uint32_t estimate_comp = cms2.countMin(p.first);
	uint32_t estimate_big = bcms.countMin(p.first);
	int true_count = p.second;
	output_file << error(estimate_deep,true_count) << ", " << error(estimate_comp,true_count) << "," << error(estimate_big,true_count)<<",";
	output_file << estimate_deep-true_count << ", " << estimate_comp-true_count << "," << estimate_big-true_count <<"," << true_count << std::endl;
      }

      else {
	//printf("check not topk\n");
	int tmp = acms.at_capacity(p.first);
	keys_at_capacity_all += tmp; //= acms.at_capacity(p.first);
      }
      topk ++;
    }
    double multiple_hh = 0;
    for (auto &[key, value]:touched_blocks){
      //std::cout<<"multiple_hh "<<key<<" "<<value<<std::endl;
      if(value >1)
	multiple_hh ++;
    }
    multiple_hh_p_topk += multiple_hh / touched_blocks.size();
    multiple_hh = multiple_hh / (W*H);
    multiple_hh_p += multiple_hh;

    for (auto &[key, value]:touched_blocks_deep){
      //std::cout<<"multiple_hh "<<key<<" "<<value<<std::endl;
      if(value >1)
	multiple_hh_deep ++;
    }
    multiple_hh_p_topk_deep += multiple_hh_deep / touched_blocks_deep.size();
    multiple_hh_deep = multiple_hh_deep / (W*H*R);
    multiple_hh_p_deep += multiple_hh_deep;
    
    std::cout<<"reached capacity proportion "<<acms.at_capacity_proportion()<<std::endl;
    acms.del();
    cms2.del();
    bcms.del();
  }
  multiple_hh_p = multiple_hh_p / 10;
  multiple_hh_p_topk = multiple_hh_p_topk / 10;
  multiple_hh_p_deep = multiple_hh_p_deep / 10;
  multiple_hh_p_topk_deep = multiple_hh_p_topk_deep / 10;
  std::cout<<"adaptive construction time" <<adaptive_time / 10 << std::endl;
  std::cout<<"adaptive keys at capacity from 200 keys "<<(double)keys_at_capacity_200 / (10 * topkk)<<std::endl;
  std::cout<<"adaptive keys at capacity counts "<< keys_at_capacity_all <<", from all keys proportion"<<(double)keys_at_capacity_all / (10 * counts.size()) <<std::endl;
  std::cout<<"multiple heavy hitters proportion from all "<< multiple_hh_p<<std::endl;
  std::cout<<"multiple heavy hitters proportion from topk "<< multiple_hh_p_topk<<std::endl;
  std::cout<<"multiple heavy hitters proportion deep from all "<< multiple_hh_p_deep<<std::endl;
  std::cout<<"multiple heavy hitters proportion deep from topk "<< multiple_hh_p_topk_deep<<std::endl;
  std::cout<<"bcms construction time" <<bcms_time / 10 << std::endl;

  output_file.close();
  counts_file.close();
  return 1;
}
    
    
