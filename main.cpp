#include <cstdio>
#include <cstdlib>


int main(int _argc, char* _argv[]) {

	for (size_t i = 0; i < _argc; i++) {
		printf(_argv[i]);
		printf("\n");
	}

	system("pause");

	return 0;
}


