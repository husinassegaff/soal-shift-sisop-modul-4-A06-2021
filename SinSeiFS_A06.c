#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>

static  const  char *dirpath = "/home/osd0081/Desktop/Sisop/Ujicoba/modul4/com";
static  const  char *logpath = "/home/osd0081/Desktop/Sisop/Ujicoba/modul4/com/SinFei.log";

static  const  char *atozTag = "AtoZ_";
static  const  char *rxTag = "RX_";
static  const  char *aisaTag = "A_is_a_";

void listFilesRecursively(char *basePath){
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            listFilesRecursively(path);
            
            printf("%s\n", path);

        }
    }
    closedir(dir);
}

// string setelah titik terakhir (termasuk titik tersebut)

char *getFile(char *old_name){
    char *e = strrchr (old_name,'/');
    if (e == NULL)
        e = "";
    return e;
}

// string setelah titik terakhir (termasuk titik tersebut)

char *getExt(char *old_name){
    char *e = strrchr (old_name,'.');
    if (e == NULL)
        e = "";
    return e;
}

// nama file

char *getName(char *old_name){
    int nameLen;
    int extLen;
    int len;

    nameLen = strlen(old_name);
    extLen =  strlen(getExt(old_name));
    len = (nameLen-extLen);
    
    char *name = (char *) malloc(len);


    for(int i = 0;i<len;i++){
        name[i]=old_name[i];
    }

    strcpy(old_name,name);

    free(name);
    
    return old_name;
}
// string setelah titik terakhir (termasuk titik tersebut)

char *getPath(char *old_name){
    int pathLen;
    int fileLen;

    char *path = (char*) malloc(1024);
    
    pathLen = strlen(old_name);
    fileLen = strlen(getFile(old_name));

    for(int i = 0;i<(pathLen-fileLen);i++){
        path[i]=old_name[i];
    }

    strcpy(old_name,path);

    free(path);

    return old_name;
}

// enkripsi & dekripsi AtoZ_ file dan RX_ (mkdir)

char *atBash(char *old_name,int mode){
    
    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(ext,getExt(old_name));
    strcpy(name,getName(old_name));

    for(int i = 0; i < strlen(name);i++){
        if('a'<=name[i] && name[i]<='z'){
            if(mode == 0)
                name[i]= 'z' - name[i] + 'a';
            else
                name[i]= 'a' + (name[i] - 'a' + 13) % 26;
        }else if('A'<=name[i] && name[i]<='Z'){
            if(mode == 0)
                name[i]= 'Z' - name[i] + 'A';
            else
                name[i]= 'A' + (name[i] - 'A' + 13) % 26;
        }
    }

    strcpy(old_name,name);
    strcat(old_name,ext);

    free(name);
    free(ext);

    return old_name;
}

// nilai binary dari decimal beda huruf

char *binDif(char *dec){
   int decimal = atoi(dec);
   long bno=0,rem,f=1;

   while(decimal != 0){
      rem = decimal % 2;
      bno = bno + rem * f;
      f = f * 10;
      decimal = decimal / 2;
   }

   sprintf(dec,"%ld",bno);
   return dec;
}

// string binary dengan panjang yang sama dengan namfile

char *trueBin (char *bin, int len){
    char *binary = (char *) malloc(128);
    int length = len - strlen(bin);
    
    while(length!=0){
        strcat(binary,"0");
        length--;
    }

    strcat(binary,bin);
    strcpy(bin,binary);

    free(binary);

    return bin;
}

// nilai desimal dari binary beda huruf

int decDif(char *bin){
    return (int) strtol(bin, 0, 2);
}

// enkripsi file folder A_is_a

char *toLow(char *old_name){
    int i = 0;

    char *bin = (char *) malloc(128);
    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(ext,getExt(old_name));
    strcpy(name,getName(old_name));

    while(name[i]){
        if('a'<=name[i] && name[i]<='z'){
            strcat(&bin[i],"0");
            name[i] = tolower(name[i]);
        }else if('A'<=name[i] && name[i]<='Z'){
            strcat(&bin[i],"1");
            name[i] = tolower(name[i]);
        }else{
            strcat(&bin[i],"0");
            name[i] = name[i];
        }
        i++;
    }

    sprintf(bin,".%d", decDif(bin));

    strcpy(old_name,name);
    strcat(old_name,ext);
    strcat(old_name,bin);

    free(name);
    free(ext);
    free(bin);

    return old_name;
}

// dekripsi file folder A_is_a

char *toNor(char *old_name){
    int i = 0;

    char *dec = (char *) malloc(128);
    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(dec,getExt(old_name));
    strcpy(ext,getExt(getName(old_name)));
    strcpy(name,getName(old_name));

    memmove(dec, dec+1, strlen(dec));

    dec = binDif(dec);

    dec = trueBin(dec,strlen(name));

    while(dec[i]){
        if(dec[i]=='1'){
            name[i] = toupper(name[i]);            
        }else{
            name[i] = name[i];   
        }
        i++;
    }

    strcpy(old_name,name);
    strcat(old_name,ext);

    free(name);
    free(ext);
    free(dec);

    return old_name;
}

