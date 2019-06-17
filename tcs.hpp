#ifndef TCS_HPP
#define TCS_HPP

#include <string>
#include <vector>
#include <boost/iostreams/device/mapped_file.hpp>
//#include "KdTreeBB.hpp"
#include <map>

typedef std::pair<uint32_t,uint64_t> CountKeyword;
typedef std::vector<CountKeyword> CountKeywordList;

bool CountKeywordSort(CountKeyword w1, CountKeyword w2);

class CountSketch;
struct Tag;

static inline std::string num2kw(uint64_t value)
{
  const char base36[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
  /* log(2**64) / log(36) = 12.38 => max 13 char + '\0' */
  char buffer[14];
  int offset = sizeof(buffer);

  buffer[--offset] = '\0';
  for (; value; value/=36)
    buffer[--offset] = base36[value % 36];
  // do {                                                                                                                                                                                    
  //   buffer[--offset] = base36[value % 36];                                                                                                                                                
  // } while (value /= 36);                                                                                                                                                                  

  return std::string(&buffer[offset]);
}

static inline uint64_t base36dec(const char *text)
{
  char buf[13];
  strncpy(buf, text, 12);
  buf[12] = '\0';
  return strtoul(buf, NULL, 36);
}

inline uint64_t kw2num(const std::string &kw) {
  return kw2num(kw.c_str());
}

inline uint64_t kw2num(const char *cur) {
  uint64_t result = 0;
  for (int digitCnt=0; *cur; ++cur) {
    int digit = (*cur>'9')?(tolower(*cur)+10-(int)'a'):(*cur-'0');
    if (digit<0 || digit>35)
      break;
    else if (++digitCnt<13)
      result = result*36UL + digit;
  }
  return result;
}

#endif
