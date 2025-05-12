/**************************************
* NOMBRE: #Pau#
* PRIMER APELLIDO: #Culla#
* SEGUNDO APELLIDO: #Loren#
* DNI: #49382362#
* EMAIL: #paucullaloren@gmail.com#
**************************************/

#pragma once
#include "../SistemaGesRAE.h"
#include "../../Calendario/Calendario.h"

typedef struct TipoInfoReserva {
    int idEdificio;
    TipoNombreEdificio nombreEdificio;

    int idApt;
    char tipoApt;
    TipoReferenciaApartamento referenciaApt;

    TipoFecha fechaEntrada;
    TipoFecha fechaSalida;
    int duracionEstancia;
};

typedef struct TipoInfoVerReservas {
    int idEd, idApt;
    int seleccionMes, seleccionAnno;
    char tipoApt;
    TipoReferenciaApartamento referenciaApt;
};

typedef struct GestorReservas {
    const static int MAX_DIAS_RESERVA = 100;
    const static int MIN_ANNO_RESERVA = 2025;
    const static int MAX_ANNO_RESERVA = 2050;

    /// RESERVAR APT ///
    void conformarArrayDias(TipoLinkedList &diasReservados, TipoFecha fechaEntrada, int duracionEstancia);
    bool recogerDatosReserva(SistemaGesRAE &s, TipoInfoReserva &info);
    void formarDiasPorReservar(TipoInfoReserva &info, TipoLinkedList diasPorReservar);
    bool verificarReserva(TipoLinkedList diasReservados, TipoLinkedList diasPorReservar);
    bool procesarReserva(SistemaGesRAE &s, TipoInfoReserva &info, TipoLinkedList diasPorReservar);
    void imprimirInfoReserva(TipoInfoReserva info, int numReserva);
    void confirmarReserva(SistemaGesRAE &s, TipoInfoReserva info);
    void reservarApartamento(SistemaGesRAE &s);

    /// VER RESERVAS APT ///
    bool estudiarReferencia(SistemaGesRAE s, TipoReferenciaApartamento referencia, int &idEd, int &idApt, char &tipoApt);
    void prepararAplicacionDiasReservados(SistemaGesRAE &s, Calendario &c, TipoInfoVerReservas &info, int &totalDiasReservados, int &totalDiasLibres);
    void aplicarDiasReservados(Calendario &c, TipoFecha f, SistemaGesRAE s, TipoLinkedList l, int &totalDiasReservados, int &totalDiasLibres);
    void recogerDatosVerReservas(SistemaGesRAE &s, Calendario &c, TipoInfoVerReservas &info);
    void prepararCalendario(Calendario &c, SistemaGesRAE s, TipoInfoVerReservas info);
    void verReservasApartamento(SistemaGesRAE &s);
};