// pembuatan kunci toVig

char *key(char *keys, char *name){
    int i=0;
    char code[] = "sisop";

    while(name[i]){
        if('a'<=name[i] && name[i]<='z'){
            keys[i] = tolower(code[i%5]);
        }else if('A'<=name[i] && name[i]<='Z'){
            keys[i] = toupper(code[i%5]);
        }else{
            keys[i] = name[i];
        }
        i++;
    }
    return keys;
}

// enkripsi RZ_ (mv atau rename)

char *toVig(char *old_name, int mode){

    char *ext = (char *) malloc(128);
    char *name = (char *) malloc(1024);

    strcpy(ext,getExt(old_name));
    strcpy(name,getName(old_name));
    
    int nameLen = strlen(old_name);
    char keys[nameLen];
    
    key(keys,name);

    for(int i = 0; i<nameLen;i++){
        if(name[i]==keys[i]&&('a'>=name[i] && name[i]>='z')&&('A'>=name[i] && name[i]>='Z')){
            continue;
        }else if(mode == 0){
            if('a'<=name[i] && name[i]<='z'){
                name[i] = (name[i] + keys[i] - 2 * 'a') % 26 + 'a';
            }else if('A'<=name[i] && name[i]<='Z'){
                name[i] = (name[i] + keys[i] - 2 * 'A') % 26 + 'A';
            }
        }else if(mode == 1){
            if('a'<=name[i] && name[i]<='z'){
                name[i] = ((name[i] - keys[i] + 26)%26) + 'a';
            }else if('A'<=name[i] && name[i]<='Z'){
                name[i] = ((name[i] - keys[i] + 26)%26) + 'A';
            }
        }
    }

    strcpy(old_name,name);
    strcat(old_name,ext);

    free(name);
    free(ext);
    
    return old_name;
}
// mkdir atau renam AtoZ_
char *atoz(char *old_name){
    return atBash(old_name,0);
}

// mkdir RX_ decript & encript
char *rxMk(char *old_name){
    return atBash(atBash(old_name,0),1);
}

// rename RX_ en
char *rxRnEn(char *old_name){
    return toVig(atBash(old_name,0),0);
}

// rename RX_ de
char *rxRnDe(char *old_name){
    return atBash(atBash(old_name,1),0);
}

// mkdir atau renam A_is_a En
char *aIsaEn(char *old_name){
    return toLow(old_name);
}

// mkdir atau renam A_is_a De
char *aIsaDe(char *old_name){
    return toNor(old_name);
}

void logFile(char *level, char *cmd,char *desc) {
    FILE *fp = fopen(logpath, "a");
    time_t t;
    struct tm *tmp;
    char tmBuff[100];
    time(&t);
    tmp = localtime(&t);
    strftime(tmBuff, sizeof(tmBuff), "%y%m%d-%H:%M:%S", tmp);

    fprintf(fp, "%s::%s::%s::%s", level, tmBuff, cmd, desc);
    fprintf(fp, "\n");
    
    fclose(fp);
}

void enFilesRecursively(char *basePath){
    int res;
    char fpath[1000],tpath[1000];
    char path[1000];
    char file[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;


    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(fpath, basePath);
            strcat(fpath, "/");
            strcat(fpath, dp->d_name);
            enFilesRecursively(fpath);
            
            strcpy(path,basePath);
            strcat(path,"/");

            strcpy(tpath,dp->d_name);
            strcpy(file,tpath);
            if(strstr(fpath,atozTag)!= NULL){
                printf("atoz\n");
                if(strstr(dp->d_name,atozTag)==NULL){
                    atoz(file);
                    strcat(path, file);
                    printf("%s\n%s\n",fpath,path);
                    res = rename(fpath, path);
                }
                if(res == -1) printf("%d\n",errno);
            }else if(strstr(fpath,rxTag)!= NULL){
                printf("rx\n");
                if(strstr(dp->d_name,rxTag)==NULL){
                    rxRnEn(file);
                    strcat(path, file);
                    printf("%s\n%s\n",fpath,path);
                    res = rename(fpath, path);
                }
                if(res == -1) printf("%d\n",errno);
            }else if(strstr(fpath,aisaTag)!= NULL){
                printf("aisa\n");
                if(strstr(dp->d_name,aisaTag)==NULL){
                    aIsaEn(file);
                    strcat(path, file);
                    printf("%s\n%s\n",fpath,path);
                    res = rename(fpath, file);
                }
                if(res == -1) printf("%d\n",errno);
            }

            char desc[100];
            sprintf(desc,"%s::%s",fpath,tpath);
            logFile("INFO","RENAME",desc);

        }
    }
    closedir(dir);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    // printf("getattr: %s\n",stbuf);
    printf("%ld\n",(uintmax_t)stbuf->st_dev);
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);

    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    //log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","GETATTR",desc);

    return 0;
}

