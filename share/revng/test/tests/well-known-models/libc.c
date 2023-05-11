#include <stdlib.h>
#include <stdnoreturn.h>
#include <setjmp.h>

#define abort __builtin_trap

void *memchr(const void *s, int c, size_t n) { abort(); }
void *memrchr(const void *s, int c, size_t n) { abort(); }
void *rawmemchr(const void *s, int c) { abort(); }
int memcmp(const void *s1, const void *s2, size_t n) { abort(); }
void *memcpy(void *dest, const void *src, size_t n) { abort(); }
void *mempcpy(void *dest, const void *src, size_t n) { abort(); }
void *memmove(void *dest, const void *src, size_t n) { abort(); }
void *memset(void *s, int c, size_t n) { abort(); }

size_t strlen(const char *s) { abort(); }
size_t strnlen(const char *s, size_t maxlen) { abort(); }
char *strchr(const char *s, int c) { abort(); }
char *strrchr(const char *s, int c) { abort(); }
char *strchrnul(const char *s, int c) { abort(); }
char *stpcpy(char *dst, const char *src) { abort(); }
char *strcpy(char *dst, const char *src) { abort(); }
char *stpncpy(char *dst, const char *src, size_t sz) { abort(); }
char *strncpy(char *dst, const char *src, size_t sz) { abort(); }
char *strcat(char *dst, const char *src) { abort(); }
char *strncat(char *dst, const char *src, size_t sz) { abort(); }
int strcmp(const char *s1, const char *s2) { abort(); }
int strncmp(const char *s1, const char *s2, size_t n) { abort(); }
int strcasecmp(const char *s1, const char *s2) { abort(); }
int strncasecmp(const char *s1, const char *s2, size_t n) { abort(); }
size_t strspn(const char *s, const char *accept) { abort(); }
size_t strcspn(const char *s, const char *reject) { abort(); }

char *index(const char *s, int c) { abort(); }
char *rindex(const char *s, int c) { abort(); }

wchar_t *wcpcpy(wchar_t *dest, const wchar_t *src) { abort(); }
wchar_t *wcpncpy(wchar_t *dest, const wchar_t *src, size_t n) { abort(); }
wchar_t *wcscat(wchar_t *dest, const wchar_t *src) { abort(); }
wchar_t *wcschr(const wchar_t *wcs, wchar_t wc) { abort(); }
int wcscmp(const wchar_t *s1, const wchar_t *s2) { abort(); }
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src) { abort(); }
size_t wcslen(const wchar_t *s) { abort(); }
wchar_t *wcsncat(wchar_t *dest, const wchar_t *src, size_t n) { abort(); }
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n) { abort(); }
wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n) { abort(); }
size_t wcsnlen(const wchar_t *s, size_t maxlen) { abort(); }
wchar_t *wcsrchr(const wchar_t *wcs, wchar_t wc) { abort(); }
wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n) { abort(); }
int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n) { abort(); }
wchar_t *wmemset(wchar_t *wcs, wchar_t wc, size_t n) { abort(); }

int setjmp(jmp_buf env) { abort(); }
noreturn void longjmp(jmp_buf env, int val) { abort(); }

int getdomainname(char *name, size_t len) { abort(); }
int setdomainname(const char *name, size_t len) { abort(); }
int gethostname(char *name, size_t len) { abort(); }
int sethostname(const char *name, size_t len) { abort(); }

noreturn void __stack_check_fail(void) { abort(); };

