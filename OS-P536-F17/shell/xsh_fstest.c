#include <xinu.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <fs.h>
#define SIZE 1200

// void fs_testbitmask(void);

/**
 * @ingroup shell
 *
 * Shell command fstest.
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */
shellcmd xsh_fstest(int nargs, char *args[]) {
        int rval;
        int fd, i, j;
        char *buf1, *buf2;


        /* Output help, if '--help' argument was supplied */
        if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
        {
                printf("Usage: %s\n\n", args[0]);
                printf("Description:\n");
                printf("\tFilesystem Test\n");
                printf("Options:\n");
                printf("\t--help\tdisplay this help and exit\n");
                return OK;
        }

        /* Check for correct number of arguments */
        if (nargs > 1)
        {
                fprintf(stderr, "%s: too many arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
                return SYSERR;
        }
        if (nargs < 1)
        {
                fprintf(stderr, "%s: too few arguments\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n",
                        args[0]);
                return SYSERR;
        }

#ifdef FS
        // printf("MDEV_BLOCK_SIZE --> %d, MDEV_NUM_BLOCKS -->  %d \n", MDEV_BLOCK_SIZE, MDEV_NUM_BLOCKS);
        // printf("DEFAULT_NUM_INODES --> %d\n", DEFAULT_NUM_INODES);
        bs_mkdev(0, MDEV_BLOCK_SIZE, MDEV_NUM_BLOCKS); /* device "0" and default blocksize (=0) and count */
        fs_mkfs(0,DEFAULT_NUM_INODES); /* bsdev 0*/
        fs_testbitmask();


        buf1 = getmem(SIZE*sizeof(char));
        buf2 = getmem(SIZE*sizeof(char));

        // Create test file
        fd = fs_create("Akash", O_CREAT);
        //printf("File descriptor 1 --> %d\n", fd);
        //fs_print_fsd();
        fd = fs_open("Akash", O_RDWR);
        // int x = fs_open("Akash", O_RDWR);
        // printf("Darshan :: Open status --> %d\n", x);
        // if (x != -1) {
        //   fs_print_open(x);
        // }

        //Fill buffer with random stuff
        for(i=0; i<SIZE; i++)
        {
            j = i%(127-33);
            j = j+33;
            buf1[i] = (char) j;
        }
        // printf("buf1 --> %s\n", buf1);
        // printf("strlen(buf1) --> %d\n", strlen(buf1));
        rval = fs_write(fd,buf1,SIZE);
        // printf("After write --> %d \n", rval);
        // fs_printfreemask();
        if(rval == 0 || rval != SIZE )
        {
             printf("\n\r File write failed");
             goto clean_up;
        }


        // Now my file offset is pointing at EOF file, i need to bring it back to start of file
        // Assuming here implementation of fs_seek is like "original_offset = original_offset + input_offset_from_fs_seek"
        fs_seek(fd,-rval);

        //read the file
        rval = fs_read(fd, buf2, rval);
        //printf("buf2 in main file --> %s\n", buf2);
        buf2[rval] = '\0'; // TODO: Write end of file symbol i.e. slash-zero instead of EOF. I can not do this because of WIKI editor limitation

        if(rval == 0)
        {
                 printf("\n\r File read failed");
                 goto clean_up;
        }

        printf("\n\rContent of file %s",buf2);

        printf("\n");
        printf("\n");
        rval = fs_close(fd);
        if(rval != OK) {
            printf("\n\rReturn val for fclose : %d \n",rval);
        }
        fs_printfreemask();
        //fs_print_close(x);
        // char *buf3, *buf4;
        // buf3 = getmem(SIZE*sizeof(char));
        // buf4 = getmem(SIZE*sizeof(char));
        // int new_fd;
        // new_fd = fs_create("SHETH", O_CREAT);
        // new_fd = fs_open("SHETH", O_RDWR);
        // strcpy(buf3, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
        // rval = fs_write(new_fd,buf3,strlen(buf3));
        // fs_seek(new_fd,-rval);
        // rval = fs_read(new_fd, buf4, rval);
        // //printf("buf2 in main file --> %s\n", buf2);
        // buf4[rval] = '\0';
        // printf("\n\rContent of file %s",buf4);
        //
        // printf("\n");
        // printf("\n");
        // rval = fs_close(new_fd);
        // fs_printfreemask();
clean_up:
        freemem(buf1,SIZE);
        freemem(buf2,SIZE);

#else
        printf("No filesystem support\n");
#endif

        return OK;
}

void fs_testbitmask(void) {

        fs_setmaskbit(31); fs_setmaskbit(95); fs_setmaskbit(159); fs_setmaskbit(223);
        fs_setmaskbit(287); fs_setmaskbit(351); fs_setmaskbit(415); fs_setmaskbit(479);
        fs_setmaskbit(90); fs_setmaskbit(154); fs_setmaskbit(218); fs_setmaskbit(282);
        fs_setmaskbit(346); fs_setmaskbit(347); fs_setmaskbit(348); fs_setmaskbit(349);
        fs_setmaskbit(350); fs_setmaskbit(100); fs_setmaskbit(164); fs_setmaskbit(228);
        fs_setmaskbit(292); fs_setmaskbit(356); fs_setmaskbit(355); fs_setmaskbit(354);
        fs_setmaskbit(353); fs_setmaskbit(352);

        fs_printfreemask();

        fs_clearmaskbit(31); fs_clearmaskbit(95); fs_clearmaskbit(159); fs_clearmaskbit(223);
        fs_clearmaskbit(287); fs_clearmaskbit(351); fs_clearmaskbit(415); fs_clearmaskbit(479);
        fs_clearmaskbit(90); fs_clearmaskbit(154); fs_clearmaskbit(218); fs_clearmaskbit(282);
        fs_clearmaskbit(346); fs_clearmaskbit(347); fs_clearmaskbit(348); fs_clearmaskbit(349);
        fs_clearmaskbit(350); fs_clearmaskbit(100); fs_clearmaskbit(164); fs_clearmaskbit(228);
        fs_clearmaskbit(292); fs_clearmaskbit(356); fs_clearmaskbit(355); fs_clearmaskbit(354);
        fs_clearmaskbit(353); fs_clearmaskbit(352);

        fs_printfreemask();
}
