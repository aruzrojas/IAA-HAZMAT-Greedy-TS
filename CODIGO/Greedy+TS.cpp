#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <tuple>

using namespace std;

int Seed;

void Capture_Params(int argc, char **argv) {
    Seed = atoi(argv[1]);
}


float float_rand(float a, float b) {
    float retorno = 0;

    if (a < b) {
        retorno = (float) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (float) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}


bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void tokenize(std::string const &str, const char delim,   //SEPARA LINEA DE TEXTO POR ESPACIOS
            std::vector<std::string> &out)
{
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

struct Nodo {
    string tipo_material;
    int posicion;   //posicion dentro de la matriz (en que fila se encuentra)
    int carga;

};

struct Camion{
    int capacidad;
    int nodo_actual;
    string mat_dom;
    string ruta;
    vector<string> cargados;
    vector<int> nodos;
};

int letra_to_pos(string letra, vector<string> materiales){
        unsigned co;
        for (co = 0; co < materiales.size(); co++){
            if (materiales[co] == letra){
                return co;
            }
        }
        return -1;
}

char compatible(string& m, string& r, vector<string> materiales){   //STRING M ES EL QUE TIENE, STRING R ES EL QUE QUIERE LLEVAR
    vector<vector<char>> COM{ {'A', '-', 'C', 'D', '-' }, 
                        {'-', 'B', 'C', 'D', 'E' }, 
                        {'C', 'C', 'C', '-', 'E' },
                        {'D', 'D', '-', 'D', 'E' },
                        {'-', 'E', 'E', 'E', 'E' }};     

    int posi = letra_to_pos(m, materiales);
    int posj = letra_to_pos(r, materiales);
    return COM[posi][posj];
}

int compatible_todos(vector<string> cargados, string & r, vector<string> materiales){
    int pos = letra_to_pos(r, materiales); //POSICION DEL MATERIAL r
    vector<vector<char>> COM{ {'A', '-', 'C', 'D', '-' }, 
                        {'-', 'B', 'C', 'D', 'E' }, 
                        {'C', 'C', 'C', '-', 'E' },
                        {'D', 'D', '-', 'D', 'E' },
                        {'-', 'E', 'E', 'E', 'E' }};     
    unsigned i;
    int pos_c;
    char guion = '-';

    for (i = 0; i < cargados.size(); i++){
        pos_c = letra_to_pos(cargados[i],materiales);
        if (COM[pos_c][pos] == guion){
            return 0;
        }
    }
    return 1;
}

char actualizar_dominante(vector<string> cargados, vector<string> materiales){
   vector<vector<char>> COM{ {'A', '-', 'C', 'D', '-' }, 
                    {'-', 'B', 'C', 'D', 'E' }, 
                    {'C', 'C', 'C', '-', 'E' },
                    {'D', 'D', '-', 'D', 'E' },
                    {'-', 'E', 'E', 'E', 'E' }};

    int dom = letra_to_pos(cargados[0],materiales);  

    char m_dom = COM[dom][dom];
    unsigned i;
    int posi;
    for(i = 0; i < cargados.size(); i++){
        if (m_dom == '-'){
            return '-';
        }
            posi = letra_to_pos(cargados[i],materiales);
            m_dom = COM[dom][posi];
            dom = letra_to_pos(std::string(1, m_dom),materiales);
    }
    return m_dom;

}

int obtener_minimo(vector<float> costos, vector<Nodo> Nodos){ 

    float min = 99999999.0;
    int i;
    int posi;

    for (i = 0; i < costos.size(); i++){
        if (costos[i] < min && Nodos[i].carga != 0){

            min = costos[i];
            posi = i;
        }
    }
    return posi;

}

int obtener_minimo_compatibles(vector<float> costos, vector<Nodo> Nodos, string &dom, vector<string> materiales, vector<string> cargados){

    float min = 99999999.0;
    int i;
    int posi;
    int pos_dom = letra_to_pos(dom, materiales);
    int pos_nodo;
    vector<vector<char>> COM{ {'A', '-', 'C', 'D', '-' }, 
                    {'-', 'B', 'C', 'D', 'E' }, 
                    {'C', 'C', 'C', '-', 'E' },
                    {'D', 'D', '-', 'D', 'E' },
                    {'-', 'E', 'E', 'E', 'E' }};
    char guion = '-';                    
    int no_entra = 1;
    for (i = 0; i < costos.size(); i++){
        if (Nodos[i].tipo_material != "-"){
            pos_nodo = letra_to_pos(Nodos[i].tipo_material,materiales);   
        }
        if (costos[i] < min && Nodos[i].carga != 0 && COM[pos_dom][pos_nodo] != guion &&
            costos[i] != 0 && compatible_todos(cargados, Nodos[i].tipo_material, materiales) == 1
            && Nodos[i].tipo_material != "-"){
            min = costos[i];
            posi = i;
            no_entra = 0;
        }

    }
    if (no_entra == 1){
        return -1;
    }
    return posi;

}



int main (int argc, char** argv){

    int i_alfa;
    int i_prob;
    float alpha;
    unsigned i_arc;
    int maxlistatabu;
    int max_its;
    fstream file;  
    string nombre_archivo;       
    vector<float> probs {-1, 2, 0.5};           
    file.open("output.txt",ios::out);
    alpha = atof(argv[2]);
    maxlistatabu = atof(argv[3]);
    max_its = atoi(argv[4]);
    nombre_archivo = argv[5];

    file << "Archivo: " << nombre_archivo << endl;
    file << endl;
    file << "Valor de alfa = "<< alpha << endl;
           
    cout << "Valor de la semilla: " << argv[1] << endl;
    cout << "Valor de alfa: " << alpha << endl;
    cout << "Largo máximo de la lista tabu: " << maxlistatabu << endl;
    cout << "Cantidad máxima de iteraciones: " << max_its << endl;
    cout << "Nombre del archivo: " << nombre_archivo << endl; 

    cout << endl << "Ejecutando... " << endl;

 
    for(i_prob = 0; i_prob < 3; i_prob++){
        vector<string> q[100];
        string line;
        ifstream myfile;
        myfile.open(nombre_archivo);
        vector<float> c_vacio;
        vector<float> capacidades;
        vector<string> materiales;
        vector<Nodo> Nodos;
        vector<Camion> Camiones;
        int n_camiones;
        int n_nodos;
        int cont = -1;
        int i, j = 0;
        int n_materiales = 0;
        if(!myfile.is_open()) {
          perror("Error open");
          exit(EXIT_FAILURE);
        }
        while(getline(myfile, line)) {
            cont = cont + 1;
            if (cont == 0){ 
                n_camiones = std::stoi(line);     //cantidad de camiones

            }
            else if (cont == 1){
                i = 0;
                std::vector<std::string> out;
                tokenize(line, ' ', out);
                for (auto &line: out) {
                   capacidades.push_back(std::stoi(line));
                }
                i++;

            }

            else if (cont == 2){
                n_nodos = std::stoi(line);   //cantidad de nodos
                
            
            }
            else if (cont == 3){ 
                i = 0;
                std::vector<std::string> out;
                tokenize(line, ' ', out);
                for (auto &line: out) {
                   q[i].push_back(line);
                }
                i++;
                cont = cont + 1;
                while (getline(myfile,line) && cont != n_nodos + 3){ 

                    cont = cont + 1;
                    
                    j = 0;

                    std::vector<std::string> out;
                    tokenize(line, ' ', out);
                    for (auto &line: out) {
                        q[i].push_back(line);

                        j = j + 1;
                        if (j == 3){
                               
                            if (std::find(materiales.begin(), materiales.end(), line) != materiales.end() ){
                                continue;
                            }
                            else{
                                materiales.push_back(line);
                                n_materiales = n_materiales + 1;
                            }
                        }
                    }
                    i++;
                    
                }
            }
            
            if(cont == n_nodos + 3){
                i = 0;
                std::vector<std::string> out;
                tokenize(line, ' ', out);
                for (auto &line: out) {
                   c_vacio.push_back(std::stof(line));
                }
                i++;
                break;
            }
        }
        float matrices[n_materiales][n_nodos][n_nodos];
        float distancias[n_materiales][n_nodos][n_nodos];
        float riesgos[n_materiales][n_nodos][n_nodos];
        vector<int> visitados;
        for (j = 0; j < n_nodos; j++){
            visitados.push_back(0);  
        }

        int fila = 0;
        int columna = 0;
        int n_matriz = 0;
        while (getline(myfile, line) ){ 

            if(fila < n_nodos){          //VA DE 0 A 11
                columna = 0;
                std::vector<std::string> out;
                tokenize(line, ' ', out);
                for (auto &line: out) {
                    matrices[n_matriz][fila][columna] = (1-alpha)*std::stoi(line); //distancia
                    distancias[n_matriz][fila][columna] = std::stoi(line);
                    columna = columna + 1;     //actualizar columna
                }            

            }
            fila = fila + 1;
            if (n_matriz == n_materiales - 1 && fila == n_nodos && columna == n_nodos ){
                break;
            }
            if(fila == n_nodos){
                fila = 0;
                columna = 0;
                n_matriz = n_matriz + 1;
            } 

        }

        fila = 0;
        columna = 0;
        n_matriz = 0;
        while (getline(myfile, line) ){ 

            if(fila < n_nodos){
                columna = 0;
                std::vector<std::string> out;
                tokenize(line, ' ', out);
                for (auto &line: out) {
                    matrices[n_matriz][fila][columna] = matrices[n_matriz][fila][columna] + alpha*std::stoi(line); //riesgo
                    riesgos[n_matriz][fila][columna] = std::stoi(line);
                    columna = columna + 1;     //actualizar columna
                }            

            }
            fila = fila + 1;
            if (n_matriz == n_materiales - 1 && fila == n_nodos && columna == n_nodos ){
                break;
            }
            if(fila == n_nodos){
                fila = 0;
                columna = 0;
                n_matriz = n_matriz + 1;
            }


        }

        int smallest = 99999;
        int second = 99999;
        int pos = 0;

        i = 0;
        sort(materiales.begin(), materiales.end()); //ORDENAR TIPO MATERIALES, COMO EN LA LA VARIABLE MATRIZ

        char guion = '-';
        for (auto it = c_vacio.begin(); 
            it != c_vacio.end(); it++) { 
            if (*it <= smallest){
                second = smallest;
                smallest = *it;
            }
            else if (*it < second){
                pos = i;
                second = *it;
            }
            i++;
        }                  //SE OBTIENE EL NODO MAS CERCANO PARA EL PRIMER CAMION (pos)


        vector<float> rutas[n_camiones];

        for (j = 0; j < n_nodos; j++){
            Nodos.push_back(Nodo());
            Nodos[j].tipo_material = q[j][2];
            Nodos[j].posicion = j;
            Nodos[j].carga = std::stoi(q[j][1]);
        }

        for (j = 0; j < n_camiones; j++){
            Camiones.push_back(Camion());
            Camiones[j].capacidad = (capacidades[j]);
            Camiones[j].nodo_actual = 0;
            Camiones[j].ruta = "Deposito";
        }
        int cantidad = 0;

        for (i = 0; i < n_nodos; i++){
            cantidad = cantidad + Nodos[i].carga;
        };
        int pos_letra_solucion;
        int flag;
        float minimo = 99999;
        float Z = 0;
        float Z_distancias = 0;
        float Z_riesgos = 0;
        int iteraciones = 0;
        j = 0;

        int i_min;
        int posi;
        vector<float> vc_vacio;
        vector<float> vect;
        for (i = 0; i < n_nodos; i++){
            vc_vacio.push_back(c_vacio[i]);
        }
        int iters = 0;

        /* ALGORITMO GREEDY*/
        /* A CADA NODO SE LE ASIGNA UNA RUTA CUMPLIENDO LAS RESTRICCIONES
            Y CON FUNCION MIOPE EL MENOR COSTO ASOCIADO
        */

        while(cantidad > 0 && iters != 30){
            for (i = 0; i < n_camiones; i++){
                if (Camiones[i].nodo_actual == 0){
                    posi = obtener_minimo(vc_vacio,Nodos);
                    Camiones[i].capacidad = Camiones[i].capacidad - Nodos[posi].carga;
                    Camiones[i].cargados.push_back(Nodos[posi].tipo_material);
                    Camiones[i].nodos.push_back(Nodos[posi].posicion);
                    pos_letra_solucion = letra_to_pos(Nodos[posi].tipo_material,materiales);
                    Camiones[i].nodo_actual = posi;
                    Camiones[i].ruta = Camiones[i].ruta + " - " + std::to_string(posi);
                    cantidad = cantidad - Nodos[posi].carga;
                    Z = Z + (1 - alpha) * c_vacio[posi];
                    Z_distancias = Z_distancias + c_vacio[posi];
                    Camiones[i].mat_dom = Nodos[posi].tipo_material;
                    Nodos[posi].carga = 0;
                }
                else{
                    vect.clear();
                    for(j = 0; j < n_nodos; j++){
                        pos_letra_solucion = letra_to_pos(Camiones[i].mat_dom, materiales);
                        vect.push_back(matrices[pos_letra_solucion][Camiones[i].nodo_actual][j]);    
                        
                        
                    }

                    posi = obtener_minimo_compatibles(vect, Nodos,Camiones[i].mat_dom,materiales, Camiones[i].cargados);
                    for (i_min = 0; i_min < vect.size(); i_min++){
                    }
                    if (posi != -1){
                        Camiones[i].capacidad = Camiones[i].capacidad - Nodos[posi].carga;
                        Camiones[i].cargados.push_back(Nodos[posi].tipo_material);
                        Camiones[i].nodos.push_back(Nodos[posi].posicion);
                        pos_letra_solucion = letra_to_pos(Nodos[posi].tipo_material,materiales);

                        Z = Z + matrices[pos_letra_solucion][Camiones[i].nodo_actual][posi];  
                        Z_distancias = Z_distancias + distancias[pos_letra_solucion][Camiones[i].nodo_actual][posi];
                        Z_riesgos = Z_riesgos + riesgos[pos_letra_solucion][Camiones[i].nodo_actual][posi]; 

                        Camiones[i].nodo_actual = posi;
                        Camiones[i].ruta = Camiones[i].ruta + " - " + std::to_string(posi);
                        cantidad = cantidad - Nodos[posi].carga;
                        Camiones[i].mat_dom = Nodos[posi].tipo_material;
                        Nodos[posi].carga = 0;                    
                    }

                }
            }
            iters ++;
        }
        /*SI HAY NODOS SIN DESCARGAR, ES DECIR, CANTIDAD DISTINTO DE 0,
            SE DEBEN REINICIAR LOS DATOS Y UTILIZAR GREEDY, EN DONDE 
            POR CADA CAMION SE VAN ASIGNANDO NODOS, A DIFERENCIA DEL 
            ALGORITMO GREEDY DE ARRIBA QUE CADA NODO ERA ASIGNADO A UN 
            CAMION*/
        if (cantidad != 0){
            Z = 0;
            Z_distancias = 0;
            Z_riesgos = 0;
            cantidad = 0;
            for (i = 0; i < n_camiones; i ++){
                Camiones[i].nodos.clear();
                Camiones[i].cargados.clear();
                Camiones[i].ruta = "Deposito";
                Camiones[i].capacidad = capacidades[i];
                Camiones[i].nodo_actual = 0;
            }
            for (j = 0; j < n_nodos; j++){
                Nodos[j].carga = std::stoi(q[j][1]);
                cantidad = cantidad + Nodos[j].carga;
            }

        }
        j = 0;
        /* ALGORITMO GREEDY EN EL QUE SE VAN ASIGNANDO RUTAS 
            A CADA CAMION*/
        while (cantidad != 0 && j < n_camiones){
            for (j = 0; j < n_camiones; j++){
              
                minimo = 99999999;
                if (Camiones[j].nodo_actual == 0){  //SI ESTA EN EL NODO DEPOSITO
                    for (i = 0; i < n_nodos; i++){
                        if (Nodos[i].carga != 0 && c_vacio[i] != 0 && c_vacio[i] < minimo){
                            pos = i;
                            minimo = c_vacio[i];              //SE GUARDA LA POS DEL VECTOR DEPOSITO -> CLIENTES Y EL VALOR MINIMO (LA DIST)
                        }
                    }
                }
                if (Nodos[pos].carga != 0 && Camiones[j].nodo_actual == 0){                //SI EL NODO NO HA SIDO DESCARGADO Y CAMION ESTA EN DEPOSITOl;
                        Camiones[j].capacidad = Camiones[j].capacidad - Nodos[pos].carga;
                        Camiones[j].cargados.push_back(Nodos[pos].tipo_material);
                        Camiones[j].nodos.push_back(Nodos[pos].posicion);
                        pos_letra_solucion = letra_to_pos(Nodos[pos].tipo_material, materiales);
                        Camiones[j].nodo_actual = pos;
                        Camiones[j].ruta = Camiones[j].ruta + " - " + std::to_string(pos);
                        cantidad = cantidad - Nodos[pos].carga;
                        Nodos[pos].carga = 0;
                        Z = Z + (1 - alpha ) * minimo;                           //SE LLEVA EL CAMION AL NODO NO DESCARGADO Y SE SUMA SOLO DISTANCIA, NO HAY RIESGO
                        Z_distancias = Z_distancias + (1 - alpha) * minimo;
                        Camiones[j].mat_dom = Nodos[pos].tipo_material;
                     
                }

                if (/*Nodos[pos].carga != 0 &&*/ Camiones[j].nodo_actual != 0){
                    pos_letra_solucion = letra_to_pos(Camiones[j].mat_dom, materiales);
                    flag = 1;
                    while(flag){ 
                        
                        //pos = -1;
                        minimo = 9999999;
                        for (i = 0; i < n_nodos; i++){
                            
                            if (Nodos[i].carga != 0 && compatible(Camiones[j].mat_dom, Nodos[i].tipo_material, materiales) != guion && 
                                matrices[pos_letra_solucion][Camiones[j].nodo_actual][i] != 0 && 
                                matrices[pos_letra_solucion][Camiones[j].nodo_actual][i] < minimo &&
                                compatible_todos(Camiones[j].cargados, Nodos[i].tipo_material,materiales)                                
                                ){
                               
                                pos = i;
                                minimo = matrices[pos_letra_solucion][Camiones[j].nodo_actual][i];
                                
                            }

                        }
                        if (Nodos[pos].carga != 0 && Camiones[j].nodo_actual != 0 && pos != -1 &&
                            compatible(Camiones[j].mat_dom, Nodos[pos].tipo_material, materiales) && minimo != 9999999 
                            && compatible_todos(Camiones[j].cargados, Nodos[pos].tipo_material,materiales) 
                            ){
                            Camiones[j].capacidad = Camiones[j].capacidad - Nodos[pos].carga;
                            Camiones[j].cargados.push_back(Nodos[pos].tipo_material);
                            Camiones[j].nodos.push_back(Nodos[pos].posicion);
                            pos_letra_solucion = letra_to_pos(Camiones[j].mat_dom, materiales);

                            Z = Z + matrices[pos_letra_solucion][Camiones[j].nodo_actual][pos];  
                            Z_distancias = Z_distancias + distancias[pos_letra_solucion][Camiones[j].nodo_actual][pos];
                            Z_riesgos = Z_riesgos + riesgos[pos_letra_solucion][Camiones[j].nodo_actual][pos]; 
                            Camiones[j].nodo_actual = pos;
                            Camiones[j].ruta = Camiones[j].ruta + " - " + std::to_string(pos);
                            Camiones[j].mat_dom =  compatible(Camiones[j].mat_dom, Nodos[pos].tipo_material, materiales);
                            cantidad = cantidad - Nodos[pos].carga;
                            Nodos[pos].carga = 0;               
             
                        }
                        else{
                            //Camiones[j].nodo_actual = 0;
                            flag = 0;

                        }
                    }
                }
                /*SI NO HAY NODOS CON CARGA, SE LLEVAN LOS CAMIONES AL DEPOSITO*/

                if (cantidad == 0){ //CAMIONES AL DEPOSITO
                    j = n_camiones + 1;
                    for(i = 0; i < n_camiones; i++){
                        minimo = 999999;
                        Camiones[i].ruta = Camiones[i].ruta + " - Deposito";
                        pos_letra_solucion = letra_to_pos(Camiones[i].mat_dom, materiales);
                        if (matrices[pos_letra_solucion][Camiones[i].nodo_actual][0] != 0 &&
                            Camiones[i].cargados.size() != 0){ 
                            
                            Z = Z + matrices[pos_letra_solucion][Camiones[i].nodo_actual][0];
                            Z_distancias = Z_distancias + distancias[pos_letra_solucion][Camiones[i].nodo_actual][0];
                            Z_riesgos = Z_riesgos + riesgos[pos_letra_solucion][Camiones[i].nodo_actual][0]; 
                            Camiones[i].nodo_actual = 0;
                        }
                        
                    }
                }            
            }
        } 
       

        float z1 = 0;
        float z2 = 0;
        float z3 = 0;
        char domin;
        string sdomin;
        vector<string> carg;
        vector<int> nods;     

        /*CALCULO DE FUNCION OBJETIVO*/ 

        for (i = 0; i < n_camiones; i++){
            carg.clear();
            nods.clear();
            for (j = 0; j < Camiones[i].cargados.size(); j++){
                if (j == 0){
                    carg.push_back(Camiones[i].cargados[j]);
                    nods.push_back(Camiones[i].nodos[j]);
                    domin = actualizar_dominante(carg,materiales);
                    std::string sdomin(1,domin);
                    posi = letra_to_pos(sdomin,materiales);
                    z1 = z1 + (1-alpha) * c_vacio[Camiones[i].nodos[j]];
                    z2 = z2 + (1-alpha) * c_vacio[Camiones[i].nodos[j]];
                }
                else{
                    carg.push_back(Camiones[i].cargados[j]);
                    nods.push_back(Camiones[i].nodos[j]);
                    domin = actualizar_dominante(carg,materiales);
                    std::string sdomin(1,domin);
                    posi = letra_to_pos(sdomin,materiales);
                    z1 = z1 + matrices[posi][nods[j]][nods[j-1]];
                    z2 = z2 + distancias[posi][nods[j]][nods[j-1]];
                    z3 = z3 + riesgos[posi][nods[j]][nods[j-1]];
                }
            }
        }
        if (probs[i_prob] == -1){
            file << "Greedy: " << endl;
            file << "Valor riesgos: " << z3 << " " << "Valor costos: " << z2 << " " << "Valor Función objetivo" << " " << (1-alpha)*z2+alpha*z3 << endl;
            file << endl;
        }

        Z = z1;
        Z_distancias = z2;
        Z_riesgos = z3;

    /*----------------------------TABU SEARCH-----------------------------------------------------------------------------------*/



        vector<Camion> aux_Camiones;



        int k, l = 0;
        int nodo_aux;
        string mat_aux;
        float new_min;
        float new_min_distancias;
        float new_min_riesgos;
        float valor_minimo = Z;
        aux_Camiones = Camiones;
        typedef tuple<int, int, int, int, int>  Tupla;
        vector <Tupla> ListaTabu; //LISTA TABU: [CAMION 1, CAMION 2, NODO (DEL CAMION 1), NODO (DEL CAMION 2), TIPO_MOVIMIENTO]
        int cam1, cam2, pos1, pos2;
        int its = 0;
        int a = 2;
        int b = 5;  //VER COMO DEFINIR ESTOS VALORES
        int route = 1;
        int flag_entra = 0;
        int w,e;
        float valor_minimo_distancias = Z_distancias;
        float valor_minimo_riesgos = Z_riesgos;
        int esta = 0;
        float Z_greedy = Z;
        float Z_greedy_d = Z_distancias;
        float Z_greedy_r = Z_riesgos;
        int j_i;
        int l_i;
        new_min = Z;
        new_min_distancias = Z_distancias;
        new_min_riesgos = Z_riesgos;
        Capture_Params(argc,argv);
        srand48(Seed);
        float prob;
        int inst;
        string mat_ins;
        int nodo_ins;
        
        /*CANTIDAD DE ITERACIONES*/

        while(its < max_its){      //EN UNA ITERACION SE GENERA TODO EL VECINDARIO PARA UNA SOLUCION
            esta = 0;
            minimo = 99999999;
            flag_entra = 0;

            /*EN ESTOS 4 FOR SE GENERA EL VECINDARIO
            DE UNA SOLUCION CANDIDATA*/

            for (i = 0; i < n_camiones; i++){
                for (k = 0; k < n_camiones; k++){  
                    for (j = 0; j < aux_Camiones[i].nodos.size(); j++){
                        for(l = 0; l < aux_Camiones[k].nodos.size(); l++){
                            
                            prob = float_rand(0,1);
                            /*SI PROB < 0,5 EL MOV ES INSERT
                            EN CASO CONTRARIO ES    2-INTERCAMBIO*/

                            /*MOVIMIENTO INSERT*/

                            if (prob < probs[i_prob] && i != k && j != 0 && l != 0 && j != aux_Camiones[i].nodos.size() - 1 && l != aux_Camiones[k].nodos.size() - 1 && j!= 1 && l != 1) {
                                nodo_ins = aux_Camiones[k].nodos[l];
                                mat_ins = aux_Camiones[k].cargados[l];

                                pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                } 

                                new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                
                                pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                for (l_i = l-1; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                }

                                new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];  

                                aux_Camiones[i].cargados.insert(aux_Camiones[i].cargados.begin()+j+1, aux_Camiones[k].cargados[l]); 
                                aux_Camiones[i].nodos.insert(aux_Camiones[i].nodos.begin()+j+1, aux_Camiones[k].nodos[l]);

                                aux_Camiones[k].cargados.erase(aux_Camiones[k].cargados.begin()+l);
                                aux_Camiones[k].nodos.erase(aux_Camiones[k].nodos.begin()+l);

                                aux_Camiones[i].capacidad = aux_Camiones[i].capacidad + Nodos[j].carga;
                                aux_Camiones[k].capacidad = aux_Camiones[k].capacidad + Nodos[l].carga;

                                aux_Camiones[i].capacidad = aux_Camiones[i].capacidad - Nodos[l].carga;
                                aux_Camiones[k].capacidad = aux_Camiones[k].capacidad - Nodos[j].carga;

                                if (actualizar_dominante(aux_Camiones[i].cargados, materiales) != guion){
                                    aux_Camiones[i].mat_dom = actualizar_dominante(aux_Camiones[i].cargados, materiales);
                                }
                                if (actualizar_dominante(aux_Camiones[k].cargados, materiales) != guion){
                                    aux_Camiones[k].mat_dom = actualizar_dominante(aux_Camiones[k].cargados, materiales);
                                }

                                pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                    for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                        new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                    } 

                                new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];

                                pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                    for (l_i = l-1; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                        new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                    }   
                                new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];      

                                esta = 0;      
                                if (new_min <= minimo && 
                                    compatible_todos(aux_Camiones[i].cargados, Nodos[aux_Camiones[i].nodos[j+1]].tipo_material, materiales) && 
                                    ListaTabu.size() != 0){
                                    for (flag = 0; flag < ListaTabu.size(); flag++){
                                            if (get<0>(ListaTabu[flag]) == i && get<1>(ListaTabu[flag]) == k && get<2>(ListaTabu[flag]) == j &&
                                                get<3>(ListaTabu[flag]) == l && get<4>(ListaTabu[flag]) == 1){
                                                esta = 1;

                                            } 
                                    }                   
                                    if (esta != 1){
                                        minimo = new_min;
                                        flag_entra = 1;
                                        cam1 = i;
                                        cam2 = k;
                                        pos1 = j;
                                        pos2 = l;
                                        inst = 1;
                                    } 
                                }
                                else if (new_min <= minimo && 
                                    compatible_todos(aux_Camiones[i].cargados, Nodos[aux_Camiones[i].nodos[j+1]].tipo_material, materiales) && 
                                    ListaTabu.size() == 0){
                                        minimo = new_min;
                                        flag_entra = 1;
                                        cam1 = i;
                                        cam2 = k;
                                        pos1 = j;
                                        pos2 = l;
                                        esta = 1;
                                        inst = 1;
                                }    
                                    //VOLVER A LA NORMALIDAD
                                pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                    for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                        new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                    } 

                                new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];

                                pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                    for (l_i = l-1; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                        new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                    }  

                                new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];                                                             

                                aux_Camiones[k].cargados.insert(aux_Camiones[k].cargados.begin()+l, mat_ins); 
                                aux_Camiones[k].nodos.insert(aux_Camiones[k].nodos.begin()+l, nodo_ins);

                                aux_Camiones[i].cargados.erase(aux_Camiones[i].cargados.begin()+j+1);
                                aux_Camiones[i].nodos.erase(aux_Camiones[i].nodos.begin()+j+1);


                                aux_Camiones[i].capacidad = aux_Camiones[i].capacidad - Nodos[j].carga;
                                aux_Camiones[k].capacidad = aux_Camiones[k].capacidad - Nodos[l].carga;

                                aux_Camiones[i].capacidad = aux_Camiones[i].capacidad + Nodos[l].carga;
                                aux_Camiones[k].capacidad = aux_Camiones[k].capacidad + Nodos[j].carga;

                                aux_Camiones[i].mat_dom = actualizar_dominante(aux_Camiones[i].cargados, materiales);
                                aux_Camiones[k].mat_dom = actualizar_dominante(aux_Camiones[k].cargados, materiales);

                                pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                    for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                        new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                    } 

                                new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];                                

                                pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                    for (l_i = l-1; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                        new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                    }         
                                new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];                                        

                            }

                            /* MOVIMIENTO 2-INTERCAMBIO*/
                            else{ 
                                if (i != k && j != 0 && l != 0 && j != aux_Camiones[i].nodos.size() - 1 && l != aux_Camiones[k].nodos.size() - 1 && j!= 1 && l != 1 ){ 
                                    //QUITAR EL ARCO DEL ANTERIOR AL ACTUAL
                                    //QUITAR EL ARCO DEL ACTUAL AL SIGUIENTE
                                    //QUITAR J-1 -> J, J -> J+1
                                    //QUITAR L-1 -> L, L -> L+1
                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[i].nodos[j]];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[i].nodos[j]];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[i].nodos[j]];

                                    for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                        new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                    }

                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0]; 

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[k].nodos[l]];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[k].nodos[l]];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[k].nodos[l]];

                                    for (l_i = l; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                        new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                    }

                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];                                    
                                    //AGREGAR EL ARCO DEL ANTERIOR AL NUEVO ACTUAL
                                    //AGREGAR J-1 -> L
                                    //AGREGAR L-1 -> J
                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom, materiales);
                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[k].nodos[l]];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[k].nodos[l]];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[k].nodos[l]];
                                    
                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom, materiales);
                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[i].nodos[j]];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[i].nodos[j]];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[i].nodos[j]];

                                    /*MOVIMIENTO ENTRE LOS ARCOS*/
                                    aux_Camiones[i].capacidad = aux_Camiones[i].capacidad + Nodos[j].carga;
                                    aux_Camiones[k].capacidad = aux_Camiones[k].capacidad + Nodos[l].carga;  //DESCONTANDO LA CARGA POR EL CAMBIO

                                    mat_aux = aux_Camiones[i].cargados[j];
                                    aux_Camiones[i].cargados[j] = aux_Camiones[k].cargados[l];
                                    aux_Camiones[k].cargados[l] = mat_aux;  //CRUZA MATERIALES
                                    nodo_aux = aux_Camiones[i].nodos[j];
                                    aux_Camiones[i].nodos[j] = aux_Camiones[k].nodos[l];
                                    aux_Camiones[k].nodos[l] = nodo_aux;  //CRUZA NODOS

                                    //fin (string, search, replace)                               
                                    //replace(aux_Camiones[i].ruta, std::to_string(aux_Camiones[i].nodos[j]), std::to_string(aux_Camiones[i].nodos[l]));
                                    //replace(aux_Camiones[k].ruta, std::to_string(aux_Camiones[k].nodos[l]), std::to_string(aux_Camiones[k].nodos[j]));                                                        
                                    aux_Camiones[i].capacidad = aux_Camiones[i].capacidad - Nodos[l].carga;
                                    aux_Camiones[k].capacidad = aux_Camiones[k].capacidad - Nodos[j].carga;  //CONTANDO LA CARGA POR EL CAMBIO
                                    //CAMBIAR LOS 
                                    if (actualizar_dominante(aux_Camiones[i].cargados, materiales) != guion){
                                        aux_Camiones[i].mat_dom = actualizar_dominante(aux_Camiones[i].cargados, materiales);
                                    }
                                    if (actualizar_dominante(aux_Camiones[k].cargados, materiales) != guion){
                                        aux_Camiones[k].mat_dom = actualizar_dominante(aux_Camiones[k].cargados, materiales);
                                    }
                                    //NUEVO ACTUAL AL SIGUIENTE (DESPUES DE ACTUALIZAR EL MATERIAL DOMINANTE)
                                    //AGREGAR L -> J+1, J -> L+1

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                        for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                            new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                            new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                            new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        }

                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0]; 

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                        for (l_i = l; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                            new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                            new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                            new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        }

                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0]; 

                                    esta = 0;
                                    if (new_min <= minimo && 
                                        compatible_todos(aux_Camiones[i].cargados, Nodos[aux_Camiones[i].nodos[j]].tipo_material, materiales) && 
                                        compatible_todos(aux_Camiones[k].cargados, Nodos[aux_Camiones[k].nodos[l]].tipo_material, materiales)
                                        && ListaTabu.size() != 0){
                                        
                                        for (flag = 0; flag < ListaTabu.size(); flag++){
                                                if (get<0>(ListaTabu[flag]) == i && get<1>(ListaTabu[flag]) == k && get<2>(ListaTabu[flag]) == j &&
                                                    get<3>(ListaTabu[flag]) == l && get<4>(ListaTabu[flag]) == 0){
                                                    esta = 1;


                                                } 
                                        }                   
                                        if (esta != 1){
                                            minimo = new_min;
                                            flag_entra = 1;
                                            cam1 = i;
                                            cam2 = k;
                                            pos1 = j;
                                            pos2 = l;
                                            inst = 0;
                                        }       

                                    }
                                    else if (new_min <= minimo && 
                                        compatible_todos(aux_Camiones[i].cargados, Nodos[aux_Camiones[i].nodos[j]].tipo_material, materiales) && 
                                        compatible_todos(aux_Camiones[k].cargados, Nodos[aux_Camiones[k].nodos[l]].tipo_material, materiales) &&
                                        ListaTabu.size() == 0){
                                            minimo = new_min;
                                            flag_entra = 1;
                                            cam1 = i;
                                            cam2 = k;
                                            pos1 = j;
                                            pos2 = l;
                                            esta = 1;
                                            inst = 0;
                                    }
                                  
                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                        for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                            new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                            new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                            new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        }

                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];           

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                        for (l_i = l; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                            new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                            new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                            new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        }

                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0]; 

                                    //std::replace(aux_Camiones[i].ruta.begin(), aux_Camiones[i].ruta.end(), std::to_string(l), std::to_string(j));
                                    //std::replace(aux_Camiones[k].ruta.begin(), aux_Camiones[k].ruta.end(), std::to_string(j), std::to_string(l));
                                    //replace(aux_Camiones[i].ruta, std::to_string(aux_Camiones[i].nodos[l]), std::to_string(aux_Camiones[i].nodos[j]));
                                    //replace(aux_Camiones[k].ruta, std::to_string(aux_Camiones[k].nodos[j]), std::to_string(aux_Camiones[k].nodos[l]));
                                    //FUNCION FINDANDREPLACE CAMBIA TODOS LOS SUBSINTRGS, EJ SI HAY UN 1 Y UN 17 CAMBIA TODOS ESOS 1!!! REVISAR
                                    aux_Camiones[i].capacidad = aux_Camiones[i].capacidad + Nodos[l].carga;
                                    aux_Camiones[k].capacidad = aux_Camiones[k].capacidad + Nodos[j].carga;

                                    aux_Camiones[i].capacidad = aux_Camiones[i].capacidad - Nodos[j].carga;
                                    aux_Camiones[k].capacidad = aux_Camiones[k].capacidad - Nodos[l].carga;

                                    mat_aux = aux_Camiones[i].cargados[j];
                                    aux_Camiones[i].cargados[j] = aux_Camiones[k].cargados[l];
                                    aux_Camiones[k].cargados[l] = mat_aux;  //CRUZA MATERIALES
                                    nodo_aux = aux_Camiones[i].nodos[j];
                                    aux_Camiones[i].nodos[j] = aux_Camiones[k].nodos[l];
                                    aux_Camiones[k].nodos[l] = nodo_aux;               //MOVIMIENTO

                                    aux_Camiones[i].mat_dom = actualizar_dominante(aux_Camiones[i].cargados, materiales);
                                    aux_Camiones[k].mat_dom = actualizar_dominante(aux_Camiones[k].cargados, materiales);


                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom, materiales);
                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[k].nodos[l]];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[k].nodos[l]];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[k].nodos[l]];
                                    
                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom, materiales);
                                    new_min = new_min - matrices[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[i].nodos[j]];
                                    new_min_distancias = new_min_distancias - distancias[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[i].nodos[j]];
                                    new_min_riesgos = new_min_riesgos - riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[i].nodos[j]];                          

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[i].nodos[j]];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[i].nodos[j]];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j-1]][aux_Camiones[i].nodos[j]];
        
                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[k].nodos[l]];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[k].nodos[l]];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l-1]][aux_Camiones[k].nodos[l]];

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[i].mat_dom,materiales);
                                        for (j_i = j; j_i < aux_Camiones[i].nodos.size() - 1; j_i++){
                                            new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                            new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                            new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[j_i]][aux_Camiones[i].nodos[j_i+1]];
                                        }

                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[i].nodos[aux_Camiones[i].nodos.size()-1]][0]; 

                                    pos_letra_solucion = letra_to_pos(aux_Camiones[k].mat_dom,materiales);
                                        for (l_i = l; l_i < aux_Camiones[k].nodos.size() - 1; l_i++){ 
                                            new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                            new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                            new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[l_i]][aux_Camiones[k].nodos[l_i+1]];
                                        }   
                                    new_min = new_min + matrices[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_distancias = new_min_distancias + distancias[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];
                                    new_min_riesgos = new_min_riesgos + riesgos[pos_letra_solucion][aux_Camiones[k].nodos[aux_Camiones[k].nodos.size()-1]][0];                          
                                                          
                                }

                            }
                        }
                    }
                }
            }

            /* SI DEL VECINDARIO SE ENCONTRO UNA MEJOR SOLUCION*/

            if (flag_entra == 1){          
                //DE TODO EL VECINDARIO SE ACTUALIZA VALOR_MINIMO COMO LA MEJOR SOL DEL VECINDARIO (VER LISTA TABU)
                //AL ENTRAR SE DEBE ACTUALIZAR EL AUX_CAMIONES Y HACER VECINDARIO CON ESO 
                //ACTUALIZAR VALOR_MINIMO_DISTANCIAS, RIESGOS Y EL NORMAL = NEW_MIN_DISTANCIAS, RIESGOS Y EL NORMAL
                if (inst == 1){
                    ListaTabu.push_back(make_tuple(cam1,cam2,pos1,pos2,1));
                    //cam1 = i;cam2 = k;pos1 = j;pos2 = l;
                    nodo_ins = aux_Camiones[cam2].nodos[pos2];
                    mat_ins = aux_Camiones[cam2].cargados[pos2];
                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam1].mat_dom,materiales);
                        for (j_i = pos1; j_i < aux_Camiones[cam1].nodos.size() - 1; j_i++){
                            valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                        } 

                    valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];   
                                

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam2].mat_dom,materiales);
                        for (l_i = pos2-1; l_i < aux_Camiones[cam2].nodos.size() - 1; l_i++){ 
                            valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                        }                  
                    valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];                     

                    aux_Camiones[cam1].cargados.insert(aux_Camiones[cam1].cargados.begin()+pos1+1, mat_ins); 
                    aux_Camiones[cam1].nodos.insert(aux_Camiones[cam1].nodos.begin()+pos1+1, nodo_ins);

                    aux_Camiones[cam2].cargados.erase(aux_Camiones[cam2].cargados.begin()+pos2);
                    aux_Camiones[cam2].nodos.erase(aux_Camiones[cam2].nodos.begin()+pos2);

                    aux_Camiones[cam1].capacidad = aux_Camiones[cam1].capacidad + Nodos[pos1].carga;
                    aux_Camiones[cam2].capacidad = aux_Camiones[cam2].capacidad + Nodos[pos2].carga;

                    aux_Camiones[cam1].capacidad = aux_Camiones[cam1].capacidad - Nodos[pos2].carga;
                    aux_Camiones[cam2].capacidad = aux_Camiones[cam2].capacidad - Nodos[pos1].carga;


                    if (actualizar_dominante(aux_Camiones[cam1].cargados, materiales) != guion){
                        aux_Camiones[cam1].mat_dom = actualizar_dominante(aux_Camiones[cam1].cargados, materiales);
                    }
                    if (actualizar_dominante(aux_Camiones[cam2].cargados, materiales) != guion){
                        aux_Camiones[cam2].mat_dom = actualizar_dominante(aux_Camiones[cam2].cargados, materiales);
                    }

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam1].mat_dom,materiales);
                        for (j_i = pos1; j_i < aux_Camiones[cam1].nodos.size() - 1; j_i++){
                            valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                        } 
                    valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam2].mat_dom,materiales);
                        for (l_i = pos2-1; l_i < aux_Camiones[cam2].nodos.size() - 1; l_i++){ 
                            valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                        }   

                    valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    
                }
                else{
                    ListaTabu.push_back(make_tuple(cam1,cam2,pos1,pos2, 0));
                    //cam1 = i;cam2 = k;pos1 = j;pos2 = l;
                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam1].mat_dom,materiales);
                    valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[pos1-1]][aux_Camiones[cam1].nodos[pos1]];
                    valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[pos1-1]][aux_Camiones[cam1].nodos[pos1]];
                    valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[pos1-1]][aux_Camiones[cam1].nodos[pos1]];

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam1].mat_dom,materiales);
                        for (j_i = pos1; j_i < aux_Camiones[cam1].nodos.size() - 1; j_i++){
                            valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                        }

                    valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0]; 

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam2].mat_dom,materiales);
                    valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[pos2-1]][aux_Camiones[cam2].nodos[pos2]];
                    valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[pos2-1]][aux_Camiones[cam2].nodos[pos2]];
                    valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[pos2-1]][aux_Camiones[cam2].nodos[pos2]];

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam2].mat_dom,materiales);
                        for (l_i = pos2; l_i < aux_Camiones[cam2].nodos.size() - 1; l_i++){ 
                            valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                        }

                    valor_minimo = valor_minimo - matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias - distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos - riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];                        

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam1].mat_dom, materiales);
                    valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[pos1-1]][aux_Camiones[cam2].nodos[pos2]];
                    valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[pos1-1]][aux_Camiones[cam2].nodos[pos2]];
                    valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[pos1-1]][aux_Camiones[cam2].nodos[pos2]];

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam2].mat_dom, materiales);
                    valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[pos2-1]][aux_Camiones[cam1].nodos[pos1]];
                    valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[pos2-1]][aux_Camiones[cam1].nodos[pos1]];
                    valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[pos2-1]][aux_Camiones[cam1].nodos[pos1]];
                    /*MOVIMIENTO ENTRE LOS ARCOS*/
                    aux_Camiones[cam1].capacidad = aux_Camiones[cam1].capacidad + Nodos[pos1].carga;
                    aux_Camiones[cam2].capacidad = aux_Camiones[cam2].capacidad + Nodos[pos2].carga;  //DESCONTANDO LA CARGA POR EL CAMBIO

                    mat_aux = aux_Camiones[cam1].cargados[pos1];
                    aux_Camiones[cam1].cargados[pos1] = aux_Camiones[cam2].cargados[pos2];
                    aux_Camiones[cam2].cargados[pos2] = mat_aux;  //CRUZA MATERIALES
                    nodo_aux = aux_Camiones[cam1].nodos[pos1];
                    aux_Camiones[cam1].nodos[pos1] = aux_Camiones[cam2].nodos[pos2];
                    aux_Camiones[cam2].nodos[pos2] = nodo_aux;  //CRUZA NODOS

                    //fin (string, search, replace)                               
                    //replace(aux_Camiones[cam].ruta, std::to_string(aux_Camiones[cam1].nodos[pos1]), std::to_string(aux_Camiones[cam1].nodos[pos2]));
                    //replace(aux_Camiones[cam2].ruta, std::to_string(aux_Camiones[cam2].nodos[pos2]), std::to_string(aux_Camiones[cam2].nodos[pos1]));                                                        
                    aux_Camiones[cam1].capacidad = aux_Camiones[cam1].capacidad - Nodos[pos2].carga;
                    aux_Camiones[cam2].capacidad = aux_Camiones[cam2].capacidad - Nodos[pos1].carga;  //CONTANDO LA CARGA POR EL CAMBIO
                    //CAMBIAR LOS 
                    if (actualizar_dominante(aux_Camiones[cam1].cargados, materiales) != guion){
                        aux_Camiones[cam1].mat_dom = actualizar_dominante(aux_Camiones[cam1].cargados, materiales);
                    }
                    if (actualizar_dominante(aux_Camiones[cam2].cargados, materiales) != guion){
                        aux_Camiones[cam2].mat_dom = actualizar_dominante(aux_Camiones[cam2].cargados, materiales);
                    }
                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam1].mat_dom,materiales);
                        for (j_i = pos1; j_i < aux_Camiones[cam1].nodos.size() - 1; j_i++){
                            valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[j_i]][aux_Camiones[cam1].nodos[j_i+1]];
                        }

                    valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam1].nodos[aux_Camiones[cam1].nodos.size()-1]][0]; 

                    pos_letra_solucion = letra_to_pos(aux_Camiones[cam2].mat_dom,materiales);
                        for (l_i = pos2; l_i < aux_Camiones[cam2].nodos.size() - 1; l_i++){ 
                            valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                            valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[l_i]][aux_Camiones[cam2].nodos[l_i+1]];
                        }

                    valor_minimo = valor_minimo + matrices[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_distancias = valor_minimo_distancias + distancias[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];
                    valor_minimo_riesgos = valor_minimo_riesgos + riesgos[pos_letra_solucion][aux_Camiones[cam2].nodos[aux_Camiones[cam2].nodos.size()-1]][0];                        

                }
                new_min = valor_minimo;
                new_min_distancias = valor_minimo_distancias;
                new_min_riesgos = valor_minimo_riesgos;
            }
            /* SI LA SOLUCION CANDIDATA ES MEJOR QUE LA MEJOR SOLUCION (Z) */
            if(valor_minimo <= Z){
                //SI EL VALOR_MINIMO (MEJOR SOL CANDIDATA) ES MEJOR QUE LA SOL GENERAL
                //SE ACTUALIZA 
                Z = valor_minimo;
                Z_riesgos = valor_minimo_riesgos;
                Z_distancias = valor_minimo_distancias;
                Camiones = aux_Camiones;
            }
            if (ListaTabu.size() == maxlistatabu){
                ListaTabu.erase(ListaTabu.begin());
            }
            its++;

        }
         z1 = 0;
         z2 = 0;
         z3 = 0;
         carg.clear();
         nods.clear();
        
        for (i = 0; i < n_camiones; i++){
            carg.clear();
            nods.clear();
            for (j = 0; j < Camiones[i].cargados.size(); j++){
                if (j == 0){
                    carg.push_back(Camiones[i].cargados[j]);
                    nods.push_back(Camiones[i].nodos[j]);
                    domin = actualizar_dominante(carg,materiales);
                    std::string sdomin(1,domin);
                    posi = letra_to_pos(sdomin,materiales);
                    z2 = z2 + (1-alpha)*c_vacio[nods[j]];
                }
                else{
                    carg.push_back(Camiones[i].cargados[j]);
                    nods.push_back(Camiones[i].nodos[j]);
                    domin = actualizar_dominante(carg,materiales);
                    std::string sdomin(1,domin);
                    posi = letra_to_pos(sdomin,materiales);
                    //z1 = z1 + matrices[posi][nods[j]][nods[j-1]];
                    z2 = z2 + distancias[posi][nods[j]][nods[j-1]];
                    z3 = z3 + riesgos[posi][nods[j]][nods[j-1]];
                }
            }
        }

        if (probs[i_prob] == -1){
            file << "Movimiento: 2-Intercambio " << endl;
            file << "Valor riesgos: " << z3 << " " << "Valor costos: " << z2 << " " << "Valor Función objetivo" << " " << (1-alpha)*z2+alpha*z3 << endl;
            file << endl;
        }
        else if(probs[i_prob] == 0.5){
            file << "Movimiento: 2-Intercambio e Insert " << endl;
            file << "Valor riesgos: " << z3 << " " << "Valor costos: " << z2 << " " << "Valor Función objetivo" << " " << (1-alpha)*z2+alpha*z3 << endl;
            file << endl;
        }
        else if(probs[i_prob] == 2){
            file << "Ambos movimientos: Insert " << endl;
            file << "Valor riesgos: " << z3 << " " << "Valor costos: " << z2 << " " << "Valor Función objetivo" << " " << (1-alpha)*z2+alpha*z3 << endl;
            file << endl;
        }
        //PASAR POR PARAMETRO SEMILLA, VALOR DE ALFA, LARGO LISTA TABU, ARCHIVO
    }

    file << endl; 
    cout << endl << "Resultados creados!, revise el archivo output.txt" << endl;
    return 0;
}