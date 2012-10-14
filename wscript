#

top = '.'
out = '__build__'

def options(ctx):
    ctx.load('compiler_c')
    ctx.load('compiler_cxx')
    
def configure(ctx):

    ctx.load('compiler_c')
    ctx.load('compiler_cxx')
    ctx.load('go cgo', tooldir='common')

    # locate ROOT
    ctx.check_cfg(path="root-config",
                  package="",
                  uselib_store="ROOT",
                  args='--libs --cflags', mandatory=True)

    # locate croot
    ctx.check_cfg(package="croot",
                  uselib_store="croot",
                  args='--libs --cflags', mandatory=True)
    
def build(ctx):

    ctx(features='c cprogram',
        source  = 'cmd/c-croot-write-tree/main.c',
        target  = 'c-croot-write-tree',
        use     = ['croot',],
        cflags  = ['-std=c99'],
        )

    ctx(features='cxx cxxprogram',
        source  = 'cmd/cxx-croot-write-tree/main.cxx',
        target  = 'cxx-croot-write-tree',
        use     = ['ROOT', 'croot'],
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-croot-write-tree/main.go',
        target  = 'go-croot-write-tree',
        use     = ['github.com/sbinet/go-croot/pkg/croot',]
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-h5-write-tree/main.go',
        target  = 'go-h5-write-tree',
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-gob-write-tree/main.go',
        target  = 'go-gob-write-tree',
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-hio-write-tree/main.go',
        target  = 'bench-hio-ex-tree',
        )

    # readers...
    
    ctx(features='c cprogram',
        source  = 'cmd/c-croot-read-tree/main.c',
        target  = 'c-croot-read-tree',
        use     = ['croot',],
        cflags  = ['-std=c99'],
        )

    ctx(features='cxx cxxprogram',
        source  = 'cmd/cxx-croot-read-tree/main.cxx',
        target  = 'cxx-croot-read-tree',
        use     = ['ROOT', 'croot',],
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-gob-read-tree/main.go',
        target  = 'go-gob-read',
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-hio-read-tree/main.go',
        target  = 'go-hio-read-tree',
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-croot-read-tree/main.go',
        target  = 'go-croot-read-tree',
        )

def run(ctx):
    import os, sys
    try:
        from commands import getstatusoutput
    except ImportError: # py3
        from subprocess import getstatusoutput
    import shutil
    try:
        shutil.rmtree("test-io-benchs")
    except OSError:
        pass
    cwd = os.getcwd()
    for d in ["go","cxx"]:
        os.chdir(cwd)
        testdir = os.path.join("test-io-benchs",d)
        os.makedirs(testdir)
        os.chdir(testdir)
        for step in ("write", "read"):
            cmd = "time ../../__build__/%s-croot-%s-tree >& /dev/null" % (d, step)
            status, output = getstatusoutput(cmd)
            sys.stdout.write("%s\n" % ("*"*80),)
            if status == 0:
                sys.stdout.write("%s-%s:%s\n" % (step, d,output))
            else:
                sys.stdout.write("%s-%s: **ERROR**\n%s\n" % (step,d,output))
