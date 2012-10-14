#ifndef CROOT_TEST_EVENT_H
#define CROOT_TEST_EVENT_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memset */

#include "croot/croot.h"

#ifdef __cplusplus
#include <typeinfo>
#endif

typedef struct {
  double E;
  double T;
} Det;

typedef struct {
  Det A;
  Det B;
  //double C[2];
  //char *D;
} Event;

size_t g_sz_det = sizeof(Det);
size_t g_sz_evt = sizeof(Event);

/* stub function for Det */
void ctor_Det(void *retaddr, void *mem, void* args, void* ctx)
{
  //#ifdef __cpl
  void* dummy = 0;
  fprintf(stderr,"::ctor-fct[Det] [%p] [%p] [%p] [%p]\n", retaddr, mem, args, ctx);
  if (retaddr) *(void**)retaddr = (Det*)realloc(mem, sizeof(Det));
  else dummy = (Det*)realloc(mem, sizeof(Det));
  if (dummy) {}
}

void dtor_Det(void* retaddr, void *o, void* args, void *ctx)
{
  fprintf(stderr, "::dtor-fct[Det] [%p] [%p] [%p] [%p]\n", retaddr, o, args, ctx);
  free((Det*)o);
}

/* stub functions for Event */
void ctor_Event(void *retaddr, void *mem, void* args, void* ctx)
{
  size_t* sz = (size_t*)ctx;
  fprintf(stderr, "::ctor-fct[Event] [%p] [%p] [%p] [%p] [%zd]\n", retaddr, mem, args, ctx, *sz);
  //#ifdef __cpl
  //void* dummy = 0;
  if (retaddr) *(void**)retaddr = memset(mem, 0, *sz);
  else retaddr = memset(mem, 0, *sz);
}

void dtor_Event(void* retaddr, void *o, void* args, void *ctx)
{
  fprintf(stderr, "::dtor-fct[Event] [%p] [%p] [%p] [%p]\n", retaddr, o, args, ctx);
  free(o);
}

