#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../SistemaGesRAE.h"
#include "GestorReservas.h"

bool GestorReservas::recogerDatosReserva(SistemaGesRAE &s, TipoInfoReserva &info) {
    GestorReservas r;
    Calendario c;
    bool existeEdificio = false;

    do {
        printf("    Identificador de Edificio (numero entre 1 y 5) --> ");
        scanf("%d", &info.idEdificio);
        fflush(stdin);
    } while (info.idEdificio < 1 || info.idEdificio > 5);

    for (int i=1; i <= 5; i++) {
        if (s.listaEdificios[i].id == info.idEdificio) {
            existeEdificio = true;
            strcpy(info.nombreEdificio, s.listaEdificios[info.idEdificio].nombre);
        }
    }
    if (!existeEdificio) {
        printf("No existe ningun edificio con el identificador asignado\n");
        return false;
    }

    do {
        printf("    Tipo Apartamento (B/N/L) --> ");
        scanf(" %c", &info.tipoApt);
        fflush(stdin);
        info.tipoApt = tolower(info.tipoApt);
    } while (info.tipoApt != 'b' &&
             info.tipoApt != 'n' &&
             info.tipoApt != 'l'
            );

    do {
        printf("    Fecha Entrada: Dia --> ");
        scanf("%d", &info.fechaEntrada.dia);
        fflush(stdin);

        printf("    Fecha Entrada: Mes --> ");
        scanf("%d", &info.fechaEntrada.mes);
        fflush(stdin);

        printf("    Fecha Entrada: Anno --> ");
        scanf("%d", &info.fechaEntrada.anno);
        fflush(stdin);

    } while(!c.esFechaValida(info.fechaEntrada));

    do {
        printf("    Dias de duracion de la estancia --> ");
        scanf("%d", &info.duracionEstancia);
    } while (info.duracionEstancia < 1 || info.duracionEstancia > r.MAX_DIAS_RESERVA);

    return true;
}

/* Esta funci�n sirve para verificar si un apartamento est� libre de reserva un d�a concreto */
bool GestorReservas::verificarReserva(TipoLinkedList diasReservados, TipoLinkedList diasPorReservar) {
    TipoPunteroNodo cursorReservar, cursorReservados;
    cursorReservar = diasPorReservar.cabeza;
    cursorReservados = diasReservados.cabeza;

    // Se itera la lista de d�as por reservar, asegur�ndose de que ninguno est� ya reservado
    while (cursorReservar != NULL) {
        cursorReservados = diasReservados.cabeza;
        while (cursorReservados != NULL) {
            if ((cursorReservar->fecha.anno == cursorReservados->fecha.anno) &&
                (cursorReservar->fecha.mes == cursorReservados->fecha.mes) &&
                (cursorReservar->fecha.dia == cursorReservados->fecha.dia)) {

                return false;
            }
            cursorReservados = cursorReservados->siguiente;
        }
        cursorReservar = cursorReservar->siguiente;
    }
    return true;
}

bool GestorReservas::procesarReserva(SistemaGesRAE &s, TipoInfoReserva &info, TipoLinkedList diasPorReservar) {
    GestorReservas r;
    bool apartamentoLibre;

    // Variables locales para el acceso:
    int numApts;
    TipoLinkedList diasReservadosApt;

    /// SE PROCESA LA RESERVA ///
    if (info.tipoApt == 'b') {
        numApts = s.listaEdificios[info.idEdificio].numBasicos;
    } else if (info.tipoApt == 'n') {
        numApts = s.listaEdificios[info.idEdificio].numNormales;
    } else if (info.tipoApt == 'l') {
        numApts = s.listaEdificios[info.idEdificio].numLujo;
    }

    // Se itera entre todos los apartamentos
    for (int apartamento=1; apartamento <= numApts; apartamento++) {
        s.sacarLinkedList(s, diasReservadosApt, info.idEdificio, apartamento, info.tipoApt, 'd');

        apartamentoLibre = r.verificarReserva(diasReservadosApt, diasPorReservar);

        if (apartamentoLibre) {

            /// Se a�ade la informaci�n restante a infoReserva ///
            info.idApt = apartamento;
            if (info.tipoApt == 'b') {
                strcpy(info.referenciaApt, s.listaEdificios[info.idEdificio].listaAptsBasicos[apartamento].referencia);
                printf("\nEl apartamento %s esta libre para la fecha indicada", s.listaEdificios[info.idEdificio].listaAptsBasicos[apartamento].referencia);
            } else if (info.tipoApt == 'n') {
                strcpy(info.referenciaApt, s.listaEdificios[info.idEdificio].listaAptsNormales[apartamento].referencia);
                printf("\nEl apartamento %s esta libre para la fecha indicada", s.listaEdificios[info.idEdificio].listaAptsNormales[apartamento].referencia);
            } else if (info.tipoApt == 'l') {
                strcpy(info.referenciaApt, s.listaEdificios[info.idEdificio].listaAptsLujo[apartamento].referencia);
                printf("\nEl apartamento %s esta libre para la fecha indicada", s.listaEdificios[info.idEdificio].listaAptsLujo[apartamento].referencia);
            }
            return true;
        }
    }

    if (!apartamentoLibre) {
        printf("No existe ningun apartamento libre con las condiciones solicitadas en la fecha indicada\n");
        return false;
    }
}

