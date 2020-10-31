#include <cstring>
#include <cstdlib>
#include <cstdio>

int main(int argc, char *argv[])
{
    char buf[8];
    int i = 100;
    sprintf(buf,"%d",i);
    printf("%s\n",buf);
    return 0;
}
