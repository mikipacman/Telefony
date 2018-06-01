#!/bin/bash

# @author Mikołaj Pacek <miki.pacman@gmail.com>

# Skrypt ma za zadanie dla danego programu z testową obsługą baz przekierowań
# telefonicznych, pliku z komendami do tego programu oraz numeru X ma
# wyznaczyć wszystkie takie numery Y, że Y przekierowany jest na X.

# Działanie piku z komendami na program powinno nie produkować
# żadnych błędów, ani nie powinno nic wypisywać na ekran.

if (( $# != 3 )); then
	echo "Poprawne użycie skrytpu: ./phone_forward.sh <ścieżka pliku wykonywalnego> <ścieżka do pliku z przekierowaniami> <numer>"
	exit 1
fi

PROG=$1
FILE=$2
NUM=$3

# Sprawdzamy poprawność pliku wejściowego.

echo "NEW base " > programInput.in
cat ${FILE} >> programInput.in



./${PROG} < programInput.in > temp.out 2> temp.err

if [ "${temp.out}" != "" ] || [ "${temp.err}" != "" ]; then
	cat < temp.out
	cat < temp.err
	echo "Działanie pliku wejściowego na program jest nieodpowiednie (wyrzuca ono błąd lub wypisuje cokolwiek na ekran)!"
	rm -f programInput.in temp.out temp.err
	exit 1
fi

# Sprawdzamy co generuje funkcja reverse dla numeru NUM. 

echo "NEW base " > programInput.in
cat ${FILE} >> programInput.in
echo "  ? ${NUM}" >> programInput.in

# Zapisujemy wynik reverse'a w pliku reverseOfNum.

./${PROG} < programInput.in > reverseOfNum.out

# Dopisujemy pytajniki na końcu każdego wiersza.

awk '{ print $0 "?" }' < reverseOfNum.out > reverseOfNumWithQMark.out

# Sprawdzamy dla jakich numerów X z reverse'a otrzymamy get(X) = NUM.
# Najpierw przygotujemy plik z wynikami get'ów.

echo "NEW base " > programInput.in
cat ${FILE} >> programInput.in
echo " " >> programInput.in
cat reverseOfNumWithQMark.out>> programInput.in

./${PROG} < programInput.in > get.out

# A teraz sprawdzimy, które linijki są równe NUM i wypiszemy odpowiadające im numery.

while read getOutput && read number <&3 ; do
	
	if [ "${getOutput}" == "${NUM}" ]; then
		echo "${number}"
	fi

done < get.out 3< reverseOfNum.out

rm -f programInput.in reverseOfNum.out reverseOfNumWithQMark.out get.out temp.out temp.err