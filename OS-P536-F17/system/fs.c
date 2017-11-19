#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#ifdef FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
        int diskblock;

        if (fileblock >= INODEBLOCKS - 2) {
                printf("No indirect block support\n");
                return SYSERR;
        }

        diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

        return diskblock;
}

/* read in an inode and fill in the pointer */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
        int bl, inn;
        int inode_off;

        if (dev != 0) {
                printf("Unsupported device\n");
                return SYSERR;
        }
        if (inode_number > fsd.ninodes) {
                printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
                return SYSERR;
        }

        bl = inode_number / INODES_PER_BLOCK;
        inn = inode_number % INODES_PER_BLOCK;
        bl += FIRST_INODE_BLOCK;

        inode_off = inn * sizeof(struct inode);

        /*
           printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
           printf("inn*sizeof(struct inode): %d\n", inode_off);
         */

        bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
        memcpy(in, &block_cache[inode_off], sizeof(struct inode));

        return OK;

}

int fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
        int bl, inn;

        if (dev != 0) {
                printf("Unsupported device\n");
                return SYSERR;
        }
        if (inode_number > fsd.ninodes) {
                printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
                return SYSERR;
        }

        bl = inode_number / INODES_PER_BLOCK;
        inn = inode_number % INODES_PER_BLOCK;
        bl += FIRST_INODE_BLOCK;

        /*
           printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
         */

        bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
        memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
        bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

        return OK;
}

int fs_mkfs(int dev, int num_inodes) {

        int i;

        if (dev == 0) {
                fsd.nblocks = dev0_numblocks;
                fsd.blocksz = dev0_blocksize;
        }
        else {
                printf("Unsupported device\n");
                return SYSERR;
        }

        if (num_inodes < 1) {
                fsd.ninodes = DEFAULT_NUM_INODES;
        }
        else {
                fsd.ninodes = num_inodes;
        }
        i = fsd.nblocks;
        while ( (i % 8) != 0) {
                i++;
        }
        fsd.freemaskbytes = i / 8;
        if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
                printf("fs_mkfs memget failed.\n");
                return SYSERR;
        }
        /* zero the free mask */
        for(i=0; i<fsd.freemaskbytes; i++) {
                fsd.freemask[i] = '\0';
        }

        fsd.inodes_used = 0;

        /* write the fsystem block to SB_BLK, mark block used */
        fs_setmaskbit(SB_BLK);
        bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));

        /* write the free block bitmask in BM_BLK, mark block used */
        fs_setmaskbit(BM_BLK);
        bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

        return 1;
}


/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
        int mbyte, mbit;
        mbyte = b / 8;
        mbit = b % 8;
        fsd.freemask[mbyte] |= (0x80 >> mbit);
        return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
        int mbyte, mbit;
        mbyte = b / 8;
        mbit = b % 8;

        return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
        return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
        int mbyte, mbit, invb;
        mbyte = b / 8;
        mbit = b % 8;

        invb = ~(0x80 >> mbit);
        invb &= 0xFF;

        fsd.freemask[mbyte] &= invb;
        return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
        int i,j;
        for (i=0; i < fsd.freemaskbytes; i++) {
                for (j=0; j < 8; j++) {
                        printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
                }
                if ( (i % 8) == 7) {
                        printf("\n");
                }
        }
        printf("\n");
}

void fs_print_fsd(void) {
        printf("**********************\n");
        printf("inodes_used --> %d \n", fsd.ninodes);
        int x;
        printf("fsd.root_dir.numentries --> %d\n", fsd.root_dir.numentries);
        for (x = 0; x < fsd.root_dir.numentries; x++) {
                printf("File name --> %s\n", fsd.root_dir.entry[x].name);
        }
        printf("**********************\n");
}

