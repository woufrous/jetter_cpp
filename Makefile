CMAKE		?= cmake

BUILDDIR	?= build

DOXYFILE	= Doxyfile

all: $(BUILDDIR)/Makefile
	$(CMAKE) --build $(BUILDDIR)

doc: $(DOXYFILE)
	doxygen $<

$(BUILDDIR)/Makefile: CMakeLists.txt
	$(CMAKE) -H. -B$(BUILDDIR)

clean: $(BUILDDIR)/Makefile
	$(CMAKE) --build $(BUILDDIR) -- clean

distclean:
	rm -fr $(BUILDDIR)
	rm -fr doc

.phony: all doc clean distclean
