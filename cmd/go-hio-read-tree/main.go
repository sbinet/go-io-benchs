package main

import (
	"fmt"
	"flag"

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
var fname *string = flag.String("fname", "event.hep", "hepfile to read back")

func tree0(f *hio.File) {
	t,err := f.OpenTuple("events")
	if err != nil {
		panic(err)
	}
	
	// fill some events with random numbers
	nevents := int64(*evtmax)
	if nevents < 0 {
		nevents = t.Entries()
	}
	if nevents > t.Entries() {
		nevents = t.Entries()
	}
	for iev := int64(0); iev != nevents; iev++ {
		if iev%1000 == 0 || true {
			fmt.Printf(":: processing event %d...\n", iev)
		}
		e := Event{}
		err = t.Read(&e)
		if err != nil {
			panic(err)
		}
		if iev%1000 == 0 || true {
			fmt.Printf("ievt= %d\n", iev)
			fmt.Printf("evt.a.e= %8.3f\n", e.A.E)
			fmt.Printf("evt.a.t= %8.3f\n", e.A.T)
			fmt.Printf("evt.b.e= %8.3f\n", e.B.E)
			fmt.Printf("evt.b.t= %8.3f\n", e.B.T)
		}
	}
}

func main() {
	flag.Parse()
	fname := "event.hep"
	
	f,err := hio.Open(fname)
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
