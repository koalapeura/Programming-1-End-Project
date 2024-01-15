#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define SYOTTEEN_PITUUS 128

void rahanNosto(double *saldo);                             // Rahan nostaminen automaatista
void saldonTarkistus(double *saldo);                        // Tilin saldon tarkistus
void kysyToiminto(char valinta[]);                          // Kysyy kayttajalta hanen haluamansa toiminnon
void tulostaVaihtoehdot();                                  // Tulostaa toiminnot, jotka pankkiautomaatissa on
int valitseNostettavaSumma();                               // Summan valinta rahan nostoa varten
void tulostaNostettavatSummat();                            // Tulostaa summat, jotka ovat nostettavissa
int valitseMuuSumma();                                      // Kysyy kayttajalta jonkin muun summan, kuin normaalivaihtoehdot
int varmistaOnkoLuku(char luku[]);                          // Varmistaa, onko annettu syote luku
void lueMerkkijono(char merkkijono[], int pituus);			// Lukee kayttajalta merkkijonon
void lueRoskat();											// Poistaa puskurista sinne jaaneet roskat
void lajitteleRahat(int otto);								// Tulostaa nostetun rahan mahd.suurina seteleina
void aukaiseTili(double * saldo, char tilinumero[], int * paaSilmukanToistoehto, FILE ** osoitinPolkuun);		// Kysyy kayttajan tilinumeron ja aukaisee kyseisen tilin tekstitiedostona
int tallennaSaldo(FILE * polku, char tilinumero[], double * saldo); // Tallentaa saldon aukaistuun tekstitiedostoon



int main()
{
	int paaSilmukanToistoehto = 1;
	int lopetus;

	char valinta[SYOTTEEN_PITUUS];
	char tilinumero[SYOTTEEN_PITUUS];

	double saldo;

	FILE * polku;
	FILE ** osoitinPolkuun;

	osoitinPolkuun = &polku;

	printf("\n-----------");
	printf("\nTervetuloa!");
	printf("\n-----------\n");

	aukaiseTili(&saldo, tilinumero, &paaSilmukanToistoehto, osoitinPolkuun);

	while (paaSilmukanToistoehto)
	{
		kysyToiminto(valinta);


		if ((int)strlen(valinta) != 1) // Jos syote on pidempi kuin yksi kirjain, tulosta virheviesti
		{
			printf("\n------------------------------");
			printf("\nValitsemaasi toimintoa ei ole!");
			printf("\n------------------------------\n");
		}



		else if (strcmp(valinta, "O") == 0 || strcmp(valinta, "o") == 0)
		{
			rahanNosto(&saldo);
		}


		else if (strcmp(valinta, "S") == 0 || strcmp(valinta, "s") == 0)
		{
			saldonTarkistus(&saldo);
		}


		else if (strcmp(valinta, "L") == 0 || strcmp(valinta, "l") == 0)
		{
			lopetus = tallennaSaldo(polku, tilinumero, &saldo);

			if (!lopetus)
			{
				return 1;
			}

			else
			{
				paaSilmukanToistoehto = 0;
			}

		}


		else
		{
			printf("\n------------------------------");
			printf("\nValitsemaasi toimintoa ei ole!");
			printf("\n------------------------------\n");
		}
	}


	printf("\n-------------------------------");
	printf("\nNakemiin, tervetuloa uudestaan!");
	printf("\n-------------------------------\n");

	return 0;
}

void tulostaNostettavatSummat()
{
	/* tulostaNostettavatSummat - Tulostaa summat, joita tililta voi nostaa. 
	
	Parametrit:
	- ei ole

	Paluuarvo:
	- ei ole

	*/

	printf("\n----------------------------------");
	printf("\n(20) euroa");
	printf("\n(40) euroa");
	printf("\n(60) euroa");
	printf("\n(90) euroa");
	printf("\n(140) euroa");
	printf("\n(240) euroa");
	printf("\n(M)uu summa");
	printf("\nMaksimi nosto on 1000 euroa");
	printf("\n----------------------------------\n");
	printf(">>> ");
}



