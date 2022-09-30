#include <iostream>
#include <ctime>

using namespace std;

//int brojacIN = 0; bool KRAJ = false; int brojC = 0; int brojP = 0; int proveraStepena = 0;//BITNO ZA STVARANJE NOVOG CVORA


struct Cvor {
	char win;
	int stepen;//Koristiti kod +=
	int pobednik; //Koristili kod resetke, kod operatora +=
	int stanje;//Za ispis 1,0,-1

	int* slobodno;//Niz koji nam govori koliko imamo slobodnih pozicija
	char** mat;//Matrica
	Cvor** sinovi;//Pokazivaci na sinove
	Cvor* otac;//Padre

	bool mesto(int kol);//Koristili smo kod +=
	bool pobeda(char c) const;//Za ispis 1,0,-1
	Cvor& uredi(int kol, char c);//Uredjivanje matrice nakon unosa
	void pisi() const;//Ispisuje matricu

	Cvor(Cvor* papi = nullptr, int kol = 0, char c = ' ') {
		win = ' ';
		stepen = 0; //Stepen je za svaki novi cvor 0 pa se azurira
		pobednik = 0;//Za svaki cvor ce se znati da li je pobednik ili ne
		stanje = 0;//Ovo ce nositi vrednosit 1,0,-1
		slobodno = new int[5];//Alociramo memoriju za niz od 5 elemenata koji ce nam govoriti koliko ima slobodnih mesta po kolonama
		for (int i = 0; i < 5; i++)
			slobodno[i] = 5;
		otac = papi;//Pokazivacu otac dodeljujemo parametar ovog konstruktora.Samo koren stabla nece imati oca, pa je zato podrazumevana vrednost parametra nullptr
		sinovi = new Cvor * [5];//Alociramo prostor za niz od 5 pokazivaca na sinove
		for (int i = 0; i < 5; i++)
			sinovi[i] = nullptr;
		mat = new char* [5];//Alociramo prostor za pet pokazivaca na char
		for (int i = 0; i < 5; i++)
			mat[i] = new char[5];
		if (otac == nullptr) {//Ako je koren onda mu postavljamo vrednost svakog polja u matrici na 0(tj. na prazno polje)
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					mat[i][j] = '0';
		}
		else {
			for (int i = 0; i < 5; i++)         //Ako pokazivac na oca nije nullptr onda kopiramo matricu oca i dodamo taj jedan karakter koji predstavlja potez igraca 
				for (int j = 0; j < 5; j++)
					mat[i][j] = otac->mat[i][j];
			for (int i = 0; i < 5; i++)
				slobodno[i] = otac->slobodno[i];
			uredi(kol, c);
		}
	}
	~Cvor() {
		delete[] sinovi;                //Dealociranje dinamicke memorije koja je dodeljena objektu strukture cvor
		for (int i = 0; i < 5; i++)
			delete mat[i];
		delete[] mat;
	}
};

class Red {
private:
	struct Elem {
		Cvor* pok;
		Elem* sled;
		Elem(Cvor* p, Elem* sl = nullptr) :pok(p), sled(sl) {};
	};
	Elem* head, * tale;
	void kopiraj(const Red&);
	void premesti(Red&);
public:
	void brisi();//Zbog brisanja redova pri brisanju objekta
	Red() { head = tale = nullptr; }
	Red(const Red& red) { kopiraj(red); }
	Red& operator=(const Red& red) {
		if (this != &red) { brisi(); kopiraj(red); }
		return *this;
	}
	Red& operator=(Red&& red) {
		if (this != &red) { brisi(); premesti(red); }
		return *this;
	}
	Red(Red&& red) { premesti(red); }
	~Red() { brisi(); }
	Red& dodaj(Cvor*);
	Cvor* uzmi();
	bool prazan();
};

class Stek {
private:
	struct Elem {
		Cvor* pok;
		Elem* sled;
		Elem(Cvor* p, Elem* sl = nullptr) : pok(p), sled(sl) {};
	};
	Elem* head;
	void kopiraj(const Stek&);
	void premesti(Stek&);
public:
	void brisi();
	Stek() { head = nullptr; }
	Stek(const Stek& stek) { kopiraj(stek); }
	Stek(Stek&& stek) { premesti(stek); }
	Stek& operator =(const Stek& stek) {
		if (this != &stek) { brisi(); kopiraj(stek); }
		return *this;
	}
	Stek& operator =(Stek&& stek) {
		if (this != &stek) { brisi(); kopiraj(stek); }
		return *this;
	}
	~Stek() { brisi(); }
	Stek& dodaj(Cvor*);
	Cvor* uzmi();
	bool prazan();
};

