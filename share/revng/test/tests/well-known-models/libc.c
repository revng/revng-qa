#include <stdlib.h>
#include <stdnoreturn.h>
#include <setjmp.h>

#define trap __builtin_trap

void *memchr(const void *s, int c, size_t n) { trap(); }
void *memrchr(const void *s, int c, size_t n) { trap(); }
void *rawmemchr(const void *s, int c) { trap(); }
int memcmp(const void *s1, const void *s2, size_t n) { trap(); }
void *memcpy(void *dest, const void *src, size_t n) { trap(); }
void *mempcpy(void *dest, const void *src, size_t n) { trap(); }
void *memmove(void *dest, const void *src, size_t n) { trap(); }
void *memset(void *s, int c, size_t n) { trap(); }

size_t strlen(const char *s) { trap(); }
size_t strnlen(const char *s, size_t maxlen) { trap(); }
char *strchr(const char *s, int c) { trap(); }
char *strrchr(const char *s, int c) { trap(); }
char *strchrnul(const char *s, int c) { trap(); }
char *stpcpy(char *dst, const char *src) { trap(); }
char *strcpy(char *dst, const char *src) { trap(); }
char *stpncpy(char *dst, const char *src, size_t sz) { trap(); }
char *strncpy(char *dst, const char *src, size_t sz) { trap(); }
char *strcat(char *dst, const char *src) { trap(); }
char *strncat(char *dst, const char *src, size_t sz) { trap(); }
int strcmp(const char *s1, const char *s2) { trap(); }
int strncmp(const char *s1, const char *s2, size_t n) { trap(); }
int strcasecmp(const char *s1, const char *s2) { trap(); }
int strncasecmp(const char *s1, const char *s2, size_t n) { trap(); }
size_t strspn(const char *s, const char *accept) { trap(); }
size_t strcspn(const char *s, const char *reject) { trap(); }

char *index(const char *s, int c) { trap(); }
char *rindex(const char *s, int c) { trap(); }

wchar_t *wcpcpy(wchar_t *dest, const wchar_t *src) { trap(); }
wchar_t *wcpncpy(wchar_t *dest, const wchar_t *src, size_t n) { trap(); }
wchar_t *wcscat(wchar_t *dest, const wchar_t *src) { trap(); }
wchar_t *wcschr(const wchar_t *wcs, wchar_t wc) { trap(); }
int wcscmp(const wchar_t *s1, const wchar_t *s2) { trap(); }
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src) { trap(); }
size_t wcslen(const wchar_t *s) { trap(); }
wchar_t *wcsncat(wchar_t *dest, const wchar_t *src, size_t n) { trap(); }
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n) { trap(); }
wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n) { trap(); }
size_t wcsnlen(const wchar_t *s, size_t maxlen) { trap(); }
wchar_t *wcsrchr(const wchar_t *wcs, wchar_t wc) { trap(); }
wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n) { trap(); }
int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n) { trap(); }
wchar_t *wmemset(wchar_t *wcs, wchar_t wc, size_t n) { trap(); }

int setjmp(jmp_buf env) { trap(); }
noreturn void longjmp(jmp_buf env, int val) { trap(); }

int getdomainname(char *name, size_t len) { trap(); }
int setdomainname(const char *name, size_t len) { trap(); }
int gethostname(char *name, size_t len) { trap(); }
int sethostname(const char *name, size_t len) { trap(); }

noreturn void __stack_check_fail(void) { trap(); };
