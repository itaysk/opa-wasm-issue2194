#include <stdio.h>
#include "wasmer.h"
#include <assert.h>
#include <string.h>

void print_wasmer_error()
{
  int error_len = wasmer_last_error_length();
  char *error_str = malloc(error_len);
  wasmer_last_error_message(error_str, error_len);
  printf("ERROR (print_wasmer_error): `%s`\n", error_str);
}

char* get_string_from_memory(uint8_t *memory_data, int addr) {
  int len = 0;
  while (memory_data[addr+len] != 0) {
    len++;
  }
  char *s = malloc(sizeof(char)*len);
  if (s == 0)
		{
			printf("ERROR (get_string_from_memory): failed to allocate memory for string\n");
      exit(1);
		}
  for (int i=0; i<len; i++) {
    s[i] = memory_data[addr+i];
  }
  return s;
}

void opa_abort(wasmer_instance_context_t *ctx, int addr) {
  printf("opa_abort: %d\n", addr);
  const wasmer_memory_t *memory = wasmer_instance_context_memory(ctx, 0);
  uint8_t *memory_data = wasmer_memory_data(memory);
  char *s = get_string_from_memory(memory_data, addr);
  printf("ERROR (opa_abort): %s\n", s);
  free(s);
}
//TODO: what type is builtin_id, args?
void opa_builtin0(wasmer_instance_context_t *ctx, int builtin_id, void *opa_ctx) {
  printf("opa_builtin0: builtin_id: %d\n", builtin_id);
}
void opa_builtin1(wasmer_instance_context_t *ctx, int builtin_id, void *opa_ctx, int _1) {
  printf("opa_builtin1: builtin_id: %d, _1: %d\n", builtin_id, _1);
}
void opa_builtin2(wasmer_instance_context_t *ctx, int builtin_id, void *opa_ctx, int _1, int _2) {
  printf("opa_builtin2: builtin_id: %d, _1: %d, _2: %d\n", builtin_id, _1, _2);
}
void opa_builtin3(wasmer_instance_context_t *ctx, int builtin_id, void *opa_ctx, int _1, int _2, int _3) {
  printf("opa_builtin3: builtin_id: %d, _1: %d, _2: %d, _3: %d\n", builtin_id, _1, _2, _3);
}
void opa_builtin4(wasmer_instance_context_t *ctx, int builtin_id, void *opa_ctx, int _1, int _2, int _3, int _4) {
  printf("opa_builtin4: builtin_id: %d, _1: %d, _2: %d, _3: %d, _4: %d\n", builtin_id, _1, _2, _3, _4);
}

wasmer_memory_t *create_wasmer_memory() {
  wasmer_memory_t *memory = NULL;
  wasmer_limit_option_t max = { .has_some = true, .some = 256 };
  wasmer_limits_t descriptor = { .min = 256, .max = max };
  wasmer_result_t memory_result = wasmer_memory_new(&memory, descriptor);
  if (memory_result != WASMER_OK)
  {
    print_wasmer_error();
  }
  return memory;
}

