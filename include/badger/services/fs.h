#ifndef _BADGER_SERVICES_FS_H_
#define _BADGER_SERVICES_FS_H_

#include <badger/service.h>

extern badger_service_t badger_fs_service;

extern const badger_function_t badger_fs_stat_func;
extern int badger_fs_stat(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_unlink_func;
extern int badger_fs_unlink(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_open_func;
extern int badger_fs_open(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_read_func;
extern int badger_fs_read(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_write_func;
extern int badger_fs_write(int argc,const badger_data_t *args,badger_caller_t *caller);

extern const badger_function_t badger_fs_close_func;
extern int badger_fs_close(int argc,const badger_data_t *args,badger_caller_t *caller);

#endif
