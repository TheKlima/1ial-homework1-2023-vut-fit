// File: c206.c
// Subject: IAL
// Homework: #1
// Author: Andrii Klymenko, FIT VUT
// Login: xklyme00
// Date: 20.12.2023

/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    list->activeElement = list->firstElement;

    // loop through every list element
    while(list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->nextElement;

        // set current list element pointers to NULL
        list->firstElement->nextElement = NULL;
        list->firstElement->previousElement = NULL;

        // free current list element
        free(list->firstElement);

        // update current list element
        list->firstElement = list->activeElement;
    }

    // reinitialize list (list->activeElement is NULL after while loop)
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(struct DLLElement));

    if(new_element == NULL)
    {
        DLL_Error();
        return;
    }

    new_element->data = data;

    new_element->nextElement = list->firstElement;
    new_element->previousElement = NULL;

    // if list is not empty
    if(list->firstElement != NULL)
    {
        list->firstElement->previousElement = new_element;
    }
    else // if list is empty
    {
        list->lastElement = new_element;
    }

    // new element will become first list element
    list->firstElement = new_element;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(struct DLLElement));

    if(new_element == NULL)
    {
        DLL_Error();
        return;
    }

    new_element->data = data;

    new_element->nextElement = NULL;
    new_element->previousElement = list->lastElement;

    // if list is not empty
    if(list->lastElement != NULL)
    {
        list->lastElement->nextElement = new_element;
    }
    else // if list is empty
    {
        list->firstElement = new_element;
    }

    // new element will become last list element
    list->lastElement = new_element;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if(list->firstElement != NULL) // if list is not empty
    {
        *dataPtr = list->firstElement->data;
    }
    else // if list is empty
    {
        DLL_Error();
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    if(list->lastElement != NULL) // if list is not empty
    {
        *dataPtr = list->lastElement->data;
    }
    else // if list is empty
    {
        DLL_Error();
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    if(list->firstElement != NULL) // if list is not empty
    {
        DLLElementPtr firstElement = list->firstElement;

        // if first element was active, the activity is lost
        if(list->activeElement == firstElement)
        {
            list->activeElement = NULL;
        }

        // update list first element
        list->firstElement = firstElement->nextElement;

        // if list had at least two elements
        if(list->firstElement != NULL)
        {
            list->firstElement->previousElement = NULL; // update left ptr of the new list first element
        }
        else // if list had a single element it becomes empty
        {
            list->lastElement = NULL;
        }

        // set pointers of the deleted element to NULL
        firstElement->nextElement = NULL;
        firstElement->previousElement = NULL;

        free(firstElement);
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    if(list->lastElement != NULL) // if list is not empty
    {
        DLLElementPtr last_element = list->lastElement;

        // if last element was active, the activity is lost
        if(list->activeElement == last_element)
        {
            list->activeElement = NULL;
        }

        // update list last element
        list->lastElement = last_element->previousElement;

        // if list had at least two elements
        if(list->lastElement != NULL)
        {
            list->lastElement->nextElement = NULL; // update right ptr of the new list last element
        }
        else // if list had a single element it becomes empty
        {
            list->firstElement = NULL;
        }

        // set pointers of the deleted element to NULL
        last_element->nextElement = NULL;
        last_element->previousElement = NULL;

        free(last_element);
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    // if the list is active and the active element is not the last list element
    if(list->activeElement != NULL && list->activeElement != list->lastElement)
    {
        // element that will be deleted from the list
        DLLElementPtr to_delete = list->activeElement->nextElement;

        // bridging the deleted element
        list->activeElement->nextElement = to_delete->nextElement;

        // if element that will be deleted is not last list element
        if(to_delete != list->lastElement)
        {
            // the element after the deleted one points left to active element
            to_delete->nextElement->previousElement = list->activeElement;
        }
        else // if element that will be deleted is the last list element
        {
            // active element will become last list element
            list->lastElement = list->activeElement;
        }

        // set pointers of the deleted element to NULL
        to_delete->nextElement = NULL;
        to_delete->previousElement = NULL;

        free(to_delete);
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    // if the list is active and the active element is not the first list element
    if(list->activeElement != NULL && list->activeElement != list->firstElement)
    {
        // element that will be deleted from the list
        DLLElementPtr to_delete = list->activeElement->previousElement;

        // bridging the deleted element
        list->activeElement->previousElement = to_delete->previousElement;

        // if element that will be deleted is not the first list element
        if(to_delete != list->firstElement)
        {
            // the element after the deleted one points right to active element
            to_delete->previousElement->nextElement = list->activeElement;
        }
        else // if element that will be deleted is the first element
        {
            // active element will become first list element
            list->firstElement = list->activeElement;
        }

        // set pointers of the deleted element to NULL
        to_delete->nextElement = NULL;
        to_delete->previousElement = NULL;

        free(to_delete);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    if(list->activeElement != NULL) // if list is active
    {
        DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(struct DLLElement));

        if(new_element == NULL)
        {
            DLL_Error();
            return;
        }

        new_element->data = data;

        new_element->nextElement = list->activeElement->nextElement;
        new_element->previousElement = list->activeElement;

        list->activeElement->nextElement = new_element;

        // if active element is not last list element
        if(list->activeElement != list->lastElement)
        {
            new_element->nextElement->previousElement = new_element;
        }
        else // if active element is last list element
        {
            // new element will become last list element
            list->lastElement = new_element;
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    if(list->activeElement != NULL)
    {
        DLLElementPtr new_element = (DLLElementPtr) malloc(sizeof(struct DLLElement));

        if(new_element == NULL)
        {
            DLL_Error();
            return;
        }

        new_element->data = data;

        new_element->nextElement = list->activeElement;
        new_element->previousElement = list->activeElement->previousElement;

        list->activeElement->previousElement = new_element;

        // if active element is not first list element
        if(list->activeElement != list->firstElement)
        {
            new_element->previousElement->nextElement = new_element;
        }
        else // if active element is first list element
        {
            // new element will become first list element
            list->firstElement = new_element;
        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    if(list->activeElement != NULL)
    {
        *dataPtr = list->activeElement->data;
    }
    else
    {
        DLL_Error();
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    if(list->activeElement != NULL)
    {
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    if(list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->nextElement;
    }
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    if(list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return (list->activeElement != NULL);
}

/* Konec c206.c */