void GestorReservas::imprimirInfoReserva(TipoInfoReserva info, int numReserva) {
    printf("\n\nDatos de la Reserva\n");
    printf("    Numero de la Reserva: %d/%d\n", numReserva, info.fechaEntrada.anno);
    printf("    Edificio: %s (Id = %d)\n", info.nombreEdificio, info.idEdificio);
    printf("    Referencia Apartamento: %s\n", info.referenciaApt);
    printf("    Fecha de Entrada: %d/%d/%d\n", info.fechaEntrada.dia, info.fechaEntrada.mes, info.fechaEntrada.anno);
    printf("    Duracion estancia: %d dias\n", info.duracionEstancia);
    printf("    Fecha de Salida: %d/%d/%d\n", info.fechaSalida.dia, info.fechaSalida.mes, info.fechaSalida.anno);
}

// Esta funci�n coge una reserva de un apt y llena la lista de dias reservados del mismo apartamento con los dias que quedan reservados por dicha reserva
void GestorReservas::conformarArrayDias(TipoLinkedList &diasReservados, TipoFecha fechaEntrada, int duracionEstancia) {
    TipoFecha fecha;
    Calendario c;

    // Se a�aden los dias reservados a diasReservados
    for (int i=0; i <= duracionEstancia-1; i++) {
        fecha = c.sumarDiasFecha(fechaEntrada, i);
        diasReservados.agregarNodo(diasReservados, fecha);
    }
}

void GestorReservas::confirmarReserva(SistemaGesRAE &s, TipoInfoReserva info) {
    GestorReservas r;
    TipoReserva reserva;
    TipoReferenciaApartamento refeAptVisto;

    /// SE PREPARAN LOS DATOS DE LA RESERVA PARA ALMACENARSE EN EL SISTEMA ///
    sprintf(reserva.codigo, "%d/%d", s.reservasRealizadas, info.fechaEntrada.anno);
    reserva.fecha.dia = info.fechaEntrada.dia;
    reserva.fecha.mes = info.fechaEntrada.mes;
    reserva.fecha.anno = info.fechaEntrada.anno;
    reserva.duracion = info.duracionEstancia;

    /// SE AGREGAN LOS DATOS AL SISTEMA ///
    if (info.tipoApt == 'b') {
        // Se almacena la reserva en la lista de reservas del apt:
        s.listaEdificios[info.idEdificio].listaAptsBasicos[info.idApt].reservas.agregarNodoReserva(s.listaEdificios[info.idEdificio].listaAptsBasicos[info.idApt].reservas, reserva);
        // Se almacenan los dias reservados por la reserva en la lista de dias reservados del apt:
        r.conformarArrayDias(s.listaEdificios[info.idEdificio].listaAptsBasicos[info.idApt].diasReservados, reserva.fecha, reserva.duracion);
        // Se actualiza la referencia del apartamento para el mensaje de exito:
        strcpy(refeAptVisto, s.listaEdificios[info.idEdificio].listaAptsBasicos[info.idApt].referencia);

    } else if (info.tipoApt == 'n') {
        s.listaEdificios[info.idEdificio].listaAptsNormales[info.idApt].reservas.agregarNodoReserva(s.listaEdificios[info.idEdificio].listaAptsNormales[info.idApt].reservas, reserva);
        r.conformarArrayDias(s.listaEdificios[info.idEdificio].listaAptsNormales[info.idApt].diasReservados, reserva.fecha, reserva.duracion);
        strcpy(refeAptVisto, s.listaEdificios[info.idEdificio].listaAptsNormales[info.idApt].referencia);
    } else if (info.tipoApt == 'l') {
        s.listaEdificios[info.idEdificio].listaAptsLujo[info.idApt].reservas.agregarNodoReserva(s.listaEdificios[info.idEdificio].listaAptsLujo[info.idApt].reservas, reserva);
        r.conformarArrayDias(s.listaEdificios[info.idEdificio].listaAptsLujo[info.idApt].diasReservados, reserva.fecha, reserva.duracion);
        strcpy(refeAptVisto, s.listaEdificios[info.idEdificio].listaAptsLujo[info.idApt].referencia);
    }

    printf("\n\n    - Se ha realizado una reserva con exito en el apartamento %s\n", refeAptVisto);
}