int valitseNostettavaSumma()
{
	/* valitseNostettavaSumma - Kysyy kayttajalta nostettavan summan. Halutessaan kayttaja
	voi myos syottaa jonkin muun summan, jolloin ohjelma menee nostaMuuSumma funktioon,
	josta palautetaan arvo tälle funktiolle ja tästä funktiosta palautetaan 
	arvo rahanNosto funktiolle. Jos käyttäjä haluaa palata päävalikkoon, palautetaan nolla 
	
	Parametrit:
	- ei ole

	Paluuarvo (int):
	- palauttaa käyttäjän valitseman rahasumman tai nollan, jos käyttäjä haluaa
	palata päävalikkoon

	*/

	char summa[SYOTTEEN_PITUUS];
	int muuSumma;

	printf("\nSyota nostosumma (\"e\" palauttaa paavalikoon): ");
	tulostaNostettavatSummat();


	while (1)
	{

		fgets(summa, SYOTTEEN_PITUUS, stdin);
		fflush(stdin);

		summa[strcspn(summa, "\n")] = 0; // Poistaa syotteesta rivinvaihdon

		if (strcmp(summa, "E") == 0 || strcmp(summa, "e") == 0)
		{
			return 0;
		}

		else if (strcmp(summa, "M") == 0 || strcmp(summa, "m") == 0)
		{
			muuSumma = valitseMuuSumma();
			return muuSumma;
		}

		else if (strcmp(summa, "20") == 0)
		{
			return 20;
		}

		else if (strcmp(summa, "40") == 0)
		{
			return 40;
		}

		else if (strcmp(summa, "60") == 0)
		{
			return 60;
		}

		else if (strcmp(summa, "90") == 0)
		{
			return 90;
		}

		else if (strcmp(summa, "140") == 0)
		{
			return 140;
		}

		else if (strcmp(summa, "240") == 0)
		{
			return 240;
		}

		else
		{
			printf("\n---------------------------------------");
			printf("\nValitsemasi summa ei ollut kelvollinen!");
			printf("\nSyota summa uudestaan");
			printf("\n---------------------------------------");
			printf("\n>>> ");
		}
	}
}


void tulostaVaihtoehdot()
{
	/* tulostaVaihtoehdot - Tulostaa kaikki toiminnot, jota pankkiautomaatissa on. Toimii
	siis pankkiautomaatin päävalikkona. 

	Parametrit:
	- ei ole

	Paluuarvo:
	- ei ole

	*/


	printf("\n\n----------------------------------");
	printf("\nValitse toiminto:\n");
	printf("(O)tto\n");
	printf("(S)aldo\n");
	printf("(L)opeta\n");
	printf("----------------------------------\n");
	printf(">>> ");
}


void kysyToiminto(char valinta[])
{
	/* kysyToiminto - Kysyy käyttäjältä syötteen päävalikossa 
	ja palauttaa sen pääohjelmaan verrattavaksi. Tällä funktiolla
	käyttäjä  valitsee toiminnan päävalikosta 

	Parametrit:
	- char valinta[]: taulukko, johon tallennetaan käyttäjän syöttämä merkkijono

	Paluuarvo:
	- ei ole

	*/

	tulostaVaihtoehdot();

	fgets(valinta, SYOTTEEN_PITUUS, stdin); // Kysyy kayttajalta toiminnon
	fflush(stdin);

	valinta[strcspn(valinta, "\n")] = 0; // Poistaa syotteen lopusta rivinvaihdon
}


void rahanNosto(double *saldo)
{
	/* rahanNosto - Rahan nostoa varten tehty funktio. Summa kysytaan kayttajalta funktiolla valitseNostettavaSumma().
	Jos kayttajan valitsemalle summalle ei loydy tililta katetta, tulee virheviesti.
	Jos kayttaja on halunnut palata päävalikkoon, valitseNostettavaSumma on palauttanut nollan 

	Parametrit:
	- double *saldo: saldo-muuttujan osoite muistissa

	Paluuarvo:
	- ei ole

	*/

	int otto = 0;

	otto = valitseNostettavaSumma();

	if (otto != 0)
	{
		if (*saldo >= otto)
			{
				*saldo -= otto;
				lajitteleRahat(otto);
				saldonTarkistus(saldo);
			}
		else
		{
			printf("\n-------------------------");
			printf("\nSaldosi ei riita nostoon!");
			printf("\n-------------------------\n");
		}
	}
	else
	{
		printf("\n-----------------------");
		printf("\nPalataan paavalikkoon");
		printf("\n-----------------------\n");
	}

}

