package main

import (
	"fmt"
	"flag"

	"github.com/sbinet/go-croot/pkg/croot"
)

type Det struct {
	E float64
	T float64
}

type Event struct {
	A Det
	B Det
	C [2]float64
	D string
	E []float64
}

var evtmax *int = flag.Int("evtmax", 10000, "number of events to generate")

func tree0(f croot.File) {
	// create a tree
	tree := croot.NewTree("tree", "tree", 32)
	e := &Event{A: Det{E: 42., T: 666.},
		B: Det{E: -42., T: -666.},
		C: [2]float64{55., 66.},
		D: "foo",
	E: []float64{1.,2.,3.},
	}

	bufsiz := 32000

	// create a branch with energy
	tree.Branch("evt", &e, bufsiz, 0)
	/*
		tree.Branch2("evt_a_t", &e.A.T, "evt_a_e/D", bufsiz)
		tree.Branch2("evt_b_e", &e.B.E, "evt_b_e/D", bufsiz)
		tree.Branch2("evt_b_t", &e.B.T, "evt_b_t/D", bufsiz)
		n_elmts := len(e.C)
		tree.Branch2("evt_c_nbr", &n_elmts, "evt_c_nbr/I", bufsiz)
		tree.Branch2("evt_c",     &e.C[0], "evt_c[evt_c_nbr]/D", bufsiz)
	*/

	// fill some events with random numbers
	nevents := *evtmax
	for iev := 0; iev != nevents; iev++ {
		if iev%1000 == 0 {
			fmt.Printf(":: processing event %d...\n", iev)
		}

		// the two energies follow a gaussian distribution
		ea, eb := croot.GRandom.Rannord()
		e.A.E = ea
		e.B.E = eb

		e.A.T = croot.GRandom.Rndm(1)
		e.B.T = e.A.T * croot.GRandom.Gaus(0., 1.)
		e.C[0] = ea
		e.C[1] = eb
		e.D = fmt.Sprintf("-ievt--00%d", iev)
		if iev%2 == 0 {
			e.E = e.C[:1]
		} else {
			e.E = e.C[0:0]
		}
		if iev%1000 == 0 {
			fmt.Printf("ievt: %d\n", iev)
			fmt.Printf("evt.a.e= %8.3f\n", e.A.E)
			fmt.Printf("evt.a.t= %8.3f\n", e.A.T)
			fmt.Printf("evt.b.e= %8.3f\n", e.B.E)
			fmt.Printf("evt.b.t= %8.3f\n", e.B.T)
			fmt.Printf("evt.c= %v\n", e.C)
			fmt.Printf("evt.d= %s\n", e.D)
			fmt.Printf("evt.e= %v\n", e.E)
		}
		tree.Fill()
	}
	f.Write("", 0, 0)
}

func main() {
	flag.Parse()
	f := croot.OpenFile("event.go.root", "recreate", "my event file", 1, 0)
	tree0(f)
	f.Close("")
}
// EOF
