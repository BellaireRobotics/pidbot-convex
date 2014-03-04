char jhc_c_compile[] = "gcc build_haskell/rts/profile.c build_haskell/rts/rts_support.c build_haskell/rts/gc_none.c build_haskell/rts/jhc_rts.c build_haskell/lib/lib_cbits.c build_haskell/rts/gc_jgc.c build_haskell/rts/stableptr.c build_haskell/rts/conc.c -Ibuild_haskell/cbits -Ibuild_haskell build_haskell/hs_main.c -o build_haskell/hs_main.c '-std=gnu99' -D_GNU_SOURCE '-falign-functions=4' -ffast-math -Wextra -Wall -Wno-unused-parameter -fno-strict-aliasing -DNDEBUG -O3 '-D_JHC_GC=_JHC_GC_JGC' '-D_JHC_CONC=_JHC_CONC_NONE'";
char jhc_command[] = "ajhc -fffi --tdir=build_haskell -C --include=hs_src -o build_haskell/hs_main.c hs_src/Main.hs";
char jhc_version[] = "ajhc 0.8.0.10 (66a602abc10dec74e2c0bb9289819a015bf21e4f)";

#include "jhc_rts_header.h"
struct s_caches_pub {
};


void jhc_hs_init(gc_t gc,arena_t arena) ;
void _amain(void) ;
static void b__main(gc_t gc,arena_t arena) A_STD;
static void ftheMain(gc_t gc,arena_t arena) A_STD;
/* CAFS */

const void * const nh_stuff[] = {
NULL
};


void 
jhc_hs_init(gc_t gc,arena_t arena)
{
        alloc_public_caches(arena,sizeof(struct s_caches_pub));
}

void 
_amain(void)
{
        arena_t arena;
        gc_t gc;
        gc = NULL;
        arena = NULL;
        jhc_alloc_init(&gc,&arena);
        jhc_hs_init(gc,arena);
        b__main(gc,arena);
        jhc_alloc_fini(gc,arena);
}

static void A_STD
b__main(gc_t gc,arena_t arena)
{
        ftheMain(gc,arena);
        return;
}

static void A_STD
ftheMain(gc_t gc,arena_t arena)
{
        vexMotorSet((int)1,(int)60);
        return;
}