wasmer_instance_t *create_wasmer_instance(uint8_t *wasmBytes, long wasmLen, wasmer_memory_t *memory) {
  // prepare imports
  const char *module_name = "env";
  wasmer_byte_array module_name_bytes = { .bytes = (const uint8_t *) module_name, .bytes_len = strlen(module_name) };

  const char *import_memory_name = "memory";
  wasmer_byte_array import_memory_name_bytes = { .bytes = (const uint8_t *) import_memory_name, .bytes_len = strlen(import_memory_name) };
  wasmer_import_t memory_import = { .module_name = module_name_bytes, .import_name = import_memory_name_bytes, .tag = WASM_MEMORY, .value.memory = memory };

  const char *import_opa_abort_name = "opa_abort"; 
  wasmer_byte_array import_opa_abort_name_bytes = { .bytes = (const uint8_t *) import_opa_abort_name, .bytes_len = strlen(import_opa_abort_name) };
  wasmer_value_tag opa_abort_params_sig[] = {WASM_I32};
  wasmer_value_tag opa_abort_returns_sig[] = {};
  wasmer_import_func_t *opa_abort_import_function = wasmer_import_func_new((void (*)(void *)) opa_abort, opa_abort_params_sig, 1, opa_abort_returns_sig, 0);
  wasmer_import_t opa_abort_import = { .module_name = module_name_bytes, .import_name = import_opa_abort_name_bytes, .tag = WASM_FUNCTION, .value.func = opa_abort_import_function };

  const char *import_opa_builtin0_name = "opa_builtin0"; 
  wasmer_byte_array import_opa_builtin0_name_bytes = { .bytes = (const uint8_t *) import_opa_builtin0_name, .bytes_len = strlen(import_opa_builtin0_name) };
  wasmer_value_tag opa_builtin0_params_sig[] = {WASM_I32, WASM_I32};
  wasmer_value_tag opa_builtin0_returns_sig[] = {WASM_I32};
  wasmer_import_func_t *opa_builtin0_import_function = wasmer_import_func_new((void (*)(void *)) opa_builtin0, opa_builtin0_params_sig, 2, opa_builtin0_returns_sig, 1);
  wasmer_import_t opa_builtin0_import = { .module_name = module_name_bytes, .import_name = import_opa_builtin0_name_bytes, .tag = WASM_FUNCTION, .value.func = opa_builtin0_import_function };

  const char *import_opa_builtin1_name = "opa_builtin1"; 
  wasmer_byte_array import_opa_builtin1_name_bytes = { .bytes = (const uint8_t *) import_opa_builtin1_name, .bytes_len = strlen(import_opa_builtin1_name) };
  wasmer_value_tag opa_builtin1_params_sig[] = {WASM_I32, WASM_I32, WASM_I32};
  wasmer_value_tag opa_builtin1_returns_sig[] = {WASM_I32};
  wasmer_import_func_t *opa_builtin1_import_function = wasmer_import_func_new((void (*)(void *)) opa_builtin1, opa_builtin1_params_sig, 3, opa_builtin1_returns_sig, 1);
  wasmer_import_t opa_builtin1_import = { .module_name = module_name_bytes, .import_name = import_opa_builtin1_name_bytes, .tag = WASM_FUNCTION, .value.func = opa_builtin1_import_function };

  const char *import_opa_builtin2_name = "opa_builtin2"; 
  wasmer_byte_array import_opa_builtin2_name_bytes = { .bytes = (const uint8_t *) import_opa_builtin2_name, .bytes_len = strlen(import_opa_builtin2_name) };
  wasmer_value_tag opa_builtin2_params_sig[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32};
  wasmer_value_tag opa_builtin2_returns_sig[] = {WASM_I32};
  wasmer_import_func_t *opa_builtin2_import_function = wasmer_import_func_new((void (*)(void *)) opa_builtin2, opa_builtin2_params_sig, 4, opa_builtin2_returns_sig, 1);
  wasmer_import_t opa_builtin2_import = { .module_name = module_name_bytes, .import_name = import_opa_builtin2_name_bytes, .tag = WASM_FUNCTION, .value.func = opa_builtin2_import_function };

  const char *import_opa_builtin3_name = "opa_builtin3"; 
  wasmer_byte_array import_opa_builtin3_name_bytes = { .bytes = (const uint8_t *) import_opa_builtin3_name, .bytes_len = strlen(import_opa_builtin3_name) };
  wasmer_value_tag opa_builtin3_params_sig[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32, WASM_I32};
  wasmer_value_tag opa_builtin3_returns_sig[] = {WASM_I32};
  wasmer_import_func_t *opa_builtin3_import_function = wasmer_import_func_new((void (*)(void *)) opa_builtin3, opa_builtin3_params_sig, 5, opa_builtin3_returns_sig, 1);
  wasmer_import_t opa_builtin3_import = { .module_name = module_name_bytes, .import_name = import_opa_builtin3_name_bytes, .tag = WASM_FUNCTION, .value.func = opa_builtin3_import_function };

  const char *import_opa_builtin4_name = "opa_builtin4"; 
  wasmer_byte_array import_opa_builtin4_name_bytes = { .bytes = (const uint8_t *) import_opa_builtin4_name, .bytes_len = strlen(import_opa_builtin4_name) };
  wasmer_value_tag opa_builtin4_params_sig[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32, WASM_I32, WASM_I32};
  wasmer_value_tag opa_builtin4_returns_sig[] = {WASM_I32};
  wasmer_import_func_t *opa_builtin4_import_function = wasmer_import_func_new((void (*)(void *)) opa_builtin4, opa_builtin4_params_sig, 6, opa_builtin4_returns_sig, 1);
  wasmer_import_t opa_builtin4_import = { .module_name = module_name_bytes, .import_name = import_opa_builtin4_name_bytes, .tag = WASM_FUNCTION, .value.func = opa_builtin4_import_function };

  wasmer_import_t imports[] = {memory_import, opa_abort_import, opa_builtin0_import, opa_builtin1_import, opa_builtin2_import, opa_builtin3_import, opa_builtin4_import};
  
  // create wasm instance
  wasmer_instance_t *instance = NULL;
  wasmer_result_t compile_result = wasmer_instantiate(&instance, wasmBytes, wasmLen, imports, 7);
  if (compile_result != WASMER_OK)
  {
      print_wasmer_error();
  }
  assert(compile_result == WASMER_OK);
  return instance;
}

