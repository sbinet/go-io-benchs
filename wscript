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
        #use     = ['github.com/sbinet/go-hdf5/pkg/hdf5',
        #           'github.com/sbinet/go-croot/pkg/croot',]
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-gob-write-tree/main.go',
        target  = 'go-gob-write-tree',
        use     = ['github.com/sbinet/go-croot/pkg/croot',]
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-hio-write-tree/main.go',
        target  = 'bench-hio-ex-tree',
        #use     = ['github.com/sbinet/go-croot/pkg/croot',
        #           'github.com/sbinet/go-hep/pkg/hep/io']
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
        #use     = [],
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-hio-read-tree/main.go',
        target  = 'go-hio-read-tree',
        #use     = ['github.com/sbinet/go-hep/pkg/hep/io']
        )

    ctx(features='go goprogram',
        source  = 'cmd/go-croot-read-tree/main.go',
        target  = 'go-croot-read-tree',
        #use     = ['github.com/sbinet/go-croot/pkg/croot',]
        )

    #ctx.install_files('${PREFIX}/include', 'include/croot.h')
