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
#ifndef __LIBFFS_H
#define __LIBFFS_H

#include <libflash/libflash.h>
#include <libflash/ffs.h>
#include <libflash/blocklevel.h>

/* FFS handle, opaque */
struct ffs_handle;

/* Error codes:
 *
 * < 0 = flash controller errors
 *   0 = success
 * > 0 = libffs / libflash errors
 */
#define FFS_ERR_BAD_MAGIC	100
#define FFS_ERR_BAD_VERSION	101
#define FFS_ERR_BAD_CKSUM	102
#define FFS_ERR_PART_NOT_FOUND	103
#define FFS_ERR_BAD_ECC		104

/* Init */

int ffs_init(uint32_t offset, uint32_t max_size,
		struct blocklevel_device *bl, struct ffs_handle **ffs, int mark_ecc);

/* ffs_open_image is Linux only as it uses lseek, which skiboot does not
 * implement */
#ifndef __SKIBOOT__
int ffs_open_image(int fd, uint32_t size, uint32_t toc_offset,
		   struct ffs_handle **ffs);
#endif

void ffs_close(struct ffs_handle *ffs);

int ffs_lookup_part(struct ffs_handle *ffs, const char *name,
		    uint32_t *part_idx);

int ffs_part_info(struct ffs_handle *ffs, uint32_t part_idx,
		  char **name, uint32_t *start,
		  uint32_t *total_size, uint32_t *act_size, bool *ecc);

int ffs_update_act_size(struct ffs_handle *ffs, uint32_t part_idx,
			uint32_t act_size);

#endif /* __LIBFFS_H */
