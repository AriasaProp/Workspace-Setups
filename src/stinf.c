#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048
typedef struct {
  size_t busy, total;
} cpuInf;
typedef struct {
  size_t total, avail, free;
  size_t buff, cache;
} MemInf;

static void __quick_compare(const char*,size_t,MemInf*);
int main (int argc, char **argv) {
  (void)argc;
  (void)argv;
  int i;
  size_t buff_size = BUFFER_SIZE, sz;
  FILE *f = fopen("/proc/meminfo", "r");
  if (!f) return EXIT_FAILURE;
  char *buff = (char*)malloc(BUFFER_SIZE + 1);
  char Name[64] = {0}, Unt[64] = {0};
  MemInf inf = {0};
  while ((i = getline(&buff, &buff_size, f)) > 0) {
    if (3 > sscanf(buff, "%[^:]: %zu %s", Name, &sz, Unt))
      printf("False format? %s\n", buff);
    else
      __quick_compare(Name, sz, &inf);
  }
  fclose(f);
  free(buff);
  printf(
      "| Free |Avail |Buffer|Cached|\n|"
      "\x1b[42m%05.2f%%\x1b[0m|"
      "\x1b[44m%05.2f%%\x1b[0m|"
      "\x1b[45m%05.2f%%\x1b[0m|"
      "\x1b[46m%05.2f%%\x1b[0m|\n",
    (float)inf.free/(float)inf.total*100.0f,
    (float)inf.avail/(float)inf.total*100.0f,
    (float)inf.buff/(float)inf.total*100.0f,
    (float)inf.cache/(float)inf.total*100.0f
  );
  return EXIT_SUCCESS;
}

static void __quick_compare(const char *nm, size_t v, MemInf *m) {
#define CASE(a,b,c,d) case ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24)):
  switch(*nm) {
    case 'A': break;
    case 'B':
       ++nm;
      switch(*nm) {
        case 'u': // Buffers
          m->buff = v;
          break;
      }
      break;
    case 'C':
      ++nm;
      switch(*nm) {
        case 'a': // Cached
          m->cache = v;
          break;
      }
      break;
    case 'M':
      ++nm;
      switch(*nm) {
        case 'e':
          nm += 2;
          switch (*nm) {
            case 'A': //Memory Available
              m->avail = v;
              break;
            case 'F': //Memory Free
              m->free = v;
               break;
            case 'T': //Memory Total
              m->total = v;
              break;
          }
          break;
      }
      break;
  }

/*
 * CmaFree->1904 kB
 * CmaTotal->327680 kB
 * CommitLimit->4201116 kB
 * Committed_AS->92479300 kB
 * Dirty->112 kB
 * Inactive(anon)->286976 kB
 * Inactive(file)->584568 kB
 * Inactive->871544 kB
 * KReclaimable->68284 kB
 * KernelStack->66512 kB
 * Mapped->806604 kB
 * Mlocked->67088 kB
 * NFS_Unstable->0 kB
 * PageTables->96660 kB
 * SReclaimable->64508 kB
 * SUnreclaim->202032 kB
 * Shmem->88496 kB
 * Slab->266540 kB
 * SwapCached->32236 kB
 * SwapFree->1137492 kB
 * SwapTotal->2306044 kB
 * Unevictable->67088 kB
 * VmallocChunk->0 kB
 * VmallocTotal->263061440 kB
 * VmallocUsed->117512 kB
 * Writeback->0 kB
 * WritebackTmp->0 kB
 */
}
