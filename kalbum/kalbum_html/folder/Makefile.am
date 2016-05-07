####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = images 

EXTRA_DIST = gen_src.html pho_src.html thm_src.html screenshot.png 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/kalbum_html/folder/
	$(INSTALL_DATA) $(srcdir)/gen_src.html $(kde_datadir)/kalbum/data/kalbum_html/folder/gen_src.html
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/kalbum_html/folder/
	$(INSTALL_DATA) $(srcdir)/pho_src.html $(kde_datadir)/kalbum/data/kalbum_html/folder/pho_src.html
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/kalbum_html/folder/
	$(INSTALL_DATA) $(srcdir)/thm_src.html $(kde_datadir)/kalbum/data/kalbum_html/folder/thm_src.html
	$(mkinstalldirs) $(kde_datadir)/kalbum/data/kalbum_html/folder/
	$(INSTALL_DATA) $(srcdir)/screenshot.png $(kde_datadir)/kalbum/data/kalbum_html/folder/screenshot.png

uninstall-local:
	-rm -f $(kde_datadir)/kalbum/data/kalbum_html/folder/gen_src.html
	-rm -f $(kde_datadir)/kalbum/data/kalbum_html/folder/pho_src.html
	-rm -f $(kde_datadir)/kalbum/data/kalbum_html/folder/thm_src.html
	-rm -f $(kde_datadir)/kalbum/data/kalbum_html/folder/screenshot.png

####### kdevelop will overwrite this part!!! (end)############
