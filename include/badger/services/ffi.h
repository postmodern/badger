#ifndef _BADGER_SERVICES_FFI_H_
#define _BADGER_SERVICES_FFI_H_

#include <badger/service.h>

extern const badger_service_t badger_ffi_service;

extern const badger_function_t badger_ffi_open_func;
extern int badger_ffi_open(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_libraries_func;
extern int badger_ffi_libraries(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_attach_function_func;
extern int badger_ffi_attach_function(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_exposed_functions_func;
extern int badger_ffi_exposed_functions(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_exposed_function_func;
extern int badger_ffi_exposed_function(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_call_func;
extern int badger_ffi_call(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_ffi_close_func;
extern int badger_ffi_close(int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