uint8_t *get_pointer_to_memory(wasmer_instance_t *instance) {
  const wasmer_instance_context_t *ctx = wasmer_instance_context_get(instance);
  const wasmer_memory_t *memory = wasmer_instance_context_memory(ctx, 0);
  // Return the uint8_t representation of the guest Wasm linear memory.
  return wasmer_memory_data(memory);
}

int call_wasm_function_and_return_i32(wasmer_instance_t *instance, char* functionName, wasmer_value_t params[], int num_params) {
  wasmer_value_t result_one = { 0 };
  wasmer_value_t results[] = {result_one};
  wasmer_result_t call_result = wasmer_instance_call(instance, functionName, params, num_params, results, 1);
  if (call_result != WASMER_OK) {
    printf("ERROR (call_wasm_function_and_return_i32): call to wasm function failed: func: %s, params:\n", functionName);
    for (int i=0; i<num_params; i++){
      printf("  param %d { tag: %d, i32: %d }\n", i, params[i].tag, params[i].value.I32);
    }
    print_wasmer_error();
    exit(1);
  }
  int response_value = results[0].value.I32;
  return response_value;
}

char* _dumpJSON(wasmer_instance_t *instance, wasmer_memory_t *memory, int memory_address) {
  wasmer_value_t opa_json_dump_params[] = { {.tag = WASM_I32, .value.I32 = memory_address} };
  int addr = call_wasm_function_and_return_i32(instance, "opa_json_dump", opa_json_dump_params, 1);
  uint8_t *memory_data = get_pointer_to_memory(instance);
  char* s = get_string_from_memory(memory_data, addr);
  return s;
}

int _loadJSON(wasmer_instance_t *instance, wasmer_memory_t *memory, char* str, int len) {
  // TODO: validate json
  uint8_t *memory_data = wasmer_memory_data(memory);

  wasmer_value_t opa_malloc_params[] = { {.tag = WASM_I32, .value.I32 = len} };
  int opa_malloc_result = call_wasm_function_and_return_i32(instance, "opa_malloc", opa_malloc_params, 1); 

  for (int i = 0; i<len; i++) {
    memory_data[opa_malloc_result+i] = str[i];
  }

  wasmer_value_t opa_json_parse_params[] = { {.tag = WASM_I32, .value.I32 = opa_malloc_result}, {.tag = WASM_I32, .value.I32 = len} };
  int opa_json_parse_result = call_wasm_function_and_return_i32(instance, "opa_json_parse", opa_json_parse_params, 2); 

  assert(opa_json_parse_result != 0);
  return opa_json_parse_result;
}

int main() {

  // read wasm module
  FILE *wasmFile = fopen("./module.wasm", "r");
  assert(wasmFile != NULL);
  fseek(wasmFile, 0, SEEK_END);
  long wasmLen = ftell(wasmFile);
  uint8_t *wasmBytes = malloc(wasmLen);
  fseek(wasmFile, 0, SEEK_SET);
  fread(wasmBytes, 1, wasmLen, wasmFile);
  fclose(wasmFile);
  
  // init
  wasmer_memory_t *memory = create_wasmer_memory();
  wasmer_instance_t *instance = create_wasmer_instance(wasmBytes, wasmLen, memory);
  
  // evaluate
  wasmer_value_t opa_eval_ctx_new_params[] = { 0 };
  int opa_ctx_address = call_wasm_function_and_return_i32(instance, "opa_eval_ctx_new", opa_eval_ctx_new_params, 0);
  printf("opa_ctx_address: %x\n", opa_ctx_address); //TODO: remove after debugged

  wasmer_value_t eval_params[] = { {.tag = WASM_I32, .value.I32 = opa_ctx_address} };
  call_wasm_function_and_return_i32(instance, "eval", eval_params, 1);
  // ^^ failing
  return 0;

  // vv anything beneath this line is not relevant for the repro
  wasmer_value_t opa_eval_ctx_get_result_params[] = { {.tag = WASM_I32, .value.I32 = opa_ctx_address} };
  int opa_result_address = call_wasm_function_and_return_i32(instance, "opa_eval_ctx_get_result", opa_eval_ctx_get_result_params, 1);
  
  _dumpJSON(instance, memory, opa_result_address);

  wasmer_memory_destroy(memory);
  //wasmer_import_func_destroy();
  wasmer_instance_destroy(instance);
  return 0;
}