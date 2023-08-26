TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += CuteComponent
SUBDIRS += Example

Example.depends += CuteComponent
