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

#include <stdio.h>
#include <stdint.h>

#include <cmd.h>
#include <util.h>

#include "../opts.h"
#include "../dev.h"
#include "../ps3dm_msg.h"

int
cmd_sm_get_version_exec(int cmdc, struct cmd **cmdv, int argc, char **argv)
{
	struct ps3dm_sm_get_version get_version;
	uint64_t laid, paid;
	int len;
	int ret;

	laid = opts.laid_valid ? opts.laid : 0x1070000002000001ull;
	paid = opts.paid_valid ? opts.paid : 0x10700003ff000001ull;

	ps3dm_init_header(&get_version.dm_hdr, 1, PS3DM_FID_SM,
	    sizeof(struct ps3dm_ss_header),
	    sizeof(get_version) - sizeof(struct ps3dm_header));
	ps3dm_init_ss_header(&get_version.ss_hdr, PS3DM_PID_SM_GET_VERSION,
	    laid, paid);

	len = sizeof(struct ps3dm_header) + sizeof(struct ps3dm_ss_header);

	ret = dev_write(&get_version, len);
	if (ret != len)
		return (CMD_EIO);

	ret = dev_read(&get_version, sizeof(get_version));
	if (ret != sizeof(get_version))
		return (CMD_EIO);

	if (opts.do_verbose > 0)
		fprintf(stderr, "ss status %d\n", get_version.ss_hdr.status);

	if (get_version.ss_hdr.status)
		return (CMD_EIO);

	hex_fprintf(stdout, (char *) get_version.version,
	    sizeof(get_version.version));

	return (CMD_EOK);
}

struct cmd cmd_sm_get_version = {
	.name = "get_version",
	.help = "get version",
	.usage = "get_version",

	.min_argc = 0,
	.max_argc = 0,

	.exec = cmd_sm_get_version_exec,
};
