#include <stdio.h>
#include <unistd.h>

extern unsigned char tarfile[];
extern const unsigned long tarfile_size;

int main(int ac, char *av[]) {
    // printf("tarfile_size %ld  tarfile %08lx\n", tarfile_size, tarfile);
    FILE *fp = popen("sudo -A tar -C /usr/local -xzf -", "w");
    fwrite(tarfile, 1, tarfile_size, fp);
    pclose(fp);
    printf("done\n");
}
