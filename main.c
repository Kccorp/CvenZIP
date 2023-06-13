//simple libzip example to create a zip file and add a file to it

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <errno.h>

int main() {
    struct zip *zip;
    struct zip_source *source;
    struct zip_stat st;
    char buf[100];
    int err;
    int len;
    int fd;


    //create a new zip archive
    zip = zip_open("test.zip", ZIP_CREATE, &err);
    if (zip == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        printf("can't open zip archive '%s': %s\n", "test.zip", buf);
        exit(1);
    }

    //add a file to the archive
    source = zip_source_file(zip, "test.txt", 0, -1);
    if (source == NULL) {
        zip_error_to_str(buf, sizeof(buf), errno, errno);
        printf("can't open file '%s': %s\n", "test.txt", buf);
        exit(1);
    }
    if (zip_file_add(zip, "test.txt", source, ZIP_FL_ENC_UTF_8) < 0) {
        zip_error_to_str(buf, sizeof(buf), errno, errno);
        printf("can't add file '%s': %s\n", "test.txt", buf);
        exit(1);
    }

    //close the archive
    if (zip_close(zip) < 0) {
        zip_error_to_str(buf, sizeof(buf), errno, errno);
        printf("can't close zip archive '%s': %s\n", "test.zip", buf);
        exit(1);
    }

    //open the archive again
    zip = zip_open("test.zip", 0, &err);
    if (zip == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        printf("can't open zip archive '%s': %s\n", "test.zip", buf);
        exit(1);
    }

    //get the stat of the file
    if (zip_stat(zip, "test.txt", 0, &st) < 0) {
        zip_error_to_str(buf, sizeof(buf), errno, errno);
        printf("can't stat file '%s': %s\n", "test.txt", buf);
        exit(1);
    }

    //read the file and print the contents
    fd = (int) zip_fopen(zip, "test.txt", 0);
    if (fd < 0) {
        zip_error_to_str(buf, sizeof(buf), errno, errno);
    }
    struct zip *zip_file = zip_open("test.zip", ZIP_RDONLY, NULL);
    if (zip_file == NULL) {
        printf("Failed to open ZIP file!\n");
        return 1;
    }

    // Get the number of files in the ZIP
    int num_files = zip_get_num_files(zip_file);
    printf("Number of files in the ZIP: %d\n", num_files);

    // Print the names of all files in the ZIP
    for (int i = 0; i < num_files; i++) {
        const char *file_name = zip_get_name(zip_file, i, 0);
        printf("File %d: %s\n", i + 1, file_name);
    }

    // Close the ZIP file
    zip_close(zip_file);
}