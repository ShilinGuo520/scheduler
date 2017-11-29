#!/usr/bin/python
#coding:utf8

import os
import sys

TARGET = 'bootloader'
START_FILE = './cm3/start_up.S'
LINK_FILE = './cm3/flash.ld'


# toolchain (using code sourcery now)
THUMB = '-mthumb'
MCU  = 'cortex-m3'
ARCH = 'armv7-m'


CFLAGS = THUMB + ' -mcpu=' + MCU + ' -march=' + ARCH + ' -O2 -g3 -gdwarf-2 -nostdlib -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-builtin'
ASFLAGS = '-x assembler-with-cpp'

LDFLAGS = '-T' + LINK_FILE + ' -Mmap > ' + TARGET + '.map'

#--------------compile tools-------------------
CROSS_COMPILE = 'arm-none-eabi-'
CC = CROSS_COMPILE + 'gcc '
LD = CROSS_COMPILE + 'ld -v '
OBJCOPY = CROSS_COMPILE + 'objcopy '
OBJDUMP = CROSS_COMPILE + 'objdump '

def dirlist(path, allfile):
	filelist =  os.listdir(path)

	for filename in filelist:
		filepath = os.path.join(path, filename)
		if os.path.isdir(filepath):
			dirlist(filepath, allfile)
		else:
			if os.path.splitext(filepath)[1] == '.c':
				filepath = os.path.abspath(filepath)
				allfile.append(filepath)
	return allfile

def objlist(path, allfile):
        filelist =  os.listdir(path)

        for filename in filelist:
                filepath = os.path.join(path, filename)
                if os.path.isdir(filepath):
                        objlist(filepath, allfile)
                else:
                        if os.path.splitext(filepath)[1] == '.o':
                                filepath = os.path.abspath(filepath)
                                allfile.append(filepath)
        return allfile


def inclist(path, incpath):
        filelist =  os.listdir(path)

        for filename in filelist:
                filepath = os.path.join(path, filename)
                if os.path.isdir(filepath):
                        inclist(filepath, incpath)
                else:
                        if os.path.splitext(filepath)[1] == '.h':
				fileabspath = os.path.abspath(filepath)
				fileabspath = os.path.split(fileabspath)
                                incpath.append(fileabspath[0])

	incpath = list(set(incpath))
        return incpath

def make():
	src_list = dirlist("./", [])
	inc_path = inclist("./", [])

	inc_file = ' '
	for inc in inc_path:
		inc = '-I' + inc
		inc_file = inc_file + inc + ' '

	start_file = START_FILE
	start_file_out = start_file[:-1] + 'o'
	as_make = CC + ASFLAGS + ' -o ' + start_file_out + ' -c ' + start_file
	print as_make
	os.system(as_make)

	for src_file in src_list:
		out_file = src_file[:-1] + 'o'
		cc_make = CC , CFLAGS, inc_file, ' -c ' , src_file , ' -o ' , out_file
		cc_make = ''.join(cc_make)
		print cc_make
		os.system(cc_make)

	obj_list = objlist("./", [])
	obj_file = ' '
	for obj in obj_list:
		obj_file = obj_file + obj + ' '

	obj_file = obj_file + './libgcc.A'
	print obj_file
	ld_make = LD + LDFLAGS + obj_file + ' --output ' + TARGET + '.elf'
	print ld_make
	os.system(ld_make)

	make_bin = OBJCOPY +' -O binary ' + TARGET + '.elf ' + TARGET + '.bin'
	print make_bin
	os.system(make_bin)


def clean():
	obj_file = ' '
	obj_list = objlist("./", [])
	for obj in obj_list:
		obj_file = obj_file + obj + ' '

	rm_file = obj_file	

	file_ext = ['.elf', '.map', '.bin']
	for ext in file_ext:
		rm_file = rm_file + ' ' + TARGET + ext

	rm_cmd = 'rm -f' + rm_file
	print rm_cmd
	os.system(rm_cmd)


def help():
	print 'Use the following command:'
	print 'python make.py make'
	print 'python make.py clean'

if len(sys.argv) < 2:
	print 'error'
	help()
else:
	print sys.argv[0], sys.argv[1]
	if sys.argv[1] == 'make':
		print 'make'
		make()
	elif sys.argv[1] == 'clean':
		print 'clean'
		clean()
	else:
		print 'error'
		help()

