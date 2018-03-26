
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	// kontrola ukazatela
	if(ptrht == NULL){
		return;
	}
	else{
		//vsetky prvky tabulky sa nastavia na NULL
		for(int i = 0; i < MAX_HTSIZE; i++) {
			(*ptrht)[i] = NULL;
		}
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/
tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	tHTItem *item = (*ptrht)[hashCode(key)];
	while(item != NULL){
		//porovnava key a v pridepade zhody vrati item
		if(strcmp(item->key,key) == 0) {
			return item;
		}
		item = item->ptrnext;
	}
	return NULL;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	tHTItem *item = htSearch(ptrht, key);
	//nahradi data ak prvok uz existuje
	if(item != NULL) {
		item->data = data;
	}
	//vlozenie noveho prvku
	else{
		tHTItem *new_item = malloc(sizeof(tHTItem));
		if(new_item == NULL){
			return;
		}
		new_item->key = key;
		new_item->data = data;
		// ak na tom miesto v tabulke uz je niaky prvok
		if((*ptrht)[hashCode(key)] != NULL) {
			new_item->ptrnext = (*ptrht)[hashCode(key)];
		}
		(*ptrht)[hashCode(key)] = new_item;
	}		
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	tHTItem *item = htSearch(ptrht, key);
	//ak sa nasla polozka vracia jej data
	if(item != NULL) {
		return &(item->data);
	}
	return NULL;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	//pointer na prvok pred vymazavanym prvkom
	tHTItem *delete_prev;
	//vymazavany prvok
	tHTItem *delete = (*ptrht)[hashCode(key)];
	if(delete == NULL) {
		return;
	}
	//vyhladavany prvok je prvy prvok tabulky
	if(strcmp(delete->key, key) == 0) {
		(*ptrht)[hashCode(key)] = delete->ptrnext;
	}
	else {
		//hladanie v zozname prvkov a repisanie pointerov
		while(delete != NULL) {
			if(strcmp(delete->key,key) == 0) {
				break;
			}
			delete_prev= delete;
			delete = delete->ptrnext;
		}
		if(delete != NULL) {
			delete_prev->ptrnext = delete->ptrnext;
		}
	}
	free(delete);
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	tHTItem *next_item,*cur_item;
	//prechod celou tabulkou
	for(int i = 0; i < MAX_HTSIZE; i++) {
		cur_item = (*ptrht)[i];
		//prechod vsetkymi prvkami polozky tabulky
		while(cur_item != NULL){
			next_item = cur_item->ptrnext;
			free(cur_item);
			cur_item = next_item;
		}
		(*ptrht)[i] = NULL;
	}
}
