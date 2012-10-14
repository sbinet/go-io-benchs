package main

import (
	"compress/gzip"
	"compress/lzw"
	"compress/zlib"
	"encoding/gob"
	"flag"
	"fmt"
	"io"
	"os"

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
var compr *string = flag.String("compr", "gzip", "name of the compression method (none for none)")

func tree0(f io.Writer) {
	t := gob.NewEncoder(f)

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
		err := t.Encode(&e)
		if err != nil {
			panic(err)
		}
	}
}

func main() {
	flag.Parse()
	fname := "event.gob"
	switch *compr {
	case "gzip":
		fname = fmt.Sprintf("%s.gz", fname)
	case "zlib":
		fname = fmt.Sprintf("%s.z", fname)
	case "lzw":
		fname = fmt.Sprintf("%s.lzw", fname)
	case "none", "":
		fname = fname
	default:
		fname = fname
	}

	f, err := os.Create(fname)
	if err != nil {
		panic(err)
	}

	var ff io.WriteCloser = nil
	switch *compr {
	case "gzip":
		ff = gzip.NewWriter(f)
	case "zlib":
		ff = zlib.NewWriter(f)
	case "lzw":
		ff = lzw.NewWriter(f, lzw.MSB, 8)
		err = nil
	default:
		ff = f
	}

	if err != nil {
		panic(err)
	}

	tree0(ff)
	ff.Close()
	f.Close()
}

// EOF
