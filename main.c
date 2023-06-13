#include <stdio.h>
#include <zip.h>
#include <getopt.h>

int main() {

    // Open the ZIP file
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
        printf("\033[1;31mFile %d: %s\033[0m\n", i + 1, file_name);   // Texte en rouge
    }

    // Close the ZIP file
    zip_close(zip_file);

    return 0;
}