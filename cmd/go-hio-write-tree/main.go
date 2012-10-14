package main

import (
	"flag"
	"fmt"

	"github.com/sbinet/go-croot/pkg/croot"
	hio "github.com/sbinet/go-hep/pkg/hep/io"
)

type Det struct {
	E float64
	T float64
}

type Event struct {
	A Det
	B Det
}

var evtmax *int = flag.Int("evtmax", 10000, "number of events to generate")

func tree0(f *hio.File) {
	t, err := f.CreateTuple("events")
	if err != nil {
		panic(err)
	}

	// fill some events with random numbers
	nevents := *evtmax
	for iev := 0; iev != nevents; iev++ {
		if iev%1000 == 0 {
			fmt.Printf(":: processing event %d...\n", iev)
		}

		e := Event{}
		// the two energies follow a gaussian distribution
		ea, eb := croot.GRandom.Rannord()
		e.A.E = ea
		e.B.E = eb

		e.A.T = croot.GRandom.Rndm(1)
		e.B.T = e.A.T * croot.GRandom.Gaus(0., 1.)

		if iev%1000 == 0 {
			fmt.Printf("evt.a.e= %8.3f\n", e.A.E)
			fmt.Printf("evt.a.t= %8.3f\n", e.A.T)
			fmt.Printf("evt.b.e= %8.3f\n", e.B.E)
			fmt.Printf("evt.b.t= %8.3f\n", e.B.T)
		}
		err = t.Write(e)
		if err != nil {
			panic(err)
		}
	}
}

func main() {
	flag.Parse()
	fname := "event.hep"

	f, err := hio.Create(fname)
	if err != nil {
		panic(err)
	}

	tree0(f)
	err = f.Close()
	if err != nil {
		panic(err)
	}
}

// EOF