class Stablo {
private:
	int brojacIN = 0; bool KRAJ = false; int brojC = 0; int brojP = 0; int proveraStepena = 0;	int BR = 0;
	Cvor* prvi, * tek;
	Red redIN, redOUT; Stek stekNUM;
public:
	void brisi();
	Stablo() { prvi = tek = nullptr; }
	Stablo(const Stablo&) = delete;
	Stablo& operator=(const Stablo&) = delete;
	~Stablo() { brisi(); }
	void popuniResetku();
	Stablo& operator+=(char);
	void pisi() { prvi->pisi(); }
	void pokreniStabloIgara();
	void dodeliBroj();
	int naEkran();
	int brojCvorova() const;
};

int main()
{
	Stablo stablo;
	cout << "UPUTSTVO!!!" << endl;
	cout << "Pred vama ce iskociti meni sa opcijama." << endl << "Opciju birate tako sto s tastature unosite vrednost koja se nalazi ispred zeljene opcije." << endl;
	char c;
	cout << "Unesite opciju.(Ako unesete opciju koja nije u meniju, prekida se program.)" << endl;
	cout << "1.Zapocinjanje nove igre i formiranje stabla" << endl;
	cout << "2.Ispis stabla igre" << endl;
	cout << "3.(bonus opcija) Broj cvorova u stablu" << endl;
	cout << "4.Brisanje stabla" << endl;

	while (1) {

		cout << "Unesite opciju:" << endl;
		cin >> c;

		if (c == '1') {
			cout << endl << "Opcija 1.Prvo unesite trenutno stanje igre(matrica je dimenzija 5x5)," << endl
				<< "a zatim pritiskom tastera ENTER pocinje automatsko generisanje stabla." << endl;
			stablo.popuniResetku();
			stablo.pokreniStabloIgara();
			stablo.dodeliBroj();
			cout << endl;
		}
		else if (c == '2') {
			cout << endl << "Opcija 2.Ispis stabla igara." << endl;
			stablo.naEkran();
			cout << endl;
		}
		else if (c == '3') {
			cout << endl << "Opcija 3.(bonus opcija) Broj cvorova u stablu" << endl;
			cout << stablo.brojCvorova() << endl;

		}
		else if (c == '4') {
			cout << endl << "Opcija 4.Brisanje stabla" << endl;
			stablo.brisi();
			cout << endl;
		}
		else {
			cout << endl << "Kraj programa" << endl;
			break;
		}

	}

	return 0;

}

/*Metode iz klase Red*/

void Red::brisi()
{
	while (head) {
		Elem* stari = head; head = head->sled; delete stari;
	}
	head = tale = nullptr;
}

void Red::kopiraj(const Red& red)
{
	Elem* tek = red.head;
	while (tek) {
		Elem* novi = new Elem(tek->pok);
		if (head == nullptr) head = novi;
		else tale->sled = novi;
		tale = novi;
		tek = tek->sled;
	}
}

void Red::premesti(Red& red)
{
	head = red.head; tale = red.tale;
	red.head = red.tale = nullptr;
}


Red& Red::dodaj(Cvor* p)
{
	Elem* novi = new Elem(p);
	if (head == nullptr) head = novi;
	else tale->sled = novi;
	tale = novi;
	return *this;
}

Cvor* Red::uzmi()
{
	if (head == nullptr) { exit(1); }
	Elem* stari = head; head = head->sled; Cvor* vred = stari->pok; delete stari;
	if (head == nullptr) tale = nullptr;
	return vred;
}

bool Red::prazan()
{
	if (head == nullptr) return true;
	else return false;
}

/*Metode iz klase Stek*/

void Stek::brisi()
{
	while (head) {
		Elem* stari = head; head = head->sled; delete stari;
	}
}


void Stek::kopiraj(const Stek& stek)
{
	Elem* tek = stek.head;
	while (tek) {
		Elem* novi = new Elem(tek->pok);
		novi->sled = head;
		head = novi;
		tek = tek->sled;
	}
}

