#pragma once

#include "../Calendario/Calendario.h"

typedef char TipoReferenciaApartamento[9];

typedef struct TipoReserva {
    TipoReferenciaApartamento codigo;
    TipoFecha fecha;
    int duracion;
};

typedef struct TipoNodo {
    TipoFecha fecha; // Se usa para gestionar la reserva

    TipoReserva reserva; // Se usa para confirmar una reserva y recordarlas en el futuro

    TipoNodo* siguiente;
};

typedef TipoNodo* TipoPunteroNodo;

typedef struct TipoLinkedList {
    TipoPunteroNodo cabeza;

    void inicializarLista(TipoLinkedList &lista);              // Inicializa la lista
    void agregarNodo(TipoLinkedList &lista, TipoFecha fecha);  // Agrega un nodo
    void agregarNodoReserva(TipoLinkedList &lista, TipoReserva reserva);
    void imprimirLista(TipoLinkedList &lista);           // Imprime la lista
    void imprimirReservasMes(TipoLinkedList &lista, int mes);
    void liberarLista(TipoLinkedList &lista);                  // Liberar la memoria de la lista
};
