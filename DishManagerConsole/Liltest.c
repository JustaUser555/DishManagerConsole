//Setup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

int main() {
	char buf[1024];
	_getcwd(buf, sizeof(buf));
	printf("%s", buf);
	return 0;
}