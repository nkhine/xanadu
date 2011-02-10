#
#	Makefile for Udanax.com Distribution stuff
# 


SHELL=/bin/csh

code:  backend

clean:
	(cd fe_source; make clean)
	(cd be_source; make clean)
	(cd glue_source; make clean)

fex:
	(cd fe_source; make && cp fex ..) || exit 1

glue:
	(cd glue_source; make && cp intf ints intx xlog ..) || exit 1

backend:
	(cd be_source; make) || exit 1

emptydemo: backend
	if (-f enf.enf) rm enf.enf
	be_source/xumain < edemo/demo_scripts/demodocs
	be_source/backend < edemo/demo_scripts/demolinks
	cp enf.enf enf.empty

olddemo: backend
	if (-f enf.enf) rm enf.enf
	be_source/xumain < olddemo/demo_scripts/demodocs
	be_source/backend < olddemo/demo_scripts/demolinks
	cp enf.enf enf.olddemo
