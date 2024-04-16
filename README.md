## Genetic Algorithms

- Implementati un algoritm genetic pentru determinarea maximului unei functii pozitive pe un domeniu dat. Functia va fi un polinom de gradul 2, cu coeficienti dati. Algoritmul trebuie sa cuprinda etapele de selectie, incrucisare si mutatie.

## Input format
```
Population_size
Interval
Parameters
Precision
Crossover probability
Mutation probability
Steps
```
- [Example](https://github.com/Andrei137/Genetic-Algorithms/blob/main/bin/Genetics.in)

## Output requirements
- [x] Populatia initiala
    - [x] B_i = reprezentarea pe biti a cromozomului
    - [x] X_i = valoarea corespunzatoare cromozomului (numar real)
    - [x] f(X_i) = valoarea functiei in punctul respectiv
- [x] Probabilitatea de selectie pentru fiecare cromozom
    - [x] p_i = f(X_i) / f(X) 
- [x] Probabilitatile cumulate care dau intervalele pentru selectie
    - [x] q_i = p_1 + ... + p_i
- [x] Evidentierea procesului de selectie
    - [x] Cautare binara pentru cautarea intervalului
- [x] Evidentierea cromozomilor care participa la incrucisare, al punctul de rupere si al cromozomilor rezultati
- [x] Populatia rezultata dupa incrucisare
- [x] Populatia rezultata dupa mutatii
- [x] Pentru restul generatiilor, se afiseaza doar valoarea medie si valoarea maxima a performantei populatiei

- [Solution](https://github.com/Andrei137/Genetic-Algorithms/blob/main/bin/Genetics.out)

## Build
```bash
cd scripts
build.bat
```

## Run
```bash
cd scripts
run.bat
```