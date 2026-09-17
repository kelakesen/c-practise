#include <apr_thread_proc.h>
#include <stdio.h>

int main(){

	apr_pool_t *pool;

	apr_pool_initialize();
	apr_pool_create(&pool,NULL);

	apr_procattr_t *attr;

	apr_procattr_create(&attr,pool);
	apr_procattr_io_set(attr, APR_NO_PIPE, APR_NO_PIPE, APR_NO_PIPE);
	apr_procattr_cmdtype_set(attr, APR_PROGRAM_PATH);

	apr_proc_t proc;
	const char *prog = "cd";
	const char *args[] = { "cd", "test/", "ex_practice/","ex26/",NULL };

	apr_proc_create(&proc, prog, args, NULL, attr, pool);

	int exit_code;
	apr_exit_why_e exit_why;
	apr_proc_wait(&proc, &exit_code, &exit_why, APR_WAIT);	

	apr_pool_destroy(pool);   // 自动释放 attr、proc 等关联的内存
	apr_terminate();          // 清理 APR 库

	return 0;

	
}

