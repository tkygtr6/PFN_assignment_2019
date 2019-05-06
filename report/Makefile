FILENAME=report

pdf: $(FILENAME).tex
	sed -e 's/。/. /g' $(FILENAME).tex | sed -e 's/、/, /g' > $(FILENAME).mid.tex && \
	echo "" | uplatex $(FILENAME).mid && \
	echo "" | uplatex $(FILENAME).mid && \
	echo "" | uplatex $(FILENAME).mid && \
	dvipdfmx $(FILENAME).mid && \
	cp -f $(FILENAME).mid.pdf $(FILENAME).pdf && \
	rm -f $(FILENAME).mid* && \
	open $(FILENAME).pdf

up:
	cp $(BIBTEX_PATH) ./

clean:
	rm -f $(FILENAME).mid*

distclean: clean
	rm -f $(FILENAME).pdf
