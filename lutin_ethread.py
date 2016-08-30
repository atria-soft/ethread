#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol thread tools"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1,"dev"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_compile_flags()
	# add the file to compile:
	my_module.add_src_file([
		'ethread/tools.cpp',
		])
	
	my_module.add_header_file([
		'ethread/tools.h',
		])
	
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	# add dependency of the generic C++ library:
	my_module.add_module_depend([
	    'cxx',
	    'pthread'
	    ])
	
	my_module.add_path(tools.get_current_path(__file__))
	return my_module


