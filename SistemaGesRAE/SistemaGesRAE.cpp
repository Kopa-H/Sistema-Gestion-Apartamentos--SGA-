#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "SistemaGesRAE.h"
#include "GestorReservas/GestorReservas.h"
#include "../Calendario/Calendario.cpp"
#include "../LinkedList/LinkedList.cpp"

void SistemaGesRAE::inicializarSistema(SistemaGesRAE &s) {
    s.reservasRealizadas = 0;
    s.numEdificios = 0;
    s.mostrarDescripciones = true;

    // Se inicializan los 5 edificios con informaci�n vac�a:
    for (int i=1; i <= 5; i++) {
        s.listaEdificios[i].id = 0;
        strcpy(s.listaEdificios[i].nombre, "");
        s.listaEdificios[i].numBasicos = 0;
        s.listaEdificios[i].numNormales = 0;
        s.listaEdificios[i].numLujo = 0;
    }
    // printf("Sistema inicializado con exito\n");
}

void crearReferenciaApt(TipoEdificio edificio, TipoApartamento &apt) {
    TipoBuffer buffer;
    int numApts;
    TipoBuffer tipoAptMayus;

    if (apt.tipo == 'b') {
        numApts = edificio.numBasicos;
    } else if (apt.tipo == 'n') {
        numApts = edificio.numNormales;
    } else if (apt.tipo == 'l') {
        numApts = edificio.numLujo;
    }

    apt.referencia[0] = '\0';

    // Se a�ade el prefijo APT
    strcat(apt.referencia, "APT");

    // Se a�ade el id del edificio
    if (edificio.id < 10) {
        strcat(apt.referencia, "0");
    }
    sprintf(buffer, "%d", edificio.id);
    strcat(apt.referencia, buffer);

    // Se a�ade el tipo de apartamento
    tipoAptMayus[0] = toupper(apt.tipo);
    tipoAptMayus[1] = '\0';
    strcat(apt.referencia, tipoAptMayus);

    // Se a�ade el id del apartamento
    if (numApts < 10) {
        strcat(apt.referencia, "0");
    }
    sprintf(buffer, "%d", apt.id);
    strcat(apt.referencia, buffer);
}

void SistemaGesRAE::sacarLinkedList(SistemaGesRAE s, TipoLinkedList &linkedList, int idEd, int idApt, char tipoApt, char opcion) {

    // Sacar linkedList de reservas
    if (opcion == 'r') {
        if (tipoApt == 'b') {
            linkedList = s.listaEdificios[idEd].listaAptsBasicos[idApt].reservas;
        } else if (tipoApt == 'n') {
            linkedList = s.listaEdificios[idEd].listaAptsNormales[idApt].reservas;
        } else if (tipoApt == 'l') {
            linkedList = s.listaEdificios[idEd].listaAptsLujo[idApt].reservas;
        }
    // Sacar linkedList de dias reservados
    } else if (opcion == 'd') {
        if (tipoApt == 'b') {
            linkedList = s.listaEdificios[idEd].listaAptsBasicos[idApt].diasReservados;
        } else if (tipoApt == 'n') {
            linkedList = s.listaEdificios[idEd].listaAptsNormales[idApt].diasReservados;
        } else if (tipoApt == 'l') {
            linkedList = s.listaEdificios[idEd].listaAptsLujo[idApt].diasReservados;
        }
    }
}

void SistemaGesRAE::addApartamento(SistemaGesRAE &s, TipoEdificio &edificio, char tipoApt, int identificador) {
    TipoApartamento apt;

    apt.id = identificador; // Los identificadores del sistema van de 1 hacia arriba
    apt.tipo = tipoApt;

    crearReferenciaApt(edificio, apt);

    apt.reservas.inicializarLista(apt.reservas);
    apt.diasReservados.inicializarLista(apt.diasReservados);

    if (tipoApt == 'b') {
        edificio.listaAptsBasicos[identificador] = apt;
    } else if (tipoApt == 'n') {
        edificio.listaAptsNormales[identificador] = apt;
    } else if (tipoApt == 'l') {
        edificio.listaAptsLujo[identificador] = apt;
    }
}

// A esta funci�n ya le llega la informaci�n del edificio, solo la hace efectiva.
void SistemaGesRAE::addEdificio(SistemaGesRAE &s, TipoEdificio &nuevoEdificio) {

    // printf("\nSe incorporan al sistema %d apartamentos basicos", nuevoEdificio.numBasicos);
    for (int i=1; i <= nuevoEdificio.numBasicos; i++) {
        s.addApartamento(s, nuevoEdificio, 'b', i);
    }
    // printf("\nSe incorporan al sistema %d apartamentos normales", nuevoEdificio.numNormales);
    for (int i=1; i <= nuevoEdificio.numBasicos; i++) {
        s.addApartamento(s, nuevoEdificio, 'n', i);
    }
    // printf("\nSe incorporan al sistema %d apartamentos de lujo", nuevoEdificio.numLujo);
    for (int i=1; i <= nuevoEdificio.numBasicos; i++) {
        s.addApartamento(s, nuevoEdificio, 'l', i);
    }

    // Se a�ade finalmente el edificio moldeado
    s.listaEdificios[nuevoEdificio.id] = nuevoEdificio; // Se resta 1 para adaptarlo al sistema intero de 0 a 4
    s.numEdificios = s.numEdificios + 1;

    printf("\n    - Se ha incorporado al sistema el edificio %s", nuevoEdificio.nombre);
}

