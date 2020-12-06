/*
int chmod(const char *path, mode_t mode);

Upon successful completion, 0 is returned. Otherwise, -1 is returned and errno is set to indicate the error. If -1 is returned, no change to the file mode will occur.
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char** argv)
{
	int u_mode;	// owner
	int g_mode;	// belonged group
	int o_mode;	// others
	int mode = atoi(argv[1]);

	// Check argv format
	if(argc < 3){
		printf("Usage: %s <mode num> <target file>\n", argv[0]);
		exit(0);
	}

	// Check mode
	if(mode < 0 || mode > 777){
		printf("Invalid mode num\n");
		exit(0);
	}

	// Separate mode for each one
	u_mode = mode/100;
	g_mode = (mode - (u_mode*100))/10;
	o_mode = mode - u_mode*100 - g_mode*10;

	// Covert mode num into Octal (八进制)
	mode = (u_mode*8*8) + (g_mode*8) + (o_mode);

	// Change mode of file
	if(chmod(argv[2], mode)){
		perror("chmod error");
		exit(1);
	}

	return 0;
}
