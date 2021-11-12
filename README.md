# BlockChainVU2

Antrasis VU Grandinių technologijų modulio laboratorinis darbas

Failus sukompiliuoti galima naudojant MacOS kompiliatorių Clang naudojant komandinę eilutė terminale clang++ src.cpp -o output

Programos veikimas:
Programos veikimas

1. Programa 1000 sugeneruoja atsitiktinių vartotojų su atsitiktiniais duomenimis.
2. Programa 10000 sugeneruoja tranzakcijų
3. Sugeneravus duomenis, sugeneruojami blokų kandidatai.
4. Iškasus bloką, vykdomos tranzakcijos
5. Vykdoma tranzakcijos validacija ir atitinkamai pakeičiamos turimos valiutos sumos pas vartotojus
6. Tranzakcijos yra išsaugomos į sąrašą. Jei visos trankzakcijos sėkmingos - blokas pridedamas į blokų grandinę.
7. Duomenys išsaugomi txt failę.

