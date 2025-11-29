#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_FILENAME_LEN 256
#define MAX_SECTIONS 100

typedef struct {
    char name[MAX_LINE_LEN];
    char control[MAX_LINE_LEN];
    char session[MAX_LINE_LEN];
    char pmin[MAX_LINE_LEN];
    char psec[MAX_LINE_LEN];
    char pframe[MAX_LINE_LEN];
    char plba[MAX_LINE_LEN];
} Section;

typedef struct {
    Section sections[MAX_SECTIONS];
    int count;
} Config;

void config_init(Config *config) {
    config->count = 0;
}

int is_section_line(const char *line) {
    return line[0] == '[' && strchr(line, ']') != NULL;
}

void parse_section_name(const char *line, char *section_name) {
    const char *start = strchr(line, '[') + 1;
    const char *end = strchr(line, ']');
    strncpy(section_name, start, end - start);
    section_name[end - start] = '\0';
}

void trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int parse_config_file(const char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    char line[MAX_LINE_LEN];
    int current_section = -1;

    while (fgets(line, sizeof(line), file)) {
        trim_whitespace(line);
        
        if (strlen(line) == 0) continue;

        if (is_section_line(line)) {
            current_section = config->count++;
            if (current_section >= MAX_SECTIONS) break;
            parse_section_name(line, config->sections[current_section].name);
            continue;
        }

        if (current_section >= 0) {
            char *sep = strchr(line, '=');
            if (!sep) continue;
            *sep = '\0';
            char *key = line;
            char *value = sep + 1;
            trim_whitespace(key);
            trim_whitespace(value);

            Section *sec = &config->sections[current_section];
            if (strcmp(key, "Control") == 0) strcpy(sec->control, value);
            else if (strcmp(key, "Session") == 0) strcpy(sec->session, value);
            else if (strcmp(key, "PMin") == 0) strcpy(sec->pmin, value);
            else if (strcmp(key, "PSec") == 0) strcpy(sec->psec, value);
            else if (strcmp(key, "PFrame") == 0) strcpy(sec->pframe, value);
            else if (strcmp(key, "PLBA") == 0) strcpy(sec->plba, value);
        }
    }

    fclose(file);
    return 0;
}

int find_image_file(char *found_name) {
    const char *extensions[] = {".img", ".bin", ".iso", NULL};
    DIR *dir = opendir(".");
    if (!dir) return -1;

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type != DT_REG) continue;
        
        const char *name = entry->d_name;
        const char *dot = strrchr(name, '.');
        if (!dot) continue;

        for (int i = 0; extensions[i]; i++) {
            if (strcasecmp(dot, extensions[i]) == 0) {
                strcpy(found_name, name);
                closedir(dir);
                return 0;
            }
        }
    }

    closedir(dir);
    return -1;
}

void write_cue_file(const char *ccd_file, const Config *config, const char *img_file) {
    char cue_file[MAX_FILENAME_LEN];
    strcpy(cue_file, ccd_file);
    char *dot = strrchr(cue_file, '.');
    if (dot) *dot = '\0';
    strcat(cue_file, ".cue");

    FILE *cue_fp = fopen(cue_file, "w");
    if (!cue_fp) {
        perror("Failed to create CUE file");
        return;
    }

    fprintf(cue_fp, "FILE \"%s\" BINARY\n", img_file);

    int track_counter = 0;
    int begin_found = 0;

    for (int i = 0; i < config->count; i++) {
        const Section *sec = &config->sections[i];
        if (strstr(sec->name, "Entry") != sec->name) continue;

        int plba = atoi(sec->plba);
        if (plba == 0) begin_found = 1;

        if (!begin_found) continue;

        track_counter++;
        int minute = atoi(sec->pmin);
        int second = atoi(sec->psec);
        int frame = atoi(sec->pframe);

        if (second == 0) {
            if (minute >= 1) {
                minute--;
                second = 60;
            }
        }
        second -= 2;

        const char *track_type = strcmp(sec->control, "0x04") == 0 ? "MODE1/2352" : "AUDIO";
        int track_index = atoi(sec->session);

        fprintf(cue_fp, "  TRACK %02d %s\n", track_counter, track_type);
        fprintf(cue_fp, "    INDEX %02d %02d:%02d:%02d\n", 
                track_index, minute, second, frame);
    }

    fclose(cue_fp);
}

int main(int argc, char *argv[]) {
    char *ccd_file = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "c:")) != -1) {
        if (opt == 'c') ccd_file = optarg;
    }

    if (!ccd_file) {
        fprintf(stderr, "Usage: %s -c <ccd_file>\n", argv[0]);
        return 1;
    }

    Config config;
    config_init(&config);

    if (parse_config_file(ccd_file, &config) != 0) {
        fprintf(stderr, "Failed to parse CCD file\n");
        return 1;
    }

    char img_file[MAX_FILENAME_LEN] = "";
    if (find_image_file(img_file) != 0) {
        fprintf(stderr, "No image file found\n");
        return 1;
    }

    write_cue_file(ccd_file, &config, img_file);
    return 0;
}
