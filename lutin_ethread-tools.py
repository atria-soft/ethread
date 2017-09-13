#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol thread tools"

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
	    'ethread/tools.cpp',
	    ])
	
	my_module.add_header_file([
	    'ethread/tools.hpp',
	    'ethread/Thread.hpp',
	    'ethread/Mutex.hpp',
	    'ethread/MutexRecursive.hpp',
	    'ethread/Semaphore.hpp',
	    ])
	
	if "Windows" in target.get_type():
		my_module.add_src_file([
		    'ethread/Mutex.Windows.cpp',
		    'ethread/MutexRecursive.Windows.cpp',
		    'ethread/Thread.Windows.cpp',
		    ])
	else:
		my_module.add_src_file([
		    'ethread/Mutex.pthread.cpp',
		    'ethread/MutexRecursive.pthread.cpp',
		    'ethread/Thread.pthread.cpp',
		    ])
		my_module.add_depend([
		    'pthread',
		    ])
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	# add dependency of the generic C++ library:
	my_module.add_depend([
	    'etk-base',
	    ])
	#pthread is not availlable on Windows
	if    "Linux" in target.get_type() \
	   or "Android" in target.get_type():
		my_module.add_depend([
		    'pthread'
		    ])
	
	return True


