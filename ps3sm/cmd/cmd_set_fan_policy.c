/*-
 * Copyright (C) 2018 sguerrini97 <sguerrini97@gmail.com>
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
#include <stdlib.h>
#include <stdint.h>

#include <cmd.h>

#include "../opts.h"
#include "../dev.h"
#include "../ps3sm_msg.h"

int
cmd_set_fan_policy_exec(int cmdc, struct cmd **cmdv, int argc, char **argv)
{
	struct ps3sm_set_fan_policy set_fan_policy;

	uint8_t arg1, arg2, arg3;

	char *endptr1, *endptr2, *endptr3;
	int ret;

	arg1 = strtoul(argv[0], &endptr1, 0);
	arg2 = strtoul(argv[1], &endptr2, 0);
	arg3 = strtoul(argv[2], &endptr3, 0);

	if (*endptr1 != '\0' || *endptr2 != '\0' || *endptr3 != '\0')
		return (CMD_EINVAL);

	if (arg2 == 2 && (arg3 < 0x33 || arg3 > 0xfc))
	{
		fprintf(stderr, "Dangerous fan speed, aborting\n");
		return (CMD_EINVAL);
	}

	ps3sm_init_header(PS3SM_HDR(&set_fan_policy),
    	sizeof(set_fan_policy) - sizeof(struct ps3sm_header),
    	PS3SM_SID_FAN_POLICY_1, 0);

	set_fan_policy.version = PS3SM_SET_FAN_POLICY_VERSION;
	set_fan_policy.arg1 = arg1;
	set_fan_policy.arg2 = arg2;
	set_fan_policy.arg3 = arg3;

	ret = dev_write(&set_fan_policy, sizeof(set_fan_policy));
	if (ret != sizeof(set_fan_policy))
		return (CMD_EIO);

	return (CMD_EOK);
}

struct cmd cmd_set_fan_policy = {
	.name = "set_fan_policy",
	.help = "set_fan_policy",
	.usage = "set_fan_policy <id (0)> <0 = full speed, 1 = syscon, 2 = manual> <speed>",

	.min_argc = 3,
	.max_argc = 3,

	.exec = cmd_set_fan_policy_exec,
};
