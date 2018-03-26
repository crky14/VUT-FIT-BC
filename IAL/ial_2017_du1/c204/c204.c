
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, říjen 2017                                        */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;


/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
	// pomocna premenna typu char
	char tmp;
	// nacitanie znaku na vrchole zasobniku ak zasobnik nie je prazdny
	if(!stackEmpty(s)){
		stackTop(s, &tmp);
	}
	// kym zasobnik nie je prazdny alebo nenajde '(' odstrani znak z vrcholu zasobniku a zapise do vystuneho pola
	while(!stackEmpty(s)) {
		if(tmp != '(') {
			postExpr[*postLen] = tmp;
			(*postLen)++;			
			stackPop(s);	
			stackTop(s, &tmp);
		}
		else {
			// odstrani '(' zo zasobniku a skonci
			stackPop(s);
			return;
		}
	}
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
	char tmp;
	// nacitanie znaku na vrchole zasobniku ak zasobnik nie je prazdny
	if(!stackEmpty(s)) {
		stackTop(s, &tmp);
	}
	// podmienka zlozena zo vsetkych moznosti kedy sa ma operator zapisat na zasobnik
	if((stackEmpty(s)) || (tmp == '(') || (tmp == '+' && c != '-' && c != '+') || (tmp == '-' && c != '-' && c != '+')) {
		stackPush(s, c);
	}
	else {
		postExpr[*postLen] = tmp;	//vyprazdnovanie zasobniku ak je na vrchole znamienko s vascou prioritou
		(*postLen)++;
		stackPop(s);
		doOperation(s, c, postExpr, postLen);	//rekurzivne volanie funkcie
	}
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {

//  solved = 0;                        /* V případě řešení smažte tento řádek! */
//  return NULL;                /* V případě řešení můžete smazat tento řádek. */
	char *post_expr;
	unsigned int idx = 0;
	unsigned int post_len = 0;
	tStack *ptr_stack;

	// vytvorenie zasobnika a jeho inicializacia
	ptr_stack = malloc(sizeof(tStack));
	if(ptr_stack == NULL) {
		return NULL;
	}
	stackInit(ptr_stack);

	// vytvorenie postfixoveho retazca
	post_expr = malloc(sizeof(char) * MAX_LEN); 
	if(post_expr == NULL) {
		return NULL;
	}
	// kontrola prijateho retazca
	if(infExpr == NULL) {
		return NULL;
	}
	// prevod infixoveho retazca znak po znaku az kym nenarazi na '\0'
	while(infExpr[idx] != '\0') {
		//Spracovanie operatora podla funkcie doOperation()
		if(infExpr[idx] == '+' || infExpr[idx] == '-' || infExpr[idx] == '/' || infExpr[idx] == '*') {
			doOperation(ptr_stack, infExpr[idx], post_expr, &post_len);
		}
		//Spracovanie '(' -> na zasobnik
		else if(infExpr[idx] == '(') {
			stackPush(ptr_stack, infExpr[idx]);
		}
		//Spracovanie ')' -> vyprazdnenie zasobniku po najblizsiu '(' pomocou untilLeftPar()
		else if(infExpr[idx] == ')') {
			untilLeftPar(ptr_stack, post_expr, &post_len);
		}
		//Spracovanie '=' pricom je najskor vypisane vsetko zo zasobnika
		else if(infExpr[idx] == '=') {
			while(!stackEmpty(ptr_stack)) {
				stackTop(ptr_stack, &post_expr[post_len]);
				post_len++;
				stackPop(ptr_stack);
			}
			post_expr[post_len] = '=';
			post_len++;
		}
		// spracovanie operandov -> vypisanie na vystupne pole (mozno by bolo treba osetrit vstup podla zadania len na 0..9, a..z, A..Z)
		else{
			post_expr[post_len] = infExpr[idx];
			post_len++;
		}
		idx++;
	}
	//umiestnenie znaku konca riadku na koniec retazcas
	post_expr[post_len] = '\0'; 
	free(ptr_stack);
	return post_expr;

}

/* Konec c204.c */
