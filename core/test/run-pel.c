/* Copyright 2013-2014 IBM Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Test for our PEL record generation. Currently this doesn't actually
 * test that the records we generate are correct, but it at least lets
 * us run valgrind over the generation routines to check for buffer
 * overflows, etc.
 */

#include <skiboot.h>
#include <inttypes.h>
#include <assert.h>
#include <pel.h>
#include <errorlog.h>

#define TEST_ERROR 0x1234
#define TEST_SUBSYS 0x5678

DEFINE_LOG_ENTRY(TEST_ERROR, OPAL_PLATFORM_ERR_EVT, TEST_SUBSYS,
			OPAL_PLATFORM_FIRMWARE, OPAL_INFO,
			OPAL_NA, NULL);

#include "../pel.c"

struct dt_node *dt_root = NULL;
char dt_prop[] = "DUMMY DT PROP";
const void *dt_prop_get(const struct dt_node *node __unused, const char *prop __unused)
{
	return dt_prop;
}

int fsp_rtc_get_cached_tod(uint32_t *year_month_day,
			   uint64_t *hour_minute_second_millisecond)
{
	*year_month_day = 0;
	*hour_minute_second_millisecond = 0;

	return 0;
}

int main(void)
{
	char *pel_buf;
	size_t size;
	struct errorlog *elog;
	struct opal_err_info *opal_err_info = &err_TEST_ERROR;

	elog = malloc(sizeof(struct errorlog));
	pel_buf = malloc(PEL_MIN_SIZE + 4);
	assert(elog);
	assert(pel_buf);

	memset(elog, 0, sizeof(struct errorlog));

	elog->error_event_type = opal_err_info->err_type;
	elog->component_id = opal_err_info->cmp_id;
	elog->subsystem_id = opal_err_info->subsystem;
	elog->event_severity = opal_err_info->sev;
	elog->event_subtype = opal_err_info->event_subtype;
	elog->reason_code = opal_err_info->reason_code;
	elog->elog_origin = ORG_SAPPHIRE;

	size = pel_size(elog);
	printf("PEL Size: %ld\n", size);
	assert(size == create_pel_log(elog, pel_buf, size));

	return 0;
}
