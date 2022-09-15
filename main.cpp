#include <iostream>
#include <utility>
#include <vector>
#include <fstream>

#define RUTA_ARCHIVO_DESORDENADO "bitacora.txt"
#define RUTA_ARCHIVO_ORDENADO "bitacoraOrdenada1.3-eq8.txt"

class Entrada {
private:
    // Identificador único para cada entrada con formato MM-dd-hh:mm:ss
    std::string id;
    // El texto completo
    std::string mensaje;

    std::string getMesId() const {
        std::string mes{this->mensaje.substr(0, 3)};
        std::string mesId{};

        std::string meses[]{
                "Jan", "Feb", "Mar", "Apr",
                "May", "Jun", "Jul", "Aug",
                "Sep", "Oct", "Nov", "Dec"
        };

        for (int i{}; i < 12; ++i) {
            if (meses[i] == mes) {
                mesId = std::to_string(i + 1);
                break;
            }
        }

        if (mesId.length() == 1) {
            mesId = "0" + mesId;
        }

        return mesId;
    }

    std::string getDiaId() const {
        std::string dia{};
        if (this->mensaje[5] == ' ') {
            dia = "0" + this->mensaje.substr(4, 1);
        }
        else {
            dia = this->mensaje.substr(4, 2);
        }
        return dia;
    }

    std::string getTiempoId() const {
        std::string tiempo{};
        if (this->mensaje[5] == ' ') {
            tiempo = this->mensaje.substr(6, 8);
        }
        else {
            tiempo = this->mensaje.substr(7, 8);
        }
        return tiempo;
    }

public:
    bool operator<(const Entrada& otro) const {
        return (id < otro.getId());
    }

    bool operator>(const Entrada& otro) const {
        return (id > otro.getId());
    }

    bool operator==(const Entrada& otro) const {
        return (id == otro.getId());
    }

    bool operator<=(const Entrada& otro) const {
        return (id <= otro.getId());
    }

    bool operator>=(const Entrada& otro) const {
        return (id >= otro.getId());
    }

    std::string getMensaje() const {
        return mensaje;
    }

    std::string getId() const {
        return id;
    }

    Entrada(std::string mensaje) {
        this->mensaje = std::move(mensaje);
        this->id = this->getMesId() + "-" + this->getDiaId() + "-" + this->getTiempoId();
    }
};

void mezcla(std::vector<Entrada> &baseDeDatos, int inicio, int medio, int final) {
    int cantidadIzquierda = medio - inicio + 1;
    int cantidadDerecha = final - medio;

    std::vector<Entrada> vectorIzquierda;
    std::vector<Entrada> vectorDerecha;

    for (int i = 0; i < cantidadIzquierda; i++) {
        vectorIzquierda.push_back(baseDeDatos[inicio + i]);
    }
    for (int j = 0; j < cantidadDerecha; j++) {
        vectorDerecha.push_back(baseDeDatos[medio + 1 + j]);
    }

    int k = inicio;
    int i = 0;
    int j = 0;

    while (i < cantidadIzquierda && j < cantidadDerecha) {
        if (vectorIzquierda[i] <= vectorDerecha[j]) {
            baseDeDatos[k] = vectorIzquierda[i];
            i++;
        }
        else {
            baseDeDatos[k] = vectorDerecha[j];
            j++;
        }
        k++;
    }

    while (i < cantidadIzquierda) {
        baseDeDatos[k] = vectorIzquierda[i];
        i++;
        k++;
    }
    while (j < cantidadDerecha) {
        baseDeDatos[k] = vectorDerecha[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<Entrada> &baseDeDatos, int inicio, int final) {
    if (inicio < final) {
        int mitad = (final + inicio) / 2;
        mergeSort(baseDeDatos, inicio, mitad);
        mergeSort(baseDeDatos, mitad + 1, final);
        mezcla(baseDeDatos, inicio, mitad, final);
    }
}

void ordenarBaseDeDatos(std::vector<Entrada> &baseDeDatos) {
    mergeSort(baseDeDatos, 0, baseDeDatos.size() - 1);
}

std::vector<Entrada> datosEnRango(const Entrada &fechaInicio, const Entrada &fechaFinal, const std::vector<Entrada> &baseDeDatos) {
    std::vector<Entrada> datos;
    for (const Entrada& entrada: baseDeDatos) {
        if (fechaInicio <= entrada && entrada <= fechaFinal) {
            datos.push_back(entrada);
        }
    }
    return datos;
}

int main() {
    std::ifstream archivoDesordenado(RUTA_ARCHIVO_DESORDENADO);

    if (!archivoDesordenado) {
        std::cerr << "Hubo un error al leer el archivo" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Entrada> baseDeDatos;
    std::string linea;

    while (std::getline(archivoDesordenado, linea)) {
        baseDeDatos.push_back(Entrada(linea));
    }
    archivoDesordenado.close();

    ordenarBaseDeDatos(baseDeDatos);

    std::string fechaDeInicio;
    std::string fechaDeFinal;

    std::cout << "Ingresa la fecha de inicio: ";
    std::getline(std::cin, fechaDeInicio);

    std::cout << "Ingresa la fecha final: ";
    std::getline(std::cin, fechaDeFinal);

    Entrada registroDeInicio(fechaDeInicio + " 00:00:00");
    Entrada registroDeFinal(fechaDeFinal + " 23:59:59");

    std::vector<Entrada> datos = datosEnRango(registroDeInicio, registroDeFinal, baseDeDatos);

    std::ofstream archivoOrdenado(RUTA_ARCHIVO_ORDENADO);

    if (!archivoOrdenado) {
        return EXIT_FAILURE;
    }

    if (!datos.empty()) {
        for (const Entrada &dato: datos) {
            archivoOrdenado << dato.getMensaje() << "\n";
            std::cout << dato.getMensaje() << std::endl;
        }
        archivoOrdenado.close();
    }
    else {
        std::cout << "No se encontraron registros en ese periodo" << std::endl;
    }

    return EXIT_SUCCESS;
}
