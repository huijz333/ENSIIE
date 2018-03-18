TARGET	= subset-sum-opt.out

ML	= utils.ml\
	  approche_naive.ml\
	  approche_plus_directe.ml\
	  approche_avec_nettoyage.ml

CMO	= $(ML:%.ml=%.cmo)

MLI	= $(ML:%.ml=%.mli)
CMI	= $(MLI:%.mli=%.cmi)

all: $(TARGET)

$(TARGET): $(CMI) $(CMO)
	ocamlc -c tests.ml
	ocamlc $(CMO) tests.cmo -o $(TARGET)

$(CMI): %.cmi : %.mli
	ocamlc -c $<

$(CMO): %.cmo : %.ml
	ocamlc -c $<
clean:
	rm -rf $(CMI) $(CMO) tests.cmo tests.cmi

fclean: clean
	rm -rf $(TARGET)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean
