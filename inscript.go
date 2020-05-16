package main

import (
	"bufio"
	"log"
	"os"
	"strings"
)

func main() {
	argv := os.Args[1:]

	if len(argv) < 1 || len(argv) > 2 {
		log.Fatal("Usage: ./inscript [input] (output)\n")
	}

	mapping := [28]rune{'α', 'β', 'χ', 'δ', 'ε', 'φ', 'γ', 'η', 'Ι', 'Ι', 'κ', 'λ', 'μ', 'ν', 'ο', 'π', 'Ϙ', 'ρ', 'σ', 'τ', 'υ', 'υ', 'ω', 'ξ', 'υ', 'ζ', 'ψ', 'θ'}

	input, err := os.Open(argv[0])
	if err != nil {
		log.Fatal(err)
	}
	defer input.Close()

	var output *os.File
	if len(argv) > 1 {
		output, err = os.Create(argv[1])
		if err != nil {
			log.Fatal(err)
		}
	} else {
		output, err = os.Create("output.txt")
		if err != nil {
			log.Fatal(err)
		}
	}
	defer output.Close()

	bufin := bufio.NewScanner(input)
	bufout := bufio.NewWriter(output)
	for bufin.Scan() {
		line := strings.ToLower(bufin.Text())
		l := len(line)
		skipNext := false
		for p, c := range line {
			if skipNext {
				skipNext = false
				continue
			}
			i := c - 'a'
			if i == 15 && p < l-1 {
				if line[p+1]-'a' == 18 {
					bufout.WriteRune(mapping[26])
					skipNext = true
					continue
				}
				bufout.WriteRune(mapping[i])
			} else if i == 19 && p < l-1 {
				if line[p+1]-'a' == 7 {
					bufout.WriteRune(mapping[27])
					skipNext = true
					continue
				}
				bufout.WriteRune(mapping[i])
			} else if i < 0 || i >= 26 {
				bufout.WriteRune(c)
			} else {
				bufout.WriteRune(mapping[i])
			}
		}
		bufout.WriteString("\n")
	}
	bufout.Flush()

}
