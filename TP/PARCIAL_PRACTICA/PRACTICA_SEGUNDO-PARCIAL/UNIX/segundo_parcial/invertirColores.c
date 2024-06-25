#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PGM_FILE "cat.pgm"
#define INVERT_FILE "catInvertColores.pgm"
#define HEADER_SIZE 15
#define MAX_IMAGE_SIZE 1024*1024  // Maximum size of 1MB for image data

void something_wrong(int fd, const char *m) {
    if (fd >= 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main() {
    int in_fd = open(PGM_FILE, O_RDONLY);
    if (in_fd < 0) {
        something_wrong(in_fd, "Error opening input file");
    }

    int out_fd = open(INVERT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        something_wrong(out_fd, "Error opening output file");
    }

    char header[HEADER_SIZE];
    if (read(in_fd, header, HEADER_SIZE) != HEADER_SIZE) {
        something_wrong(in_fd, "Error reading header");
    }
    if (write(out_fd, header, HEADER_SIZE) != HEADER_SIZE) {
        something_wrong(out_fd, "Error writing header");
    }
    struct stat st;
    if (fstat(in_fd, &st) != 0) {
        something_wrong(in_fd, "Error getting file size");
    }
    off_t file_size = st.st_size;
    // Read image data
    unsigned char image_data[MAX_IMAGE_SIZE];
    off_t image_size = file_size - HEADER_SIZE;
    if (read(in_fd, image_data, image_size) != image_size) {
        something_wrong(in_fd, "Error reading image data");
    }
    // Invertir colores
    for (off_t i = 0; i < image_size; i++) {
        image_data[i] = 255 - image_data[i];
    }
    
    

    // Write inverted image data
    if (write(out_fd, image_data, image_size) != image_size) {
        something_wrong(out_fd, "Error writing inverted image data");
    }
  

      close(in_fd);
      close(out_fd);

}