void saldonTarkistus(double *saldo)
{
	/* saldonTarkistus - Saldon tarkistusta varten tehty funktio. Tulostaa
	saldon käyttäjälle. 

	Parametrit:
	- double * saldo: saldo-muuttujan osoite muistissa

	Paluuarvo:
	- ei ole

	*/


	printf("\n---------------------------");
	printf("\nTilisi saldo on %.2lf euroa", *saldo);
	printf("\n---------------------------\n");
}



int valitseMuuSumma()
{
	/* valitseMuuSumma - Funktio kysyy kayttajalta jonkin normaalivaihtoehtoista poikkeavan summan. Syote ei saa
	sisaltaa kirjaimia tai olla liukuluku.
	Summan tulee olla myos valilta 20 - 1000 euroa mutta se ei voi olla 30 euroa.
	Palauttaa lopulta nostetun summan valitseNostettavaSumma funktiolle. Jos palataan
	päävalikkoon (käyttäjä on syöttänyt e:n), funktio palauttaa nollan
	
	Parametrit:
	- ei ole

	Paluuarvo (int):
	- palauttaa käyttäjän valitseman summan tai nollan, jos käyttäjä haluaa palata päävalikkoon

	*/

	char summa[SYOTTEEN_PITUUS];

	printf("\n----------------------------------");
	printf("\nVoit nostaa rahaa valilta 20 - 1000 euroa");
	printf("\nNostot ovat kymmenen euron valein (poislukien 30 euroa)\n");
	printf("\"e\" palauttaa paavalikkoon)");
	printf("\n----------------------------------\n");
	printf(">>> ");

	while (1)
	{
		fgets(summa, SYOTTEEN_PITUUS, stdin);
		fflush(stdin);

		summa[strcspn(summa, "\n")] = 0;


		if (strcmp(summa, "E") == 0 || strcmp(summa, "e") == 0)
		{
			return 0;
		}

		if (!varmistaOnkoLuku(summa)) // Tarkistaa, etta sisaltaako syote kirjaimia, jos sisaltaa niin virheellinen syote
		{
			printf("\n-------------------------------------------------------");
			printf("\nSyottamasi summa ei ollut kelvollinen! Syota uusi summa");
			printf("\n-------------------------------------------------------");
			printf("\n>>> ");
		}

		else if (atoi(summa) >= 20 && atoi(summa) <= 1000 && atoi(summa) % 10 == 0) // Tarkistaa onko syote tietylla valilla ja kahdellakymmenella jaollinen
		{
			if (atoi(summa) == 30)
			{
				printf("\n----------------------------------------");
				printf("\nEt voi nostaa 30 euroa! Syota uusi summa");
				printf("\n----------------------------------------");
				printf("\n>>> ");
			}

			else
			{
				return atoi(summa);
			}

		}
		else
		{
			printf("\n-------------------------------------------------------");
			printf("\nSyottamasi summa ei ollut kelvollinen! Syota uusi summa");
			printf("\n-------------------------------------------------------");
			printf("\n>>> ");
		}
	}
}

int varmistaOnkoLuku(char luku[])
{
	/* varmistaOnkoLuku - Tarkistaa sisaltaako valitseMuuSumma funktion kayttajalta kysyma syote kirjaimia.
	Jos sisaltaa niin funktio palauttaa nollan. 

	Parametrit:
	- char luku[]: taulukko, jossa on käyttäjän syöte

	Paluuarvo (int):
	- 1, jos käyttäjän syöte on luku ja 0, jos käyttäjän syöte ei ole luku

	*/

	int i;

	for (i = 0; i < strlen(luku); i++)
	{
		if (!isdigit(luku[i]))
		{
			return 0;
		}

		else if (i == 0 && luku[i] == '0')
		{
			return 0;
		}
	}

	return 1;
}



void lueMerkkijono(char merkkijono[], int pituus)
{
	/* lueMerkkijono - kysyy käyttäjältä merkkijonon ja tallentaa sen taulukkoon.

	Parametrit:
	- char merkkijono[]: taulukko, johon merkkijono tallennetaan
	- int pituus: maksimi pituus merkkijonolle

	Paluuarvo:
	- ei ole

	*/

	fgets( merkkijono, pituus, stdin );

	// Jos merkkijonon lopussa, ennen lopetusmerkkiä on ylimääräinen
	// rivinvaihto korvataan se lopetusmerkillä
	if( merkkijono[ strlen(merkkijono)-1 ] == '\n')
		merkkijono[ strlen(merkkijono)-1 ] = '\0';
	else
		lueRoskat();
}