void GestorReservas::reservarApartamento(SistemaGesRAE &s) {
    GestorReservas r;
    Calendario c;
    TipoInfoReserva info;
    bool resultadoRecogidaDatos;
    bool resultadoReserva;
    bool confirmacion;

    TipoPunteroNodo cursorReservas; // Sirve para almacenar las reservas realizadas con exito
    TipoLinkedList diasPorReservar; // LISTA DIN�MICA QUE ALMACENA EN FORMATO FECHA LOS D�AS POR RESERVAR:
    diasPorReservar.inicializarLista(diasPorReservar);

    printf("\n    - Reservar Apartamento\n\n");
    if (s.mostrarDescripciones) {
        printf("A continuacion se le pediran datos para realizar una reserva en un hotel determinado durante los dias que considere necesarios.\n");
        printf("\nEl sistema le asignara un apartamento automaticamente segun el edificio y el tipo de apartamento indicado, en caso de que resten apartamentos libres de dicho tipo en las fechas indicadas.\n");
        printf("\n\nPor resultado obtendra una hoja con la informacion de la reserva (por ejemplo el apartamento asignado) y un panel de confirmacion.\n\n");
    }

    resultadoRecogidaDatos = recogerDatosReserva(s, info);
    if (!resultadoRecogidaDatos) {
        return;
    }

    /// GUARDA EN UN ARRAY TODOS LOS D�AS EN FORMATO FECHA POR RESERVAR ///
    conformarArrayDias(diasPorReservar, info.fechaEntrada, info.duracionEstancia);
    info.fechaSalida = c.sumarDiasFecha(info.fechaEntrada, info.duracionEstancia);

    /// PROCESAR RESERVA ///
    resultadoReserva = procesarReserva(s, info, diasPorReservar);
    if (!resultadoReserva) {
        return;
    }
    s.reservasRealizadas = s.reservasRealizadas + 1;

    /// SE IMPRIME EL RESULTADO ///
    imprimirInfoReserva(info, s.reservasRealizadas);

    /// CONFIRMAR OPERACI�N ///
    do {
        printf("\n\nEs correcta la operacion? (S/N)\n   --> ");
        scanf(" %c", &confirmacion);

        if (tolower(confirmacion) == 's') {
            r.confirmarReserva(s, info);
        } else if (tolower(confirmacion) == 'n') {
            printf("\nLa reserva ha sido cancelada!\n");
        }
    } while (tolower(confirmacion) != 's' && tolower(confirmacion) != 'n');
}

// Esta funci�n se ejecuta antes de ejecutar la aplicaci�n de los d�as reservados en el calendario.
void GestorReservas::prepararAplicacionDiasReservados(SistemaGesRAE &s, Calendario &c, TipoInfoVerReservas &info, int &totalDiasReservados, int &totalDiasLibres) {
    /// NOMBRE PENDIENTE DE REVISI�N ///
    GestorReservas r;
    TipoPunteroNodo cursorDiasReservados;
    TipoLinkedList diasReservados;

    s.sacarLinkedList(s, diasReservados, info.idEd, info.idApt, info.tipoApt, 'd');

    r.aplicarDiasReservados(c, c.fechaEscogida, s, diasReservados, totalDiasReservados, totalDiasLibres);
}

