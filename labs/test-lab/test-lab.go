package main

import (
	"fmt"
	"os"
)

func main() {
  var nombre string
  for i := 0; i < len(os.Args); i++ {
    if (i != 0){
      nombre += os.Args[i] + " "
    }
  }
  if (len(os.Args) == 1){
      fmt.Println("Error")
    }else {
    fmt.Println("Hello " + nombre + "Welcome to the Jungle")
  }
}