/*-
 * Copyright (C) 2011, 2012 glevand <geoffrey.levand@mail.ru>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification, immediately at the beginning of the file.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdint.h>

#include <cmd.h>

extern struct cmd cmd_um_read_eprom;
extern struct cmd cmd_um_write_eprom;

static struct cmd *um_cmdv[] = {
	&cmd_help,
	&cmd_list,
	&cmd_um_read_eprom,
	&cmd_um_write_eprom,
};

int
cmd_um_exec(int cmdc, struct cmd **cmdv, int argc, char **argv)
{
	return (cmd_exec(sizeof(um_cmdv) / sizeof(um_cmdv[0]),
	    um_cmdv, argc, argv));
}

struct cmd cmd_um = {
	.name = "um",
	.help = "update manager",
	.usage = "um <cmd> <arg>*",

	.min_argc = 1,
	.max_argc = -1,

	.exec = cmd_um_exec,
};