void SistemaGesRAE::editarEdificio(SistemaGesRAE &s) {
    size_t len;
    char confirmacion;
    bool existiaEdificio = false;

    TipoEdificio nuevoEdificio;
    nuevoEdificio.numApts = 0;

    /// RECIBIMIENTO DE LOS DATOS ///
    printf("\n  - Editar/Crear Edificio\n\n");
    if (s.mostrarDescripciones) {
        printf("Seleccione un identificador de edificio en uso para editarlo.\n");
        printf("\nSeleccione un identificador nuevo para crear un edificio.\n\n");
    }

    do {
        printf("    Identificador de Edificio (numero entre 1 y 5) --> ");
        scanf(" %d", &nuevoEdificio.id);
        fflush(stdin);
    } while (nuevoEdificio.id <= 0 || nuevoEdificio.id > 5);

    do {
        printf("    Nombre (entre 1 y 20 caracteres) --> ");
        scanf("%s", &nuevoEdificio.nombre);
        fflush(stdin);
    } while (strlen(nuevoEdificio.nombre) < 1 || strlen(nuevoEdificio.nombre) > 20);

    do {
        if (nuevoEdificio.numApts > 0) {
            printf("    - El edificio no puede tener mas de 20 apartamentos en total\n\n");
        }

            printf("    Numero de Apartamentos Basicos (0,20) --> ");
            scanf(" %d", &nuevoEdificio.numBasicos);
            fflush(stdin);

            printf("    Numero de Apartamentos Normales (0,20) --> ");
            scanf(" %d", &nuevoEdificio.numNormales);
            fflush(stdin);

            printf("    Numero de Apartamentos de Lujo (0,20) --> ");
            scanf(" %d", &nuevoEdificio.numLujo);
            fflush(stdin);

        nuevoEdificio.numApts = nuevoEdificio.numBasicos + nuevoEdificio.numNormales + nuevoEdificio.numLujo;

    } while (nuevoEdificio.numApts <= 0 || nuevoEdificio.numApts > 20);

    /// PROCESAMIENTO DE LOS DATOS ///
    // Se verifica si el edificio por editar ya exist�a:
    for (int i=1; i <= s.numEdificios; i++) {
        if (s.listaEdificios[i].id == nuevoEdificio.id) {
            existiaEdificio = true;

            // Mensaje de confirmaci�n
            do {
                printf("IMPORTANTE: Esta opcion sobreescribira los datos anteriores.\nDesea continuar? (S/N)\n  --> ");
                scanf(" %c", &confirmacion);
                fflush(stdin);
            } while (confirmacion != 's' && confirmacion != 'S' && confirmacion != 'n' && confirmacion != 'N');

            if (confirmacion == 's' || confirmacion == 'S') {
                s.addEdificio(s, nuevoEdificio);
                printf("Edificio con identificador %d sobreescrito correctamente\n", nuevoEdificio.id);
            } else {
                printf("La edicion del edificio con identificador %d se ha cancelado.", nuevoEdificio.id);
            }
            return;
        }
    }

    // Si el edificio NO exist�a:
    s.addEdificio(s, nuevoEdificio);
}

void printearEspacios(int num) {
    for (int i=0; i < (num); i++) {
        printf(" ");
    }
}

void SistemaGesRAE::listarEdificios(SistemaGesRAE &s) {
    int edificiosExistentes = 0;
    char vacio = ' ';
    char titulo[36] = "Edificios Registrados en el Sistema";
    char id[3] = "Id";
    char nombre[7] = "Nombre";
    char aptsBasicos[13] = "Apts Basicos";
    char aptsNormales[14] = "Apts Normales";
    char aptsLujo[13] = "Apts de Lujo";

    int espacioNombre;

    printf("\n%10c-----------------------------------\n", vacio);
    printf("%45s", titulo);
    printf("\n%10c-----------------------------------\n", vacio);
    printf("%s%3c%s%24s  %s  %s\n\n", id, vacio, nombre, aptsBasicos, aptsNormales, aptsLujo);

    for (int i=0; i <= s.numEdificios; i++) {
        if (s.listaEdificios[i].id > 0) {
            espacioNombre = strlen(s.listaEdificios[i].nombre);

            printf(" %d", s.listaEdificios[i].id);
            printf("%3c%s", vacio, s.listaEdificios[i].nombre);
            printearEspacios(24-espacioNombre);

            printf("%d", s.listaEdificios[i].numBasicos);
            if (s.listaEdificios[i].numBasicos < 10) {
                printearEspacios(13);
            } else {
                printearEspacios(12);
            }
            printf("%d", s.listaEdificios[i].numNormales);
            if (s.listaEdificios[i].numNormales < 10) {
                printearEspacios(14);
            } else {
                printearEspacios(13);
            };
            printf("%d", s.listaEdificios[i].numLujo);
            printf("\n");
            edificiosExistentes++;
        }
    }
    if (edificiosExistentes <= 0) {
        printf("No hay ningun edificio registrado en el sistema.");
    }
}

