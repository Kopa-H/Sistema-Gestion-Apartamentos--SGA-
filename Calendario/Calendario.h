#pragma once

typedef char TipoRepresentacionNumero[2];
typedef char* TipoCadena;
typedef TipoCadena TipoArrayCadena[31];

const TipoArrayCadena arrayMeses = {
    "ENERO", "FEBRERO", "MARZO", "ABRIL", "MAYO", "JUNIO", "JULIO",
    "AGOSTO", "SEPTIEMBRE", "OCTUBRE", "NOVIEMBRE", "DICIEMBRE"
};

const TipoArrayCadena arrayDias = {
    "LU", "MA", "MI", "JU",
    "VI", "SA", "DO"
};

typedef struct TipoFecha {
    int dia;
    TipoCadena diaSemana;
    int diaSemanaNum;
    int mes;
    int diasMes;
    int anno;
};

const int filasCalendario = 6;
const int columnasCalendario = 8;
typedef int TipoMatriz[filasCalendario][columnasCalendario];

typedef struct Calendario {
    TipoMatriz matriz;
    TipoFecha fechaEscogida;
    TipoFecha fechaPrimigenia;

    int filas;
    int columnas;

    bool esBisiesto(int anno);
    bool esFechaValida(const TipoFecha fecha);
    void sacarDiaSemanaNum(TipoFecha &fechaEscogida);
    int sacarDiasMes(int mes, int anno);
    bool pedirFecha(Calendario &c);
    void asignarValoresPrimigenios(Calendario &c);
    void inicializarCalendario(TipoMatriz &matriz, int filas, int columnas);
    int calcularDiasEntreFechas(TipoFecha fechaEscogida, TipoFecha fechaPrimigenia);
    TipoFecha sumarDiasFecha(TipoFecha fecha, int diasPorSumar);
    void asignarDiasCalendario(Calendario &c);
    void imprimirCalendario(const Calendario c);
};
