package main

import (
	"fmt"
	"flag"

	"github.com/sbinet/go-hdf5/pkg/hdf5"
	"github.com/sbinet/go-croot/pkg/croot"
)

type Det struct {
	e float64
	t float64
}

type Event struct {
	a Det
	b Det
}

var evtmax *int = flag.Int("evtmax", 10000, "number of events to generate")

func tree0(f *hdf5.File) {
	// create a tree
	e := Event{}
	chunk_size := 1024*2
	compression:=  3
	t,err := f.CreateTableFrom("tree", e, chunk_size, compression)
	if err != nil {
		panic(err)
	}
	
	// fill some events with random numbers
	nevents := *evtmax
	for iev := 0; iev != nevents; iev++ {
		if iev%1000 == 0 {
			fmt.Printf(":: processing event %d...\n", iev)
		}

		// the two energies follow a gaussian distribution
		ea, eb := croot.GRandom.Rannord()
		e.a.e = ea
		e.b.e = eb

		e.a.t = croot.GRandom.Rndm(1)
		e.b.t = e.a.t * croot.GRandom.Gaus(0., 1.)

		if iev%1000 == 0 {
			fmt.Printf("evt.a.e= %8.3f\n", e.a.e)
			fmt.Printf("evt.a.t= %8.3f\n", e.a.t)
			fmt.Printf("evt.b.e= %8.3f\n", e.b.e)
			fmt.Printf("evt.b.t= %8.3f\n", e.b.t)
		}
		t.Append(&e)
	}
	t.Close()
}

func main() {
	flag.Parse()
	f,err := hdf5.CreateFile("event.h5", hdf5.F_ACC_TRUNC)
	if err != nil {
		panic(err)
	}
	tree0(f)
	f.Close()
}
// EOF
