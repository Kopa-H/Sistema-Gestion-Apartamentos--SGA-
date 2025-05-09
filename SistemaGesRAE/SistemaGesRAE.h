#pragma once
#include "../Calendario/Calendario.h"
#include "../LinkedList/LinkedList.h"

typedef char TipoBuffer[3];

typedef char TipoReferenciaApartamento[9];

typedef struct TipoApartamento {
    int id;
    TipoReferenciaApartamento referencia;
    char tipo;

    TipoLinkedList reservas; // Almacena la informacion principal de las reservas realizadas
    TipoLinkedList diasReservados; // Almacena todos los d�as reservados del apartamento
};

typedef TipoApartamento TipoListaApartamentos[20];

typedef char TipoNombreEdificio[21];

typedef struct TipoEdificio {
    int id;
    TipoNombreEdificio nombre;
    int numBasicos;
    int numNormales;
    int numLujo;
    int numApts;

    TipoListaApartamentos listaAptsBasicos;
    TipoListaApartamentos listaAptsNormales;
    TipoListaApartamentos listaAptsLujo;
};

typedef TipoEdificio ListaEdificios[5];

typedef struct TipoInfoAptsDisponibles {
    int idEdificio, duracionEstancia;
    TipoFecha fechaEntrada;
};

typedef struct SistemaGesRAE {
    bool mostrarDescripciones;
    int reservasRealizadas;

    int numEdificios;
    ListaEdificios listaEdificios;

    void inicializarSistema(SistemaGesRAE &s);

    void sacarLinkedList(SistemaGesRAE s, TipoLinkedList &linkedList, int idEd, int idApt, char tipoApt, char opcion);

    void addEdificio(SistemaGesRAE &s, TipoEdificio &nuevoEdificio);
    void addApartamento(SistemaGesRAE &s, TipoEdificio &edificio, char tipoApt, int identificador);

    void editarEdificio(SistemaGesRAE &s);
    void listarEdificios(SistemaGesRAE &s);

    void recogerDatosApartamentosDisponibles(SistemaGesRAE s, TipoInfoAptsDisponibles &info);
    void verApartamentosDisponibles(SistemaGesRAE &s);
};
