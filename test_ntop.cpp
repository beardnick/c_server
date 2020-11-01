
#include <arpa/inet.h>
#include <bitset>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

std::string itobs(int n) { return std::bitset<8>(n).to_string(); }

int main(int argc, char *argv[]) {
  int buf_len = sizeof(struct in6_addr);
  unsigned char buf[buf_len];
  int domain, s;
  char str[INET6_ADDRSTRLEN];

  if (argc != 3) {
    fprintf(stderr, "Usage: %s {i4|i6|<num>} string\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  domain = (strcmp(argv[1], "i4") == 0)
               ? AF_INET
               : (strcmp(argv[1], "i6") == 0) ? AF_INET6 : atoi(argv[1]);

  s = inet_pton(domain, argv[2], buf);
  if (s <= 0) {
    if (s == 0)
      fprintf(stderr, "Not in presentation format");
    else
      perror("inet_pton");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < buf_len; i++) {
    printf("%s ", itobs((int)buf[i]).c_str());
  }
  //printf("%08x\n", (u_int16_t)*buf);
  printf("\n");
  if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
    perror("inet_ntop");
    exit(EXIT_FAILURE);
  }

  printf("inet_ntop: %s\n", str);

  exit(EXIT_SUCCESS);
}