static int xmp_create(const char *path, mode_t mode,
                      struct fuse_file_info *fi)
{
    printf("Create\n");
    int res;
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    res = open(fpath, fi->flags, mode);
    if (res == -1)
            return -errno;
    fi->fh = res;

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","CREATE",desc);

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    printf("readdir\n");
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
		
        char custPath[1000];
		
        strcpy(custPath, de->d_name);

		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			res = (filler(buf, de->d_name, &st, 0));
		else 
			res = (filler(buf, custPath, &st, 0));

        if(res!=0) break;
    }

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","READDIR",desc);

    closedir(dp);

    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode){
	printf("mkdir\n");
    char fpath[1000];
	
	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	int res;
	res = mkdir(fpath, mode);
	if(res == -1) return -errno;

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","MKDIR",desc);

	return 0;
}

static int xmp_rmdir(const char *path)
{
    printf("rmdir\n");
    int res;
    char fpath[1000];

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}
	else sprintf(fpath, "%s%s", dirpath, path);
    res = rmdir(fpath);
    if (res == -1)
            return -errno;

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("WARNING","RMDIR",desc);

    return 0;
}

static int xmp_rename(const char *from, const char *to){
	printf("rename\n");
    
    char fpath[1000], tpath[1000];
	
	if(strcmp(from, "/") == 0){
		from = dirpath;
		sprintf(fpath, "%s", from);
	}
	else{
		sprintf(fpath, "%s%s", dirpath, from);
	}

	if(strcmp(to, "/") == 0){
		to = dirpath;
		sprintf(tpath, "%s", to);
	}
	else{
		sprintf(tpath, "%s%s", dirpath, to);
	}

	int res;

	res = rename(fpath, tpath);
	if(res == -1) return -errno;
    // printf("%s\n",tpath);
    enFilesRecursively(tpath);

    // log

    char desc[100];
    sprintf(desc,"%s::%s",fpath,tpath);
    logFile("INFO","RENAME",desc);

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    printf("open\n");
    int res;

    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    res = open(fpath, fi->flags);
    if (res == -1)
            return -errno;
    fi->fh = res;

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","OPEN",desc);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    printf("read\n");
    int res = 0;
    int fd = 0 ;

    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","READ",desc);

    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    printf("write\n");
    int fd;
    int res;
    (void) fi;

    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    if(fi == NULL)
            fd = open(path, O_WRONLY);
    else
            fd = fi->fh;
    
    if (fd == -1)
            return -errno;
    res = pwrite(fd, buf, size, offset);
    if (res == -1)
            res = -errno;
    if(fi == NULL)
            close(fd);

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","WRITE",desc);

    return res;
}

#ifdef HAVE_UTIMENSAT
static int xmp_utimens(const char *path, const struct timespec ts[2],
                       struct fuse_file_info *fi)
{
    printf("utimse\n");
    (void) fi;
    int res;
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    /* don't use utime/utimes since they follow symlinks */
    res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
    if (res == -1)
            return -errno;

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","UTIMENS",desc);

    return 0;
}
#endif

static int xmp_link(const char *from, const char *to)
{
    printf("link\n");
    char fpath[1000], tpath[1000];
	
	if(strcmp(from, "/") == 0){
		from = dirpath;
		sprintf(fpath, "%s", from);
	}
	else{
		sprintf(fpath, "%s%s", dirpath, from);
	}

	if(strcmp(to, "/") == 0){
		to = dirpath;
		sprintf(tpath, "%s", to);
	}
	else{
		sprintf(tpath, "%s%s", dirpath, to);
	}
    
    int res;
    res = link(fpath, tpath);
    if (res == -1)
            return -errno;

    // log

    char desc[100];
    sprintf(desc,"%s::%s",fpath,tpath);
    logFile("INFO","LINK",desc);

    return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    printf("readlink\n");
    
    int res;

    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    res = readlink(path, buf, size - 1);
    if (res == -1)
            return -errno;
    buf[res] = '\0';

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("INFO","READLINK",desc);

    return 0;
}

static int xmp_unlink(const char *path)
{
    int res;
    
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    
    res = unlink(fpath);
    if (res == -1)
            return -errno;

    // log

    char desc[100];
    strcpy(desc, path);
    logFile("WARNING","UNLINK",desc);

    return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.mkdir = xmp_mkdir,
	.readdir = xmp_readdir,
	.rmdir = xmp_rmdir,
	.read = xmp_read,
	.rename = xmp_rename,
	.create = xmp_create,
	.open = xmp_open,
	.write = xmp_write,
	.link = xmp_link,
	.readlink = xmp_readlink,
	.unlink = xmp_unlink,
	// .symlink = xmp_symlink,
	// .utimens = xmp_utimens,
};

int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}