void define_structs_reflex() 
{
  CRoot_Reflex_Type ty_double = CRoot_Reflex_Type_ByName("double");
  CRoot_Reflex_Type ty_void   = CRoot_Reflex_Type_ByName("void");
  CRoot_Reflex_Type ty_det = 0;//CRoot_Reflex_Type_new("Det", 0);
  CRoot_Reflex_Type ty_evt = 0;//CRoot_Reflex_Type_new("::Event", 0);
  // define Det...
  {
    printf("==> Det <===\n");
    printf("... classbuilder ...\n");
    CRoot_Reflex_ClassBuilder cb = CRoot_Reflex_ClassBuilder_new
      ("Det",
#ifdef __cplusplus
       (void*)&typeid(Det), 
#else
       0,
#endif
       sizeof(Det),
       CRoot_Reflex_PUBLIC | CRoot_Reflex_ARTIFICIAL,
       CRoot_Reflex_STRUCT );

    printf("... classbuilder ...[done]\n");
    size_t offset = 0;
    CRoot_Reflex_ClassBuilder_AddDataMember(cb, 
                                            ty_double,
                                            "E",
                                            offset,
                                            CRoot_Reflex_PUBLIC
                                            );
    printf("... classbuilder ... added data-member\n");
    offset += sizeof(double);
    CRoot_Reflex_ClassBuilder_AddDataMember(cb,
                                            ty_double,
                                            "T",
                                            offset,
                                            CRoot_Reflex_PUBLIC
                                            );
    printf("... classbuilder ... added data-member\n");
    void *ctx = &g_sz_det;
    CRoot_Reflex_Type ty_ctor = CRoot_Reflex_FunctionTypeBuilder_new(ty_void);
    CRoot_Reflex_ClassBuilder_AddFunctionMember(cb,
                                                ty_ctor,
                                                "Det",
                                                (CRoot_Reflex_StubFunction)&ctor_Det,
                                                ctx,
                                                0,
                                                CRoot_Reflex_PUBLIC | CRoot_Reflex_CONSTRUCTOR
                                                );
    printf("... classbuilder ... added ctor\n");
    CRoot_Reflex_Type_delete(ty_ctor);
    ty_det = CRoot_Reflex_ClassBuilder_ToType(cb);
    printf("... classbuilder ... to-type\n");
    CRoot_Reflex_ClassBuilder_delete(cb);
    printf("... classbuilder ... deleted\n");
    printf("... fireing class callback...\n");
    CRoot_Reflex_FireClassCallback(ty_det);
    printf("... fireing class callback... [done]\n");

    //CRoot_Reflex_Type_UpdateMembers(ty_det);
    printf(":: Det-size: %zd\n", CRoot_Reflex_Type_SizeOf(ty_det));
    printf(":: Det-mbrs: %zd\n", CRoot_Reflex_Type_DataMemberSize(ty_det, CRoot_Reflex_INHERITEDMEMBERS_NO));

    {
      size_t nmbrs = CRoot_Reflex_Type_MemberSize(ty_det, CRoot_Reflex_INHERITEDMEMBERS_ALSO);
      for (size_t i=0; i<nmbrs; i++) {
        CRoot_Reflex_Member mbr = CRoot_Reflex_Type_MemberAt(ty_det, i, CRoot_Reflex_INHERITEDMEMBERS_ALSO);
        const char* mbr_name = CRoot_Reflex_Member_Name(mbr);
        printf(":: -- Det.Mbr[%zd] -- [%s]\n", i, mbr_name);
        CRoot_Reflex_Member_delete(mbr);
      }
    }
  }
  // define Event...
  {
    printf("==> Event <===\n");
    //CRoot_Reflex_Type_SetSize(ty_evt, sizeof(Event));
    printf("... classbuilder ...\n");
    CRoot_Reflex_ClassBuilder cb = CRoot_Reflex_ClassBuilder_new
      ("Event",
#ifdef __cplusplus
       (void*)&typeid(Event), 
#else
       0,
#endif
       sizeof(Event),
       CRoot_Reflex_PUBLIC | CRoot_Reflex_ARTIFICIAL,
       CRoot_Reflex_STRUCT);
    printf("... classbuilder ...[done]\n");
    size_t offset = 0;
    CRoot_Reflex_ClassBuilder_AddDataMember(cb,
                                            ty_det, 
                                            "A",
                                            offset,
                                            CRoot_Reflex_PUBLIC
                                            );
    printf("... classbuilder ... added data-member\n");
    offset += sizeof(Det);//CRoot_Reflex_Type_SizeOf(ty_det);
    CRoot_Reflex_ClassBuilder_AddDataMember(cb,
                                            ty_det,
                                            "B",
                                            offset,
                                            CRoot_Reflex_PUBLIC
                                            );
    printf("... classbuilder ... added data-member\n");
    void *ctx = &g_sz_evt;
    printf("... classbuilder ctx [%p]...\n", ctx);
    printf("... classbuilder stu [%p]...\n", (void*)(&ctor_Event));
    CRoot_Reflex_Type ty_ctor = CRoot_Reflex_FunctionTypeBuilder_new(ty_void);
    CRoot_Reflex_ClassBuilder_AddFunctionMember(cb,
                                                ty_ctor,
                                                "Event",
                                                (CRoot_Reflex_StubFunction)&ctor_Event,
                                                ctx,
                                                "",
                                                CRoot_Reflex_PUBLIC | CRoot_Reflex_CONSTRUCTOR
                                                );
    printf("... classbuilder ... added ctor\n");
    CRoot_Reflex_Type_delete(ty_ctor);
    ty_evt = CRoot_Reflex_ClassBuilder_ToType(cb);
    printf("... classbuilder ... to-type\n");
    CRoot_Reflex_ClassBuilder_delete(cb);
    printf("... classbuilder ... deleted\n");
    CRoot_Reflex_Type_delete(ty_evt);
    ty_evt = CRoot_Reflex_Type_ByName("Event");
    printf("... fireing class callback...\n");
    CRoot_Reflex_FireClassCallback(ty_evt);
    printf("... fireing class callback... [done]\n");

    //CRoot_Reflex_Type_UpdateMembers(ty_evt);
    //CRoot_Reflex_Type_SetSize(ty_evt, sizeof(Event));
    printf(":: Evt-size: %zd %zd\n",
           CRoot_Reflex_Type_SizeOf(ty_evt),
           sizeof(Event));
    size_t nmbrs = CRoot_Reflex_Type_DataMemberSize(ty_evt, CRoot_Reflex_INHERITEDMEMBERS_NO);
    printf(":: Evt-mbrs: %zd\n", nmbrs);
    printf(":: Evt-type: %d\n", CRoot_Reflex_Type_RepresType(ty_evt));
    printf(":: Evt-name: %s\n", CRoot_Reflex_Type_Name(ty_evt));
    printf(":: Evt-id:   %p\n", CRoot_Reflex_Type_Id(ty_evt));

    size_t nfmbrs = CRoot_Reflex_Type_FunctionMemberSize(ty_evt, CRoot_Reflex_INHERITEDMEMBERS_NO);
    printf(":: Evt-fct-mbrs: %zd\n", nfmbrs);
    for (size_t ii = 0; ii < nfmbrs; ii++) {
      CRoot_Reflex_Member mbr = CRoot_Reflex_Type_FunctionMemberAt(ty_evt, ii, CRoot_Reflex_INHERITEDMEMBERS_NO);
      printf("::  fct-mbr[%zd][%s] - ctx [%p] fct[%p]\n",
             ii,
             CRoot_Reflex_Member_Name(mbr), 
             CRoot_Reflex_Member_Stubcontext(mbr),
             CRoot_Reflex_Member_Stubfunction(mbr));
    }
    /* for (size_t imbr = 0; imbr < nmbrs; ++imbr) { */
    /*   CRoot_Reflex_Member mbr = CRoot_Reflex_Type_ */
    /* } */
  }

}