void Stek::premesti(Stek& stek)
{
	head = stek.head;
	stek.head = nullptr;
}


Stek& Stek::dodaj(Cvor* p)
{
	Elem* novi = new Elem(p);
	novi->sled = head;
	head = novi;
	return *this;
}

Cvor* Stek::uzmi()
{
	if (head == nullptr) exit(1);
	else {
		Cvor* vred = head->pok;
		Elem* stari = head; head = head->sled;
		delete stari;
		return vred;
	}
}

bool Stek::prazan()
{
	if (head == nullptr) return true;
	else return false;
}

/*Metode iz klase Stablo*/

void Stablo::brisi()
{
	brojacIN = 0; KRAJ = false; brojC = 0; brojP = 0;  proveraStepena = 0;
	redIN.brisi(); redOUT.brisi(); stekNUM.brisi();
	BR = 0;
	Red red; red.dodaj(prvi); int moze = 1; Cvor* pom = nullptr;
	if (prvi == nullptr) { cout << "Pokusavate da obrisete prazno stablo."; moze = 0; }
	else {
		while (moze) {
			if (red.prazan() == false) {
				pom = red.uzmi();
			}
			else {
				pom = nullptr; moze = 0;
			}
			if (pom != nullptr) {
				for (int i = 0; i < 5; i++) {
					if (pom->sinovi[i] != nullptr) red.dodaj(pom->sinovi[i]);
				}
				delete pom; pom = nullptr;
			}
		}
	}
	prvi = tek = nullptr;
}

void Stablo::popuniResetku()
{
	brisi();
	if (prvi == nullptr) { prvi = tek = new Cvor; redOUT.dodaj(prvi); }
	cout << "Unesite matricu koja predstavlja pocetnu resetku.(P za plavo, C za crveno, 0 za prazno polje)" << endl;
	char c;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			cin >> c; prvi->mat[i][j] = c;
		}
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++) {
			if (prvi->mat[i][j] != '0')
				prvi->slobodno[j]--;
		}
	BR++;
	/*
	char c1 = 'P'; char c2 = 'C';
	if (prvi->pobeda(c1) || prvi->pobeda(c2)) {
		cout << "Vec imate pobednika." << endl; //KRAJ = true;
	}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			if (prvi->mat[i][j] == 'C') brojC++;
			if (prvi->mat[i][j] == 'P') brojP++;
		}
	if (abs(brojC - brojP) > 1) {
		cout << "Za dva ima vise poteza jednog igraca nego drugog tako da nije dobar unos." << endl; //KRAJ = true;
	}
	*/
}

Stablo& Stablo::operator+=(char c)
{
	while (1) {
		if (brojacIN == 5) {
			if (redIN.prazan() == false) {
				tek = redIN.uzmi(); brojacIN = 0;
				if (tek->stepen > proveraStepena) { proveraStepena++; return *this; }
			}
			else {
				tek = nullptr; brojacIN = 0;
			}
		}
		if (tek == nullptr) { KRAJ = true; return *this; }
		else {
			if ((tek->sinovi[brojacIN] == nullptr) && (tek->mesto(brojacIN))) {
				tek->win = c;
				tek->sinovi[brojacIN] = new Cvor(tek, brojacIN, c); BR++;
				tek->sinovi[brojacIN]->stepen = (tek->stepen) + 1;
				stekNUM.dodaj(tek->sinovi[brojacIN]);//Za dodelu vrednosti 1,0,-1
				redOUT.dodaj(tek->sinovi[brojacIN]);//Za ispis stabla
				if (tek->sinovi[brojacIN]->pobeda(c) == true) { tek->sinovi[brojacIN]->pobednik = 1; }
				if (tek->sinovi[brojacIN]->pobeda(c) == false) {
					redIN.dodaj(tek->sinovi[brojacIN]);
				}
				brojacIN++;
			}
			else if (tek->mesto(brojacIN) == false) brojacIN++;
		}
	}

}

void Stablo::pokreniStabloIgara()
{
	int i;
	if (brojP > brojC) i = 1;
	else if (brojP < brojC) i = 0;
	else {
		srand((unsigned int)time(NULL));
		i = rand() % 2;
	}

	while (KRAJ == false) {
		if (i % 2 == 0) {
			*this += 'P';
		}
		else {
			*this += 'C';
		}
		i++;
	}
}