void GestorReservas::aplicarDiasReservados(Calendario &c, TipoFecha f, SistemaGesRAE s, TipoLinkedList l, int &totalDiasReservados, int &totalDiasLibres) {
    GestorReservas r;
    int dia;
    TipoLinkedList linkedList;

    // printf("\n  -Se ejecuta la funcion de aplicar los dias reservados al calendario\n");

    if (l.cabeza == NULL) {
        printf("\nNo existe ninguna reserva asociada al apartamento\n");
        return;
    }

    // Iteramos sobre la matriz de dias del mes:
    for (int i=0; i <= c.filas-1; i++) {
        for (int j=0; j <= c.columnas-1; j++) {
            if (c.matriz[i][j] < 1) {
                continue;
            }

            dia = c.matriz[i][j];
            f.dia = dia;

            linkedList.inicializarLista(linkedList);
            linkedList.agregarNodo(linkedList, f);

            if (!r.verificarReserva(l, linkedList)) {
                c.matriz[i][j] = -2;
                totalDiasReservados++;
                // printf("\nSe ha cambiado un dia a RE\n\n");
            } else {
                totalDiasLibres++;
            }

            // Se libera la memoria de la linkedList creada
            linkedList.liberarLista(linkedList);
        }
    }
}

bool GestorReservas::estudiarReferencia(SistemaGesRAE s, TipoReferenciaApartamento referencia, int &idEd, int &idApt, char &tipoApt) {
    int numApts;

    /// VERIFICAR LONGITUD DE LA CADENA
    if (strlen(referencia) != 8) {
        printf("\nLa referencia del apartamento tiene una longitud indebida. Se espera un total de 8 caracteres!\n");
        return false;
    }

    /// VERIFICAR EL TIPO DE LOS CARACTERES ///
    if (!(isdigit(referencia[3]) && isdigit(referencia[4]))) {
        printf("\nLos caracteres numero cuatro y cinco deben ser numeros que identifiquen a un edificio!\n");
        return false;
    }
    tipoApt = tolower(referencia[5]);
    if (tipoApt != 'b' && tipoApt!= 'n' && tipoApt != 'l') {
        printf("\nEl caracter numero seis debe corresponder a un tipo valido de apartamento (B,N,L)\n");
    }

    if (!isdigit(referencia[6]) || !isdigit(referencia[7])) {
        printf("\nLos caracteres numero siete y ocho deben ser numeros que identifiquen a un apartamento!\n");
        return false;
    }

    /// VERIFICAR EXISTENCIA DEL EDIFICIO ///
    idEd = (referencia[3] - '0') * 10 + (referencia[4] - '0');
    if (s.listaEdificios[idEd].nombre[0] == '\0') {
        printf("\nEl edificio con id %d seleccionado NO existe!\n", idEd);
        return false;
    }

    /// VERIFICAR EXISTENCIA DEL APARTAMENTO ///
    if (tipoApt == 'b') {
        numApts = s.listaEdificios[idEd].numBasicos;
    } else if (tipoApt == 'n') {
        numApts = s.listaEdificios[idEd].numNormales;
    } else if (tipoApt == 'l') {
        numApts = s.listaEdificios[idEd].numLujo;
    }
    idApt = (referencia[6] - '0') * 10 + (referencia[7] - '0');
    if (numApts < idApt) {
        printf("\nEl apartamento seleccionado de tipo %c con id %d NO existe!\n", tipoApt, idApt);
        return false;
    }

    return true;
}

void GestorReservas::recogerDatosVerReservas(SistemaGesRAE &s, Calendario &c, TipoInfoVerReservas &info) {
    GestorReservas r;
    const TipoReferenciaApartamento constReferenciaApt = "APT";
    TipoReferenciaApartamento referenciaApt;
    TipoReferenciaApartamento referenciaRecogida;

    int idEdExtraida;
    int idAptExtraida;
    char tipoApt;

    int seleccionMes;
    int seleccionAnno;
    TipoFecha fechaComprobacion;

    int diasEntreFechas;

    TipoLinkedList listaReservas;

     do {
        printf("    Referencia Apartamento --> APT");
        strcpy(referenciaApt, constReferenciaApt);

        scanf("%s", &referenciaRecogida);
        fflush(stdin);
        strcat(referenciaApt, referenciaRecogida);
    // Se comprueba que la referencia tenga sentido y se extrae la informaci�n:
    } while (!r.estudiarReferencia(s, referenciaApt, idEdExtraida, idAptExtraida, tipoApt));

    do {
        printf("\n    Seleccion Mes --> ");
        scanf("%d", &seleccionMes);
        fflush(stdin);

        printf("\n    Seleccion Anno --> ");
        scanf("%d", &seleccionAnno);
        fflush(stdin);

        fechaComprobacion.dia = 1;
        fechaComprobacion.mes = seleccionMes;
        fechaComprobacion.anno = seleccionAnno;
    } while(!c.esFechaValida(fechaComprobacion));

    // ACTUALIZAMOS LOS VALORES DE INFO PARA PASARLO FUERA DE LA FUNCI�N
    info.idEd = idEdExtraida;
    info.idApt = idAptExtraida;
    info.seleccionMes = seleccionMes;
    info.seleccionAnno = seleccionAnno;
    info.tipoApt = tipoApt;
}

