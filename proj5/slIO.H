#ifndef SLIO_is_defined
#define SLIO_is_defined

#include <iostream>
inline std::istream &eatChar(char c,std::istream &buf) {
	char r;
	buf >> r;
	if (r!=c) {
		buf.clear(buf.rdstate() | std::ios::failbit);
	}
	return buf;
}

inline std::istream &eatStr(const char *s,std::istream &buf) {
  while (*s != '\0') {
    eatChar(*s,buf);
    s++;
  }
  return buf;
}

#endif
