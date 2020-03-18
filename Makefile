opa: opa.c module.wasm
	clang opa.c -o opa -I ./include -L ./lib -l wasmer -rpath ./lib