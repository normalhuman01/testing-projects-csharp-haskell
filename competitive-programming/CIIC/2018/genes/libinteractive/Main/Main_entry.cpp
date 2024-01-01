/* Auto-generated by libinteractive. Do not edit. */
#define _XOPEN_SOURCE 600
#if !defined(_WIN32) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <inttypes.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "genes.h"

#if defined(__GNUC__)
#define __NOINLINE __attribute__((noinline))
#else
#define __NOINLINE
#endif

#if defined(_WIN32)
#if !defined(PRIuS)
#define PRIuS "Iu"
#endif
#else
#if !defined(PRIuS)
#define PRIuS "zu"
#endif
// Windows requires this flag to open files in binary mode using the
// open syscall.
#define O_BINARY 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct __stream {
	int fd;
	size_t capacity;
	size_t pos;
	char buffer[4096];
};

static void openstream(struct __stream* stream, const char* path, int flags) {
	stream->fd = open(path, flags | O_BINARY);
	if (stream->fd == -1) {
		perror("open");
		exit(243);
	}
	stream->pos = 0;
	stream->capacity = 0;
}

static void closestream(struct __stream* stream) {
	if (close(stream->fd) == -1) {
		perror("close");
	}
}

static int readfull(struct __stream* stream, void* buf, size_t count, int fatal) {
	ssize_t bytes;
	while (count > 0) {
		if (stream->pos == stream->capacity) {
			stream->pos = 0;
			bytes = read(stream->fd, stream->buffer, sizeof(stream->buffer));
			if (bytes <= 0) {
				if (!fatal) return 0;
				fprintf(stderr, "Incomplete message missing %" PRIuS " bytes\n", count);
				exit(239);
			}
			stream->capacity = (size_t)bytes;
		}

		bytes = (count < stream->capacity - stream->pos) ? count : (stream->capacity - stream->pos);
		memcpy(buf, stream->buffer + stream->pos, bytes);
		stream->pos += bytes;
		count -= bytes;
		buf = bytes + (char*)buf;
	}
	return 1;
}

static void writeflush(struct __stream* stream) {
	const char* to_write = stream->buffer;
	size_t remaining = stream->pos;
	while (remaining > 0) {
		ssize_t bytes = write(stream->fd, to_write, remaining);
		if (bytes <= 0) {
			fprintf(stderr, "Incomplete message missing %" PRIuS " bytes\n", remaining);
			exit(239);
		}
		to_write = bytes + to_write;
		remaining -= bytes;
	}
	stream->pos = 0;
}

static void writefull(struct __stream* stream, const void* buf, size_t count) {
	ssize_t bytes;
	while (count > 0) {
		bytes = (count < sizeof(stream->buffer) - stream->pos) ? count : (sizeof(stream->buffer) - stream->pos);
		memcpy(stream->buffer + stream->pos, buf, bytes);
		stream->pos += bytes;
		buf = bytes + (char*)buf;
		count -= bytes;

		if (stream->pos == sizeof(stream->buffer)) {
			writeflush(stream);
		}
	}
}

static struct __stream __genes_in, __genes_out;

void __NOINLINE __libinteractive_init() {
	openstream(&__genes_in, "./genes_in", O_WRONLY);
	openstream(&__genes_out, "./genes_out", O_RDONLY);
}

#if defined(_WIN32)
// declared in windows.h
void mainCRTStartup();

void __entry() {
	__libinteractive_init();

	// Perform regular libc startup
	mainCRTStartup();
}
// Some versions of mingw's name mangling add an extra underscore. Sometimes.
// So let's just define both flavors so that the symbol is always found.
void _entry() __attribute__ ((alias ("__entry")));

#elif defined(__APPLE__)

// OS X makes it harder to define the entrypoint in pure assembly, so let's
// rely on using a real C function with inline assembly.
void _entry() {
	__asm__(
		"popq %%rbp\n"  // Remove %rbp from the stack that gcc helpfully added.
		"pushq %%rdx\n" // Store %rdx since we will need it later.
		"callq ___libinteractive_init\n"
		"popq %%rdx\n"
		"jmp _main\n"
		:::
	);
}

#endif // _WIN32 || __APPLE__

#ifdef __cplusplus
}
#endif

static void __message_loop_genes(int __current_function, int __noreturn) {
	uint32_t __msgid;
	while (readfull(&__genes_out, &__msgid, sizeof(__msgid), 0)) {
		if (__msgid == __current_function) return;
		switch (__msgid) {
			case 0x71651df7u: {
				// genes -> Main.measureBacteria
				uint32_t __cookie;
				int measureBacteria_gene1;
				readfull(&__genes_out, &measureBacteria_gene1, sizeof(int), 1);
				int measureBacteria_gene2;
				readfull(&__genes_out, &measureBacteria_gene2, sizeof(int), 1);
				readfull(&__genes_out, &__cookie, sizeof(__cookie), 1);

				long long result =
				measureBacteria(measureBacteria_gene1, measureBacteria_gene2);

				writefull(&__genes_in, &__msgid, sizeof(__msgid));
				writefull(&__genes_in, &result, sizeof(result));
				writefull(&__genes_in, &__cookie, sizeof(__cookie));
				writeflush(&__genes_in);
				break;
			}

			case 0x685d93e7u: {
				// genes -> Main.report
				uint32_t __cookie;
				int report_K;
				readfull(&__genes_out, &report_K, sizeof(int), 1);
				int (*report_kind) = (int(*))malloc(sizeof(int) * report_K);
				readfull(&__genes_out, report_kind, sizeof(int) * report_K, 1);
				long long (*report_idx) = (long long(*))malloc(sizeof(long long) * report_K);
				readfull(&__genes_out, report_idx, sizeof(long long) * report_K, 1);
				readfull(&__genes_out, &__cookie, sizeof(__cookie), 1);

				report(report_K, report_kind, report_idx);

				writefull(&__genes_in, &__msgid, sizeof(__msgid));
				writefull(&__genes_in, &__cookie, sizeof(__cookie));
				writeflush(&__genes_in);
				free(report_kind);
				free(report_idx);
				break;
			}

			default: {
				fprintf(stderr, "Unknown message id 0x%x\n", __msgid);
				exit(241);
			}
		}
	}
	if (__noreturn) {
		exit(0);
	}
	if (__current_function != -1) {
		fprintf(stderr, "Confused about exiting\n");
		exit(242);
	}
}

void tryScience(int T, int C, int cost[], int gene1[], int gene2[]) {
	const uint32_t __msgid = 0x13857295u;
	const uint32_t __cookie = 0x43f73b41u;

	writefull(&__genes_in, &__msgid, sizeof(__msgid));
	writefull(&__genes_in, &T, sizeof(int));
	writefull(&__genes_in, &C, sizeof(int));
	writefull(&__genes_in, cost, sizeof(int) * (C));
	writefull(&__genes_in, gene1, sizeof(int) * (C));
	writefull(&__genes_in, gene2, sizeof(int) * (C));
	writefull(&__genes_in, &__cookie, sizeof(__cookie));
	writeflush(&__genes_in);

	__message_loop_genes(__msgid,  1 );
	uint32_t __cookie_result = 0;
	readfull(&__genes_out, &__cookie_result, sizeof(__cookie_result), 1);

	if (__cookie != __cookie_result) {
		fprintf(stderr, "invalid cookie\n");
		exit(240);
	}
}