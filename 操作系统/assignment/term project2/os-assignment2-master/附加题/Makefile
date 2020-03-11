all: address pic vm

address: 
	gcc -o test test.c
	./test > out.txt

pic: 
	python3 run.py > addresses-locality.txt

vm: 
	gcc -o vm vm.c
	./vm addresses-locality.txt

clean:
	rm -rf test
	rm -rf vm
	rm -rf out.txt
	rm -rf addresses-locality.txt