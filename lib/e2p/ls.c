/*
 * ls.c			- List the contents of an ext2fs superblock
 *
 * Copyright (C) 1992, 1993, 1994  Remy Card <card@masi.ibp.fr>
 *                                 Laboratoire MASI, Institut Blaise Pascal
 *                                 Universite Pierre et Marie Curie (Paris VI)
 *
 * This file can be redistributed under the terms of the GNU Library General
 * Public License
 */

#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <time.h>

#include <linux/ext2_fs.h>

#include "e2p.h"

static void print_user (unsigned short uid)
{
	struct passwd *pw;

	printf ("%u ", uid);
	pw = getpwuid (uid);
	if (pw == NULL)
		printf ("(user unknown)\n");
	else
		printf ("(user %s)\n", pw->pw_name);
}

static void print_group (unsigned short gid)
{
	struct group *gr;

	printf ("%u ", gid);
	gr = getgrgid (gid);
	if (gr == NULL)
		printf ("(group unknown)\n");
	else
		printf ("(group %s)\n", gr->gr_name);
}

#ifndef EXT2_INODE_SIZE
#define EXT2_INODE_SIZE(s) sizeof(struct ext2_inode)
#endif

void list_super (struct ext2_super_block * s)
{
	int inode_blocks_per_group;

	inode_blocks_per_group = (((s->s_inodes_per_group *
				    EXT2_INODE_SIZE(s)) +
				   EXT2_BLOCK_SIZE(s) - 1) /
				  EXT2_BLOCK_SIZE(s));
	
	printf ("Filesystem magic number:  0x%04X\n", s->s_magic);
	printf ("Filesystem revision #:    %d\n", s->s_rev_level);
	printf ("Filesystem state:        ");
	print_fs_state (stdout, s->s_state);
	printf ("\n");
	printf ("Errors behavior:          ");
	print_fs_errors (stdout, s->s_errors);
	printf ("\n");
	printf ("Inode count:              %u\n", s->s_inodes_count);
	printf ("Block count:              %u\n", s->s_blocks_count);
	printf ("Reserved block count:     %u\n", s->s_r_blocks_count);
	printf ("Free blocks:              %u\n", s->s_free_blocks_count);
	printf ("Free inodes:              %u\n", s->s_free_inodes_count);
	printf ("First block:              %u\n", s->s_first_data_block);
	printf ("Block size:               %u\n", EXT2_BLOCK_SIZE(s));
	printf ("Fragment size:            %u\n", EXT2_FRAG_SIZE(s));
	printf ("Blocks per group:         %u\n", s->s_blocks_per_group);
	printf ("Fragments per group:      %u\n", s->s_frags_per_group);
	printf ("Inodes per group:         %u\n", s->s_inodes_per_group);
	printf ("Inode blocks per group:   %u\n", inode_blocks_per_group);
	printf ("Last mount time:          %s", ctime ((time_t *) &s->s_mtime));
	printf ("Last write time:          %s", ctime ((time_t *) &s->s_wtime));
	printf ("Mount count:              %u\n", s->s_mnt_count);
	printf ("Maximum mount count:      %d\n", s->s_max_mnt_count);
	printf ("Last checked:             %s", ctime ((time_t *) &s->s_lastcheck));
	printf ("Check interval:           %u\n", s->s_checkinterval);
	if (s->s_checkinterval)
	{
		time_t next;

		next = s->s_lastcheck + s->s_checkinterval;
		printf ("Next check after:         %s", ctime (&next));
	}
#ifdef	EXT2_DEF_RESUID
	printf ("Reserved blocks uid:      ");
	print_user (s->s_def_resuid);
	printf ("Reserved blocks gid:      ");
	print_group (s->s_def_resgid);
#endif
#ifdef EXT2_DYNAMIC_REV
	if (s->s_rev_level >= EXT2_DYNAMIC_REV) {
		printf("First inode:              %d\n", s->s_first_ino);
		printf("Inode size:		  %d\n", s->s_inode_size);
	}
#endif
}
