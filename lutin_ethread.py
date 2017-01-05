#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol thread"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
	    'ethread/debug.cpp',
	    'ethread/Future.cpp',
	    'ethread/Promise.cpp',
	    'ethread/Pool.cpp',
	    'ethread/PoolAction.cpp',
	    'ethread/PoolExecutor.cpp',
	    ])
	
	my_module.add_header_file([
	    'ethread/Future.hpp',
	    'ethread/Promise.hpp',
	    'ethread/Pool.hpp',
	    'ethread/PoolAction.hpp',
	    'ethread/PoolExecutor.hpp',
	    ])
	
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	# add dependency of the generic C++ library:
	my_module.add_depend([
	    'cxx',
	    'elog',
	    'ethread-tools',
	    'echrono',
	    'ememory'
	    ])
	return True


