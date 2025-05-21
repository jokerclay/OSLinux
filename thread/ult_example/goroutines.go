package main

import (
    "fmt"
    "sync"
    "time"
)

func printNumbers(wg *sync.WaitGroup, id int) {
    defer wg.Done()

    for i := 1; i <= 5; i++ {
        time.Sleep(time.Millisecond * 1500) // Simulate work
        fmt.Printf("Goroutine %d: %d\n", id, i)
    }
}

func main() {
    var wg sync.WaitGroup

    // Start multiple goroutines
    for i := 1; i <= 3; i++ {
        wg.Add(1)
        go printNumbers(&wg, i)
    }

    wg.Wait() // Wait for all goroutines to finish
}