void define_structs_cint() 
{
  // define Det...
  {
    CRoot_Cint_TagInfo ti = CRoot_Cint_TagInfo_new();
    CRoot_Cint_TagInfo_SetTagNum(ti, -1);
    CRoot_Cint_TagInfo_SetTagType(ti, 's');
    CRoot_Cint_TagInfo_SetTagName(ti, "Det");
    int tagnum = CRoot_Cint_Defined_TagName("Det", 2);
    CRoot_Cint_TagInfo_SetTagNum(ti, tagnum);
    printf("Det.tagnum: %d\n", tagnum);
    {
      printf("==> Det <===\n");
      int rc = -1;
      rc = CRoot_Cint_TagInfo_GetLinkedTagNum(ti);
      printf("%d: %d\n", __LINE__, rc);
      tagnum = CRoot_Cint_Defined_TagName("Det", 2);
      printf("Det.tagnum: %d\n", tagnum);

      rc = CRoot_Cint_TagTable_Setup
        (
         tagnum,
         2*sizeof(double),   // size
         -1,                 // cpplink
         0x00020000,         // isabstract
         0,                  // comment
         0,                  // G__setup_memvarMyClass
         0                   // G__setup_memfuncMyClass
         );
      printf("%d: %d\n", __LINE__, rc);

      rc = CRoot_Cint_Tag_MemVar_Setup(tagnum);
      printf("%d: %d\n", __LINE__, rc);

      /* Det.e */
      int mbr_offset = 0;
      int mbr_type = 'd';
      int mbr_indir = 0;
      int mbr_tagnum = -1;
      int mbr_typnum = -1;
      int mbr_isstatic = -1;
      int mbr_access = 0x01; /* public */
      rc = CRoot_Cint_MemVar_Setup
        ((void*)&mbr_offset, 
         mbr_type,
         mbr_indir,
         0,
         mbr_tagnum,
         mbr_typnum,
         mbr_isstatic,
         mbr_access,
         "E=",
         0,
         0
         );
      printf("%d: %d\n", __LINE__, rc);

      /* Det.t */
      mbr_offset += sizeof(double);
      rc = CRoot_Cint_MemVar_Setup
        ((void*)&mbr_offset, 
         mbr_type,
         mbr_indir,
         0,
         mbr_tagnum,
         mbr_typnum,
         mbr_isstatic,
         mbr_access,
         "T=",
         0,
         0
         );
      printf("%d: %d\n", __LINE__, rc);

      rc = CRoot_Cint_Tag_MemVar_Reset();
      printf("%d: %d\n", __LINE__, rc);

    }
  }
  // define Event...
  {
    CRoot_Cint_TagInfo ti = CRoot_Cint_TagInfo_new();
    CRoot_Cint_TagInfo_SetTagNum(ti, -1);
    CRoot_Cint_TagInfo_SetTagType(ti, 's');
    CRoot_Cint_TagInfo_SetTagName(ti, "Event");
    int tagnum = CRoot_Cint_Defined_TagName("Event", 2);
    CRoot_Cint_TagInfo_SetTagNum(ti, tagnum);
    printf("Event.tagnum: %d\n", tagnum);
    {
      printf("==> Event <===\n");
      int rc = -1;
      rc = CRoot_Cint_TagInfo_GetLinkedTagNum(ti);
      printf("%d: %d\n", __LINE__, rc);
      tagnum = CRoot_Cint_Defined_TagName("Event", 2);
      printf("Event.tagnum: %d\n", tagnum);

      rc = CRoot_Cint_TagTable_Setup
        (
         tagnum,
         2*sizeof(Det),   // size
         -1,                 // cpplink
         0x00020000,         // isabstract
         0,                  // comment
         0,                  // G__setup_memvarMyClass
         0                   // G__setup_memfuncMyClass
         );
      printf("%d: %d\n", __LINE__, rc);

      rc = CRoot_Cint_Tag_MemVar_Setup(tagnum);
      printf("%d: %d\n", __LINE__, rc);

      /* Event.a */
      int mbr_offset = 0;
      int mbr_type = 's';
      int mbr_indir = 0;
      int mbr_tagnum = CRoot_Cint_Defined_TagName("Det", 2);
      int mbr_typnum = -1;
      int mbr_isstatic = -1;
      int mbr_access = 0x01; /* public */
      rc = CRoot_Cint_MemVar_Setup
        ((void*)&mbr_offset, 
         mbr_type,
         mbr_indir,
         0,
         mbr_tagnum,
         mbr_typnum,
         mbr_isstatic,
         mbr_access,
         "A=",
         0,
         0
         );
      printf("%d: %d\n", __LINE__, rc);

      /* Event.b */
      mbr_offset += sizeof(Det);
      rc = CRoot_Cint_MemVar_Setup
        ((void*)&mbr_offset, 
         mbr_type,
         mbr_indir,
         0,
         mbr_tagnum,
         mbr_typnum,
         mbr_isstatic,
         mbr_access,
         "B=",
         0,
         0
         );
      printf("%d: %d\n", __LINE__, rc);

      rc = CRoot_Cint_Tag_MemVar_Reset();
      printf("%d: %d\n", __LINE__, rc);

    }
  }
}

void define_structs()
{
  //define_structs_cint();
  define_structs_reflex();
}

#endif /* !CROOT_TEST_EVENT_H */
