#include <stdio.h>
#include <stdlib.h>

#include "croot/croot.h"

#ifdef __cplusplus
#include <typeinfo>
#endif

#include "croot_event.h"

int evtmax = 1000;

void tree0(CRoot_File f) 
{
  /* create a tree */
  CRoot_Tree tree = (CRoot_Tree)CRoot_File_Get(f, "tree");
  Event *e = (Event*)malloc(sizeof(Event));

  /*int32_t bufsiz = 32000;*/

  /* create a branch with energy */
  CRoot_Tree_SetBranchAddress(tree, "evt", &e, 0);

  /* fill some events with random numbers */
  int32_t nevents = evtmax;
  for (int32_t iev=0; iev<nevents; iev++) {
    if (iev%1000 == 0) {
      printf("processing event %i...\n", iev);
    }
    {
      int64_t nb = CRoot_Tree_LoadTree(tree, iev);
      if (nb < 0) {
        printf("** error loading tree [%i]\n", iev);
        break;
      }
    }
    
    {
      int32_t nb = CRoot_Tree_GetEntry(tree, iev, 1);
      if (nb < 0) {
        printf("** error loading event [%i]\n", iev);
        break;
      }
    }

    if (iev%1000 == 0) {
      printf("evt.a.e= %8.3f\n", e->A.E);
      printf("evt.a.t= %8.3f\n", e->A.T);
      printf("evt.b.e= %8.3f\n", e->B.E);
      printf("evt.b.t= %8.3f\n", e->B.T);
    }
  }
}

int main(int argc, char **argv)
{
  CRoot_Cintex_Enable();
  define_structs();

  const char *fname = "event.c.root";
  //const char *fname = "event.go.root";
  printf(":: opening [%s]...\n", fname);
  CRoot_File f = CRoot_File_Open(fname, "read",
                                 "my event file", 1, 0);

  if (argc>1) {
    evtmax = atoi(argv[1]);
  }

  tree0(f);
  CRoot_File_Close(f, "");

  return 0;
}
