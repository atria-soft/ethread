Build lib & build sample                           {#ethread_build}
========================

@tableofcontents

Download:                                          {#ethread_build_download}
=========

ethread use some tools to manage source and build it:

lutin (build-system):                              {#ethread_build_download_lutin}
---------------------

```{.sh}
	pip install lutin --user
	# optionnal dependency of lutin (manage image changing size for application release)
	pip install pillow --user
```

sources:                                           {#ethread_build_download_sources}
--------

```{.sh}
	cd framework
	git clone https://github.com/atria-soft/ethread.git
	cd ..
```

Build:                                             {#ethread_build_build}
======


library:                                           {#ethread_build_build_library}
--------

```{.sh}
	lutin -mdebug ethread
```
