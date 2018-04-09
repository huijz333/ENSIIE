# Executables
OSTYPE = $(shell uname -s)
JAVAC = javac
JAVA = java
# A2PS = a2ps-utf8
A2PS = a2ps
GHOSTVIEW = gv
DOCP = javadoc
ARCH = zip
#ARCH = tar zcvf
PS2PDF = ps2pdf -dPDFX=true -sPAPERSIZE=a4
DATE = $(shell date +%Y-%m-%d)
# Options de compilation
#CFLAGS = -verbose
CFLAGS =
ifeq ($(findstring Darwin,$(OSTYPE)),Darwin)
    # MacOS systems
	CLASSPATH=.:/opt/local/share/java/junit.jar:/opt/local/share/java/hamcrest-core.jar
else
    # Other systems
    CLASSPATH=.
endif

JAVAOPTIONS = --verbose

PROJECT=Ensembles
# nom du fichier d'impression
OUTPUT = $(PROJECT)
# nom du répertoire ou se situera la documentation
DOC = doc
# lien vers la doc en ligne du JDK
WEBLINK = "http://docs.oracle.com/javase/6/docs/api/"
# lien vers la doc locale du JDK
LOCALLINK = "file:///Users/davidroussel/Documents/docs/java/api/"
# nom de l'archive
ARCHIVE = $(PROJECT)
# format de l'archive pour la sauvegarde
ARCHFMT = zip
#ARCHFMT = tgz
# Répertoire source
SRC = src
# Répertoire bin
BIN = bin
# Répertoire Listings
LISTDIR = listings
# Répertoire Archives
ARCHDIR = archives
# Répertoire Figures
FIGDIR = graphics
# noms des fichiers sources
MAIN = RunAllTests
SOURCES = $(foreach name, $(MAIN), $(SRC)/$(name).java) \
$(SRC)/listes/package-info.java \
$(SRC)/listes/IListe.java \
$(SRC)/listes/Liste.java \
$(SRC)/tableaux/package-info.java \
$(SRC)/tableaux/Tableau.java \
$(SRC)/ensembles/package-info.java \
$(SRC)/ensembles/Ensemble.java \
$(SRC)/ensembles/EnsembleGenerique.java \
$(SRC)/ensembles/EnsembleVector.java \
$(SRC)/ensembles/EnsembleListe.java \
$(SRC)/ensembles/EnsembleTableau.java \
$(SRC)/ensembles/EnsembleFactory.java \
$(SRC)/ensembles/EnsembleTri.java \
$(SRC)/ensembles/EnsembleTriVector.java \
$(SRC)/ensembles/EnsembleTriListe.java \
$(SRC)/ensembles/EnsembleTriTableau.java \
$(SRC)/ensembles/EnsembleTriGenerique.java \
$(SRC)/ensembles/EnsembleTriVector2.java \
$(SRC)/ensembles/EnsembleTriListe2.java \
$(SRC)/ensembles/EnsembleTriTableau2.java \
$(SRC)/ensembles/EnsembleTriFactory.java \
$(SRC)/tests/package-info.java \
$(SRC)/tests/AllTests.java \
$(SRC)/tests/AllEnsembleTest.java \
$(SRC)/tests/ListeTest.java \
$(SRC)/tests/TableauTest.java \
$(SRC)/tests/EnsembleTriTest.java

OTHER = Sujet.pdf

.PHONY : doc ps

# Les targets de compilation
# pour générer l'application
all : $(foreach name, $(MAIN), $(BIN)/$(name).class)

#règle de compilation générique
$(BIN)/%.class : $(SRC)/%.java
	$(JAVAC) -sourcepath $(SRC) -classpath $(BIN):$(CLASSPATH) -d $(BIN) $(CFLAGS) $<

# Edition des sources $(EDITOR) doit être une variable d'environnement
edit :
	$(EDITOR) $(SOURCES) Makefile &

# nettoyer le répertoire
clean :
	find bin/ -type f -name "*.class" -exec rm -f {} \;
	rm -rf *~ $(DOC)/* $(LISTDIR)/*

realclean : clean
	rm -f $(ARCHDIR)/*.$(ARCHFMT)

# générer le listing
$(LISTDIR) : 
	mkdir $(LISTDIR)

ps : $(LISTDIR)
	$(A2PS) -2 --file-align=fill --line-numbers=1 --font-size=10 \
	--chars-per-line=100 --tabsize=4 --pretty-print \
	--highlight-level=heavy --prologue="gray" \
	-o$(LISTDIR)/$(OUTPUT).ps Makefile $(SOURCES)

pdf : ps 
	$(PS2PDF) $(LISTDIR)/$(OUTPUT).ps $(LISTDIR)/$(OUTPUT).pdf

# générer le listing lisible pour Gérard
bigps :
	$(A2PS) -1 --file-align=fill --line-numbers=1 --font-size=10 \
	--chars-per-line=100 --tabsize=4 --pretty-print \
	--highlight-level=heavy --prologue="gray" \
	-o$(LISTDIR)/$(OUTPUT).ps Makefile $(SOURCES)

bigpdf : bigps
	$(PS2PDF) $(LISTDIR)/$(OUTPUT).ps $(LISTDIR)/$(OUTPUT).pdf

# voir le listing
preview : ps
	$(GHOSTVIEW) $(LISTDIR)/$(OUTPUT); rm -f $(LISTDIR)/$(OUTPUT) $(LISTDIR)/$(OUTPUT)~

# générer la doc avec javadoc
doc : $(SOURCES)
	$(DOCP) -private -d $(DOC) -author -link $(LOCALLINK) $(SOURCES)
#	$(DOCP) -private -d $(DOC) -author -linkoffline $(WEBLINK) $(LOCALLINK) $(SOURCES)

# générer une archive de sauvegarde
$(ARCHDIR) : 
	mkdir $(ARCHDIR)

archive : pdf $(ARCHDIR)
	$(ARCH) $(ARCHDIR)/$(ARCHIVE)-$(DATE).$(ARCHFMT) $(SOURCES) $(LISTDIR)/*.pdf $(OTHER) $(BIN) Makefile

# exécution des programmes de test
run : all
	$(foreach name, $(MAIN), $(JAVA) -classpath $(BIN):$(CLASSPATH) $(name) $(JAVAOPTIONS) )
