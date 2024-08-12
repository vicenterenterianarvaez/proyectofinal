// ConexionBD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h> // Ayuda a usar la autentificacion sql
#include <sql.h> // Ayuda a dar elementos para crear entornos de conexion y para conectarlo
#include <sqlext.h> // Para utilizar la informacion de la BD en c++
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    SQLHENV hEnv; //variable enviroment o entorno
    SQLHDBC hDbc; // conector de la base de datos 
    SQLRETURN ret; // sirve para guardar el ambiente o cuando regresa las consultas

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos. Escribir el nombre del servidor creado en ODBC en la seccion DNS sistema
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"servidor1", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        // la doble barra significa operador logico or, es decir si no
        // se cumple se ejecura el else de linea 86 que dice Fallo la conexion a la base de datos
        cout << "Conectado a la base de datos exitosamente." << endl;

        // Ejemplo de ejecución de una consulta
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Imprimir encabezados de la tabla
        cout << "+-------------+---------------------------+--------------+-------------+----------+-------------------+-----------------------------+-----------+" << endl;
        cout << "| No.Empleado | Nombre                    | F_Nacimiento | RFC         | Centro   | Puesto            | Descripcion                 | Directivo |" << endl;
        cout << "+-------------+---------------------------+--------------+-------------+----------+-------------------+-----------------------------+-----------+" << endl;


        // Ejemplo de consulta SELECT tabla Empleados
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleados", SQL_NTS); //En From escribir nombre de la tabla
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int NumeroEmpleado;
            SQLCHAR Nombre[50];
            SQLCHAR ApellidoPaterno[50];
            SQLCHAR ApellidoMaterno[50];
            SQLDATE FechaNacimiento[12];
            SQLCHAR RFC[12];
            SQLCHAR CentroTrabajo[50];
            SQLCHAR Puesto[50];
            SQLCHAR DescripcionPuesto[255];
            SQLCHAR Directivo[2];

            while (SQLFetch(hStmt) == SQL_SUCCESS) 
            {
                SQLGetData(hStmt, 1, SQL_C_LONG, &NumeroEmpleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, Nombre, sizeof(Nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, ApellidoPaterno, sizeof(ApellidoPaterno), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, ApellidoMaterno, sizeof(ApellidoMaterno), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, FechaNacimiento, sizeof(FechaNacimiento), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, RFC, sizeof(RFC), NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, CentroTrabajo, sizeof(CentroTrabajo), NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, Puesto, sizeof(Puesto), NULL);
                SQLGetData(hStmt, 9, SQL_C_CHAR, DescripcionPuesto, sizeof(DescripcionPuesto), NULL);
                SQLGetData(hStmt, 10, SQL_C_CHAR, Directivo, sizeof(Directivo), NULL);

                // Concatenar nombre y apellido paterno
                string Apellidos = string((char*)ApellidoPaterno) + " " + string((char*)ApellidoMaterno);
                string nom_completo = string((char*)Nombre) + " " + string(Apellidos);
                
                // Cambiar dato binario por si/no
                string dir = string((char*)Directivo);
                string respuesta;
                if (dir == "1") 
                {
                    respuesta = "SI";
                }
                else
                {
                    respuesta = "NO";
                }
               

                // Imprimir datos de la fila con alineación
                cout << "| " << setw(12) << left << NumeroEmpleado
                     << "| " << setw(26) << left << nom_completo
                     << "| " << setw(13) << left << FechaNacimiento
                     << "| " << setw(12) << left << RFC
                     << "| " << setw(9) << left << CentroTrabajo
                     << "| " << setw(18) << left << Puesto
                     << "| " << setw(28) << left << DescripcionPuesto
                     << "| " << setw(9) << left << respuesta << " |" << endl;
            }
        }

        // Liberar el manejador de conexión
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    }

    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    cout << "+-------------+---------------------------+--------------+-------------+----------+-------------------+-----------------------------+-----------+" << endl;

    // Imprimir encabezados de la tabla.
    cout << "+---------------+--------------------+---------------------------+" << endl;
    cout << "| No. Empleado  | Centro Supervisado | Prestacion de Combustible |" << endl;
    cout << "+---------------+--------------------+---------------------------+" << endl;

    SQLHSTMT hStmt;
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);


    // consulta SELECT tabla Directivos
    ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Directivos", SQL_NTS); //En From escribir nombre de la tabla
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        int NumeroEmpleado;
        SQLCHAR NumeroCentroSupervisado[50];
        SQLCHAR PresacionCombustible[2];


        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 1, SQL_C_LONG, &NumeroEmpleado, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, NumeroCentroSupervisado, sizeof(NumeroCentroSupervisado), NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, PresacionCombustible, sizeof(PresacionCombustible), NULL);


            // Cambiar dato binario por si/no
            string com = string((char*)PresacionCombustible);
            string respuesta_c;
            if (com == "1")
            {
                respuesta_c = "SI";
            }
            else
            {
                respuesta_c = "NO";
            }

            cout << "| " << setw(14) << left << NumeroEmpleado
                << "| " << setw(19) << left << NumeroCentroSupervisado
                << "| " << setw(25) << left << respuesta_c << " |" << endl;


        }

        cout << "+---------------+--------------------+---------------------------+" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}