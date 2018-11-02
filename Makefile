CMAKE		?= cmake
CONAN		?= conan

BUILDDIR	?= build

DOXYFILE	= Doxyfile

all: $(BUILDDIR)/Makefile
	$(CMAKE) --build $(BUILDDIR)

doc: $(DOXYFILE)
	doxygen $<

$(BUILDDIR)/conanbuildinfo.cmake: conanfile.txt
	$(CONAN) install --build=missing -if $(BUILDDIR) .

CMakeLists.txt: $(BUILDDIR)/conanbuildinfo.cmake

$(BUILDDIR)/Makefile: CMakeLists.txt
	$(CMAKE) -H. -B$(BUILDDIR)

clean: $(BUILDDIR)/Makefile
	$(CMAKE) --build $(BUILDDIR) -- clean

distclean:
	rm -fr $(BUILDDIR)
	rm -fr doc

.phony: all doc clean distclean