void fs_print_open(int inode_number) {
        printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("State --> %d\n", oft[inode_number].state);
        printf("File opened --> %s \n", oft[inode_number].de->name);
        printf("File type --> %d \n", oft[inode_number].in.type);
        printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void fs_print_close(int inode_number) {
        printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("State --> %d\n", oft[inode_number].state);
        printf("File closed --> %s \n", oft[inode_number].de->name);
        printf("File type --> %d \n", oft[inode_number].in.type);
        printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
}

int fs_open(char *filename, int flags) {
        struct inode open_inode;
        int inode_number;
        bool found = false;
        int x;
        if (fsd.root_dir.numentries > 0) {
                for (x = 0; x < fsd.root_dir.numentries; x++) {
                        // printf("%s\n", fsd.root_dir.entry[x].name);
                        if (strcmp(fsd.root_dir.entry[x].name, filename) == 0) {
                                // printf("FOUND.\n");
                                inode_number = x;
                                found = true;
                                break;
                        }
                }
        }
        if (!found) {
                printf("File not found.\n");
                return SYSERR;
        }
        fs_get_inode_by_num(0, inode_number, &open_inode);
        oft[inode_number].state = FSTATE_OPEN;
        oft[inode_number].fileptr = 0;
        oft[inode_number].de = &fsd.root_dir.entry[inode_number];
        oft[inode_number].in = open_inode;
        oft[inode_number].mode = flags;
        return inode_number;
}

int fs_close(int fd) {
        if(fd < 0 || fd > NUM_FD) {
          printf("INVALID FILE.\n");
          return SYSERR;
        }
        struct inode inode_to_closed;
        if (oft[fd].state == FSTATE_CLOSED) {
                printf("File already closed.\n");
                return SYSERR;
        }
        oft[fd].state = FSTATE_CLOSED;
        oft[fd].fileptr = 0;
        inode_to_closed = oft[fd].in;
        fs_put_inode_by_num(0, fd, &inode_to_closed);
        //oft[fd].in = NULL;

        return OK;
}

int fs_create(char *filename, int mode) {
        if (fsd.root_dir.numentries > 0) {
                int x;
                for (x = 0; x < fsd.root_dir.numentries; x++) {
                        //printf("%s\n", fsd.root_dir.entry[x].name);
                        if (strcmp(fsd.root_dir.entry[x].name, filename) == 0) {
                                printf("File already exists.\n");
                                return SYSERR;
                        }
                }
        }
        struct inode new_inode;
        int x;
        fs_get_inode_by_num(0, fsd.inodes_used, &new_inode);

        new_inode.id = fsd.inodes_used;
        new_inode.type = INODE_TYPE_FILE;
        new_inode.device = 0;
        new_inode.size = 0;
        fs_put_inode_by_num(0, fsd.inodes_used, &new_inode);

        fsd.root_dir.entry[new_inode.id].inode_num = new_inode.id;
        strcpy(fsd.root_dir.entry[new_inode.id].name, filename);
        fsd.root_dir.numentries = fsd.root_dir.numentries + 1;
        fsd.inodes_used = fsd.inodes_used + 1;
        return new_inode.id;
}

int fs_seek(int fd, int offset) {
        if(fd < 0 || fd > NUM_FD) {
          printf("INVALID FILE.\n");
          return SYSERR;
        }
        oft[fd].fileptr = oft[fd].fileptr + offset;
        printf("Seek :: oft[fd].fileptr --> %d \n", oft[fd].fileptr);
}

int fs_read(int fd, void *buf, int nbytes) {
        if(fd < 0 || fd > NUM_FD) {
          printf("INVALID FILE.\n");
          return SYSERR;
        }
        if (oft[fd].state != FSTATE_OPEN) {
                printf("File is not open.\n");
                return SYSERR;
        }
        if (oft[fd].mode == O_WRONLY) {
                printf("Insufficient priviledges :: No read access.\n");
                return SYSERR;
        }
        int loop_var;
        loop_var = nbytes / MDEV_BLOCK_SIZE + 1;
        int block_no;
        int block_size_to_read = 0;
        for (block_no = 0; block_no < loop_var; block_no++ ) {
                if (block_no+1 == loop_var) {
                        block_size_to_read = nbytes % MDEV_BLOCK_SIZE;
                } else {
                        block_size_to_read = MDEV_BLOCK_SIZE;
                }
                int z = bs_bread(dev0, oft[fd].in.blocks[block_no], 0, buf+oft[fd].fileptr, block_size_to_read);
                if (z == OK) {
                        oft[fd].fileptr+=block_size_to_read;
                } else {
                        printf("Read failed.\n");
                        return SYSERR;
                }
        }
        return oft[fd].fileptr;
}

int fs_write(int fd, void *buf, int nbytes) {
        if(fd < 0 || fd > NUM_FD) {
          printf("INVALID FILE.\n");
          return SYSERR;
        }
        if (oft[fd].state != FSTATE_OPEN) {
                printf("File is not open.\n");
                return SYSERR;
        }
        if (oft[fd].mode == O_RDONLY) {
                printf("Insufficient priviledges :: No write access.\n");
                return SYSERR;
        }
        int loop_var;
        loop_var = strlen(buf) / MDEV_BLOCK_SIZE + 1;
        int block_no;
        int y;
        int inode_block_count = 0;
        int block_size_to_write = 0;
        oft[fd].fileptr = 0;

        for (block_no = 15; block_no < fsd.nblocks; block_no++ ) {
                if (oft[fd].fileptr == strlen(buf)) {
                        printf("EVERYTHING WRITTEN.\n");
                        break;
                }
                if (fs_getmaskbit(block_no) != 0) {
                        continue;
                }
                if ( y+1 == loop_var) {
                        block_size_to_write = strlen(buf) % MDEV_BLOCK_SIZE;
                } else {
                        block_size_to_write = MDEV_BLOCK_SIZE;
                }
                y++;
                int z = bs_bwrite(dev0, block_no, 0, buf+oft[fd].fileptr, block_size_to_write);
                if (z == OK) {
                        oft[fd].fileptr += block_size_to_write;
                        fs_setmaskbit(block_no);
                        oft[fd].in.blocks[inode_block_count] = block_no;
                } else {
                        printf("Write failed.\n");
                        return SYSERR;
                }
                inode_block_count++;
        }

        return oft[fd].fileptr;
}

#endif /* FS */
