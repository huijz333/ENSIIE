#a
./cmd

#b
./cmd > sortie

#c
./cmd 2> erreur

#d
./cmd > sortie 2> poubelle

#e
./cmd | less

#f (-n => pas de '\n')
echo -n "bonjour monsieur" | cmd

#g
ls | cmd 2> poubelle | less

#h
cat * | cmd 2> poubelle | less

# h bis
cat $(ls) | cmd 2> poubelle | less
