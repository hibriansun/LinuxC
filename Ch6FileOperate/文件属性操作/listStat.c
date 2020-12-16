/*
  文件属性(ls -l所列format)保存在一个结构体中
    struct stat {
        dev_t     st_dev;         /* ID of device containing file 
        ino_t     st_ino;         /* Inode number 
        mode_t    st_mode;        /* File type and mode 
        nlink_t   st_nlink;       /* Number of hard links 
        uid_t     st_uid;         /* User ID of owner 
        gid_t     st_gid;         /* Group ID of owner 
        dev_t     st_rdev;        /* Device ID (if special file) 
        off_t     st_size;        /* Total size, in bytes 
        blksize_t st_blksize;     /* Block size for filesystem I/O 
        blkcnt_t  st_blocks;      /* Number of 512B blocks allocated 

        /* Since Linux 2.6, the kernel supports nanosecond
        precision for the following timestamp fields.
        For the details before Linux 2.6, see NOTES. 

        struct timespec st_atim;  /* Time of last access 
        struct timespec st_mtim;  /* Time of last modification 
        struct timespec st_ctim;  /* Time of last status change 

        #define st_atime st_atim.tv_sec      /* Backward compatibility 
        #define st_mtime st_mtim.tv_sec
        #define st_ctime st_ctim.tv_sec
    };
*/


#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char** argv)
{
    struct stat buf;

    if(argc != 2){
        printf("Usage:%s <filename>", argv[0]);
    }

    // stat函数：接受路径参数获取文件的属性信息存入struct stat结构体
    // int stat(const char *pathname, struct stat *statbuf);
    // int fstat(int fd, struct stat *statbuf);
    // int lstat(const char *pathname, struct stat *statbuf);
    
    if(stat(argv[1], &buf) == -1){
        perror("stat:");
        exit(1);
    }

    // perror(const char* s);
    // The  perror()  function produces a message on standard error describing
    // the last error encountered during a call to a system or  library  func‐
    // tion.
    // First (if s is not NULL and *s is not a null byte ('\0')), the argument
    // string s is printed, followed by a colon and a blank.   Then  an  error
    // message corresponding to the current value of errno and a new-line.
    // To  be  of most use, the argument string should include the name of the
    // function that incurred the error.

    // test macro S_ISREG
    printf("%d\n%d\n%o\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n%s\n%s\n", buf.st_dev, buf.st_ino, buf.st_mode/
    buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_rdev, buf.st_size, buf.st_blksize, buf.st_blocks,\
    ctime(&buf.st_atim), ctime(&buf.st_mtim), ctime(&buf.st_ctim));

    return 0;
}