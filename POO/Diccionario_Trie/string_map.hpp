template <typename T>
string_map<T>::string_map() : raiz(nullptr), _size(0), claves(0), significados(0){}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    borrarNodo(raiz);
    copiarClaves(d);
    return *this;
}

template <typename T>
void string_map<T>::borrarNodo(Nodo* n){
    if(n != nullptr) {
        if(n -> definicion != nullptr){
            delete n-> definicion;
            n-> definicion = nullptr;
        }
        for (int i = 0; i < 256; i++) {
            borrarNodo( n-> siguientes[i]);
        }
        delete n;
    }
}

template <typename T>
void string_map<T>::copiarClaves(const string_map<T> &d) {
    for(int i = 0; i < d.claves.size(); i++){
        insert(make_pair(d.claves[i],d.significados[i]));
    }
}

template <typename T>
void string_map<T>::insert(const pair<std::string, T> &p) {
    T *def = new T(p.second);
    if (raiz == nullptr) {
        //Si el trie esta vacio creamos un nuevo nodo al que apunta la raiz
        raiz = new Nodo();
    }
    Nodo *actual = raiz;
    for (int i = 0; i < p.first.length(); ++i) {
        int c = int(p.first[i]);    //int(char) de C++ devuelve un numero: el codigo ASCII de un caracter
        //Buscamos en que lugar del trie debe ir la nueva clave
        if (actual->siguientes[c] == nullptr) {
            actual->siguientes[c] = new Nodo();
            actual = actual->siguientes[c];
        } else {
            actual = actual->siguientes[c];
        }
    }
    delete actual->definicion;
    actual->definicion = def;
    claves.push_back(p.first);
    significados.push_back(p.second);
    _size++;
}

template <typename T>
string_map<T>::~string_map(){
    borrarNodo(raiz);
}


template <typename T>
int string_map<T>::count(const string& clave) const{
    Nodo* actual = raiz;
    if(raiz == nullptr){
        return 0;
    } else{
        if(clave.length() == 0){
            return 0;
        }
        for(int j = 0; j < clave.length(); j++){
            unsigned int b = int(clave[j]);
            if(actual->siguientes[b] == nullptr){
                return 0;
            } else{
                actual = actual->siguientes[b];
            }
        }
        if(actual -> definicion != nullptr){
            return 1;
        } else {
            return 0;
        }
    }
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* actual = raiz;
    for(char i : clave){
        unsigned int c = int(i);
        actual = actual->siguientes[c];
    }
    return actual->definicion;
}


template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* actual = raiz;
    for(char i : clave){
        unsigned int c = int(i);
        actual = actual->siguientes[c];
    }
    return *(actual->definicion);
}


//Hay que borrar el significado asociado a la clave y todos los nodos intermedios que ya no tengan razon de ser
template <typename T>
void string_map<T>::erase(const string& clave) {
    Nodo* actual = raiz;
    Nodo* ult = raiz;
    int ultimapos = 0;
    //Buscamos el nodo que representa la clave y en el camino guardamos el ultimo nodo que no hay que borrar
    for(int i = 0;i < clave.length(); i++){
        unsigned int c = int(clave[i]);
        actual = actual->siguientes[c];
        int n = 0;
        for(int j = 0; j < 256; j++){
            if(actual->siguientes[j] != nullptr){
                n++;
            }
        }
        //Si el nodo actual tiene mas de un hijo o tiene significado, el ultimo nodo es el nodo actual y ultimo indice es nuestra posicion en la clave
        if(n > 1 || (actual->definicion != nullptr && n > 0)){
            ult = actual;
            ultimapos = i + 1;
        }
    }
    // nodo actual es el nodo de la clave. Borramos su significado
    delete actual->definicion;
    actual->definicion = nullptr;
    //ultimo nodo es el ultimo nodo en el camino de la clave que no hay que borrar.
    if(actual != ult){
        unsigned int m = int(clave[ultimapos]);
        Nodo* sig = ult-> siguientes[m];
        ult-> siguientes[m] = nullptr;
        ult = sig;
        ultimapos++;
        while(ultimapos<clave.length()){
            m = int(clave[ultimapos]);
            sig = ult->siguientes[m];
            delete ult;
            ult = sig;
            ultimapos++;
        }
        delete ult;
    }
    _size--;
    vector<string> resclaves;
    vector<T> ressignificados;
    for(int i = 0; i < claves.size(); i++){
        if(claves[i] != clave){
            resclaves.push_back(claves[i]);
            ressignificados.push_back(significados[i]);
        }
    }
    claves = resclaves;
    significados = ressignificados;
    if(_size == 0){
        delete raiz;
        raiz = nullptr;
    }


}

template <typename T>
int string_map<T>::size() const{
    return _size;
}

template <typename T>
bool string_map<T>::empty() const{
    if(_size == 0){
        return true;
    } else {
        return false;
    }
}
