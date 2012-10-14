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

var evtmax *int = flag.Int("evtmax", 10000, "number of events to read")
var fname *string = flag.String("fname", "event.go.root", "file to read back")

func tree0(f croot.File) {
	t := f.Get("tree").(croot.Tree)
	//e := (*Event)(nil)//
	e := Event{}
	e.E = make([]float64,0)

	t.SetBranchAddress("evt", &e)
	/*
	t.SetBranchAddress("evt_a_e", &e.A.E)
	t.SetBranchAddress("evt_a_t", &e.A.T)
	t.SetBranchAddress("evt_b_e", &e.B.E)
	t.SetBranchAddress("evt_b_t", &e.B.T)
	t.SetBranchAddress("evt_c", &e.C[0])
	 */

	// fill some events with random numbers
	nevents := int64(*evtmax)
	if nevents < 0 || nevents > int64(t.GetEntries()) {
		nevents = int64(t.GetEntries())
	}
	for iev := int64(0); iev != nevents; iev++ {
		if iev%1000 == 0 {
			fmt.Printf(":: processing event %d...\n", iev)
		}
		if t.GetEntry(iev, 1) <= 0 {
			panic("error")
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
	}
}

func main() {
	flag.Parse()
	
	croot.RegisterType(&Event{})
	fname := "event.go.root"
	fmt.Printf(":: opening [%s]...\n", fname)
	f := croot.OpenFile(fname, "read", "my event file", 1, 0)
	tree0(f)
	f.Close("")

}
// EOF