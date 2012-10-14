package main

import (
	"flag"
	"fmt"

	"compress/gzip"
	"compress/lzw"
	"compress/zlib"
	"encoding/gob"
	"io"
	"os"
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
var fname *string = flag.String("fname", "event.gob.gz", "hepfile to read back")
var compr *string = flag.String("compr", "gzip", "name of the compression method (none for none)")

func tree0(f io.Reader) {
	t := gob.NewDecoder(f)

	// fill some events with random numbers
	nevents := int64(*evtmax)
	for iev := int64(0); iev != nevents; iev++ {
		if iev%1000 == 0 {
			fmt.Printf(":: processing event %d...\n", iev)
		}

		e := Event{}
		err := t.Decode(&e)
		if err != nil {
			panic(err)
		}
		if iev%1000 == 0 || true {
			fmt.Printf("ievt: %d\n", iev)
			fmt.Printf("evt.a.e= %8.3f\n", e.A.E)
			fmt.Printf("evt.a.t= %8.3f\n", e.A.T)
			fmt.Printf("evt.b.e= %8.3f\n", e.B.E)
			fmt.Printf("evt.b.t= %8.3f\n", e.B.T)
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

	f, err := os.Open(fname)
	if err != nil {
		panic(err)
	}

	var ff io.ReadCloser = nil
	switch *compr {
	case "gzip":
		ff, err = gzip.NewReader(f)
	case "zlib":
		ff, err = zlib.NewReader(f)
	case "lzw":
		ff = lzw.NewReader(f, lzw.MSB, 8)
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
