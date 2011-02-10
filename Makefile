demo:
	(cd green; make)
	(echo; echo; cd pyxi; python pyxi)

clean:
	(cd green; make clean)
	(cd pyxi; rm -f core *.pyc)