void lueRoskat()
{
	/* lueRoskat - Tyhjentää puskurin sinne jääneistä merkeistä.

	Parametrit:
	- ei ole

	Paluuarvo:
	-ei ole

	*/

	// Tyhjennetään puskuri
	while( fgetc(stdin) != '\n');
}



void lajitteleRahat(int otto)
{
	/* lajitteleRahat - Lajittelee rahat siten, etta automaatti antaa kayttajalle aina noston
	mahdollisimman suurina seteleina. Tulostaa nostetut setelit komentoikkunaan 

	Parametrit:
	-int otto: rahasumma, jonka käyttäjä nosti automaatista

	Paluuarvo:
	-ei ole

	*/

	int valiaikainenOtto ;
	int laskuri50 = 0;
	int laskuri20 = 0;
	int i;

	valiaikainenOtto = otto;

	while (valiaikainenOtto != 0)
	{
		if (valiaikainenOtto % 50 == 0)
		{
			laskuri50 += 1;
			valiaikainenOtto -= 50;
		}

		else
		{
			laskuri20 += 1;
			valiaikainenOtto -= 20;
		}
	}

	printf("\nOta rahat, ole hyva.\n");

	if (laskuri50 > 0)
	{
		for (i = 0; i < laskuri50; i++)
		{
			printf("___  ___\n");
			printf("|e(50)e|\n");
			printf("********\n");
		}

		printf("\n");
	}

	if (laskuri20 > 0)
	{
		for (i = 0; i < laskuri20; i++)
		{
			printf("___  ___\n");
			printf("|e(20)e|\n");
			printf("********\n");
		}

		printf("\n");
	}
}



void aukaiseTili(double * saldo, char tilinumero[], int * paaSilmukanToistoehto, FILE ** osoitinPolkuun)
{
	/* aukaiseTili - Aukaisee kayttajan syottaman tilin. Tilit ovat tekstitiedostoja, joiden
	nimi on muotoa tilinumero.txt. Tilinumerot ovat viisinumeroisia.
	Kysyy myos kayttajalta PIN-koodin, joka on tekstitiedostossa. Kun PIN-koodi
	on kirjoitettu oikein, haetaan tekstitiedostosta kayttajan saldo ja tallentaa
	sen "saldo" muuttujaan. Halutessaan, käyttäjän voi lopettaa ohjelman syöttämällä
	kirjaimen "s" tai "S". 

	Parametrit:
	- double * saldo: saldo-muuttujan osoite muistissa
	- char tilinumero[]: taulukko, johon tallenetaan käyttäjän tilinumero
	- int * paaSilmukanToistoehto: paaSilmukanToistoehto-muuttujan osoite muistissa
	- FILE ** osoitinPolkuun: osoitinmuuttuja, joka osoittaa polku-muuttujaan pääohjelmassa

	Paluuarvo:
	- ei ole

	*/

	char pinKoodi[SYOTTEEN_PITUUS];
	char tarkistaPinKoodi[SYOTTEEN_PITUUS];
	char saldorivi[SYOTTEEN_PITUUS];

	int silmukanToistoehto = 1;


	while ( silmukanToistoehto )
	{
		printf("\n-----------------");
		printf("\nAnna tilinumerosi");
		printf("\n(\"s\" lopettaa)");
		printf("\n-----------------");
		printf("\n>>> ");

		lueMerkkijono(tilinumero, SYOTTEEN_PITUUS);

		if (strcmp(tilinumero, "s") == 0 || strcmp(tilinumero, "S") == 0)
		{
			* paaSilmukanToistoehto = 0;
			break;
		}

		else if (strlen(tilinumero) != 5)
		{
			printf("\n----------------------");
			printf("\nTilia ei ole olemassa!"); // Tilinumero on 5 numeroa pitka
			printf("\n----------------------\n");
			continue;
		}

		strcat(tilinumero, ".txt");

		if ((*osoitinPolkuun = fopen(tilinumero, "r")) == NULL)
		{
			printf("\n----------------------");
			printf("\nTilia ei ole olemassa!");
			printf("\n----------------------\n");
			continue;
		}


		fgets(tarkistaPinKoodi, SYOTTEEN_PITUUS, *osoitinPolkuun); // Lue PIN-koodi tekstitiedostosta
		tarkistaPinKoodi[strcspn(tarkistaPinKoodi, "\n")] = 0;

		while ( 1 )
		{
			printf("\n-----------------");
			printf("\nSyota PIN-koodisi");
			printf("\n(\"s\" lopettaa)");
			printf("\n-----------------");
			printf("\n>>> ");

			lueMerkkijono(pinKoodi, SYOTTEEN_PITUUS);

			if (strcmp(pinKoodi, "s") == 0 || strcmp(pinKoodi, "S") == 0)
			{
				* paaSilmukanToistoehto = 0;
				silmukanToistoehto = 0;
				break;
			}

			if (strcmp(pinKoodi, tarkistaPinKoodi) != 0)
			{
				printf("\n--------------------");
				printf("\nPIN-koodi on vaarin!");
				printf("\n--------------------\n");
				continue;
			}

			else
			{
				fgets(saldorivi, SYOTTEEN_PITUUS, *osoitinPolkuun);
				printf("\n--------------------");
				printf("\nSaldosi on %s euroa.", saldorivi);
				printf("\n--------------------\n");
				*saldo = atof(saldorivi);
				rewind(*osoitinPolkuun);
				silmukanToistoehto = 0;
				break;
			}
		}
	}


}