void SistemaGesRAE::recogerDatosApartamentosDisponibles(SistemaGesRAE s, TipoInfoAptsDisponibles &info) {
    GestorReservas r;
    Calendario c;
    bool existeEdificio;

    do {
        printf("    Identificador de Edificio (numero entre 1 y 5) --> ");
        scanf("%d", &info.idEdificio);
    } while (info.idEdificio < 1 || info.idEdificio > 5);

    for (int i=1; i <= s.numEdificios; i++) {
        if (listaEdificios[i].id == info.idEdificio) {
            existeEdificio = true;
        }
    }
    if (!existeEdificio) {
        printf("No existe ningun edificio con el identificador asignado\n");
        return;
    }

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
    } while (!c.esFechaValida(info.fechaEntrada));

    do {
        printf("    Dias de duracion de la estancia --> ");
        scanf("%d", &info.duracionEstancia);
    } while (info.duracionEstancia < 1 || info.duracionEstancia > 31);
}

void SistemaGesRAE::verApartamentosDisponibles(SistemaGesRAE &s) {
    GestorReservas r;
    TipoInfoAptsDisponibles info;
    char tipoApt;
    TipoLinkedList diasReservados, diasPorComprobar;
    int numApts;

    int aptsBasicosLibres = 0;
    int aptsNormalesLibres = 0;
    int aptsLujoLibres = 0;

    int aptsBasicosOcupados = 0;
    int aptsNormalesOcupados = 0;
    int aptsLujoOcupados = 0;

    bool apartamentoLibre;

    printf("\n    - Mes Reservas Apartamento\n\n");
    if (s.mostrarDescripciones) {
        printf("A continuacion se le pediran datos para ver las reservas existentes durante un mes y anno concreto de un edificio determinado.\n");
        printf("\nPor resultado obtendra una lista de los apartamentos libres en la fecha indicada\n\n");
    }

    recogerDatosApartamentosDisponibles(s, info);

    // Se itera entre los tres posibles tipos de apartamento:
    for (int i=1; i <= 3; i++) {
        if (i==1) {
            tipoApt = 'b';
            numApts = s.listaEdificios[info.idEdificio].numBasicos;
        } else if (i==2) {
            tipoApt = 'n';
            numApts = s.listaEdificios[info.idEdificio].numNormales;
        } else if (i==3) {
            tipoApt = 'l';
            numApts = s.listaEdificios[info.idEdificio].numLujo;
        }

        // Se itera entre todos los apartamentos
        for (int apartamento=1; apartamento <= numApts; apartamento++) {
            // Se saca la linked list de los dias reservados del apartamento:
            s.sacarLinkedList(s, diasReservados, info.idEdificio, apartamento, tipoApt, 'd');

            // Se crea el array de dias que deben comprobarse si estan libres
            diasPorComprobar.inicializarLista(diasPorComprobar);
            r.conformarArrayDias(diasPorComprobar, info.fechaEntrada, info.duracionEstancia);

            apartamentoLibre = r.verificarReserva(diasReservados, diasPorComprobar);
            // printf("\nEstado del apartamento iterado (1=libre): %d\n", apartamentoLibre);

            if (apartamentoLibre) {
                if (tipoApt == 'b') {aptsBasicosLibres++;}
                else if (tipoApt == 'n') {aptsNormalesLibres++;}
                else if (tipoApt == 'l') {aptsLujoLibres++;}
            } else {
                if (tipoApt == 'b') {aptsBasicosOcupados++;}
                else if (tipoApt == 'n') {aptsNormalesOcupados++;}
                else if (tipoApt == 'l') {aptsLujoOcupados++;}
            }
        }
    }

    printf("\nEl edificio %s desde el %d/%d/%d con %d dias de estancia, tendria disponibles:\n\n", s.listaEdificios[info.idEdificio].nombre, info.fechaEntrada.dia, info.fechaEntrada.mes, info.fechaEntrada.anno, info.duracionEstancia);
    printf("    %d apartamentos de tipo Basico libres (%d ocupados)\n", aptsBasicosLibres, aptsBasicosOcupados);
    printf("    %d apartamentos de tipo Normal (%d ocupados)\n", aptsNormalesLibres, aptsNormalesOcupados);
    printf("    %d apartamentos de tipo Lujo (%d ocupados)", aptsLujoLibres, aptsLujoOcupados);
}
