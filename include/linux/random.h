/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_RANDOM_H
#define _LINUX_RANDOM_H

#include <linux/list.h>
#include <linux/once.h>

#include <uapi/linux/random.h>

struct notifier_block;

<<<<<<< HEAD
extern void add_device_randomness(const void *, unsigned int);
=======
extern void add_device_randomness(const void *, size_t);
extern void add_bootloader_randomness(const void *, size_t);
>>>>>>> acbf6f4851e3 (random: use hash function for crng_slow_load())

#if defined(CONFIG_GCC_PLUGIN_LATENT_ENTROPY) && !defined(__CHECKER__)
static inline void add_latent_entropy(void)
{
	add_device_randomness((const void *)&latent_entropy,
			      sizeof(latent_entropy));
}
#else
static inline void add_latent_entropy(void) {}
#endif

extern void add_input_randomness(unsigned int type, unsigned int code,
				 unsigned int value) __latent_entropy;
<<<<<<< HEAD
extern void add_interrupt_randomness(int irq, int irq_flags) __latent_entropy;
=======
extern void add_interrupt_randomness(int irq) __latent_entropy;
extern void add_hwgenerator_randomness(const void *buffer, size_t count,
				       size_t entropy);
>>>>>>> 57a23e728b08 (random: pull add_hwgenerator_randomness() declaration into random.h)

extern void get_random_bytes(void *buf, size_t nbytes);
extern int wait_for_random_bytes(void);
<<<<<<< HEAD
extern int add_random_ready_callback(struct random_ready_callback *rdy);
extern void del_random_ready_callback(struct random_ready_callback *rdy);
<<<<<<< HEAD
extern void get_random_bytes_arch(void *buf, int nbytes);
=======
=======
extern int __init rand_initialize(void);
extern bool rng_is_initialized(void);
extern int register_random_ready_notifier(struct notifier_block *nb);
extern int unregister_random_ready_notifier(struct notifier_block *nb);
>>>>>>> c8e06a4dc297 (random: replace custom notifier chain with standard one)
extern size_t __must_check get_random_bytes_arch(void *buf, size_t nbytes);
>>>>>>> acbf6f4851e3 (random: use hash function for crng_slow_load())

#ifndef MODULE
extern const struct file_operations random_fops, urandom_fops;
#endif

u32 get_random_u32(void);
u64 get_random_u64(void);
static inline unsigned int get_random_int(void)
{
	return get_random_u32();
}
static inline unsigned long get_random_long(void)
{
#if BITS_PER_LONG == 64
	return get_random_u64();
#else
	return get_random_u32();
#endif
}

/*
 * On 64-bit architectures, protect against non-terminated C string overflows
 * by zeroing out the first byte of the canary; this leaves 56 bits of entropy.
 */
#ifdef CONFIG_64BIT
# ifdef __LITTLE_ENDIAN
#  define CANARY_MASK 0xffffffffffffff00UL
# else /* big endian, 64 bits: */
#  define CANARY_MASK 0x00ffffffffffffffUL
# endif
#else /* 32 bits: */
# define CANARY_MASK 0xffffffffUL
#endif

static inline unsigned long get_random_canary(void)
{
	unsigned long val = get_random_long();

	return val & CANARY_MASK;
}

/* Calls wait_for_random_bytes() and then calls get_random_bytes(buf, nbytes).
 * Returns the result of the call to wait_for_random_bytes. */
static inline int get_random_bytes_wait(void *buf, size_t nbytes)
{
	int ret = wait_for_random_bytes();
	if (unlikely(ret))
		return ret;
	get_random_bytes(buf, nbytes);
	return 0;
}

#define declare_get_random_var_wait(var) \
	static inline int get_random_ ## var ## _wait(var *out) { \
		int ret = wait_for_random_bytes(); \
		if (unlikely(ret)) \
			return ret; \
		*out = get_random_ ## var(); \
		return 0; \
	}
declare_get_random_var_wait(u32)
declare_get_random_var_wait(u64)
declare_get_random_var_wait(int)
declare_get_random_var_wait(long)
#undef declare_get_random_var

unsigned long randomize_page(unsigned long start, unsigned long range);

/*
 * This is designed to be standalone for just prandom
 * users, but for now we include it from <linux/random.h>
 * for legacy reasons.
 */
#include <linux/prandom.h>

#ifdef CONFIG_ARCH_RANDOM
# include <asm/archrandom.h>
#else
static inline bool arch_get_random_long(unsigned long *v)
{
	return 0;
}
static inline bool arch_get_random_int(unsigned int *v)
{
	return 0;
}
static inline bool arch_has_random(void)
{
	return 0;
}
static inline bool arch_get_random_seed_long(unsigned long *v)
{
	return 0;
}
static inline bool arch_get_random_seed_int(unsigned int *v)
{
	return 0;
}
static inline bool arch_has_random_seed(void)
{
	return 0;
}
#endif

#ifdef CONFIG_SMP
extern int random_prepare_cpu(unsigned int cpu);
extern int random_online_cpu(unsigned int cpu);
#endif

#endif /* _LINUX_RANDOM_H */
