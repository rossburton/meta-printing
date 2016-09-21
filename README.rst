======================================
 meta-printing layer for openembedded
======================================

This README file contains information on the contents and use of the
meta-printing layer for openembedded..

Please see the corresponding sections below for details.


Dependencies
============

This layer depends on either::

  URI: git://git.openembedded.org/bitbake
  branch: master or krogoth

  URI: git://git.openembedded.org/openembedded-core
  layers: meta, meta-python
  branch: master or krogoth

plus a BSP layer of your choice, eg, meta-ti::

  URI: git://git.yoctoproject.org/meta-ti
  layer: meta-ti
  branch: master

or::

  URI: git://git.yoctoproject.org/poky
  layers: meta, meta-yocto-bsp
  branch: master or krogoth

  URI: https://github.com/openembedded/meta-openembedded
  layers: meta, meta-python
  branch: master or krogoth

Patches
=======

Please submit any patches against the meta-printing layer by using the github
pull-request feature.  Fork the repo, make a branch, do the work, pull from
upstream, create the pull request, yada-yada.

Maintainers: Steve Arnold <nerdboy@gentoo.org>
             Fan Xin <fan.xin@jp.fujitsu.com>

Table of Contents
=================

  I. Adding the meta-printing layer to your build
 II. Misc


I. Adding the meta-printing layer to your build
===============================================

In order to use this layer, you need to make the build system aware of
it.

Assuming the meta-printing layer exists at the top-level of your
yocto build tree, you can add it to the build system by adding the
location of the meta-printing layer to bblayers.conf, along with any
other layers needed. e.g.:

  BBLAYERS ?= " \
    /path/to/yocto/meta \
    /path/to/yocto/meta-poky \
    /path/to/yocto/meta-yocto-bsp \
    /path/to/yocto/meta-openembedded/meta-oe \
    /path/to/yocto/meta-openembedded/meta-python \
    /path/to/yocto/meta-meta-printing \
    "

If you need networking support, also add meta-openembedded/meta-networking to
the above.


II. Misc
========

For newer kernel recipes, you will need the libc-headers and python-evdev (which
is the current default).  For older kernels, you should set the priority of the
meta-printing layer lower, or set preferred_versions for the above two recipes.

For (very) recent beaglebone or mips octeon kernels add the `meta-small-arm`_
layer and leave the above defaults as-is.

.. _meta-small-arm: https://github.com/sarnold/meta-small-arm-extra
