#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <err.h>

int main()
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		err(EXIT_FAILURE, "getcwd");
	}

	char** cwd_tokens;

	cwd_tokens = (char**)calloc(1, sizeof(char*));

	if (cwd_tokens == NULL) {
		err(EXIT_FAILURE, "calloc");
	}

	int cwd_token_index = 0;

	cwd_tokens[cwd_token_index] = strtok(cwd, "/");

	if (cwd_tokens[cwd_token_index] == NULL) {
		exit(1);
	}

	while (cwd_tokens[cwd_token_index] != NULL) {
		cwd_token_index++;

		cwd_tokens = realloc(cwd_tokens, (cwd_token_index + 1) * sizeof(char*));

		if (cwd_tokens == NULL) {
			err(EXIT_FAILURE, "realloc");
		}

		cwd_tokens[cwd_token_index] = strtok(NULL, "/");
	}

	char* cwd_token_last = cwd_tokens[cwd_token_index - 1];

	free(cwd_tokens);

	const char* home_directory = getenv("HOME");

	if (home_directory == NULL) {
		fprintf(stderr, "%s: environment variable \"HOME\" is not set\n", TARGET);

		exit(1);
	}

	const char* cwdnote_directory = "/.local/cwdnote";
	char absolute_cwd_note_directory[PATH_MAX];

	strcpy(absolute_cwd_note_directory, home_directory);
	strcat(absolute_cwd_note_directory, cwdnote_directory);

	DIR *d;
	struct dirent *dir;

	if ((d = opendir(absolute_cwd_note_directory)) == NULL)
	{
		err(EXIT_FAILURE, "%s", absolute_cwd_note_directory);
	}

	char* note_file = NULL;

	while ((dir = readdir(d)) != NULL) {
		if (dir->d_type == DT_REG && strstr(dir->d_name, cwd_token_last)) {
			note_file = dir->d_name;
			break;
		}
	}

	closedir(d);

	if (note_file == NULL) {
		exit(1);
	}

	char absolute_note_file[PATH_MAX];

        strcpy(absolute_note_file, absolute_cwd_note_directory);
	strcat(absolute_note_file, "/");
        strcat(absolute_note_file, note_file);

	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(absolute_note_file, "r");

	if (fp == NULL) {
		err(EXIT_FAILURE, "%s", absolute_note_file);
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		printf("%s", line);
	}

	fclose(fp);

	if (line) {
		free(line);
	}

	return 0;
}
