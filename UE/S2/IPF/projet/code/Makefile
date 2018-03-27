TARGET	= subset-sum-opt.out

ML	= listes.ml\
	  ensembles.ml\
	  approche_naive.ml\
	  approche_plus_directe.ml\
	  approche_avec_nettoyage.ml\
	  approche_diviser_pour_regner.ml

CMO	= $(ML:%.ml=%.cmo)

MLI	= $(ML:%.ml=%.mli)
CMI	= $(MLI:%.mli=%.cmi)

all: $(TARGET)

$(TARGET): $(CMI) $(CMO) tests.cmo
	ocamlc $(CMO) tests.cmo -o $(TARGET)

tests.cmo: tests.ml

%.cmo : %.ml
	ocamlc -c $<

%.cmi : %.mli
	ocamlc -c $<

clean:
	rm -rf $(CMI) $(CMO) tests.cmo tests.cmi

fclean: clean
	rm -rf $(TARGET)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean
