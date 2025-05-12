#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Calendario.h"
#include "../SistemaGesRAE/SistemaGesRAE.h"
#include "../SistemaGesRAE/GestorReservas/GestorReservas.h"

bool Calendario::esBisiesto(int anno) {
    // Si es divisible entre 4
    if (anno % 4 == 0) {
        // Si NO es divisible entre 100 OR es divisible entre 400;
        if ((anno % 100 != 0) || (anno % 400 == 0)) {
            return true;
        }
    }
    return false;
}

bool Calendario::esFechaValida(const TipoFecha fecha) {
    Calendario c;
    GestorReservas r;
    int diasMes;
    diasMes = c.sacarDiasMes(fecha.mes, fecha.anno);

    if (fecha.anno < r.MIN_ANNO_RESERVA) {
        printf("\nEl anno de la fecha escogida es incorrecto, debe ser mayor o igual a %d\n", r.MIN_ANNO_RESERVA);
        return false;
    } else if (fecha.anno > r.MAX_ANNO_RESERVA) {
        printf("\nEl anno de la fecha escogida es incorrecto, debe ser menor o igual a %d\n", r.MAX_ANNO_RESERVA);
        return false;
    }

    if (fecha.mes < 1) {
        printf("\nEl mes de la fecha escogida es incorrecto, debe ser menor o igual a 1\n");
        return false;
    } else if (fecha.mes > 12) {
        printf("\nEl mes de la fecha escogida es incorrecto, debe ser menor o igual a 12\n");
        return false;
    }

    if (fecha.dia < 1) {
        printf("\nEl dia de la fecha escogida es incorrecto, debe ser mayor o igual a 1\n");
        return false;
    } else if (fecha.dia > diasMes) {
        printf("\nEl dia de la fecha escogida es incorrecto, debe ser menor o igual a %d\n", diasMes);
        return false;
    }

    return true;
}

void Calendario::sacarDiaSemanaNum(TipoFecha &fecha) {
    if (fecha.diaSemana == "LU") {
        fecha.diaSemanaNum = 0;
    } else if (fecha.diaSemana == "MA") {
        fecha.diaSemanaNum = 1;
    } else if (fecha.diaSemana == "MI") {
        fecha.diaSemanaNum = 2;
    } else if (fecha.diaSemana == "JU") {
        fecha.diaSemanaNum = 3;
    } else if (fecha.diaSemana == "VI") {
        fecha.diaSemanaNum = 4;
    } else if (fecha.diaSemana == "SA") {
        // Se salta el 5 porque hay el s�mbolo separador
        fecha.diaSemanaNum = 6;
    } else if (fecha.diaSemana == "DO") {
        fecha.diaSemanaNum = 7;
    }
}

int Calendario::sacarDiasMes(int mes, int anno) {
    int diasMes;

    switch(mes) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                diasMes = 31;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                diasMes = 30;
                break;
            case 2:
                diasMes = 28;
                if (esBisiesto(anno)) {
                    diasMes = 29;
                }
                break;
        }

    return diasMes;
}

bool Calendario::pedirFecha(Calendario & c) {
    int mes;
    int diasMes;
    int anno;

    // Pedir mes y a�o
    printf("�Mes (1...12)? ");
    scanf("%d", &mes);

    printf("�Anno (1601...3000)? ");
    scanf("%d", &anno);
    printf("\n\n");

    diasMes = sacarDiasMes(mes, anno);

    if (mes < 1 || mes > 12) {
        return false;
    }
    if (anno < 1601 || anno > 3000) {
        return false;
    }

    c.fechaEscogida.mes = mes;
    c.fechaEscogida.diasMes = diasMes;
    c.fechaEscogida.anno = anno;
    return true;
}

void Calendario::asignarValoresPrimigenios(Calendario & c) {
    // Dia de la semana del 01/01/1601, mediante el cual se har�n comprobaciones
    c.fechaPrimigenia.dia = 1;
    c.fechaPrimigenia.diaSemana = "LU";
    c.fechaPrimigenia.diaSemanaNum = 1;
    c.fechaPrimigenia.mes = 1;
    c.fechaPrimigenia.anno = 1601;

    c.fechaEscogida.dia = 1;

    c.filas = filasCalendario;
    c.columnas = columnasCalendario;
}

void Calendario::inicializarCalendario(TipoMatriz &matriz, int filas, int columnas) {
    for (int i=0; i <= filas-1; i++) {
        for (int j=0; j <= columnas-1; j++) {
            if (j==5) {
                matriz[i][j] = -1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }
}

int Calendario::calcularDiasEntreFechas(TipoFecha fechaEscogida, TipoFecha fechaPrimigenia) {
    // Calcular d�as entre diaPrimigenio y el primero del mes y del a�o escogidos:
    int resultado = 0;
    int diferenciaDias;
    int diferenciaMeses;
    int diferenciaAnnos;
    int bisiestos = 0;

    // Calcular a�os entre las fechas
    diferenciaAnnos = fechaEscogida.anno - fechaPrimigenia.anno;
    resultado = resultado + (diferenciaAnnos * 365);

    // Calcular a�os bisiestos entre las fechas
    for (int i=0; i <= diferenciaAnnos; i++) {
        // Si es bisiesto
        if (esBisiesto(fechaPrimigenia.anno + i)) {
            if (i==diferenciaAnnos && fechaEscogida.mes <= 2) {
            } else {
                bisiestos++;
            }
        }
    }
    // Sumar 1 d�a por cada a�o bisiesto;
    resultado = resultado + bisiestos;
    // printf("Hay %d annos de diferencia y %d bisiestos", diferenciaAnnos, bisiestos);

    // Calcular meses entre las fechas
    diferenciaMeses = fechaEscogida.mes - fechaPrimigenia.mes;
    // printf("\nHay %d meses de diferencia\n", diferenciaMeses);

    // Dependiendo del mes, sumar x d�as.
    for (int i=diferenciaMeses; i >= fechaPrimigenia.mes; i--) {
        switch(i) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                resultado = resultado + 31;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                resultado = resultado + 30;
                break;
            case 2:
                resultado = resultado + 28;
                break;
        }
    }

    // Calcular d�as entre las fechas
    diferenciaDias = fechaEscogida.dia - fechaPrimigenia.dia;
    resultado = resultado + diferenciaDias;

    return resultado;
}

TipoFecha Calendario::sumarDiasFecha(TipoFecha fecha, int diasPorSumar) {
    int diasMes = 0;
    int diasRestados = 0;

    while (diasPorSumar > 0) {
        // printf("\n\nMes actual: %d", fecha.mes);
        diasMes = sacarDiasMes(fecha.mes, fecha.anno);

        if ((fecha.dia + diasPorSumar) > diasMes) {
            // printf("\nLos dias por reservar rebosan el mes");

            // Cambio de mes
            if (fecha.mes < 12) {
                fecha.mes = fecha.mes + 1;
                fecha.dia = 1;

            // Cambio de a�o
            } else {
                fecha.anno = fecha.anno + 1;
                fecha.mes = 1;
                fecha.dia = 1;
            }

            diasPorSumar = diasPorSumar - (diasMes - fecha.dia + 1);
            diasRestados = (diasMes - fecha.dia + 1);
        // Los d�as por sumar no desbordan el mes
        } else {
            fecha.dia = fecha.dia + diasPorSumar;
            diasRestados = diasPorSumar;
            diasPorSumar = 0;
        }

        // printf("\nSe restan %d dias por sumar", diasRestados);
        // printf("\nQuedan %d dias por sumar", diasPorSumar);
        // printf("\nNos quedamos a dia %d/%d/%d", fecha.dia, fecha.mes, fecha.anno);
    }

    return fecha;
}

void Calendario::asignarDiasCalendario(Calendario & c) {
    int indiceDias = 0;
    int anularFilaExtra = 0;
    int diasMes;

    c.fechaEscogida.diasMes = sacarDiasMes(c.fechaEscogida.mes, c.fechaEscogida.anno);

        // Se ajusta el numero de filas del calendario
    if (c.fechaEscogida.diaSemanaNum + c.fechaEscogida.diasMes < 36) {
        anularFilaExtra = -1;
    }

    // Se sit�a la primera fila
    for (int fil=0; fil <= c.filas-1 + anularFilaExtra; fil++) {
        for (int col=0; col <= c.columnas-1; col++) {
            // Se ignora la barra separadora
            if (col == 5) {
            // Se ignoran aquellos espacios donde no existan dias
            } else if (indiceDias < 1 || indiceDias > c.fechaEscogida.diasMes) {
                if (fil == 0 && col == c.fechaEscogida.diaSemanaNum) {
                    c.matriz[fil][col] = 1;
                    indiceDias = 2;
                }

            // Se a�aden los d�as al calendario
            } else {
                c.matriz[fil][col] = indiceDias;
                indiceDias++;
            }

        }
    }
}

void Calendario::imprimirCalendario(const Calendario c) {
    int anularFilaExtra = 0;

    // Se ajusta el numero de filas del calendario
    if (c.fechaEscogida.diaSemanaNum + c.fechaEscogida.diasMes < 37) {
        anularFilaExtra = -1;
    }

    for (int i=0; i <= c.filas-1 + anularFilaExtra; i++) {
        printf("            ");
        for (int j=0; j <= c.columnas-1; j++) {
            // Barras de separaci�n
            if (c.matriz[i][j] == -1) {
                printf(" %c", '|');
            // D�as reservados
            } else if (c.matriz[i][j] == -2) {
                if (j==0) {
                    printf("%2s", "RE");
                } else if (j==6) {
                    printf("%3s", "RE");
                } else {
                    printf("%4s", "RE");
                }
            // D�as vac�os
            } else if (c.matriz[i][j] == 0) {
                if (j==0) {
                    printf("%2c", '.');
                } else if (j==6) {
                    printf("%3c", '.');
                } else {
                    printf("%4c", '.');
                }
            // D�as reales
            } else {
                if (j==0) {
                    printf("%2d", c.matriz[i][j]);
                } else if (j==6) {
                    printf("%3d", c.matriz[i][j]);
                } else {
                    printf("%4d", c.matriz[i][j]);
                }
            }
        }
        printf("\n");
    }
}
