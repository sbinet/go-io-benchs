#include <stdio.h>
#include <stdlib.h>

#include "croot/croot.h"

#ifdef __cplusplus
#include <typeinfo>
#endif

#include "croot_event.h"

int evtmax = 10000;

void tree0(CRoot_File f) 
{
  /* create a tree */
  CRoot_Tree tree = CRoot_Tree_new("tree", "tree", 32);
  Event *e = (Event*)malloc(sizeof(Event));

  int32_t bufsiz = 32000;

  /* create a branch with energy */
  CRoot_Tree_Branch(tree, "evt", "Event", &e, bufsiz, 0);

  /* fill some events with random numbers */
  int32_t nevents = evtmax;
  for (int32_t iev=0; iev<nevents; iev++) {
    if (iev%1000 == 0) {
      printf("processing event %i...\n", iev);
    }

    float ea = 0., eb = 0.;
    /* the two energies follow a gauss distribution */
    CRoot_Random_Rannorf(CRoot_gRandom, &ea, &eb);
    e->A.E = ea;
    e->B.E = eb;
    
    e->A.T = CRoot_Random_Rndm(CRoot_gRandom, 1);
    e->B.T = e->A.T * CRoot_Random_Gaus(CRoot_gRandom, 0., 1.);

    if (iev%1000 == 0) {
      printf("evt.a.e= %8.3f\n", e->A.E);
      printf("evt.a.t= %8.3f\n", e->A.T);
      printf("evt.b.e= %8.3f\n", e->B.E);
      printf("evt.b.t= %8.3f\n", e->B.T);
    }
    /* fill the tree with the current event */
    CRoot_Tree_Fill(tree);
  }
  CRoot_File_Write(f, 0, 0, 0);
}

int main(int argc, char **argv)
{
  CRoot_Cintex_Enable();
  define_structs();

  const char *fname = "event.root";
  printf(":: writing to [%s]...\n", fname);
  CRoot_File f = CRoot_File_Open(fname, "recreate",
                                 "my event file", 1, 0);

  if (argc>1) {
    evtmax = atoi(argv[1]);
  }

  tree0(f);
  CRoot_File_Close(f, "");

  return 0;
}
