#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
//heavy hitter
#define H 1<<10
#define S H<<4
#define N 2000
#define HR 
#define SR

std::string random_string( int length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void create_counts(std::ofstream file1){
  for (int i=0; i<H; i++){ //for every heavy hitter
      num = rand() %  10000 + 10000;
      std::string name = random_string(5);
      myfile1 << "hh" << i <<name << " " << num << std::endl;
      hh_num.push_back(num);
      //for (int j=0; j<num; j++)
      //myfile2 << "hh" << i << name << std::endl;
  }
}

    
    
int main(){
  srand(time(NULL));
  std::vector<int> hh_num;
  std::vector<int> s_num;
  hh_num.reserve(H);
  s_num.reserve(S);
  int num;
  std::ifstream counts;
  std::ofstream myfile2;
  myfile1.open("counts.txt");
  myfile2.open("syn.txt");

  
  for (int i=0; i<S; i++){ //for every splatter
    num = rand() %  999 + 1;
    std::string name = random_string(5);
    myfile1 << "s" << i<< name << " " << num << std::endl;
    s_num.push_back(num);

    for (int j=0; j<num; j++)
      myfile2 << "s" << i << name << std::endl;
  }

  myfile1.close();
  myfile2.close();
  
}

    
    