int tallennaSaldo(FILE * polku, char tilinumero[], double * saldo)
{

	/* tallennaSaldo - Tallentaa saldon tekstitiedostoon (joka on kayttajan tili) kun
	kayttaja valitsee paavalikosta "Lopeta" vaihtoehdon. Toimii siten, etta
	tekee uuden tiedoston, jonne kopioidaan kayttajan PIN-koodi ja uusi saldo,
	poistaa sitten vanhan tiedoston ja nimeaa uuden tiedoston vanhan tiedoston
	nimella 

	Parametrit:
	- FILE * polku: polku-muuttujan osoite muistissa
	- char tilinumero[]: käyttäjän tilinumero
	- double * saldo: saldo-muuttujan osoite muistissa

	Paluuarvo (int):
	- palauttaa 1, jos saldon tallennus onnistuu ja 0, jos tallennus epäonnistuu

	*/

	FILE * valiaikainenTiedosto;

	char valiaikainenTiedostoNimi[SYOTTEEN_PITUUS];	// Valiaikaisen tiedoston nimi
	char buffer[SYOTTEEN_PITUUS];	// Tallentaa jokaisen korvattavan tiedoston rivin
	char uusiSaldo[SYOTTEEN_PITUUS];	// Sisaltaa uuden saldon, joka tallenetaan valiaikaiseenTiedostoon

	int korvattavaRivi = 2; // Rivi, jossa saldo on tiedostossa
	int tamanhetkinenRivi = 1;

	sprintf(uusiSaldo, "%.2lf", *saldo);
	strcpy(valiaikainenTiedostoNimi, "temp.txt");

	valiaikainenTiedosto = fopen(valiaikainenTiedostoNimi, "w");

	if (valiaikainenTiedosto == NULL)
	{
		printf("\nTapahtumia ei voitu tallentaa!");
		return 0; // Palauttaa nollan, jos uutta tiedostoa ei voi avata
	}


	while ((fgets(buffer, SYOTTEEN_PITUUS, polku)) != NULL)
	{

		if (tamanhetkinenRivi == korvattavaRivi) // Kun tulee saldorivi, korvataan se uudella saldolla uuteen tiedostoon
		{
			fputs(uusiSaldo, valiaikainenTiedosto);
		}

		else
		{
			fputs(buffer, valiaikainenTiedosto); // Lisataan uuteen tiedostoon alkuperaisen tiedoston rivit
		}

		tamanhetkinenRivi += 1; // Siirrytaan vertailemaan alkuperaisen tiedoston seuraavaa rivia
	}


	fclose(polku);
	fclose(valiaikainenTiedosto);

	remove(tilinumero);
	rename(valiaikainenTiedostoNimi, tilinumero);

	return 1;
}
