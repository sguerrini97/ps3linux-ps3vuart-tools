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
cmd_get_temperature_exec(int cmdc, struct cmd **cmdv, int argc, char **argv)
{
	struct ps3sm_get_temperature get_temperature;
	struct ps3sm_get_temperature_reply get_temperature_reply;

	uint8_t arg;
	char *endptr;
	int ret;

	arg = strtoul(argv[0], &endptr, 0);
	if (*endptr != '\0' || arg > 0xff)
		return (CMD_EINVAL);


	ps3sm_init_header(PS3SM_HDR(&get_temperature),
	    sizeof(get_temperature) - sizeof(struct ps3sm_header),
	    PS3SM_SID_GET_TEMPERATURE, 0);

	get_temperature.version = PS3SM_GET_TEMPERATURE_VERSION;
	get_temperature.arg = arg;

	ret = dev_write(&get_temperature, sizeof(get_temperature));
	if (ret != sizeof(get_temperature))
		return (CMD_EIO);
	
	ret = dev_read(&get_temperature_reply, sizeof(get_temperature_reply));
	if (ret < 0)
		return (CMD_EIO);
	
	if (arg == 0)
		fprintf(stdout, "CELL: %02d.%02d °C\n", get_temperature_reply.temp_0, ((get_temperature_reply.temp_1*0x64) >> 8));
	else if (arg == 1)
		fprintf(stdout, "RSX:  %02d.%02d °C\n", get_temperature_reply.temp_0, ((get_temperature_reply.temp_1*0x64) >> 8));

	return (CMD_EOK);
}

struct cmd cmd_get_temperature = {
	.name = "get_temperature",
	.help = "get_temperature",
	.usage = "get_temperature <arg1>",

	.min_argc = 1,
	.max_argc = 1,

	.exec = cmd_get_temperature_exec,
};
