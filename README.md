2048 în terminal folosind Ncurses 

Descriere 

Acest proiect este o implementare a jocului 2048 în C, rulând în terminal cu ajutorul bibliotecii ncurses. 

Cum funcționează jocul 

- Scopul este să combini tile-uri (plăci) de același număr pentru a ajunge la 2048. 
- Poți muta toate tile-urile sus, jos, stânga sau dreapta folosind săgețile de pe tastatură. 
- După fiecare mutare validă, un nou tile cu valoarea 2 (sau uneori 4) apare într-o celulă liberă. 
- Jocul se termină când nu mai există nicio mutare posibilă. 
- Poți părăsi jocul apăsând tasta Q. 

Cerințe 

- Sistem Linux (sau alt sistem compatibil cu ncurses) 
- Compilator C (gcc) 
- Biblioteca ncurses instalată (sudo apt install libncurses5-dev libncursesw5-dev pe Debian/Ubuntu) 

Cum compilezi 

Deschide un terminal și rulează următoarea comandă în folderul unde ai salvat fișierul sursă (de exemplu 2048.c): 

gcc 2048.c -o 2048 -lncurses 

Cum rulezi 

După ce ai compilat, rulează jocul astfel: 

./2048 

Structura codului 

- Grilă de joc: 4x4 celule, reprezentată printr-o matrice grid[4][4] 
- Control: Se folosește tastatura pentru mișcări cu săgeți sau Q pentru ieșire. 
- Ncurses: Folosit pentru a desena grila, tile-urile și pentru a captura input-ul de la utilizator. 
- Mecanică de joc: 
  - Combinați două tile-uri de aceeași valoare prin mutare. 
  - Score-ul crește când combini tile-uri. 
  - Se adaugă un nou tile după fiecare mutare validă. 

 
