/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <div64.h>
#include <vsprintf.h>

static unsigned long long get_value(const char *val)
{
	char *env = getenv((char *)val);
	if (env)
		return simple_strtoull(env, NULL, 16);
	else
		return simple_strtoull(val, NULL, 16);
}

static unsigned long long get_value_base10(const char *val)
{
	char *env = getenv((char *)val);
	if (env)
		return simple_strtoull(env, NULL, 10);
	else
		return simple_strtoull(val, NULL, 10);
}

/*
 * Top level addenv command
 */
#define IS_MATH_CMD(cmd, args) ((strcmp(argv[1], cmd) == 0) && (argc == args))
static int do_math(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char numberbuf[32];
	unsigned long long i = 0;

	if (IS_MATH_CMD("add", 5)) {
		i = get_value(argv[3]) + get_value(argv[4]);
	} else if (IS_MATH_CMD("sub", 5)) {
		i = get_value(argv[3]) - get_value(argv[4]);
	} else if (IS_MATH_CMD("mul", 5)) {
		i = get_value(argv[3]) * get_value(argv[4]);
	} else if (IS_MATH_CMD("div", 5)) {
		i = lldiv(get_value(argv[3]), get_value(argv[4]));
	} else if (IS_MATH_CMD("shl", 5)) {
		i = get_value(argv[3]) << get_value(argv[4]);
	} else if (IS_MATH_CMD("d2h", 4)) {
		i = get_value_base10(argv[3]);
	} else {
		return cmd_usage(cmdtp);
	}

	sprintf(numberbuf, "%llx", i);
	setenv(argv[2], numberbuf);
	return 0;
}

U_BOOT_CMD(math, 5, 0, do_math,
	   "Environment variable math.",
	   "add a b c\n"
	   "    - add b to c and store in a.\n"
	   "math sub a b c\n"
	   "    - subtract b from c and store in a.\n"
	   "math mul a b c\n"
	   "    - multiply b by c and store in a.\n"
	   "math div a b c\n"
	   "    - divide b by c and store in a.\n"
	   "math shl a b c\n"
	   "    - shift left b by c and store in a.\n"
	   "math d2h a b\n"
	   "    - Convert b from decimal to hex and store in a.\n");