void GestorReservas::prepararCalendario(Calendario &c, SistemaGesRAE s, TipoInfoVerReservas info) {
    int diasEntreFechas;

    c.asignarValoresPrimigenios(c);
    c.inicializarCalendario(c.matriz, c.filas, c.columnas);

    diasEntreFechas = c.calcularDiasEntreFechas(c.fechaEscogida, c.fechaPrimigenia);

    c.fechaEscogida.diaSemana = arrayDias[(diasEntreFechas % 7)];

    // Se pasa el dia de la semana de cadena a numero
    c.sacarDiaSemanaNum(c.fechaEscogida);

    printf("\n        Estado Mensual Apartamento %s", info.referenciaApt);
    printf("\n        Edificio: %s\n\n", s.listaEdificios[info.idEd].nombre);
    printf("            %-10s%17d\n", arrayMeses[c.fechaEscogida.mes-1], c.fechaEscogida.anno);
    printf("            ===========================\n");
    printf("            LU  MA  MI  JU  VI | SA  DO\n");
    printf("            ===========================\n");
}

void GestorReservas::verReservasApartamento(SistemaGesRAE &s) {
    GestorReservas r;
    Calendario c;
    bool existeApartamento = false;
    bool verOtroMes;
    char opcionUser;
    int totalDiasReservados;
    int totalDiasLibres;

    TipoReferenciaApartamento refeAptVisto;

    TipoLinkedList listaReservas;

    TipoInfoVerReservas info;

    printf("\n    - Reservas Mensuales Apartamento\n\n");
    if (s.mostrarDescripciones) {
        printf("A continuacion se le pediran datos para ver el estado de reservas de un mes concreto de un apartamento determinado.\n");
        printf("\nPor resultado obtendra una hoja de calendario donde se expondran los dias reservados del mes mediante el codigo RE.\n\n");
    }

    r.recogerDatosVerReservas(s, c, info);

    do {
        verOtroMes = false;

        totalDiasReservados = 0;
        totalDiasLibres = 0;

        if (info.tipoApt == 'b') {
            strcpy(refeAptVisto, s.listaEdificios[info.idEd].listaAptsBasicos[info.idApt].referencia);
        } else if (info.tipoApt == 'n') {
            strcpy(refeAptVisto, s.listaEdificios[info.idEd].listaAptsNormales[info.idApt].referencia);
        } else if (info.tipoApt == 'l') {
            strcpy(refeAptVisto, s.listaEdificios[info.idEd].listaAptsLujo[info.idApt].referencia);
        }
        strcpy(info.referenciaApt, refeAptVisto);

        c.fechaEscogida.mes = info.seleccionMes;
        c.fechaEscogida.anno = info.seleccionAnno;
        r.prepararCalendario(c, s, info);

        c.asignarDiasCalendario(c);

        // Esta funci�n coge el calendario del mes escogido y marca los d�as que est�n reservados en el sistema:
        r.prepararAplicacionDiasReservados(s, c, info, totalDiasReservados, totalDiasLibres);

        c.imprimirCalendario(c);

        printf("\n\nReservas de apartamento %s", refeAptVisto);

        s.sacarLinkedList(s, listaReservas, info.idEd, info.idApt, info.tipoApt, 'r');

        listaReservas.imprimirReservasMes(listaReservas, info.seleccionMes);

        printf("\n\nDias reservados del mes: %d", totalDiasReservados);
        printf("\nDias libres del mes: %d\n", totalDiasLibres);

        do {
            printf("\nDesea ver las reservas de otro mes --> (S/N)? ");
            scanf(" %c", &opcionUser);
            fflush(stdin);
        } while (tolower(opcionUser) != 's' && tolower(opcionUser) != 'n');

        if (tolower(opcionUser) == 's') {
            do {
                printf("\nIndique el mes que quiere ver: ");
                scanf("%d", &info.seleccionMes);
                fflush(stdin);
            } while (info.seleccionMes < 1 || info.seleccionMes > 12);

            verOtroMes = true;
        }
    } while (verOtroMes);
}
