####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kalbum

## INCLUDES were found outside kdevelop specific part

kalbum_SOURCES = albuminfo.cpp albuminfodlg.ui previewimage.cpp previewimagedlg.ui photodetails.cpp photodetailsdlg.ui exportweb.cpp exportwebdlg.ui kalbumnoimagedlg.ui kalbumnoimage.cpp kalbumiconview.cpp kalbumviewitem.cpp photoalbum.cpp kdateedit.cpp kalmimesource.cpp kalbumviewdlg.cpp kalbumcredits.cpp datevalidator.cpp dateedit.cpp kalbumview.cpp kalbumdoc.cpp kalbum.cpp main.cpp 
kalbum_LDADD   =  $(LIB_QT) $(LIB_KDECORE) $(LIB_KDEUI) $(LIB_KFILE)  $(LIBSOCKET)

SUBDIRS = toolbar kalbum_html 

EXTRA_DIST = main.cpp kalbum.cpp kalbum.h kalbumdoc.cpp kalbumdoc.h kalbumview.cpp kalbumview.h kalbumui.rc kalbum.desktop lo32-app-kalbum.png lo16-app-kalbum.png dateedit.cpp dateedit.h datevalidator.cpp datevalidator.h kalbumcredits.cpp kalbumcredits.h kalbumviewdlg.cpp kalbumviewdlg.h kalmimesource.cpp kalmimesource.h kdateedit.cpp kdateedit.h photoalbum.cpp photoalbum.h slide.png slide.xcf hi16-app-kalbum.png hi32-app-kalbum.png kalbumviewitem.cpp kalbumviewitem.h kalbumiconview.cpp kalbumiconview.h kalbumnoimage.cpp kalbumnoimagedlg.ui kalbumnoimage.h file_broken.png exportwebdlg.ui exportweb.cpp exportweb.h x-kalbum.desktop photodetailsdlg.ui photodetails.cpp photodetails.h previewimagedlg.ui previewimage.cpp previewimage.h slides.png albuminfodlg.ui albuminfo.cpp albuminfo.h 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/kalbum/
	$(INSTALL_DATA) $(srcdir)/kalbumui.rc $(kde_datadir)/kalbum/kalbumui.rc
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kalbum.desktop $(kde_appsdir)/Applications/kalbum.desktop
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-kalbum.png $(kde_icondir)/locolor/32x32/apps/kalbum.png
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-kalbum.png $(kde_icondir)/locolor/16x16/apps/kalbum.png
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/slides/
	$(INSTALL_DATA) $(srcdir)/slide.png $(kde_datadir)/kalbum/data/slides/slide01.png
	$(mkinstalldirs) $(kde_icondir)/hicolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/hi16-app-kalbum.png $(kde_icondir)/hicolor/16x16/apps/kalbum.png
	$(mkinstalldirs) $(kde_icondir)/hicolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/hi32-app-kalbum.png $(kde_icondir)/hicolor/32x32/apps/kalbum.png
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/icons/
	$(INSTALL_DATA) $(srcdir)/file_broken.png $(kde_datadir)/kalbum/data/icons/file_broken.png
	$(mkinstalldirs) $(kde_mimedir)/application/
	$(INSTALL_DATA) $(srcdir)/x-kalbum.desktop $(kde_mimedir)/application/x-kalbum.desktop
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/slides/
	$(INSTALL_DATA) $(srcdir)/slides.png $(kde_datadir)/kalbum/data/slides/slides01.png

uninstall-local:
	-rm -f $(kde_datadir)/kalbum/kalbumui.rc
	-rm -f $(kde_appsdir)/Applications/kalbum.desktop
	-rm -f $(kde_icondir)/locolor/32x32/apps/kalbum.png
	-rm -f $(kde_icondir)/locolor/16x16/apps/kalbum.png
	-rm -f $(kde_datadir)/kalbum/data/slides/slide01.png
	-rm -f $(kde_icondir)/hicolor/16x16/apps/kalbum.png
	-rm -f $(kde_icondir)/hicolor/32x32/apps/kalbum.png
	-rm -f $(kde_datadir)/kalbum/data/icons/file_broken.png
	-rm -f $(kde_mimedir)/application/x-kalbum.desktop
	-rm -f $(kde_datadir)/kalbum/data/slides/slides01.png

####### kdevelop will overwrite this part!!! (end)############
# These paths are KDE specific. Use them:
# kde_appsdir         Where your application's menu entry (.desktop) should go to.
# kde_icondir         Where your icon should go to - better use KDE_ICON.
# kde_sounddir        Where your sounds should go to.
# kde_htmldir         Where your docs should go to. (contains lang subdirs)
# kde_datadir         Where you install application data. (Use a subdir)
# kde_locale          Where translation files should go to. (contains lang subdirs)
# kde_cgidir          Where cgi-bin executables should go to.
# kde_confdir         Where config files should go to (system-wide ones with default values).
# kde_mimedir         Where mimetypes .desktop files should go to.
# kde_servicesdir     Where services .desktop files should go to.
# kde_servicetypesdir Where servicetypes .desktop files should go to.
# kde_toolbardir      Where general toolbar icons should go to (deprecated, use KDE_ICON).
# kde_wallpaperdir    Where general wallpapers should go to.
# kde_templatesdir    Where templates for the "New" menu (Konqueror/KDesktop) should go to.
# kde_bindir          Where executables should go to. Use bin_PROGRAMS or bin_SCRIPTS.
# kde_libdir          Where shared libraries should go to. Use lib_LTLIBRARIES.
# kde_moduledir       Where modules (e.g. parts) should go to. Use kde_module_LTLIBRARIES.
# kde_styledir        Where Qt/KDE widget styles should go to (new in KDE 3).
# kde_designerdir     Where Qt Designer plugins should go to (new in KDE 3).

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
kalbum_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rcdir = $(kde_datadir)/kalbum
rc_DATA = kalbumui.rc

messages: rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kalbum.pot; \
	fi

