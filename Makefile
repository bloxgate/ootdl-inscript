debug: inscript.go
	go build -o inscript .

release: clean inscript.go
	go build -ldflags=-w -o inscript .

clean:
	- rm inscript