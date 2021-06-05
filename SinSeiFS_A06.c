#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

char newname[1024];

static  const  char *dirpath = "/home/[user]/Downloads";

void decrypt(char *path, int isEncrypt) {
  char cursor[strlen(path)];
  strcpy(cursor,path);
  int cek;
    if(isEncrypt)
      for(int i = 0; i < strlen(path);i++){
          cek = path[i];
          if(65<= cek && cek <=90){
              path[i] = 90 - (cek%65);
          }
          if(97<=cek && cek<=121){
              path[i] = 121 - (cek%97);
          }
          printf("%c\n",path[i]);
      }
}

static int xmp_mkdir(const char *path)
{
    char fpath[1024];
    if (strcmp(path,"/AtoZ_") == 0)
    {
        change_all_filename_id_dir(path);
    } 
}

static int xmp_rename(const char *path)
{
    char fpath[1024];
    if (strcmp(path,"/AtoZ_") == 0)
    {
        change_all_filename_id_dir(path);
    }
}

void atbash(char name[])
{
    for(int a=0;a<strlen(name);a++)
    {
        if (name[a]>=65 && name[a]<=90)
        {
            if (name[a]<=77)
            {
                int selisih = 78-name[a];
                name[a]=78+selisih-1;
            }
            else
            {
                int selisih = name[a]-78;
                name[a]=78-selisih-1;
            }
        }
        else if (name[a]>=97 && name[a]<=122)
        {
            if (name[a]<=109)
            {
                int selisih = 110-name[a];
                name[a]=110+selisih-1;
            }
            else
            {
                int selisih = name[a]-110;
                name[a]=110-selisih-1;
            }
        }
    }
    memset(newname,0,sizeof(newname));
    strcpy(newname,name);
    return;
}

void change_all_filename_id_dir(char *dirname)
{
    DIR *dir;
    struct dirent *ent;
    int i = 0;
    
    if ((dir = opendir (dirname)) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL) 
        {
            atbash(ent->d_name);
            rename(ent->d_name, newname);
        }
    closedir (dir);
    } 
    else 
    {
       printf(" folder not openend");
    }
    return;
}

static struct fuse_operations xmp_oper = {
    .rename = xmp_rename,
    .mkdir = xmp_mkdir,
};

int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}