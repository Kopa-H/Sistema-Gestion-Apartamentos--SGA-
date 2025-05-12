#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

void TipoLinkedList::inicializarLista(TipoLinkedList &lista) {
    lista.cabeza = NULL;
}

void TipoLinkedList::agregarNodo(TipoLinkedList &lista, TipoFecha fecha) {
    TipoPunteroNodo cursor = lista.cabeza;

    TipoPunteroNodo nuevoNodo = new TipoNodo;
    nuevoNodo->fecha = fecha;
    nuevoNodo->siguiente = NULL;

    if (cursor == NULL) {
        lista.cabeza = nuevoNodo;
    } else {
        while (cursor->siguiente != NULL) {
            cursor = cursor->siguiente;
        }

        // A�adimos el nuevo nodo al final
        cursor->siguiente = nuevoNodo;
    }
}

void TipoLinkedList::agregarNodoReserva(TipoLinkedList &lista, TipoReserva reserva) {
    TipoPunteroNodo cursor = lista.cabeza;

    TipoPunteroNodo nuevoNodo = new TipoNodo;
    strcpy(nuevoNodo->reserva.codigo, reserva.codigo);
    nuevoNodo->reserva.fecha = reserva.fecha;
    nuevoNodo->reserva.duracion = reserva.duracion;
    nuevoNodo->siguiente = NULL;

    if (cursor == NULL) {
        lista.cabeza = nuevoNodo;
    } else {
        while (cursor->siguiente != NULL) {
            cursor = cursor->siguiente;
        }

        // A�adimos el nuevo nodo al final
        cursor->siguiente = nuevoNodo;
    }
}

void TipoLinkedList::imprimirLista(TipoLinkedList &lista) {
    TipoPunteroNodo cursor = lista.cabeza;
    int i = 1;

    if (cursor == NULL) {
        printf("\nLa lista esta vacia!");
        return;
    }

    while (cursor != NULL) {
        printf("\n%d. Fecha: %d/%d/%d", i, cursor->fecha.dia, cursor->fecha.mes, cursor->fecha.anno);
        cursor = cursor->siguiente;
        i++;
    }
    printf("\n\n");
}

void TipoLinkedList::imprimirReservasMes(TipoLinkedList &lista, int mes) {
    Calendario c;
    TipoFecha fechaIterada;
    bool impresa; // Se usa para no imprimir m�ltiples veces la misma reserva

    TipoPunteroNodo cursor = lista.cabeza;

    if (cursor == NULL) {
        printf("\nLa lista por imprimir esta completamente vacia!");
        return;
    }

    while (cursor != NULL) {
        impresa = false;
        if (cursor->reserva.fecha.mes == mes) {
            printf("\n  - Reserva %s: Fecha Entrada %d/%d/%d de %d dias",
                cursor->reserva.codigo, cursor->reserva.fecha.dia, cursor->reserva.fecha.mes,
                cursor->reserva.fecha.anno, cursor->reserva.duracion
            );
            impresa = true;
        }

        // Si la fecha de inicio no est� en el mes, verificamos los d�as siguientes:
        if (!impresa) {
            for (int i=1; i < cursor->reserva.duracion; i++) {
                fechaIterada = c.sumarDiasFecha(cursor->reserva.fecha, i);
                if (fechaIterada.mes == mes) {
                    printf("\nReserva %s: Fecha Entrada %d/%d/%d de %d dias",
                        cursor->reserva.codigo, cursor->reserva.fecha.dia, cursor->reserva.fecha.mes,
                        cursor->reserva.fecha.anno, cursor->reserva.duracion
                    );
                    impresa = true;
                }
            }
        }



        cursor = cursor->siguiente;
    }
}

void TipoLinkedList::liberarLista(TipoLinkedList &lista) {
    TipoPunteroNodo cursor = lista.cabeza;
    TipoPunteroNodo temp;

    while (cursor != NULL) {
        temp = cursor;
        cursor = cursor->siguiente;
        delete temp;
    }
    lista.cabeza = NULL;
}
