#include <stdio.h>
#include <string.h>

#include "SistemaGesRAE/SistemaGesRAE.h"
#include "SistemaGesRAE/SistemaGesRAE.cpp"

#include "SistemaGesRAE/GestorReservas/GestorReservas.h"
#include "SistemaGesRAE/GestorReservas/GestorReservas.cpp"

void mostrarPresentacion(SistemaGesRAE &s) {
    GestorReservas r;
    char proceso;
    bool ejecucion = true;

    do {
        printf("\n\n==================================================");
        printf("\nGesRAE: Gestion de Reservas Apartamentos-Edificios\n");
        printf("Opciones disponibles:\n\n");
        printf("    Crear/Editar Edificio             (Enter C/E)\n");
        printf("    Listar Edificios                  (Enter   L)\n");
        printf("                                                 \n");
        printf("    Reservar Apartamento              (Enter   R)\n");
        printf("    Disponibilidad Apartamentos       (Enter   D)\n");
        printf("    Mes Reservas Apartamento          (Enter   M)\n");
        printf("                                                 \n");
        printf("    Quitar Descripciones              (Enter   Q)\n");
        printf("    Salir                             (Enter   S)\n");
        printf("==================================================\n");
        printf("\nIntroduzca una opcion valida (C|L|D|R|M|Q|S)\n    --> ");

        scanf(" %c", &proceso);

        switch (proceso) {
            case 'C':
            case 'c':
            case 'E':
            case 'e':
                s.editarEdificio(s);
                break;
            case 'L':
            case 'l':
                s.listarEdificios(s);
                break;
            case 'R':
            case 'r':
                r.reservarApartamento(s);
                break;
            case 'D':
            case 'd':
                s.verApartamentosDisponibles(s);
                break;
            case 'M':
            case 'm':
                r.verReservasApartamento(s);
                break;
            case 'Q':
            case 'q':
                printf("\n  - Las descripciones han sido desactivadas");
                s.mostrarDescripciones = false;
                break;
            case 'S':
            case 's':
                printf("\n  - Saliendo del sistema. Tenga un buen dia\n");
                ejecucion = false;
                break;
            default:
                printf("    - Accion no reconocida. Use un comando soportado (C|L|D|R|M|Q|S)\n  --> ");
                ejecucion = true;
        }
    } while (ejecucion);

}

int main() {
    SistemaGesRAE s;
    GestorReservas r;
    TipoEdificio edificio;
    TipoApartamento apt;
    TipoInfoReserva infoR;

    s.inicializarSistema(s);

    /// SE A�ADE UN EDIFICIO DE PRUEBA ///
    edificio.id = 1;
    strcpy(edificio.nombre, "Apolo");
    edificio.numBasicos = 5;
    edificio.numNormales = 5;
    edificio.numLujo = 5;
    edificio.numApts = 15;
    s.addEdificio(s, edificio);
    /// ------------------------------ ///

    /// SE A�ADE UN EDIFICIO DE PRUEBA ///
    edificio.id = 2;
    strcpy(edificio.nombre, "Hotel Vela");
    edificio.numBasicos = 5;
    edificio.numNormales = 5;
    edificio.numLujo = 5;
    edificio.numApts = 15;
    s.addEdificio(s, edificio);
    /// ------------------------------ ///

    mostrarPresentacion(s);
}