void Stablo::dodeliBroj()
{
	Cvor* pom = nullptr; int uslov = 1;
	while (uslov) {
		if (stekNUM.prazan() == false) {
			pom = stekNUM.uzmi();
		}
		else {
			pom = nullptr; uslov = 0;
		}
		if (pom != nullptr) {
			Cvor* temp = pom->otac;
			if (pom->pobednik == 1) {
				Cvor* temp = pom->otac;
				if (temp->stepen % 2 == 0) {
					temp->stanje = 1; pom->stanje = 1;
				}
				else if (temp->stepen % 2 == 1) {
					temp->stanje = -1; pom->stanje = -1;
				}
			}
			if (pom->pobednik == 0) {
				if (pom->stanje == -1) {
					if (temp->stepen % 2 == 1) {
						temp->stanje = -1;
					}
				}
				if (pom->stanje == 1) {
					if (temp->stepen % 2 == 0) {
						temp->stanje = 1;
					}
				}
			}


		}

	}
}

int Stablo::naEkran()
{
	Cvor* temp = nullptr; Red red = redOUT;

	if (prvi == nullptr) { cout << "Ne mozete stampati prazno stablo" << endl; return 1; }

	if (red.prazan() == false) temp = red.uzmi();
	else temp = nullptr;

	if (temp == nullptr) return 1;
	else if (temp != nullptr) {
		while (temp != nullptr) {
			cout << endl << endl << endl;
			if (temp->pobednik == 1) {
				cout << "Pobeda!!!.Pobednik je:" << temp->otac->win << endl;
			}
			cout << endl;
			temp->pisi();
			cout << endl << temp->stanje;
			cout << endl << endl << endl;

			if (red.prazan() == false) temp = red.uzmi();
			else temp = nullptr;
		}
	}

	return 0;
}

int Stablo::brojCvorova() const
{
	return BR;
}

/*Metode iz strukture Cvor*/

bool Cvor::pobeda(char c) const
{
	int niz = 0;
	//po vrstama detekcija OK
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (mat[i][j] == c) niz++;
			if (niz == 3) return true;
			if (mat[i][j] != c) niz = 0;
		}
		niz = 0;
	}
	//po kolonama detekcija OK
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			if (mat[i][j] == c) niz++;
			if (niz == 3) return true;
			if (mat[i][j] != c) niz = 0;
		}
		niz = 0;
	}
	niz = 0;
	//po glavnoj dijagonali
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i == j) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//dijagonala 1-2,2-3,3-4,4-5
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 5; j++) {
			if ((i + 1) == j) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//dijagonala 2-1,3-2,4-3,5-4
	for (int i = 1; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			if ((j + 1) == i) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//dijagonala 1-3,2-4,3-5
	for (int i = 0; i < 3; i++) {
		for (int j = 2; j < 5; j++) {
			if ((i + 2) == j) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//dijagonala 3-1,4-2,5-3
	for (int i = 2; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			if ((j + 2) == i) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//na sporednoj dijagonali
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if ((i + j) == 6) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//na dijagonali 1-4,2-3,3-2,4-1
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if ((i + j) == 5) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//na dijagonali 2-5,3-4,4-3,5-2
	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 5; j++) {
			if ((i + j) == 7) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//na dijagonali 1-3,2-2,3-1
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if ((i + j) == 4) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	}
	niz = 0;
	//na dijagonali 3-5,4-4,5-3
	for (int i = 2; i < 5; i++)
		for (int j = 2; j < 5; j++) {
			if ((i + j) == 8) {
				if (mat[i][j] == c) niz++;
				if (niz == 3) return true;
				if (mat[i][j] != c) niz = 0;
			}
		}
	return false;
}

Cvor& Cvor::uredi(int kol, char c)
{
	int i = 4;
	slobodno[kol] = slobodno[kol] - 1;
	if (slobodno[kol] >= 0) {
		while (i >= 0) {
			if (mat[i][kol] == '0') {
				mat[i][kol] = c;
				return *this;
			}
			i--;
		}
	}
	else {
		cout << "Nema mesta u ovoj koloni" << endl;
		return *this;
	}
	return *this;
}

void Cvor::pisi() const
{
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			cout << mat[i][j];
		}
		cout << endl;
	}
}

bool Cvor::mesto(int kol)
{
	if (slobodno[kol] > 0)  return true;
	else
		return false;

}

