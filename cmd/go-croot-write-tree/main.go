package main

import (
	"flag"
	"fmt"
	"math/rand"

	"github.com/sbinet/go-croot/pkg/croot"
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
var fname *string = flag.String("fname", "event.root", "file to create")

func tree0(f croot.File) {
	// create a tree
	tree := croot.NewTree("tree", "tree", 32)
	e := &Event{}
	const bufsiz = 32000

	tree.Branch("evt", e, bufsiz, 0)

	// fill some events with random numbers
	nevents := *evtmax
	for iev := 0; iev != nevents; iev++ {
		if iev%1000 == 0 {
			fmt.Printf(":: processing event %d...\n", iev)
		}

		// the two energies follow a gaussian distribution
		e.A.E = rand.NormFloat64() //ea
		e.B.E = rand.NormFloat64() //eb

		e.A.T = croot.GRandom.Rndm(1)
		e.B.T = e.A.T * croot.GRandom.Gaus(0., 1.)
		if iev%1000 == 0 {
			fmt.Printf("ievt: %d\n", iev)
			fmt.Printf("evt.a.e= %8.3f\n", e.A.E)
			fmt.Printf("evt.a.t= %8.3f\n", e.A.T)
			fmt.Printf("evt.b.e= %8.3f\n", e.B.E)
			fmt.Printf("evt.b.t= %8.3f\n", e.B.T)
		}
		tree.Fill()
	}
	f.Write("", 0, 0)
}

func main() {
	flag.Parse()
	f := croot.OpenFile(*fname, "recreate", "my event file", 1, 0)
	tree0(f)
	f.Close("")
}

// EOF
